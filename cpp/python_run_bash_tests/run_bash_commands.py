#!/bin/python3

import sys
import subprocess
import codecs
from typing import Tuple


def execute_bash_cmd(bash_cmd: str) -> Tuple[str, str]:
    func_name = sys._getframe().f_code.co_name
    print(f"{func_name}: {bash_cmd=}")
    process = subprocess.Popen(bash_cmd.split(), stdout=subprocess.PIPE)
    output, error = process.communicate()
    if output is not None:
        output = output.decode("utf-8")
    if error is not None:
        error = str(error, 'utf-8')

    return output, error

def main():
    argc = len(sys.argv)
    if argc < 2:
        print("require bash argument")
        sys.exit(-1)


    bash_cmd = sys.argv[1]
    output, error = execute_bash_cmd(bash_cmd)
    print(f"output={output}")
    print(f"error={error}")


main()
