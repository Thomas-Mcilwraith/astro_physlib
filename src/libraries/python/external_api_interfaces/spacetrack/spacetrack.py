"""
SpaceTrack API.

Author: Thomas McIlwraith
Date: 20/06/2026
"""

# Imports
import requests
import logging
import os
import json

# Global variables
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s %(levelname)s %(message)s",
    datefmt="%Y-%m-%dT%H:%M:%S",
)

SPACETRACK_USERNAME = os.getenv("SPACETRACK_USERNAME", "")
SPACETRACK_PASSWORD = os.getenv("SPACETRACK_PASSWORD", "")


class SpaceTrackAPI:
    def __init__(self, username: str, password: str):
        self.domain: str = "https://www.space-track.org"
        self.latest_response: requests.Response | None = None
        self.query_tle_cat: str = "/basicspacedata/query/class/gp/decay_date/null-val/EPOCH/>now-30/orderby/NORAD_CAT_ID,EPOCH/format/json"
        logging.info("Initialized SpaceTrackAPI")

        # SpaceTrack requires authorization
        try:
            self.session = requests.Session()
            self.session_login = self.session.post(
                self.domain + "/ajaxauth/login",
                data={
                    "identity": username,
                    "password": password,
                },
            )
            self.session_login.raise_for_status()
            logging.info(f"Login message: {self.session_login.text}")
        except requests.exceptions.HTTPError as errh:
            logging.error("Failed to authorize")
            print(errh)
            return None

        return

    def get_tle_catalog(self) -> None:
        if self.session_login.status_code != 200:
            logging.warning(
                "Current session is not authorized. "
                + "Not attempting to get TLE Catalog"
            )
            return None
        logging.info("Getting TLE Catalog")
        logging.info(f"Querying {self.domain}{self.query_tle_cat}")
        try:
            response = self.session.get(self.domain + self.query_tle_cat)
            response.raise_for_status()
            logging.info("Successfully retrieved TLE Catalog")
        except requests.exceptions.HTTPError as errh:
            logging.error("Failed to retrieve TLE Catalog")
            print(errh)
            return None
        self.latest_response = response
        return None

    def save_raw_response(self, file_name: str, use_json: bool = False) -> None:
        if self.latest_response is None:
            logging.error("No response to save")
            return None
        logging.info(f"Saving raw response to {file_name}")
        try:
            with open(file_name, "w") as f:
                if not use_json:
                    f.write(self.latest_response.text)
                    logging.info(f"Successfully saved raw response to {file_name}")
                else:
                    json.dump(self.latest_response.json(), f, indent=4)
                    logging.info(f"Successfully saved JSON response to {file_name}")

        except Exception as err:
            logging.error(f"Failed to save raw response to {file_name}")
            print(err)
        return None


if __name__ == "__main__":
    if not SPACETRACK_USERNAME or not SPACETRACK_PASSWORD:
        logging.error("SpaceTrack username or password not set")

    api = SpaceTrackAPI(SPACETRACK_USERNAME, SPACETRACK_PASSWORD)
    api.get_tle_catalog()
    api.save_raw_response("/home/admin/test_spacetrack_tle_cat.json", use_json=True)
