

#include "usb_names.h"

#define MIDI_NAME                                                \
    {'M', 'I', 'D', 'I', ' ', 'S', 't', 'u', 'd', 'i', 'o', ' ', \
     '[', 'b', 'i', 't', 'w', 'i', 'g', ':', 'd', 'e', 'v', ']'}
#define MIDI_NAME_LEN 24

struct usb_string_descriptor_struct usb_string_product_name = {2 + MIDI_NAME_LEN * 2, 3, MIDI_NAME};
