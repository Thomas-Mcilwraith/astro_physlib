"""
Celestrak API.

Author: Thomas McIlwraith
Date: 20/06/2026
"""

# Imports
import requests
import logging

# Global variables
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s [%(levelname)s] %(message)s",
    datefmt="%Y-%m-%dT%H:%M:%S"
)

class CelestrakAPI:
    def __init__(self):
        self.domain: str = "https://celestrak.org" 
        self.EOP_last_5_years: str = f"/SpaceData/EOP-Last5Years.csv"
        self.latest_response: requests.Response | None = None
        logging.info("Initialized CelestrakAPI")
        return

    def get_EOP_last_5_years(self) -> None:

        logging.info("Getting EOP last 5 years (CSV format)")
        try:
            response = requests.get(self.domain + self.EOP_last_5_years)
            response.raise_for_status()
            logging.info("Successfully retrieved EOP last 5 years (CSV format)")
        except requests.exceptions.HTTPError as errh:
            logging.error("Failed to retrieve EOP last 5 years (CSV format)")
            print(errh)
            return None

        self.latest_response = response
        return None

    def save_raw_response(self, file_name: str) -> None:
        if self.latest_response is None:
            logging.error("No response to save")
            return None

        logging.info(f"Saving raw response to {file_name}")
        try:
            with open(file_name, "w") as f:
                f.write(self.latest_response.text)
                logging.info(f"Successfully saved raw response to {file_name}")
        except Exception as err:
            logging.error(f"Failed to save raw response to {file_name}")
            print(err)
        return None

if __name__ == "__main__":
    api = CelestrakAPI()
    api.get_EOP_last_5_years()
    api.save_raw_response(r"C:\Users\tmcilwraith\Documents\github\psf_physlib\src\python\eop_last_5_years.csv")
