#!/usr/bin/env python3

import argparse
import subprocess
import sys
import pathlib
import logging
import shutil
import platform


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

    system = platform.system()
    logging.info("Installing clang-tidy and clang-format...")

    if system == "Darwin":
        if not have_tool("brew"):
            logging.error(
                "Homebrew is not installed. Please install it first:\n"
            )
            sys.exit(1)

        code, stdout = run_cmd(["brew", "install", "llvm"])
        if code != 0:
            logging.error(f"Failed to install llvm via Homebrew: {stdout}")
            sys.exit(1)

        llvm_bin = None
        possible_paths = [
            "/opt/homebrew/opt/llvm/bin",
            "/usr/local/opt/llvm/bin",
        ]
        for p in possible_paths:
            if pathlib.Path(p).exists():
                llvm_bin = p
                break

        if llvm_bin and not have_tool("clang-tidy"):
            logging.warning(
                f"clang-tidy not found in PATH. You may need to add {llvm_bin} to your PATH:\n"
                f'  echo \'export PATH="{llvm_bin}:$PATH"\' >> ~/.zshrc  # or ~/.bashrc\n'
                f"  source ~/.zshrc"
            )
            sys.exit(1)

    elif system == "Linux":
        if have_tool("apt-get"):
            code, stdout = run_cmd(
                ["sudo", "apt-get", "update"]
            )
            if code != 0:
                logging.error(f"apt-get update failed: {stdout}")
                sys.exit(1)

            code, stdout = run_cmd(
                ["sudo", "apt-get", "install", "-y", "clang-format", "clang-tidy", "clang"]
            )
            if code != 0:
                logging.error(f"Error installing deps: {stdout}")
                sys.exit(1)
        else:
            logging.error("Unsupported Linux package manager. Please install clang-format and clang-tidy manually.")
            sys.exit(1)
    else:
        logging.error(f"Unsupported OS: {system}. Please install clang-format and clang-tidy manually.")
        sys.exit(1)

    logging.info("Installation complete")


def check_format(file_path: pathlib.Path) -> bool:
    logging.info(f"[clang-format] processing {file_path}")
    code, out = run_cmd(
        ["clang-format", "--dry-run", "--Werror", "-style=file", str(file_path)]
    )
    if code != 0:
        logging.info("[clang-format] check FAILED")
        logging.info(f"[clang-format] output:\n{out}")
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
    logging.info(f"[clang-tidy] output:\n{out}")

    if code != 0:
        logging.info("[clang-tidy] check FAILED")
    return code == 0

def configure_logging(verbose):
    level = logging.DEBUG if verbose else logging.INFO
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

    if not args.file.exists():
        logging.critical(f"File {args.file} was not found")
        return 1

    install_deps_if_needed()

    ok_format = check_format(args.file)
    ok_tidy = check_tidy(args.file)

    if ok_format and ok_tidy:
        logging.info("All checks passed!\n")
        return 0

    logging.info("Some errors are above. Go fix your code (＾▽＾)")
    logging.info(
        f"Summary:\n\t"
        f"Formatting: {'OK' if ok_format else 'NOT OK — run clang-format'}\n\t"
        f"Code quality: {'OK' if ok_tidy else 'NOT OK — see clang-tidy warnings'}"
    )
    return 1


if name == "__main__":
    sys.exit(main())