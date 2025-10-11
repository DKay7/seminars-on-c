#!/usr/bin/env python3

import argparse
import subprocess
import sys
import pathlib
import logging
import shutil


def run_cmd(cmd, cwd=None):
    try:
        proc = subprocess.run(
            cmd,
            cwd=cwd,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            check=False,
        )

        return proc.returncode, proc.stdout

    except FileNotFoundError:
        return 127, f"No such command: {cmd[0]}\n"


def have_tool(name: str) -> bool:
    return shutil.which(name) is not None


def install_deps_if_needed():

    if have_tool("clang-tidy") and have_tool("clang-format"):
        logging.debug("Both clang-tidy and clang-format are installed")
        return

    logging.info(f"Installing clang-tidy, clang-format")

    code, stdout = run_cmd(
        ["sudo", "apt-get", "install", "-y", "clang-format", "clang-tidy", "clang"],
    )

    if code != 0:
        logging.error(f"Error installing deps: {stdout}")
        exit(-1)

    logging.info("Installing complete")


def check_format(file_path: pathlib.Path) -> bool:
    logging.info(f"[clang-format] processing {file_path}")

    code, out = run_cmd(
        ["clang-format", "--dry-run", "--Werror", "-style=file", str(file_path)]
    )

    if code != 0:
        logging.info(f"[clang-format] check FAILED")
        logging.debug(f"clang-format retcode: {out}")

    return code == 0


def check_tidy(file_path: pathlib.Path) -> bool:
    logging.info(f"[clang-tidy] processing {file_path}")

    compiler_args = [
        "-std=c99",
        "-O2",
        "-Wall",
        "-Werror",
        "-Wformat-security",
        "-Wignored-qualifiers",
        "-Winit-self",
        "-Wswitch-default",
        "-Wfloat-equal",
        "-Wpointer-arith",
        "-Wtype-limits",
        "-Wempty-body",
        "-Wstrict-prototypes",
        "-Wmissing-field-initializers",
        "-Wnested-externs",
        "-Wno-pointer-sign",
        "-Wno-unused-result",
    ]
    cmd = [
        "clang-tidy",
        "--config-file=.clang-tidy",
        "-warnings-as-errors=*",
        str(file_path),
        "--",
    ]
    cmd.extend(compiler_args)

    code, out = run_cmd(cmd, cwd=".")
    logging.info(f"[clang-tidy output]:\n{out}")

    return code == 0


def configure_logging(verbose):
    level = None

    if verbose:
        level = logging.DEBUG
    else:
        level = logging.INFO

    logging.basicConfig(level=level, format="%(levelname)s: %(message)s")


def main() -> int:
    parser = argparse.ArgumentParser(
        description="C codestyle checker (clang-format + clang-tidy)"
    )
    parser.add_argument("file", type=pathlib.Path, help=".c file to check")
    parser.add_argument(
        "--verbose", "-v", default=False, action="store_true", help="verbose output"
    )

    args = parser.parse_args()

    configure_logging(args.verbose)
    install_deps_if_needed()

    file_path = args.file

    if not file_path.exists():
        logging.critical(f"File {file_path} was not found")
        return 1

    ok_format = check_format(file_path)
    ok_tidy = check_tidy(file_path)

    if ok_format and ok_tidy:
        logging.info("All checks were passed!\n")
        return 0

    logging.info("Some errors are above. Go fix your code (＾▽＾)")
    logging.info(
        f"Summry:\n\t"
        f"Formating: {"OK" if ok_format else "NOT OK, use clang-format"}\n\t"
        f"Code quality: {"OK" if ok_tidy else "NOT OK, see warnings above"}"
    )
    return 1


if __name__ == "__main__":
    sys.exit(main())
