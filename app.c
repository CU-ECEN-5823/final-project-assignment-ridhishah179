/***************************************************************************//**
 * @file app.c
 * @brief Application code
 * @references: Silicon labs Bluetooth Mesh example/Demo - soc-btmesh-light & soc-btmesh-switch, AN1098.
 * 				parts of code leveraged from soc-btmesh-light & soc-btmesh-switch examples
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

/* C Standard Library headers */
#include <stdio.h>

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"
#include "mesh_generic_model_capi_types.h"
#include "mesh_lib.h"
#include "src/state_handler.h"
#include "src/ble_mesh_device_type.h"
#include "src/gecko_ble_errors.h"



/* Coex header */
#include "coexistence-ble.h"

/* Device initialization header */
#include "hal-config.h"

/* GPIO peripheral library */
#include <em_gpio.h>
#include "em_emu.h"
#include "em_cmu.h"

/* Own header */
#include "app.h"


/// Flag for indicating DFU Reset must be performed
static uint8_t boot_to_dfu = 0;

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


void gecko_bgapi_classes_init_server_friend(void)
{
	gecko_bgapi_class_dfu_init();
		gecko_bgapi_class_system_init();
		gecko_bgapi_class_le_gap_init();
		gecko_bgapi_class_le_connection_init();
		gecko_bgapi_class_gatt_server_init();
		gecko_bgapi_class_hardware_init();
		gecko_bgapi_class_flash_init();
		gecko_bgapi_class_test_init();
		gecko_bgapi_class_mesh_node_init();
		gecko_bgapi_class_mesh_proxy_init();
		gecko_bgapi_class_mesh_proxy_server_init();
		gecko_bgapi_class_mesh_generic_server_init();
		gecko_bgapi_class_mesh_lpn_init();
}


/*******************************************************************************
 * Initialize used bgapi classes for server.
 ******************************************************************************/
void gecko_bgapi_classes_init_client_lpn(void)
{
	gecko_bgapi_class_dfu_init();
		gecko_bgapi_class_system_init();
		gecko_bgapi_class_le_gap_init();
		gecko_bgapi_class_le_connection_init();
		gecko_bgapi_class_gatt_server_init();
		gecko_bgapi_class_hardware_init();
		gecko_bgapi_class_flash_init();
		gecko_bgapi_class_test_init();
		gecko_bgapi_class_mesh_node_init();
		gecko_bgapi_class_mesh_proxy_init();
		gecko_bgapi_class_mesh_proxy_server_init();
		gecko_bgapi_class_mesh_generic_client_init();
		gecko_bgapi_class_mesh_lpn_init();
}

void store_data_in_flash(uint16_t KEY , uint16_t Sensor_state)
{
	int ret;
	uint8_t * persistent_data;
	persistent_data =&Sensor_state;
	ret=gecko_cmd_flash_ps_save(KEY, sizeof(Sensor_state),persistent_data)->result;
	LOG_INFO("%s in storing persistent data with return value  %d)",ret ? "Failed" : "Successful",ret);
}


uint16_t load_data_from_flash(uint16_t KEY)
{
	uint16_t persistent_data;
	struct gecko_msg_flash_ps_load_rsp_t* ret;
	ret=(gecko_cmd_flash_ps_load(KEY));
	memcpy(&persistent_data,&ret->value.data,ret->value.len);
	LOG_INFO("Persistent data is %d",persistent_data);
	return persistent_data;
}


/**
 * @brief : displaying the persistent data on LCD
 */
void display_flash_data()
{
	uint16_t persistent_data;
	persistent_data=load_data_from_flash(FLAME_KEY);
	displayPrintf(DISPLAY_ROW_ACTION+1,"Sensor state :%d",persistent_data);
	LOG_INFO("FLAME DATA %d",persistent_data);
}

void LPN_Init(void)
{
	uint16 result = 0;

	result = gecko_cmd_mesh_lpn_init()->result;
	if (result)
	{
		LOG_INFO("\nLPN init failed 0x%x\n", result);
		return;
	}

	// Configure with Minimum friend queue length = 2, Poll timeout = 1 seconds
	 result = gecko_cmd_mesh_lpn_config(mesh_lpn_queue_length, 2)->result;
	  if (result) {
	    LOG_INFO("LPN queue configuration failed (0x%x)\r\n", result);
	    return;
	  }
	  // Configure LPN Poll timeout = 5 seconds
	  result = gecko_cmd_mesh_lpn_config(mesh_lpn_poll_timeout,5000)->result;
	  if (result) {
	    LOG_INFO("LPN Poll timeout configuration failed (0x%x)\r\n", result);
	    return;
	  }

	result = gecko_cmd_mesh_lpn_establish_friendship(0)->result;
	if (result != 0)
	{
		LOG_INFO("Return Code %x", result);
	}
}
/***************************************************************************//**
 * This function is called to initiate factory reset. Factory reset may be
 * initiated by keeping one of the pushbuttons pressed during reboot.
 * Factory reset is also performed if it is requested by the provisioner
 * (event gecko_evt_mesh_node_reset_id).
 ******************************************************************************/
void initiate_factory_reset(void)
{
	LOG_INFO("\nAt Factory Reset\n");

	gecko_cmd_flash_ps_erase_all();
	// reboot after a small delay
	BTSTACK_CHECK_RESPONSE(gecko_cmd_hardware_set_soft_timer(DELAY_2S, TIMER_ID_FACTORY_RESET, true));
}

/***************************************************************************//**
 * Set device name in the GATT database. A unique name is generated using
 * the two last bytes from the Bluetooth address of this device. Name is also
 * displayed on the LCD.
 *
 * @param[in] dev_addr  Pointer to Bluetooth address.
 ******************************************************************************/
void set_device_name(bd_addr *dev_addr)
{
	char dev_name[10];

	if(DeviceIsOnOffPublisher())
		sprintf(dev_name, "5823Pub%02x%02x", dev_addr->addr[1], dev_addr->addr[0]);

	else
		sprintf(dev_name, "5823Sub%02x%02x", dev_addr->addr[1],dev_addr->addr[0]);

	BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_server_write_attribute_value(gattdb_device_name, 0, strlen(dev_name), (uint8_t*)dev_addr));
}

/*******************************************************************************
 * This function is a handler for Switch generic on/off change event.
 *
 * @param[in] model_id       Server model ID.
 * @param[in] element_index  Server model element index.
 * @param[in] current        Pointer to current state structure.
 * @param[in] target         Pointer to target state structure.
 * @param[in] remaining_ms   Time (in milliseconds) remaining before transition
 *                           from current state to target state is complete.
 ******************************************************************************/
static void switch_onoff_change(uint16_t model_id,
                         uint16_t element_index,
                         const struct mesh_generic_state *current,
                         const struct mesh_generic_state *target,
                         uint32_t remaining_ms)
{
	if (current->on_off.on != switch_state_obj.onoff_current)
	{
		LOG_INFO("\n Switch On-Off state changed %u to %u\n", switch_state_obj.onoff_current, current->on_off.on);
		switch_state_obj.onoff_current = current->on_off.on;
	}
	else
	{
		LOG_INFO("\nDummy onoff change - same state as before\n");
	}
}

/***************************************************************************//**
 * Update Switch generic on/off state and publish model state to the network.
 *
 * @param[in] element_index  Server model element index.
 * @param[in] remaining_ms   The remaining time in milliseconds.
 *
 * @return Status of the update and publish operation.
 *         Returns bg_err_success (0) if succeed, non-zero otherwise.
 ******************************************************************************/
static errorcode_t switch_onoff_publish(uint16_t element_index,
                                            uint32_t remaining_ms)
{
  errorcode_t ret;
  struct mesh_generic_state current, target;

	current.kind = mesh_generic_state_on_off;
	current.on_off.on = switch_state_obj.onoff_current;

	target.kind = mesh_generic_state_on_off;
	target.on_off.on = switch_state_obj.onoff_target;

	ret = mesh_lib_generic_server_update(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID,
		                                        element_index,
		                                        &current,
		                                        &target,
		                                        remaining_ms);

  if (ret == bg_err_success) {
    ret = mesh_lib_generic_server_publish(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID,
                                        element_index,
                                        mesh_generic_state_on_off);
  }

  return ret;
}

/*******************************************************************************
 * This function process the requests for the Switch generic on/off model.
 *
 * @param[in] model_id       Server model ID.
 * @param[in] element_index  Server model element index.
 * @param[in] client_addr    Address of the client model which sent the message.
 * @param[in] server_addr    Address the message was sent to.
 * @param[in] appkey_index   The application key index used in encrypting the request.
 * @param[in] request        Pointer to the request structure.
 * @param[in] transition_ms  Requested transition time (in milliseconds).
 * @param[in] delay_ms       Delay time (in milliseconds).
 * @param[in] request_flags  Message flags. Bitmask of the following:
 *                           - Bit 0: Nonrelayed. If nonzero indicates
 *                                    a response to a nonrelayed request.
 *                           - Bit 1: Response required. If nonzero client
 *                                    expects a response from the server.
 ******************************************************************************/
static void switch_onoff_request(uint16_t model_id,
                          uint16_t element_index,
                          uint16_t client_addr,
                          uint16_t server_addr,
                          uint16_t appkey_index,
                          const struct mesh_generic_request *request,
                          uint32_t transition_ms,
                          uint16_t delay_ms,
                          uint8_t request_flags)
{
	if (switch_state_obj.onoff_current == request->on_off)
	{
		LOG_INFO("\nRequest for current state received; no op\n");
	}
	else
	{
		if(request->on_off)
			displayPrintf(DISPLAY_ROW_CLIENTADDR,"Button Pressed");
		else
			displayPrintf(DISPLAY_ROW_CLIENTADDR,"Button Released");

		if (transition_ms == 0 && delay_ms == 0)
		{
			switch_state_obj.onoff_current = request->on_off;
			switch_state_obj.onoff_target = request->on_off;
		}
	}

	switch_onoff_publish(element_index, 0);
}

/***************************************************************************//**
 * Switch node initialization.
 * This is called at each boot if provisioning is already done.
 * Otherwise this function is called after provisioning is completed.
 ******************************************************************************/
void switch_node_init(void)
{
	uint16_t element_index = 0;

	if(DeviceIsOnOffPublisher())
		mesh_lib_init(malloc, free, 8);

	else
	{
		mesh_lib_init(malloc, free, 9);

		mesh_lib_generic_server_register_handler(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID,
														   0,
														   switch_onoff_request,
														   switch_onoff_change,NULL);
		switch_onoff_publish(element_index, 0);
	}

}

/*******************************************************************************
 * Handling of stack events. Both Bluetooth LE and Bluetooth mesh events
 * are handled here.
 * @param[in] evt_id  Incoming event ID.
 * @param[in] evt     Pointer to incoming event.
 ******************************************************************************/
void handle_ecen5823_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt)
{

  struct gecko_msg_system_get_bt_address_rsp_t *dev_addr;
  struct gecko_msg_mesh_node_initialized_evt_t *pData = (struct gecko_msg_mesh_node_initialized_evt_t *)&(evt->data);
  uint16_t result = 0;

  if (NULL == evt) {
    return;
  }

  switch (evt_id)
  {
  	  case gecko_evt_system_boot_id:
  		  element_index_global = 01;
  		  address_global = 0;
  		  switch_state_obj.onoff_current = 0;
  		  switch_state_obj.onoff_target = 0;

  		  if(DeviceIsOnOffPublisher())
  			  displayPrintf(DISPLAY_ROW_NAME, "Publisher");
		  else
			  displayPrintf(DISPLAY_ROW_NAME, "Subscriber");

    	  dev_addr = gecko_cmd_system_get_bt_address();
    	  displayPrintf(DISPLAY_ROW_BTADDR,"%x:%x:%x:%x:%x:%x",dev_addr->address.addr[5],
    			  dev_addr->address.addr[4],dev_addr->address.addr[3],dev_addr->address.addr[2],
    			 dev_addr->address.addr[1],dev_addr->address.addr[0]);

    	 /*Factory reset System if switch is pressed*/
    	 if((GPIO_PinInGet(gpioPortF, 6) == 0))
    		 {
    		 	 displayPrintf(DISPLAY_ROW_ACTION,"FACTORY RESET");
    			 initiate_factory_reset();
   			 }
    	 else
    		 {
    			 displayPrintf(DISPLAY_ROW_ACTION, "");
    			 set_device_name(&dev_addr->address);
    			 gecko_cmd_mesh_node_init();
    		 }

         break;

        /*Software timer for factory reset 2sec*/
  	  	case gecko_evt_hardware_soft_timer_id:
    		switch(evt->data.evt_hardware_soft_timer.handle)
    		{
    		case TIMER_ID_FACTORY_RESET:
  	  					LOG_INFO("\nAt Soft Timer - Timer Factory Reset\n");
  	  					gecko_cmd_system_reset(0);
  	  					break;
    		case TIMER_ID_FRIEND_FIND:
  	  					result = gecko_cmd_mesh_lpn_establish_friendship(0)->result;
  	  					if (result != 0)
  	  					{
  	  						LOG_INFO("Return Code %x", result);
  	  					}
  	  					break;

    		}
    		break;

    /*Handle events for mesh node initialization*/
  	  case gecko_evt_mesh_node_initialized_id:

  		  if (pData->provisioned && DeviceIsOnOffPublisher())
  		  {
  			  address_global = pData->address;
  			  element_index_global = 0;
  			  displayPrintf(DISPLAY_ROW_ACTION, "Provisioned");
  			  gecko_cmd_mesh_generic_client_init();
  			  LPN_Init();
  			  switch_node_init();
  			  display_flash_data();


  		  }

  	       else if(pData->provisioned && DeviceIsOnOffSubscriber())
  	       {
  	       	  address_global= pData->address;
  	       	  element_index_global = 0;
  	       	  displayPrintf(DISPLAY_ROW_ACTION, "Provisioned");
  	       	  gecko_cmd_mesh_generic_server_init();
  	          LPN_Init();
  	       	  switch_node_init();
  	          display_flash_data();

  	      	}


  	         else if(!evt->data.evt_mesh_node_initialized.provisioned)
  	         {
  	          	gecko_cmd_mesh_node_start_unprov_beaconing(0x3);   // enable ADV and GATT provisioning bearer
  	            display_flash_data();
  	         }

  	         break;


        case gecko_evt_mesh_node_provisioning_started_id:
        	LOG_INFO("\nProvisioning\n");
        	displayPrintf(DISPLAY_ROW_ACTION, "Provisioning");
        	break;

        /*mesh node provisioned*/
        case gecko_evt_mesh_node_provisioned_id:
        	element_index_global = 0;
       		if(DeviceIsOnOffPublisher())
   				gecko_cmd_mesh_generic_client_init();

       		else
 				gecko_cmd_mesh_generic_server_init();

       		switch_node_init();
       		LPN_Init();
        	displayPrintf(DISPLAY_ROW_ACTION, "Provisioned");
   			break;

        /*provisioning failed*/
        case gecko_evt_mesh_node_provisioning_failed_id:
        	LOG_INFO("\nFailed Provisioning with error  %x\n", evt->data.evt_mesh_node_provisioning_failed.result);
       		displayPrintf(DISPLAY_ROW_ACTION, "Provisioning Fail");
   			break;

        case gecko_evt_mesh_generic_server_client_request_id:
        	if(DeviceUsesServerModel())
        	{
        		mesh_lib_generic_server_event_handler(evt);
        	}
   			break;

        case gecko_evt_mesh_generic_server_state_changed_id:
        	if(DeviceUsesServerModel())
        	{
        		mesh_lib_generic_server_event_handler(evt);
        	}
        break;

        /*connection opened*/
        case gecko_evt_le_connection_opened_id:
        	BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_lpn_deinit());
        	LOG_INFO("\nConnection Opened\n");
        	displayPrintf(DISPLAY_ROW_CONNECTION, "Connected");
        break;

        /*connection closed*/
        case gecko_evt_le_connection_closed_id:
          /* Check if need to boot to dfu mode */
          if (boot_to_dfu) {
            /* Enter to DFU OTA mode */
            gecko_cmd_system_reset(2);
          }
          else
          {
        	  LPN_Init();
          }
          break;

         /*mesh node reset*/
        case gecko_evt_mesh_node_reset_id:
        	LOG_INFO("\nReset\n");
        	initiate_factory_reset();
        break;

        case gecko_evt_mesh_lpn_friendship_established_id:
        	displayPrintf(DISPLAY_ROW_BTADDR+1, "Friend Connected");
        	LOG_INFO("\nFriendship Established\n");
        	break;

        case gecko_evt_mesh_lpn_friendship_failed_id:
       		displayPrintf(DISPLAY_ROW_BTADDR+1, "");
        	LOG_INFO("\nFailed to establish Friendship\n");
        	gecko_cmd_hardware_set_soft_timer(DELAY_2S, TIMER_ID_FRIEND_FIND, true);
        	break;

        case gecko_evt_mesh_lpn_friendship_terminated_id:
        	displayPrintf(DISPLAY_ROW_BTADDR+1, "");
        	LOG_INFO("\nFriendship Terminated\n");
        	gecko_cmd_hardware_set_soft_timer(DELAY_2S, TIMER_ID_FRIEND_FIND, true);
        	break;

        /*Handle external signals from LETIMER and GPIO Interrupts */
        case gecko_evt_system_external_signal_id:

        	if(evt->data.evt_system_external_signal.extsignals & period_expired)
        		{

        			uint8_t transaction_id = 0;
        			struct mesh_generic_state req;
        			uint16 ret;
        			uint16_t fire_value_millivolts = 0;
        			period_expired = 0;
        			ADC_flag = 0;
        			fire_value_millivolts = get_adc_data();
        			LOG_INFO("\n Sensor data= %d",fire_value_millivolts);

        			req.kind= mesh_generic_state_level;
        			req.level.level=fire_value_millivolts;

        				ret = mesh_lib_generic_client_publish(
        							MESH_GENERIC_LEVEL_CLIENT_MODEL_ID,
        							element_index_global,
        							transaction_id,
        							&req,
        							0,
        							0,
        							0
        					);
        					transaction_id++;
        					if (ret) {
        						printf("gecko_cmd_mesh_generic_client_publish failed,code %x\r\n", ret);
        					} else {
        						printf("request sent, trid = %u, delay = %d\r\n", transaction_id, 0);
        					}

          		}

        	if(evt->data.evt_system_external_signal.extsignals & fire_detected)
        		{
        			uint16_t switch_val = 0;
        			uint16_t  ret = 0;
        			static uint8_t txid= 0;
        			struct mesh_generic_request Request;

        			fire_detected = 0;
        			if(DeviceUsesClientModel())
        				{
        					switch_val = GPIO_PinInGet(gpioPortD, 12);

        					if(switch_val)
        					{
        						displayPrintf(DISPLAY_ROW_CLIENTADDR,"FIRE DETECTED");
        						store_data_in_flash(FLAME_KEY,switch_val);
        					}
        					else
        					{
        						displayPrintf(DISPLAY_ROW_CLIENTADDR,"  ");
        						store_data_in_flash(FLAME_KEY,switch_val);
        					}

        					Request.kind = mesh_generic_request_on_off;

        					if(switch_val)
        						Request.on_off = MESH_GENERIC_ON_OFF_STATE_OFF;

							else
								Request.on_off = MESH_GENERIC_ON_OFF_STATE_ON;

        					ret = mesh_lib_generic_client_publish(MESH_GENERIC_ON_OFF_CLIENT_MODEL_ID, element_index_global, txid, &Request, 0,0,0);

        					if(ret)
        					{
        						LOG_INFO("\nSend Failed\n");
        					}
        					else
        					{
        						LOG_INFO("\nvalue sent successfully\n");
        					}

        					txid++;
        				}
        			}
        break;



        case gecko_evt_gatt_server_user_write_request_id:
          if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control) {
            /* Set flag to enter to OTA mode */
            boot_to_dfu = 1;
            /* Send response to Write Request */
            gecko_cmd_gatt_server_send_user_write_response(
              evt->data.evt_gatt_server_user_write_request.connection,
              gattdb_ota_control,
              bg_err_success);

            /* Close connection to enter to DFU OTA mode */
            gecko_cmd_le_connection_close(evt->data.evt_gatt_server_user_write_request.connection);
          }
          break;

      }
    }


    /** @} (end addtogroup app) */
    /** @} (end addtogroup Application) */
