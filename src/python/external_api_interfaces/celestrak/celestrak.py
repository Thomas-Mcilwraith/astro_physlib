"""
Celestrak API.

Author: Thomas McIlwraith
Date: 20/06/2026
"""

# Imports
import requests
import pandas as pd
import logging
from enum import Enum
from io import StringIO

from file_io.parameter_evolution_file.parameter_evolution_file import ParameterEvolutionFile

# Global variables
logging.basicConfig(
    level=logging.DEBUG,
    format="%(asctime)s - %(levelname)s - %(message)s"
)

class CelestrakFileFormat(Enum):
    RAW = "raw"
    PEV = "pev"

class CelestrakAPI:
    def __init__(self):
        self.domain: str = "https://celestrak.org" 
        self.EOP_last_5_years: str = f"/SpaceData/EOP-Last5Years.csv"
        self.response: requests.Response | None = None
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

        self.response = response
        return None

    def save_raw_response(self, file_name: str) -> None:
        if self.response is None:
            logging.error("No response to save")
            return None

        logging.info(f"Saving raw response to {file_name}")
        try:
            with open(file_name, "w") as f:
                f.write(self.response.text)
                logging.info(f"Successfully saved raw response to {file_name}")
        except Exception as err:
            logging.error(f"Failed to save raw response to {file_name}")
            print(err)
        return None

    def get_EOP_last_5_years_as_pev(self, file_name: str) -> ParameterEvolutionFile | None:
        if self.response is None:
            logging.error("No response to save")
            return None

        logging.info(f"Saving EOP last 5 years as PEV to {file_name}")
        try:
            df = pd.read_csv(StringIO(self.response.text))
            logging.info(f"Loaded CSV data to dataframe")
        except Exception as err:
            logging.error(f"Failed to read CSV data")
            print(err)
            return None

        df["DATE"] = pd.to_datetime(df["DATE"])
        df = df.sort_values("DATE")

        param_ev = ParameterEvolutionFile()
        param_ev.set_filename(file_name)
        param_ev.set_comment("CELESTRAK_EOP_LAST_5_YEARS")

        for col in df.columns:

            units = "-"

            match col:
                case "MJD":
                    units = "days"
                case "X":
                    units = "arcsec"
                case "Y":
                    units = "arcsec"
                case "UT1-UTC":
                    units = "s"
                case "LOD":
                    units = "s"
                case "DPSI":
                    units = "arcsec"
                case "DEPS":
                    units = "arcsec"
                case "DX":
                    units = "arcsec"
                case "DY":
                    units = "arcsec"
                case "DAT":
                    units = "s"

            # No need to write the DATE or DATA_TYPE column
            if col == "DATE" or col == "DATA_TYPE":
                continue

            param_ev.add_parameter_evolution(
                    name=col,
                    units=units,
                    values=df[col].tolist()
            )
            logging.info(f"Added {col} (units) to PEV")

        logging.info(f"Successfully saved PEV to {file_name}")

        return param_ev

if __name__ == "__main__":
    api = CelestrakAPI()
    api.get_EOP_last_5_years()
    param_ev = api.get_EOP_last_5_years_as_pev("eop_last_5_years.pev")

    if param_ev is not None:
        param_ev.write()
