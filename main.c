#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  // Include this for uint8_t and uint16_t

uint8_t dataToTest[] = {0xFF, 0x03, 0x00, 0x09, 0x00, 0x08}; //:81 D0   //0x01, 0x03, 0x00, 0x06, 0x00, 0x01 : 64 0B
#define DATA_LENGTH (sizeof(dataToTest) / sizeof(dataToTest[0])) // Calculate the original data length

uint16_t crc16_modbus(const uint8_t *data, uint8_t length) {
    uint16_t crc = 0xFFFF; // Initialize CRC to all 1s
    for (uint8_t i = 0; i < length; i++) {
        crc ^= data[i]; // XOR the current byte into the CRC
        for (uint8_t j = 0; j < 8; j++) { // Process each bit
            if (crc & 0x0001) { // If the LSB is set
                crc = (crc >> 1) ^ 0xA001; // Shift right and XOR with polynomial
            } else {
                crc >>= 1; // Just shift right
            }
        }
    }
    return crc; // Return the final CRC value
}

int main() {
    uint16_t crc = crc16_modbus(dataToTest, DATA_LENGTH); // Calculate CRC

    // Create a new array to hold the original data plus CRC
    uint8_t completeData[DATA_LENGTH + 2]; // Allocate space for original data + 2 CRC bytes
    for (int i = 0; i < DATA_LENGTH; i++) {
        completeData[i] = dataToTest[i]; // Copy original data to the new array
    }

    // Insert the CRC into the new array
    completeData[DATA_LENGTH] = (crc & 0xFF);       // Low byte of CRC
    completeData[DATA_LENGTH + 1] = (crc >> 8);     // High byte of CRC

    // Print all data in the specified format
    printf("Data: ");
    for (int i = 0; i < sizeof(completeData); i++) {
        printf("0x%02X", completeData[i]);
        if (i < sizeof(completeData) - 1) {
            printf(", "); // Add a comma except for the last element
        }
    }
    printf("\n"); // New line after printing all data

    return 0; // Exit the program
}
