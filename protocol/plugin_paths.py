"""
Bitwig Plugin Paths Configuration (Pure Python, Type-Safe)

Defines WHERE to generate protocol files for Bitwig plugin.
Replaces plugin_paths.yaml with type-safe Python configuration.
"""

from typing import TypedDict


class OutputCppPaths(TypedDict):
    """C++ output paths configuration"""
    base_path: str
    encoder: str
    messageid: str
    registry: str
    structs: str
    constants: str


class OutputJavaPaths(TypedDict):
    """Java output paths configuration"""
    base_path: str
    package: str
    encoder: str
    messageid: str
    registry: str
    structs: str
    constants: str


class PluginOptions(TypedDict):
    """Additional generation options"""
    cpp_namespace: str
    java_package: str
    generate_validation: bool
    generate_debug: bool


class PluginPathsConfig(TypedDict):
    """Complete plugin paths configuration"""
    plugin_name: str
    plugin_display_name: str
    output_cpp: OutputCppPaths
    output_java: OutputJavaPaths
    options: PluginOptions


# Bitwig plugin paths configuration
PLUGIN_PATHS: PluginPathsConfig = {
    'plugin_name': 'bitwig',
    'plugin_display_name': 'Bitwig Studio',

    'output_cpp': {
        'base_path': 'src/protocol',
        'encoder': 'src/protocol/',
        'messageid': 'src/protocol/',
        'registry': 'src/protocol/',
        'structs': 'src/protocol/struct/',
        'constants': 'src/protocol/',
    },

    'output_java': {
        'base_path': 'host/src/protocol',
        'package': 'protocol',
        'encoder': 'host/src/protocol/',
        'messageid': 'host/src/protocol/',
        'registry': 'host/src/protocol/',
        'structs': 'host/src/protocol/struct/',
        'constants': 'host/src/protocol/',
    },

    'options': {
        'cpp_namespace': 'Protocol',
        'java_package': 'protocol',
        'generate_validation': True,
        'generate_debug': True,
    },
}
