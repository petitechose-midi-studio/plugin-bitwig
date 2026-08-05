# pyright: reportUndefinedVariable=false, reportMissingImports=false
"""
Generate compile_commands.json for clangd IDE integration.

This script imports the shared utility from the Core workspace checkout.
"""
import os
import sys

Import("env")

# Core is a required sibling checkout for both dev and release profiles.
project_dir = env.subst("$PROJECT_DIR")
core_script_dir = os.path.join(project_dir, "../core/script/pio")
if not os.path.exists(core_script_dir):
    print("[ERROR] Could not find core script directory")
    Exit(1)

# Import and use shared utility
sys.path.insert(0, core_script_dir)  # pyright: ignore[reportArgumentType]
from compiledb_utils import setup_compile_commands

setup_compile_commands(env)
