
#include "usb_names.h"

// USB MIDI device name: "MIDI Studio [hw]"
// Appears in Bitwig/DAW port lists as both IN and OUT
#define MIDI_NAME                                                \
    {'M', 'I', 'D', 'I', ' ', 'S', 't', 'u', 'd', 'i', 'o', ' ', \
     '[', 'h', 'w', ']'}
#define MIDI_NAME_LEN 16

struct usb_string_descriptor_struct usb_string_product_name = {2 + MIDI_NAME_LEN * 2, 3, MIDI_NAME};
