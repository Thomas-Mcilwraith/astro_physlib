"""
Application Manager

Author: Thomas McIlwraith
Date: 08/07/2026

Responsible for creating the working directory and scheduling the execution of
a workflow.

"""

# Imports
import os
import shutil
import logging
from uuid import UUID
from pathlib import Path

WORKFLOW_MOUNT_POINT = Path(os.getenv("WORKFLOW_MOUNT_POINT", ""))

# Global variables
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s %(levelname)s %(message)s",
    datefmt="%Y-%m-%dT%H:%M:%S",
)

class Manager:
    def __init__(self):
        logging.info("Application Manager initialised")
        # TODO: Connection to DB stuff

        if not WORKFLOW_MOUNT_POINT.exists():
            logging.error(f"Workflow mount point {WORKFLOW_MOUNT_POINT} does not exist")
            raise NotADirectoryError(f"Workflow mount point {WORKFLOW_MOUNT_POINT} does not exist")

        return None

    def initialise_working_area(self, workflow_uuid: UUID) -> None:
        logging.info(f"Initialising working area {workflow_uuid}")

        # Clear a working area if it already exists
        working_area_dir = WORKFLOW_MOUNT_POINT / str(workflow_uuid)
        if working_area_dir.exists():
            logging.warning(f"Working area {working_area_dir} already exists. It will be cleared.")
            shutil.rmtree(working_area_dir)

        # Create the working area paths
        working_area_inputs_dir = working_area_dir / "inputs"
        working_area_outputs_dir = working_area_dir / "outputs"
        working_area_files_dir = working_area_dir / "files"
        working_area_logs_dir = working_area_dir / "logs"

        # Create the working area directories
        working_area_dir.mkdir(parents=True)
        working_area_inputs_dir.mkdir(parents=True)
        working_area_outputs_dir.mkdir(parents=True)
        working_area_files_dir.mkdir(parents=True)
        working_area_logs_dir.mkdir(parents=True)

        # TODO: retrieve the input JSON files from the database
        # This will be a list of JSON objects
        # write them all to working_area_inputs_dir

        # TODO: retrieve the schedule JSON file from the database
        # This will be a JSON object
        # write it to working_area_inputs_dir

        return None

