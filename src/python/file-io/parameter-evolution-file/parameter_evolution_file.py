"""
Parameter evolution file.

This module provides a class for reading and writing parameter evolution files.
Author: Thomas McIlwraith
Date: 20/06/2026
"""

# Imports
import os
from dataclasses import dataclass

# Global variables
CHARS_PER_WORD = 10
CHARS_PER_SEPARATOR = 23
WORD_PRECISION = 8
VAR_UNITS_SEPARATOR = '~'

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
        'data',
        'n_data_points'
    ]

    def __init__(self):
        self.filepath = None
        self.comment = ""
        self.data = []
        self.n_data_points = 0
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

        with open(self.filepath, 'w') as f:
            f.write(self._generate_file_string())

        return None

    def read(self, filepath: str) -> None:
        """Read the file from disk"""

        if (
                self.filepath is not None or
                len(self.data) > 0 or
                self.n_data_points > 0
                ):
            raise ValueError("Attempted to read a ParameterEvolutionFile from " +
                             "disk into an object with existing data.")

        if not os.path.isfile(filepath):
            raise FileNotFoundError(f"Attempted to read a ParameterEvolutionFile from " +
                             f"disk, but the file {filepath} does not exist.")

        self.filepath = filepath
        with open(filepath, 'r') as f:
            lines = f.readlines()

        headers = []
        data = []
        for i, line in enumerate(lines):

            # Load comment
            if i == 0 and line[0] == '#':
                self.comment = line[2:].strip("\n")

            if line[0] == '#':
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

        # Once the data is loaded, allocate the ParameterEvolution structs
        for i in range(len(headers)):
            self.add_parameter_evolution(
                    headers[i].split(VAR_UNITS_SEPARATOR)[0],
                    headers[i].split(VAR_UNITS_SEPARATOR)[1],
                    data[i]
                    )

        return None

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
        file_string += f"# {self.comment}\n" if self.comment else ""

        for param_ev in self.data:
            file_string += f"{param_ev.header}{VAR_UNITS_SEPARATOR}{param_ev.units}".ljust(CHARS_PER_SEPARATOR)

        for row in range(self.n_data_points):
            file_string += "\n"
            for col in self.data:
                file_string += f"{col.values[row]:<23.8E}".ljust(CHARS_PER_SEPARATOR)

        return file_string
     

if __name__ == '__main__':
    pef = ParameterEvolutionFile()
    # pef.set_filename('test.pev')
    # pef.set_comment('This is a comment!')
    # pef.add_parameter_evolution('a', 'm', range(10))
    # pef.add_parameter_evolution('b', 'km', range(10))
    # pef.write()
    pef.read('test.pev')
    print(pef)
