#include <stdint.h>
#include <string.h>
#include "nrf_gpio.h"
#include "footsensor_service.h"
#include "ble_srv_common.h"
#include "app_error.h"

#define SENSOR_CHAR_SIZE	4		// Characteric size
#define INITIAL_VALUE_SENSOR	0		// Initial value of charateristics

// Declaration of a function that will take care of some housekeeping of ble connections related to footsensor service and characteristic
void ble_footsensor_service_on_ble_evt(ble_fs_t * p_footsensor_service, ble_evt_t * p_ble_evt)
{
    // Implement switch case handling BLE events related to footsensor service. 
    switch (p_ble_evt->header.evt_id)
    {
	case BLE_GAP_EVT_CONNECTED:
		p_footsensor_service->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
		break;
	case BLE_GAP_EVT_DISCONNECTED:
		p_footsensor_service->conn_handle = BLE_CONN_HANDLE_INVALID;
		break;
	default:
		// No implementation needed.
		break;
   }
}

/**@brief Function for adding front footsensor characterstic to "Footsensor service".
 *
 * @param[in]   p_footsensor_service        Footsensor Service structure.
 *
 */
static uint32_t footsensor_front_char_add(ble_fs_t * p_footsensor_service)
{
    // Add a custom characteristic UUID
    uint32_t            err_code;
    ble_uuid_t          char_uuid;
    ble_uuid128_t       base_uuid = BLE_UUID_FOOTSENSOR_BASE_UUID;
    char_uuid.uuid      = BLE_UUID_FOOTSENSOR_FRONT_CHARACTERISTIC_UUID;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    APP_ERROR_CHECK(err_code);
	
    // Add read properties to footsensor characteristic
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;

    // Configuring Client Characteristic Configuration Descriptor metadata and add to char_md structure
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc                = BLE_GATTS_VLOC_STACK;    
    char_md.p_cccd_md           = &cccd_md;
    char_md.char_props.notify   = 1;

    // Configure the attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.vloc        = BLE_GATTS_VLOC_STACK;
    
    // Set read/write security levels to footsensor characteristic
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    // Configure the characteristic value attribute
    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid      = &char_uuid;
    attr_char_value.p_attr_md   = &attr_md;
    
    // Set characteristic length in number of bytes
    attr_char_value.max_len     = SENSOR_CHAR_SIZE;
    attr_char_value.init_len    = SENSOR_CHAR_SIZE;
    attr_char_value.p_value     = INITIAL_VALUE_SENSOR;

    // Add footsensor characteristic to the service
    err_code = sd_ble_gatts_characteristic_add(p_footsensor_service->service_handle,
		    &char_md,
		    &attr_char_value,
		    &p_footsensor_service->front_char_handles);
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}

/**@brief Function for adding back footsensor characterstic to "Footsensor service".
 *
 * @param[in]   p_footsensor_service        Footsensor Service structure.
 *
 */
static uint32_t footsensor_back_char_add(ble_fs_t * p_footsensor_service)
{
    // Add a custom characteristic UUID
    uint32_t            err_code;
    ble_uuid_t          char_uuid;
    ble_uuid128_t       base_uuid = BLE_UUID_FOOTSENSOR_BASE_UUID;
    char_uuid.uuid      = BLE_UUID_FOOTSENSOR_BACK_CHARACTERISTIC_UUID;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    APP_ERROR_CHECK(err_code);
	
    // Add read properties to footsensor characteristic
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;
    
    // Configuring Client Characteristic Configuration Descriptor metadata and add to char_md structure
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc                = BLE_GATTS_VLOC_STACK;    
    char_md.p_cccd_md           = &cccd_md;
    char_md.char_props.notify   = 1;
    
    // Configure the attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.vloc        = BLE_GATTS_VLOC_STACK;
    
    // Set read/write security levels to footsensor characteristic
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm); 

    // Configure the characteristic value attribute
    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid      = &char_uuid;
    attr_char_value.p_attr_md   = &attr_md;
    
    // Set characteristic length in number of bytes
    attr_char_value.max_len     = SENSOR_CHAR_SIZE;
    attr_char_value.init_len    = SENSOR_CHAR_SIZE;
    attr_char_value.p_value     = INITIAL_VALUE_SENSOR;

    // Add footsensor characteristic to the service
    err_code = sd_ble_gatts_characteristic_add(p_footsensor_service->service_handle,
		    &char_md,
		    &attr_char_value,
		    &p_footsensor_service->back_char_handles);
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}

/**@brief Function for adding left footsensor characterstic to "Footsensor service".
 *
 * @param[in]   p_footsensor_service        Footsensor Service structure.
 *
 */
static uint32_t footsensor_left_char_add(ble_fs_t * p_footsensor_service)
{
    // Add a custom characteristic UUID
    uint32_t            err_code;
    ble_uuid_t          char_uuid;
    ble_uuid128_t       base_uuid = BLE_UUID_FOOTSENSOR_BASE_UUID;
    char_uuid.uuid      = BLE_UUID_FOOTSENSOR_LEFT_CHARACTERISTIC_UUID;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    APP_ERROR_CHECK(err_code);
	
    // Add read properties to footsensor characteristic
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;
    
    // Configuring Client Characteristic Configuration Descriptor metadata and add to char_md structure
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc                = BLE_GATTS_VLOC_STACK;    
    char_md.p_cccd_md           = &cccd_md;
    char_md.char_props.notify   = 1;
    
    // Configure the attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.vloc        = BLE_GATTS_VLOC_STACK;
    
    // Set read/write security levels to footsensor characteristic
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm); 

    // Configure the characteristic value attribute
    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid      = &char_uuid;
    attr_char_value.p_attr_md   = &attr_md;
    
    // Set characteristic length in number of bytes
    attr_char_value.max_len     = SENSOR_CHAR_SIZE;
    attr_char_value.init_len    = SENSOR_CHAR_SIZE;
    attr_char_value.p_value     = INITIAL_VALUE_SENSOR;

    // Add footsensor characteristic to the service
    err_code = sd_ble_gatts_characteristic_add(p_footsensor_service->service_handle,
		    &char_md,
		    &attr_char_value,
		    &p_footsensor_service->left_char_handles);
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}

/**@brief Function for adding right footsensor characterstic to "Footsensor service".
 *
 * @param[in]   p_footsensor_service        Footsensor Service structure.
 *
 */
static uint32_t footsensor_right_char_add(ble_fs_t * p_footsensor_service)
{
    // Add a custom characteristic UUID
    uint32_t            err_code;
    ble_uuid_t          char_uuid;
    ble_uuid128_t       base_uuid = BLE_UUID_FOOTSENSOR_BASE_UUID;
    char_uuid.uuid      = BLE_UUID_FOOTSENSOR_RIGHT_CHARACTERISTIC_UUID;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    APP_ERROR_CHECK(err_code);
	
    // Add read properties to footsensor characteristic
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;
    
    // Configuring Client Characteristic Configuration Descriptor metadata and add to char_md structure
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc                = BLE_GATTS_VLOC_STACK;    
    char_md.p_cccd_md           = &cccd_md;
    char_md.char_props.notify   = 1;
    
    // Configure the attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.vloc        = BLE_GATTS_VLOC_STACK;
    
    // Set read/write security levels to footsensor characteristic
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm); 

    // Configure the characteristic value attribute
    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid      = &char_uuid;
    attr_char_value.p_attr_md   = &attr_md;
    
    // Set characteristic length in number of bytes
    attr_char_value.max_len     = SENSOR_CHAR_SIZE;
    attr_char_value.init_len    = SENSOR_CHAR_SIZE;
    attr_char_value.p_value     = INITIAL_VALUE_SENSOR;

    // Add footsensor characteristic to the service
    err_code = sd_ble_gatts_characteristic_add(p_footsensor_service->service_handle,
		    &char_md,
		    &attr_char_value,
		    &p_footsensor_service->right_char_handles);
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}

/**@brief Function for initiating footsensor new service.
 *
 * @param[in]   p_footsensor_service        Footsensor Service structure.
 *
 */
void footsensor_service_init(ble_fs_t * p_footsensor_service)
{
    uint32_t   err_code; // Variable to hold return codes from library and softdevice functions

    // Declare 16-bit service and 128-bit base UUIDs and add them to the BLE stack
    ble_uuid_t        service_uuid;
    ble_uuid128_t     base_uuid = BLE_UUID_FOOTSENSOR_BASE_UUID;
    service_uuid.uuid = BLE_UUID_FOOTSENSOR_SERVICE_UUID;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
    APP_ERROR_CHECK(err_code);    
    
    // Set footsensor service connection handle to default value. I.e. an invalid handle since we are not yet in a connection.
    p_footsensor_service->conn_handle = BLE_CONN_HANDLE_INVALID;

    // Add footsensor service
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &service_uuid,
                                        &p_footsensor_service->service_handle);
    
    APP_ERROR_CHECK(err_code);
    
    // Call the functions to add the new characteristics to the service. 
    footsensor_front_char_add(p_footsensor_service);
    footsensor_back_char_add(p_footsensor_service);
    footsensor_left_char_add(p_footsensor_service);
    footsensor_right_char_add(p_footsensor_service);
}


// Function to be called when updating front characteristic value
void footsensor_front_update(ble_fs_t *p_footsensor_service, uint8_t sensor_value)
{
    // Update characteristic value
    if (p_footsensor_service->conn_handle != BLE_CONN_HANDLE_INVALID)
    {
	    uint16_t               len = SENSOR_CHAR_SIZE;
	    ble_gatts_hvx_params_t hvx_params;
	    memset(&hvx_params, 0, sizeof(hvx_params));

	    hvx_params.handle = p_footsensor_service->front_char_handles.value_handle;
	    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
	    hvx_params.offset = 0;
	    hvx_params.p_len  = &len;
	    hvx_params.p_data = &sensor_value;  

	    sd_ble_gatts_hvx(p_footsensor_service->conn_handle, &hvx_params);
    }
}

// Function to be called when updating back characteristic value
void footsensor_back_update(ble_fs_t *p_footsensor_service, uint8_t sensor_value)
{
	// Update characteristic value
	if (p_footsensor_service->conn_handle != BLE_CONN_HANDLE_INVALID)
	{
		uint16_t	len = SENSOR_CHAR_SIZE;
		ble_gatts_hvx_params_t hvx_params;
		memset(&hvx_params, 0, sizeof(hvx_params));

		hvx_params.handle = p_footsensor_service->back_char_handles.value_handle;
		hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
		hvx_params.offset = 0;
		hvx_params.p_len  = &len;
		hvx_params.p_data = &sensor_value;

		sd_ble_gatts_hvx(p_footsensor_service->conn_handle, &hvx_params);
	}
}

// Function to be called when updating left characteristic value
void footsensor_left_update(ble_fs_t *p_footsensor_service, uint8_t sensor_value)
{
	// Update characteristic value
	if (p_footsensor_service->conn_handle != BLE_CONN_HANDLE_INVALID)
	{
		uint16_t	len = SENSOR_CHAR_SIZE;
		ble_gatts_hvx_params_t hvx_params;
		memset(&hvx_params, 0, sizeof(hvx_params));

		hvx_params.handle = p_footsensor_service->left_char_handles.value_handle;
		hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
		hvx_params.offset = 0;
		hvx_params.p_len  = &len;
		hvx_params.p_data = &sensor_value;

		sd_ble_gatts_hvx(p_footsensor_service->conn_handle, &hvx_params);
	}
}

// Function to be called when updating right characteristic value
void footsensor_right_update(ble_fs_t *p_footsensor_service, uint8_t sensor_value)
{
	// Update characteristic value
	if (p_footsensor_service->conn_handle != BLE_CONN_HANDLE_INVALID)
	{
		uint16_t	len = SENSOR_CHAR_SIZE;
		ble_gatts_hvx_params_t hvx_params;
		memset(&hvx_params, 0, sizeof(hvx_params));

		hvx_params.handle = p_footsensor_service->right_char_handles.value_handle;
		hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
		hvx_params.offset = 0;
		hvx_params.p_len  = &len;
		hvx_params.p_data = &sensor_value;

		sd_ble_gatts_hvx(p_footsensor_service->conn_handle, &hvx_params);
	}
}
