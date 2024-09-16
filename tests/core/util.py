#
# Copyright (c) 2024 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
import sys
import platform
import subprocess

RETCODE_SUCCESS = 0
RETCODE_FAILURE = 1
RETCODE_ERROR = 2

PRINT_COLOR_NONE = ""

PRINT_COLOR_RED = "\x1b[31m"
PRINT_COLOR_GREEN = "\x1b[32m"
PRINT_COLOR_YELLOW = "\x1b[33m"
PRINT_COLOR_BLUE = "\x1b[34m"

PRINT_COLOR_RESET = "\x1b[0m"


def pr_warn(text: str) -> type(None):
    """
    Print the provided text - yellow color.
    """
    pr_color(text, PRINT_COLOR_YELLOW)


def pr_error(text: str) -> type(None):
    """
    Print the provided text - red color.
    """
    pr_color(text, PRINT_COLOR_RED)


def pr_info(text: str) -> type(None):
    """
    Print the provided text - green color.
    """
    pr_color(text, PRINT_COLOR_GREEN)


def pr_debug(text: str) -> type(None):
    """
    Print the provided text - no color.
    """
    pr_color(text, PRINT_COLOR_NONE)


def pr_color(text, color) -> type(None):
    """
    Print the provided text using the provided color.
    """
    sys.stdout.write(color + text + PRINT_COLOR_RESET + "\n")


def rsubprocess(cmd) -> tuple[str, str, int]:
    """
    Creates a process using the provided command and returns the output.
    Handles the shell for Windows.
    """
    shell = False
    if platform.system() == "Windows":
        shell = True
    result = subprocess.run(
        cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=shell
    )

    stdout = result.stdout.decode("utf-8")
    stderr = result.stderr.decode("utf-8")
    returncode = result.returncode

    if returncode != 0 or stderr:
        pr_warn("[-] failed to run subprocess : %d" % (returncode))
        pr_warn("[-] stderr : %s" % (stderr))

    if stderr:
        returncode = 1

    return stdout, stderr, returncode
