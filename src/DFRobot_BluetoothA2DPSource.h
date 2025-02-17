/*!
 * @file DFRobot_BluetoothA2DPSource.h
 * @brief Constructor for the Bluetooth microphone
 * @copyright Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [TangJie](jie.tang@dfrobot.com)
 * @version V1.0
 * @date 2022-2-18
 * @url https://github.com/DFRobot/DFRobot_MSM261
 */

#ifndef _DFROBOT_A2DP_H
#define _DFROBOT_A2DP_H

#include "Arduino.h"
#include "esp_log.h"

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_a2dp_api.h"
#include "esp_avrc_api.h"
#include "freertos/xtensa_api.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include <vector> 

#define BT_APP_CORE_TAG                   "BT_APP_CORE"

#define BT_APP_SIG_WORK_DISPATCH          (0x01)
#define APP_RC_CT_TL_GET_CAPS            (0)
#define APP_RC_CT_TL_RN_VOLUME_CHANGE    (1)

//#define ENABLE_DBG //!< Enable this macro to view detailed program execution logs
#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

class DFRobot_BluetoothA2DPSource
{
  
public:
  /**
   * @fn DFRobot_BluetoothA2DPSource
   * @brief Constructor for the Bluetooth microphone
   */
  DFRobot_BluetoothA2DPSource(void);

  /**
   * @fn begin
   * @brief Initializes Bluetooth and A2DP
   * @param name Name of the Bluetooth slave device to connect to
   * @param callback Callback function for A2DP data transmission
   * @param volume Volume level, range 0~100
   * @return Returns whether the initialization was successful
   * @retval true Initialization successful
   * @retval false Initialization failed
   */

  bool begin(const char* name, esp_a2d_source_data_cb_t callback,uint8_t volume);


private:
	esp_a2d_source_data_cb_t _callback;
	const char* _bleName;
  uint8_t _volume;
  uint8_t sA2DState;

  uint8_t sPeerBdname[ESP_BT_GAP_MAX_BDNAME_LEN + 1];
  esp_bd_addr_t sPeerBda = {0};
  esp_avrc_rn_evt_cap_mask_t s_avrc_peer_rn_cap;
  
  static void callBtAppGapCb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param);
  static void callBtAppRcCtCb(esp_avrc_ct_cb_event_t event, esp_avrc_ct_cb_param_t *param);
  static void callBtAppAvStateConnecting(esp_a2d_cb_event_t event, esp_a2d_cb_param_t *param);
  /**
   * @brief  bluetooth GAP callback function type
   * @param  event : Event type
   * @param  param : Pointer to callback parameter
   * @return None
   */
  void btAppGapCb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param);
  
  /**
   * @brief Bluetooth scanning
   * @param param A2DP state callback parameters
   * @return None
   */
  void filterInquiryScanResult(esp_bt_gap_cb_param_t *param);

  /**
   * @brief Retrieve Bluetooth name
   * @param eir Raw data of the Bluetooth name
   * @param bdname Bluetooth name
   * @param bdname_len Length of the Bluetooth name
   * @return true Success
   * @return false Failure
   */
  bool getNameFromEir(uint8_t *eir, uint8_t *bdname, uint8_t *bdname_len);

  /**
   * @brief Parse Bluetooth information
   * @param bda Bluetooth device address
   * @param str Parsed string
   * @param size Length of the data
   * @return char* Bluetooth address
   */
  char *bda2str(esp_bd_addr_t bda, char *str, size_t size);

  /**
   * @brief AVRC event callback function
   * @param event AVRC Controller callback events
   * @param param AVRC controller callback parameters
   * @return None
   */
  void btAppRcCtCb(esp_avrc_ct_cb_event_t event, esp_avrc_ct_cb_param_t *param);

  /**
   * @brief A2DP event callback function
   * @param event A2DP callback events
   * @param param A2DP state callback parameters
   * @return None
   */
  void btAppAvStateConnecting(esp_a2d_cb_event_t event, esp_a2d_cb_param_t *param);

  /**
   * @brief Send register notification command to AVRCP target
   * @return None
   */
  void btAvVolumeChanged(void);
};






#endif