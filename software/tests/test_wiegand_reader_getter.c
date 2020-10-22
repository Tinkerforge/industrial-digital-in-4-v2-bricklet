#define IPCON_EXPOSE_MILLISLEEP

#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_industrial_digital_in_4_v2.h"

#define HOST "localhost"
#define PORT 4223
#define UID "HXg"

void interpret_wiegand26_data(bool *data, uint16_t data_length) {
    if (data_length != 26) {
        printf("Bad length: %u\n", data_length);
        return;
    }

    uint8_t pe = 0;
    for (int i = 0; i < 13; ++i) {
        pe = pe ^ (data[i] ? 1 : 0);
    }

    uint8_t po = 0;
    for (int i = 13; i < 26; ++i) {
        po = po ^ (data[i] ? 1 : 0);
    }

    if (pe != 0 || po != 1) {
        printf("Paritiy Error\n");
        return;
    }

    uint8_t fc = 0;
    for (int i = 1; i < 9; ++i) {
        fc = fc << 1;
        fc = fc | (data[i] ? 1 : 0);
    }

    uint16_t cc = 0;
    for (int i = 9; i < 25; ++i) {
        cc = cc << 1;
        cc = cc | (data[i] ? 1 : 0);
    }

    printf("Facility Code %u, Card Number %u\n", fc, cc);
}

void cb_wiegand_error_count(uint32_t framing_error_count, uint32_t overflow_error_count, void *user_data) {
    (void)user_data; // avoid unused parameter warning

    printf("Framing Error Count %u, Overflow Error Count %u\n", framing_error_count, overflow_error_count);
}

int main(void) {
    // Create IP connection
    IPConnection ipcon;
    ipcon_create(&ipcon);

    // Create device object
    IndustrialDigitalIn4V2 idi4;
    industrial_digital_in_4_v2_create(&idi4, UID, &ipcon);

    // Connect to brickd
    if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
        fprintf(stderr, "Could not connect\n");
        return 1;
    }
    // Don't use device before ipcon is connected

    industrial_digital_in_4_v2_register_callback(&idi4,
                                                 INDUSTRIAL_DIGITAL_IN_4_V2_CALLBACK_WIEGAND_ERROR_COUNT,
                                                 (void (*)(void))cb_wiegand_error_count,
                                                 NULL);

    industrial_digital_in_4_v2_set_wiegand_reader_config(&idi4, true, 26, 20); // 26 bit, with 50 ms timeout
    industrial_digital_in_4_v2_set_wiegand_callback_config(&idi4, false, false, true);

    while (true) {
        millisleep(100);

        bool data[26];
        uint16_t data_length;
        industrial_digital_in_4_v2_read_wiegand_data(&idi4, data, &data_length);

        if (data_length > 0) {
            interpret_wiegand26_data(data, data_length);
        }
    }

    printf("Press key to exit\n");
    getchar();
    industrial_digital_in_4_v2_destroy(&idi4);
    ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
    return 0;
}
