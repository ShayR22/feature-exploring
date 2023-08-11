#!/bin/python3

import sys
import os
import re
from collections import Counter
from pathlib import Path
import logging

class RecursiveHeaderCount:
    def __init__(self):
        self.user_home_dir = os.path.expanduser("~")

    def calculate_num_lines_in_header(self, header: str) -> int:
        logging.info(f"calculate_num_lines_in_header: {header=}")
        counted_lines = 0
        total_headers = self.get_headers_recusrivly(header)
        logging.info(f"{total_headers=}")
        for header in total_headers:
            headers_lines = self._get_lines(header)
            counted_lines += headers_lines
            logging.info(f"{header=}: {headers_lines=} {counted_lines=}")

        return counted_lines

    def _collect_include_strings(self, header: str) -> list:
        # Read the content of the header file
        with open(header, 'r') as file:
            file_content = file.readlines()

        # Find and extract content inside angle brackets
        angle_bracket_pattern = re.compile(r'^\s*#include\s+<(.+?)>')
        angle_bracket_included_content = [angle_bracket_pattern.search(line).group(1) for line in file_content if angle_bracket_pattern.search(line)]

        # Find and extract content inside double quotes
        double_quote_pattern = re.compile(r'^\s*#include\s+"(.+?)"')
        double_quote_included_content = [double_quote_pattern.search(line).group(1) for line in file_content if double_quote_pattern.search(line)]


        return angle_bracket_included_content, double_quote_included_content

    def _find(self, name: str, path: str):
        for root, dirs, files in os.walk(path):
            if name in files:
                return os.path.join(root, name)

    def _convert_system_headers_to_full_paths(self, headers: list) -> list:
        full_paths = []
        for header in headers:
            system_basename_path = os.path.basename(header)

            default_system_path = "/usr/include"
            full_header_path = self._find(system_basename_path, default_system_path)
            logging.debug(f"_find {system_basename_path=} {default_system_path=} {full_header_path=}")

            # if not found under normal system search under user includes
            if full_header_path is None:
                logging.warning(f"Failed to find {system_basename_path} under {default_system_path=}")
            else:
                logging.debug(f"{header=} {full_header_path=}")
                full_paths.append(full_header_path)

        return full_paths

    def _convert_project_headers_to_full_paths(self, headers: list) -> list:
        full_paths = []
        for header in headers:
            header_basename_path = os.path.basename(header)

            default_project_path = "./"
            full_header_path = self._find(header_basename_path, default_project_path)
            logging.debug(f"_find {header_basename_path=} {default_project_path=} {full_header_path=}")

            # if not found under normal system search under user includes
            if full_header_path is None:
                user_include_path = self.user_home_dir + "/user.include"
                full_header_path = self._find(header_basename_path, user_include_path)
                logging.debug(f"_find {header_basename_path=} {user_include_path=} {full_header_path=}")
                if full_header_path is None:
                    logging.warning(f"Failed to find {header_basename_path=} under {user_include_path=} and {default_project_path=}")


            if full_header_path is not None:
                logging.debug(f"{header=} {full_header_path=}")
                full_paths.append(full_header_path)

        return full_paths


    def get_headers_recusrivly(self, header: str) -> list:
        visited = [header]
        visting = [header]

        while visting:
            visiting_header = visting.pop(0)
            logging.debug(f"{visiting_header=}")
            system_headers, project_headers = self._collect_include_strings(visiting_header)

            system_headers_full_path = self._convert_system_headers_to_full_paths(system_headers)
            project_headers_full_path = self._convert_project_headers_to_full_paths(project_headers)

            total_headers = system_headers_full_path + project_headers_full_path
            logging.debug(f"{project_headers_full_path=}")
            logging.debug(f"{system_headers_full_path=}")
            logging.debug(f"{total_headers=}")
            logging.debug("===========================================")

            for new_header in total_headers:
                if new_header not in visited:
                    visited.append(new_header)
                    visting.append(new_header)

        return set(visited)

    def _get_lines(self, header: str) -> int:
        num_lines = 0
        with open(header, "r") as f:
            num_lines = sum(1 for _ in f)
        return num_lines

if __name__ == "__main__":
    logging.getLogger().setLevel(logging.INFO)
    if len(sys.argv) < 2:
        logging.info("require start directory")
        sys.exit(1)


    if len(sys.argv) >= 3:
        logging.basicConfig(filename="output.log",
                            filemode='w',
                            format='%(asctime)s,%(msecs)d %(name)s %(levelname)s %(message)s',
                            datefmt='%H:%M:%S',
                            level=logging.DEBUG)

        logging.getLogger().setLevel(logging.DEBUG)

    header = sys.argv[1]
    rhc = RecursiveHeaderCount()
    num_lines = rhc.calculate_num_lines_in_header(header)
    logging.info("========================")
    logging.info(f"{header=} {num_lines=}")


