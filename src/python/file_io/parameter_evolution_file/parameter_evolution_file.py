"""
Parameter evolution file.

This module provides a class for reading and writing parameter evolution files.
Author: Thomas McIlwraith
Date: 20/06/2026
"""

# Imports
import os
from dataclasses import dataclass
import pandas as pd
import logging

# Global variables
CHARS_PER_WORD = 10
CHARS_PER_SEPARATOR = 23
WORD_PRECISION = 8
VAR_UNITS_SEPARATOR = '~'
UNITS_NO_UNIT = "-"

NO_TYPE = "UNSPECIFIED"
NO_SOURCE = "UNSPECIFIED"
NO_REFERENCE = "UNSPECIFIED"

# Global variables
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s [%(levelname)s] %(message)s",
    datefmt="%Y-%m-%dT%H:%M:%S"
)

# Class definition
@dataclass
class ParameterEvolution:
    header: str
    units: str
    values: list[float]

class ParameterEvolutionFile:
    __slots__ = [
        'filepath',
        'comment',
        'type',
        'source',
        'reference',
        'data',
        'n_data_points'
    ]

    def __init__(self):
        self.filepath: str | None = None
        self.comment: str = ""
        self.type: str | None = None
        self.source: str | None = None
        self.reference: str | None = None
        self.data: list[ParameterEvolution] = []
        self.n_data_points: int = 0
        return

    def __repr__(self) -> str:
        repr_string = ""
        repr_string += f"filename= {self.filepath if self.filepath else 'NOTDEFINED'}"
        repr_string += "\n\n"
        repr_string += self._generate_file_string()
        return repr_string

    def set_filename(self, filename: str) -> None:
        """Set the filename"""
        self.filepath = filename

    def set_comment(self, comment: str) -> None:
        """Set the comment"""
        self.comment = comment

    def set_type(self, type: str) -> None:
        """Set the type"""
        self.type = type

    def set_source(self, source: str) -> None:
        """Set the source"""
        self.source = source

    def set_reference(self, reference: str) -> None:
        """Set the reference"""
        self.reference = reference

    def add_parameter_evolution(
            self,
            name: str,
            units: str,
            values: list[float]) -> None:
        """
        Add a parameter evolution to the file object, which stores many.
        """

        # Check all values are the same type
        if not all(isinstance(value, type(values[0])) for value in values):
            raise ValueError(
                    "Attempted to add a parameter evolution to a ParameterEvolutionFile " +
                    "with mixed data types."
                    )

        # Check the header is valid
        if len(name+VAR_UNITS_SEPARATOR+units) > CHARS_PER_SEPARATOR:
            raise ValueError(
                    "Name + units is too long for a parameter evolution. " +
                    f"({name+VAR_UNITS_SEPARATOR+units} > {CHARS_PER_SEPARATOR})"
                    )
        
        # Check the value length matches
        if self.n_data_points != 0 and len(values) != self.n_data_points:
            raise ValueError(
                    "Attempted to add a parameter evolution to a ParameterEvolutionFile " +
                    "with a different number of values than the number of values in the " +
                    "file."
                    )

        param_ev = ParameterEvolution(name, units, values)
        self.data.append(param_ev)

        # If the length has not been set yet (new object), set it
        if self.n_data_points == 0:
            self.n_data_points = len(values)

        return None

    def write(self) -> None:
        """Write the file to disk"""

        if self.filepath is None:
            raise ValueError("Attempted to write a ParameterEvolutionFile to disk with no filename.")

        logging.info(f"Writing ParameterEvolutionFile to {self.filepath}")

        with open(self.filepath, 'w') as f:
            f.write(self._generate_file_string())

        return None

    @classmethod
    def from_pev(cls, filepath: str) -> 'ParameterEvolutionFile':
        """Read the file from disk"""

        if not os.path.isfile(filepath):
            raise FileNotFoundError(f"Attempted to read a ParameterEvolutionFile from " +
                             f"disk, but the file {filepath} does not exist.")

        with open(filepath, 'r') as f:
            lines = f.readlines()

        logging.info(f"Reading ParameterEvolutionFile from {filepath}")

        headers = []
        data = []
        type = ""
        source = ""
        reference = ""
        comment = ""
        for i, line in enumerate(lines):

            # Load the metadata
            if line.startswith("# TYPE:"):
                type = line[8:].strip("\n")
                continue
            elif line.startswith("# SOURCE:"):
                source = line[10:].strip("\n")
                continue
            elif line.startswith("# REFERENCE:"):
                reference = line[13:].strip("\n")
                continue
            elif line.startswith("# COMMENT:"):
                comment = line[11:].strip("\n")
                continue
            elif not line.strip():
                continue

            # Load header line
            if "~" in line:
                headers = line.split()

            # Skip non data lines until we read the data lines
            if not ("~" not in line and "#" not in line):
                continue

            if len(headers) == 0:
                raise ValueError(
                        "Reached a data line before the header line in " +
                        f" ParameterEvolutionFile {filepath}."
                        )

            # Create the new empty lists in the data array
            for _ in headers:
                data.append([])

            # Load the data
            cols = line.split()
            for i, col in enumerate(cols):
                data[i].append(float(col))

        param_ev = ParameterEvolutionFile()
        param_ev.set_filename(filepath)
        param_ev.set_type(type)
        param_ev.set_source(source)
        param_ev.set_reference(reference)
        param_ev.set_comment(comment)

        # Once the data is loaded, allocate the ParameterEvolution structs
        for i in range(len(headers)):
            param_ev.add_parameter_evolution(
                        headers[i].split(VAR_UNITS_SEPARATOR)[0],
                        headers[i].split(VAR_UNITS_SEPARATOR)[1],
                        data[i]
                        ) 

        logging.info("ParameterEvolutionFile loaded from PEV")

        return param_ev
    
    @classmethod
    def from_csv(cls, 
                 filepath: str, 
                 type: str | None = None,
                 source: str | None = None,
                 reference: str | None = None,
                 comment: str = "",
                 header_units: list[str] = [],
                 skip_headers:list[str] = [],
                 ) -> 'ParameterEvolutionFile':
        """
        Read a ParameterEvolutionFile from a CSV file
        
        :param filepath: The path of the CSV file to read from.
        :param type: The type of data within the file.
        :param source: The source of the data.
        :param reference: The reference system of the data, if applicable.
        :param comment: The comment to read from the file.
        :param header_units: The units of the headers. If not provided, the
                             default is UNITS_NO_UNIT.
        :param skip_headers: The headers to skip when reading the file.
        """

        logging.info(f"Reading ParameterEvolutionFile from {filepath}")
        logging.info(f"Skipping headers: {" ".join(skip_headers)}")

        if not os.path.isfile(filepath):
            msg = (f"Attempted to read a ParameterEvolutionFile from disk, " +
                  f"but the file {filepath} does not exist.")
            logging.error(msg)
            raise FileNotFoundError(msg)

        try:
            df = pd.read_csv(filepath)
        except Exception as err:
            logging.error(f"Failed to read CSV data")
            print(err)
            raise err
        
        out = ParameterEvolutionFile()
        out.set_filename(filepath)
        if comment:
            out.set_comment(comment)

        if type:
            out.set_type(type)
        else:
            out.set_type(NO_TYPE)

        if source:
            out.set_source(source)
        else:
            out.set_source(NO_SOURCE)

        if reference:
            out.set_reference(reference)
        else:
            out.set_reference(NO_TYPE)

        n_cols = len(df.columns)
        # Check that consistent number of headers/units are provided
        if (n_cols - len(skip_headers)) != len(header_units):
            msg = (f"Attempted to read a ParameterEvolutionFile from disk, " +
                   f"but the number of headers and units do not match.")
            logging.error(msg)
            raise ValueError(msg)

        # Correct the units to the correct string, if no units are present or
        # necessary for a param
        header_units = [
                header_units[i] if header_units[i] else UNITS_NO_UNIT
                for i in range(n_cols - len(skip_headers))
                ]

        # Get the list of headers that will be used
        headers = [
                col for col in df.columns
                if col not in skip_headers
                ]

        for i, col in enumerate(headers):
            if col in skip_headers:
                continue

            out.add_parameter_evolution(
                    name=col,
                    units=header_units[i],
                    values=df[col].tolist()
            )

        logging.info("ParameterEvolutionFile loaded from CSV")

        return out

    def _generate_file_string(self) -> str:
        """Generate the file raw string for this ParameterEvolutionFile object"""

        if self.n_data_points == 0:
            raise ValueError("Attempted to generate a file string from a ParameterEvolutionFile " +
                             "with no rows.")
        if self.filepath is None:
            raise ValueError("Attempted to generate a file string from a ParameterEvolutionFile " +
                             "with no filename.")
        if len(self.data) == 0:
            raise ValueError("Attempted to generate a file string from a ParameterEvolutionFile " +
                             "with no columns.")

        file_string = ""

        file_string += f"# TYPE: {self.type}\n" if self.type else f"# TYPE: {NO_TYPE}\n"
        file_string += f"# SOURCE: {self.source}\n" if self.source else f"# SOURCE: {NO_SOURCE}\n"
        file_string += f"# REFERENCE: {self.reference}\n" if self.reference else f"# REFERENCE: {NO_REFERENCE}\n"
        file_string += f"# COMMENT: {self.comment}\n" if self.comment else "# COMMENT: \n"

        file_string += "\n"

        for param_ev in self.data:
            file_string += f"{param_ev.header}{VAR_UNITS_SEPARATOR}{param_ev.units}".ljust(CHARS_PER_SEPARATOR)

        for row in range(self.n_data_points):
            file_string += "\n"
            for col in self.data:
                file_string += f"{col.values[row]:<23.8E}".ljust(CHARS_PER_SEPARATOR)

        return file_string
     

if __name__ == '__main__':
    eop_skip_headers = ["DATE", "DATA_TYPE"]
    eop_units = ["days", "arcsec", "arcsec", "s", "s", "arcsec", "arcsec",
                 "arcsec", "arcsec", "s"]
    # pef = ParameterEvolutionFile.from_csv(
    #         "/home/thomas-mcilwraith/Documents/eop_last_5_years.csv",
    #         "CELESTRAK_EOP_LAST_5_YEARS",
    #         header_units=eop_units,
    #         skip_headers=eop_skip_headers
    #         )
    # pef.set_filename("/home/thomas-mcilwraith/Documents/test1.pev")
    # pef.write()

    # pef = ParameterEvolutionFile.from_pev(r"C:\Users\tmcilwraith\Documents\github\psf_physlib\src\python\test1.pev")
    # pef.set_filename(r"C:\Users\tmcilwraith\Documents\github\psf_physlib\src\python\test2.pev")
    # pef.set_comment("")
    # pef.set_reference(None)
    # pef.set_source(None)
    # pef.set_type(None)
    # pef.write()

    # pef_from_pev = ParameterEvolutionFile.from_pev("/home/thomas-mcilwraith/Documents/eop_last_5_years.pev")
    # pef_from_pev.set_filename("/home/thomas-mcilwraith/Documents/test2.pev")
    # pef_from_pev.write()

    csv = r"C:\Users\tmcilwraith\Documents\github\psf_physlib\src\python\eop_last_5_years.csv"
    pef = ParameterEvolutionFile.from_csv(
        filepath=csv,
        type="EOP_LAST_5_YEARS",
        source="CELESTRAK",
        header_units=eop_units,
        skip_headers=eop_skip_headers
    )
    pef.set_filename(r"C:\Users\tmcilwraith\Documents\github\psf_physlib\src\python\test3.pev")
    pef.write()
    exit()
    
