
#include "stm32f1xx_hal.h"
#include "stdint.h"        // For uint32_t, uint8_t
#include "string.h"        // For memcpy




// Define the struct to hold the parsed unique ID components
typedef struct {
    // UID[31:0]: X and Y coordinates on the wafer expressed in BCD format
    // This is the entire first 32-bit word (HAL_GetUIDw0())
    uint32_t wafer_xy_coordinates_bcd;
    char wafer_xy_coordinates[9];

    // UID[39:32]: WAF_NUM[7:0] Wafer number (8-bit unsigned number)
    // This is the least significant byte of the second 32-bit word (HAL_GetUIDw1())
    uint8_t  wafer_number;

    // UID[63:40]: LOT_NUM[23:0] Lot number (ASCII encoded, 3 bytes)
    // UID[95:64]: LOT_NUM[55:24] Lot number (ASCII encoded, 4 bytes)
    // Combined, this is 7 ASCII characters. We need 8 bytes for null termination.
    char     lot_number_ascii[8];

} STM32_UniqueId_Parsed;

// Helper function to convert a 2-digit BCD byte to decimal
uint8_t bcdToDec(uint8_t bcd) {
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}

// Function to populate the STM32_UniqueId_Parsed struct
void getParsedSTM32UniqueID(STM32_UniqueId_Parsed* id_struct)
{
    if (id_struct == NULL) {
        // Handle error: provided struct pointer is NULL
        return;
    }

    // Read the three raw 32-bit words
    uint32_t uid_w0 = HAL_GetUIDw0(); // Corresponds to UID[31:0]
    uint32_t uid_w1 = HAL_GetUIDw1(); // Corresponds to UID[63:32]
    uint32_t uid_w2 = HAL_GetUIDw2(); // Corresponds to UID[95:64]

    // Populate wafer_number (UID[39:32] is the least significant byte of uid_w1)
    id_struct->wafer_number = (uint8_t)(uid_w1 & 0xFF);

    // Populate lot_number_ascii (7 bytes: UID[63:40] from uid_w1, then UID[95:64] from uid_w2)
    // Lot_NUM[23:0] (3 bytes) comes from bits 40-63 (which are bits 8-31 of uid_w1)
    id_struct->lot_number_ascii[0] = (char)((uid_w1 >> 8) & 0xFF);
    id_struct->lot_number_ascii[1] = (char)((uid_w1 >> 16) & 0xFF);
    id_struct->lot_number_ascii[2] = (char)((uid_w1 >> 24) & 0xFF);

    // Lot_NUM[55:24] (4 bytes) comes from bits 64-95 (which are all bits of uid_w2)
    id_struct->lot_number_ascii[3] = (char)(uid_w2 & 0xFF);
    id_struct->lot_number_ascii[4] = (char)((uid_w2 >> 8) & 0xFF);
    id_struct->lot_number_ascii[5] = (char)((uid_w2 >> 16) & 0xFF);
    id_struct->lot_number_ascii[6] = (char)((uid_w2 >> 24) & 0xFF);

    // Null-terminate the lot_number_ascii string
    id_struct->lot_number_ascii[7] = '\0';

    // Populate wafer_xy_coordinates_bcd (UID[31:0])
    id_struct->wafer_xy_coordinates_bcd = uid_w0;

    // conver bcd to string ab store it inside struct

    // A 32-bit BCD value contains 8 decimal digits (4 bits per digit).

    // Iterate through the 8 nibbles (BCD digits) from left to right (most significant to least significant)
    // A uint32_t has 8 nibbles (32 / 4 = 8).
    // The loop runs from i=7 down to 0, corresponding to nibbles 7 (MSB) down to 0 (LSB).
    // The result is stored in decimalString[0] to decimalString[7].
    for (int i = 7; i >= 0; i--) {
        // Extract the current nibble
        // (bcd_value >> (i * 4)) shifts the desired nibble to the least significant position
        // & 0xF masks out only the lower 4 bits (the nibble)
        uint8_t nibble = (uid_w0 >> (i * 4)) & 0xF;

        // Convert the nibble (0-9) to its ASCII character '0'-'9'
        // Place it in the correct position in the string (from left to right)
        // Convert the nibble (0-15) to its ASCII hexadecimal character ('0'-'9', 'A'-'F')
        if (nibble < 10) {
            id_struct->wafer_xy_coordinates[7 - i] = nibble + '0'; // For 0-9
        } else {
            id_struct->wafer_xy_coordinates[7 - i] = nibble - 10 + 'A'; // For 10-15 (A-F)
        }
    }
    
    // Null-terminate the string
    id_struct->wafer_xy_coordinates[8] = '\0'; 
}