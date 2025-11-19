"""
Bitwig Plugin Paths Configuration

Defines WHERE to generate protocol files for Bitwig plugin.
"""

PLUGIN_PATHS = {
    'plugin_name': 'bitwig',
    'plugin_display_name': 'Bitwig Studio',

    'output_cpp': {
        'base_path': 'src/protocol',
        'namespace': 'Protocol',
        'structs': 'struct/',
    },

    'output_java': {
        'base_path': 'host/src/protocol',
        'package': 'protocol',
        'structs': 'struct/',
    },
}
