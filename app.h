/***************************************************************************//**
 * @file
 * @brief Application header file
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef APP_H
#define APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "src/adc.h"
#include "src/gpio.h"

#define TIMER_ID_FACTORY_RESET    77
#define TIMER_ID_FRIEND_FIND      20
#define DELAY_2S		(2 * 32768)
#define FLAME_KEY 0x4001

/*******************************************************************************
 * For indexing elements of the node
 ******************************************************************************/
uint16 element_index_global;
uint16 address_global;

/*******************************************************************************
 * Flag for indicating LETIMER event
 ******************************************************************************/
extern int period_expired;

/*******************************************************************************
 * Flag for indicating GPIO event
 ******************************************************************************/
extern int fire_detected;

/*******************************************************************************
 * Flag for indicating ADC transfer is complete
 ******************************************************************************/
extern bool ADC_flag;

struct switch_state
{
	uint8_t onoff_current;          /**< Current generic on/off value */
	uint8_t onoff_target; 	 		/**< Target generic on/off value */
}switch_state_obj;


/***********************************************************************************************//**
 * \defgroup app Application Code
 * \brief Sample Application Implementation
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

/*******************************************************************************
 * Initialize used bgapi classes for server.
 ******************************************************************************/
void gecko_bgapi_classes_init_server_friend(void);

/*******************************************************************************
 * Initialize used bgapi classes for server.
 ******************************************************************************/
void gecko_bgapi_classes_init_client_lpn(void);

/***************************************************************************//**
 * This function is used to save data in flash memory
 * @param[in] key    key
 * @param[in]  value_len   Array length
 * @param[in]  value_data   Value to store into the specified key
 ******************************************************************************/
void store_data_in_flash(uint16_t KEY , uint16_t Sensor_state);

/***************************************************************************//**
 * This function is used to store sensor data in flash
 * @param[in] KEY  key of the value to be retrieved
 ******************************************************************************/
uint16_t load_data_from_flash(uint16_t KEY);

/***************************************************************************//**
 * This function is called to Display Data from persistent memory to the LCD
 ******************************************************************************/
void display_flash_data();


/***************************************************************************//**
 * This function is called to initialize the node as an LPN and for establishing
 * friendship with friend node
 ******************************************************************************/
void LPN_Init(void);

/***************************************************************************//**
 * This function is called to initiate factory reset. Factory reset may be
 * initiated by keeping one of the pushbuttons pressed during reboot.
 * Factory reset is also performed if it is requested by the provisioner
 * (event gecko_evt_mesh_node_reset_id).
 ******************************************************************************/
void initiate_factory_reset(void);

/***************************************************************************//**
 * Set device name in the GATT database. A unique name is generated using
 * the two last bytes from the Bluetooth address of this device. Name is also
 * displayed on the LCD.
 *
 * @param[in] dev_addr  Pointer to Bluetooth address.
 ******************************************************************************/
void set_device_name(bd_addr *dev_addr);

/***************************************************************************//**
 * Switch node initialization.
 * This is called at each boot if provisioning is already done.
 * Otherwise this function is called after provisioning is completed.
 ******************************************************************************/
void switch_node_init(void);

/***************************************************************************//**
 * Handling of stack events. Both Bluetooth LE and Bluetooth mesh events
 * are handled here.
 * @param[in] evt_id  Incoming event ID.
 * @param[in] evt     Pointer to incoming event.
 ******************************************************************************/
void handle_ecen5823_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt);

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */

#ifdef __cplusplus
};
#endif

#endif /* APP_H */
