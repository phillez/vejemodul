#ifndef FOOTSENSOR_SERVICE_H__
#define FOOTSENSOR_SERVICE_H__

#include <stdint.h>
#include "ble.h"
#include "ble_srv_common.h"

// Defining 16-bit service and 128-bit base UUIDs
#define BLE_UUID_FOOTSENSOR_BASE_UUID              {{0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}} // 128-bit base UUID
#define BLE_UUID_FOOTSENSOR_SERVICE_UUID                0xF00D // Just a random, but recognizable value

// Defining 4 16-bit characteristic UUIDs
#define BLE_UUID_FOOTSENSOR_FRONT_CHARACTERISTIC_UUID          0xBEEF // Just a random, but recognizable value
#define BLE_UUID_FOOTSENSOR_BACK_CHARACTERISTIC_UUID          0xCEEF // Just a random, but recognizable value
#define BLE_UUID_FOOTSENSOR_LEFT_CHARACTERISTIC_UUID          0xDEEF // Just a random, but recognizable value
#define BLE_UUID_FOOTSENSOR_RIGHT_CHARACTERISTIC_UUID          0xFEEF // Just a random, but recognizable value

// This structure contains various status information for our service. 
// The name is based on the naming convention used in Nordics SDKs. 
// 'ble' indicates that it is a Bluetooth Low Energy relevant structure and 
// 'fs' is short for Footsensor Service). 
typedef struct
{
    uint16_t                    conn_handle;    /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection).*/
    uint16_t                    service_handle; /**< Handle of Footsensor Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t    front_char_handles;	/**< Add handles for the characteristic attributes to our struct. */
    ble_gatts_char_handles_t    back_char_handles;	/**< Add handles for the characteristic attributes to our struct. */
    ble_gatts_char_handles_t    left_char_handles;	/**< Add handles for the characteristic attributes to our struct. */
    ble_gatts_char_handles_t    right_char_handles;	/**< Add handles for the characteristic attributes to our struct. */
}ble_fs_t;

/**@brief Function for handling BLE Stack events related to our service and characteristic.
 *
 * @details Handles all events from the BLE stack of interest to Our Service.
 *
 * @param[in]   p_footsensor_service       Footsensor Service structure.
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void ble_footsensor_service_on_ble_evt(ble_fs_t * p_footsensor_service, ble_evt_t * p_ble_evt);

/**@brief Function for initializing Footsensor Service.
 *
 * @param[in]   p_footsensor_service       Pointer to Footsensor Service structure.
 */
void footsensor_service_init(ble_fs_t * p_footsensor_service);

// Functions for updating characteristic values.
void footsensor_front_update(ble_fs_t *p_footsensor_service, uint8_t sensor_value);
void footsensor_back_update(ble_fs_t *p_footsensor_service, uint8_t sensor_value);
void footsensor_left_update(ble_fs_t *p_footsensor_service, uint8_t sensor_value);
void footsensor_right_update(ble_fs_t *p_footsensor_service, uint8_t sensor_value);

#endif  /* _ FOOTSENSOR_SERVICE_H__ */
