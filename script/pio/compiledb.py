# pyright: reportUndefinedVariable=false, reportMissingImports=false
"""
Generate compile_commands.json for clangd IDE integration.

This script imports the shared utility from core library.
Supports both symlink (development) and libdeps (release) configurations.
"""
import os
import sys
import glob

Import("env")

# Find core's script directory (symlink or libdeps)
project_dir = env.subst("$PROJECT_DIR")
candidates = [
    os.path.join(project_dir, "../core/script/pio"),  # Symlink: ../core
    *glob.glob(os.path.join(
        project_dir, ".pio/libdeps/*/petitechose-midi-studio-core/script/pio"
    )),
]

core_script_dir = next((p for p in candidates if os.path.exists(p)), None)
if not core_script_dir:
    print("[ERROR] Could not find core script directory")
    Exit(1)

# Import and use shared utility
sys.path.insert(0, core_script_dir)  # pyright: ignore[reportArgumentType]
from compiledb_utils import setup_compile_commands

setup_compile_commands(env)
