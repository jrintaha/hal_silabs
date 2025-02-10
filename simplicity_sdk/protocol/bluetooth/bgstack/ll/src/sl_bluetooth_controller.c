/***************************************************************************//**
 * @file
 * @brief Functions used by the BGAPI protocol to output BGAPI trace over RTT
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc.  Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement.  This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include <stdbool.h>
#include <sl_common.h>
#include "sl_rail_util_pa_config.h"
#include "sl_btctrl_linklayer.h"
#include "sl_btctrl_config.h"
#include "sl_bt_ll_config.h"

#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#endif // SL_COMPONENT_CATALOG_PRESENT

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_POWER_CONTROL_PRESENT)
#include "sl_bt_power_control_config.h"
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_PRESENT)
#include "sl_bluetooth_connection_config.h"
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_ADVERTISER_PRESENT)
#include "sl_bluetooth_advertiser_config.h"
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PHY_SUPPORT_CONFIG_PRESENT)
#include "sl_btctrl_phy_support_config.h"
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_RESOLVING_LIST_PRESENT)
#include "sl_bt_resolving_list_config.h"
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_ACCEPT_LIST_PRESENT)
#include "sl_bt_accept_list_config.h"
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CS_PRESENT) || defined(SL_CATALOG_BLUETOOTH_FEATURE_CS_TEST_PRESENT)
#include "sl_bluetooth_cs_config.h"
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_HCI_DEBUG_PRESENT)
#include "sl_btctrl_hci.h"
#endif // SL_CATALOG_BLUETOOTH_FEATURE_HCI_DEBUG_PRESENT

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_HCI_TEST_COMMANDS_PRESENT)
#include "sl_btctrl_hci.h"
#endif // SL_CATALOG_BLUETOOTH_FEATURE_HCI_TEST_COMMANDS_PRESENT

#if defined(SL_CATALOG_RAIL_UTIL_COEX_PRESENT)
#include "coexistence-ble.h"
#endif

#if !defined(SL_CATALOG_BLUETOOTH_CONTROLLER_ONLY_PRESENT)
#include "sl_bluetooth.h"
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PERIODIC_ADVERTISER_PRESENT)
#include "sl_bt_periodic_advertiser_config.h"
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PAWR_ADVERTISER_PRESENT)
#include "sl_bt_pawr_advertiser_config.h"
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SYNC_PRESENT)
#include "sl_bluetooth_periodic_sync_config.h"
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PAWR_SYNC_PRESENT)
#include "sl_bt_pawr_sync_config.h"
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_ANALYZER_PRESENT)
#include "sl_bluetooth_connection_analyzer_config.h"
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CIS_PRESENT)
#include "sl_btctrl_iso.h"
#endif

#ifndef SL_BT_CONFIG_MAX_CONNECTIONS
#define SL_BT_CONFIG_MAX_CONNECTIONS 0
#endif
#ifndef SL_BT_CONFIG_USER_ADVERTISERS
#define SL_BT_CONFIG_USER_ADVERTISERS 0
#endif
#ifndef SL_BT_CONFIG_ACCEPT_LIST_SIZE
#define SL_BT_CONFIG_ACCEPT_LIST_SIZE 0
#endif
#ifndef SL_BT_ACTIVATE_POWER_CONTROL
#define SL_BT_ACTIVATE_POWER_CONTROL 0
#endif
#ifndef SL_BT_CONFIG_RESOLVING_LIST_SIZE
#define SL_BT_CONFIG_RESOLVING_LIST_SIZE 0
#endif
#ifndef SL_BT_COMPONENT_ADVERTISERS
#define SL_BT_COMPONENT_ADVERTISERS 0
#endif
#ifndef SL_BT_COMPONENT_CONNECTIONS
#define SL_BT_COMPONENT_CONNECTIONS 0
#endif

extern void test_protocol_init();

SL_WEAK void sl_bluetooth_controller_debug_init()
{
}

sl_status_t sl_bt_controller_init(void)
{
  sl_status_t status = SL_STATUS_OK;
#if defined(SL_CATALOG_BLUETOOTH_CONTROLLER_ONLY_PRESENT)
  if (sl_btctrl_is_initialized() == true) {
    return SL_STATUS_OK;
  } else {
    // Init the BTLE LL struct for an RCP app
    sl_btctrl_init_ll_config(SL_BT_CONTROLLER_BUFFER_MEMORY);
    sl_btctrl_configure_le_buffer_size(SL_BT_CONTROLLER_LE_BUFFER_SIZE_MAX);
  }
#endif // SL_CATALOG_BLUETOOTH_CONTROLLER_ONLY_PRESENT

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PHY_SUPPORT_CONFIG_PRESENT)
#if SL_BT_CONTROLLER_2M_PHY_SUPPORT == 0
  sl_btctrl_disable_2m_phy();
#endif
#if SL_BT_CONTROLLER_CODED_PHY_SUPPORT == 0
  sl_btctrl_disable_coded_phy();
#endif
#endif // SL_CATALOG_BLUETOOTH_FEATURE_PHY_SUPPORT_CONFIG_PRESENT

#if defined(SL_CATALOG_BLUETOOTH_CONTROLLER_ONLY_PRESENT) && !defined(SL_CATALOG_KERNEL_PRESENT)
  sli_btctrl_events_init();
#endif // SL_CATALOG_BLUETOOTH_CONTROLLER_ONLY_PRESENT && !SL_CATALOG_KERNEL_PRESENT

  sl_btctrl_init_mem();
  sl_btctrl_init_ll();
  sl_bluetooth_controller_debug_init();

#if defined(SL_CATALOG_RAIL_UTIL_COEX_PRESENT)
  sl_bt_init_coex_hal();
#endif

#if (SL_RAIL_LIB_MULTIPROTOCOL_SUPPORT == 1)
  sl_btctrl_init_multiprotocol();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_RADIO_WATCHDOG_PRESENT)
  sl_btctrl_enable_radio_watchdog();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_ADVERTISER_PRESENT)
  sl_btctrl_init_adv();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_EXTENDED_ADVERTISER_PRESENT)
  sl_btctrl_init_adv_ext();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SCANNER_PRESENT)
  sl_btctrl_init_scan();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_EVEN_SCHEDULING_PRESENT)
  sl_btctrl_enable_even_connsch();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_PAWR_SCHEDULING_PRESENT)
  sl_btctrl_enable_pawr_connsch();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_PRESENT)
  sl_btctrl_init_conn();
#if !defined(SL_CATALOG_BLUETOOTH_CONNECTION_PHY_UPDATE_INCOMPATIBLE_PRESENT)
  sl_btctrl_init_phy();
#endif
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_STATISTICS_PRESENT)
  sl_btctrl_init_conn_statistics();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_SUBRATING_PRESENT)
  sl_btctrl_init_subrate();
  status = sl_btctrl_allocate_conn_subrate_memory(SL_BT_CONFIG_MAX_CONNECTIONS + SL_BT_COMPONENT_CONNECTIONS);
  if (status != SL_STATUS_OK) {
    return status;
  }

#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_POWER_CONTROL_PRESENT)
  const sl_bt_ll_power_control_config_t power_control_config = {
    .activate_power_control = SL_BT_ACTIVATE_POWER_CONTROL,
    .golden_rssi_min_1m = SL_BT_GOLDEN_RSSI_MIN_1M,
    .golden_rssi_max_1m = SL_BT_GOLDEN_RSSI_MAX_1M,
    .golden_rssi_min_2m = SL_BT_GOLDEN_RSSI_MIN_2M,
    .golden_rssi_max_2m = SL_BT_GOLDEN_RSSI_MAX_2M,
    .golden_rssi_min_coded_s8 = SL_BT_GOLDEN_RSSI_MIN_CODED_S8,
    .golden_rssi_max_coded_s8 = SL_BT_GOLDEN_RSSI_MAX_CODED_S8,
    .golden_rssi_min_coded_s2 = SL_BT_GOLDEN_RSSI_MIN_CODED_S2,
    .golden_rssi_max_coded_s2 = SL_BT_GOLDEN_RSSI_MAX_CODED_S2
  };

  status = sl_btctrl_init_power_control((const uint8_t *) &power_control_config,
                                        SL_BT_DEFAULT_MIN_POWER_LEVEL,
                                        SL_BT_DEFAULT_MAX_POWER_LEVEL);
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_USER_POWER_CONTROL_PRESENT)
  sl_bt_init_app_controlled_tx_power();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SCANNER_PRESENT)
  sl_btctrl_init_scan_ext();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PERIODIC_ADVERTISER_PRESENT)
  sl_btctrl_init_periodic_adv();
  sl_btctrl_alloc_periodic_adv(SL_BT_CONFIG_MAX_PERIODIC_ADVERTISERS);
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PAWR_ADVERTISER_PRESENT)
  struct sl_btctrl_pawr_advertiser_config pawr_config = {
    .max_pawr_sets = SL_BT_CONFIG_MAX_PAWR_ADVERTISERS,
    .max_advertised_data_length_hint = SL_BT_CONFIG_MAX_PAWR_ADVERTISED_DATA_LENGTH_HINT,
    .subevent_data_request_count = SL_BT_CONFIG_PAWR_PACKET_REQUEST_COUNT,
    .subevent_data_request_advance = SL_BT_CONFIG_PAWR_PACKET_REQUEST_ADVANCE,
  };
  status = sl_btctrl_pawr_advertiser_configure(&pawr_config);
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SYNC_PRESENT)
  sl_btctrl_init_periodic_scan();
  status = sl_btctrl_alloc_periodic_scan(SL_BT_CONFIG_MAX_PERIODIC_ADVERTISING_SYNC);
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PAWR_SYNC_PRESENT)
  struct sl_btctrl_pawr_synchronizer_config pawr_sync_config = {
    .max_pawr_sets = SL_BT_CONFIG_MAX_PAWR_SYNCHRONIZERS,
  };
  status = sl_btctrl_pawr_synchronizer_configure(&pawr_sync_config);
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_AFH_PRESENT)
  status = sl_btctrl_init_afh(1);
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_HIGH_POWER_PRESENT)
  sl_btctrl_init_highpower();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PHY_SUPPORT_CONFIG_PRESENT)
#if SL_BT_CONTROLLER_2M_PHY_SUPPORT == 0
  sl_btctrl_disable_2m_phy();
#endif
#if SL_BT_CONTROLLER_CODED_PHY_SUPPORT == 0
  sl_btctrl_disable_coded_phy();
#endif
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_AOA_RECEIVER_PRESENT) \
  || defined(SL_CATALOG_BLUETOOTH_FEATURE_AOD_RECEIVER_PRESENT)
  status = sl_btctrl_init_cte_receiver();
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_AOA_TRANSMITTER_PRESENT) \
  || defined(SL_CATALOG_BLUETOOTH_FEATURE_AOD_TRANSMITTER_PRESENT)
  status = sl_btctrl_init_cte_transmitter();
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_ADVERTISER_PAST_PRESENT)
  sl_btctrl_init_past_local_sync_transfer();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SYNC_PAST_PRESENT)
  sl_btctrl_init_past_remote_sync_transfer();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PAST_RECEIVER_PRESENT)
  sl_btctrl_init_past_receiver();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_ANALYZER_PRESENT)
  status = sl_btctrl_init_sniff(SL_BT_CONFIG_MAX_CONNECTION_ANALYZERS);
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CS_PRESENT) || defined(SL_CATALOG_BLUETOOTH_FEATURE_CS_TEST_PRESENT)
  struct sl_btctrl_cs_config cs_config = { 0 };
  cs_config.configs_per_connection = SL_BT_CONFIG_MAX_CS_CONFIGS_PER_CONNECTION;
  cs_config.procedures = SL_BT_CONFIG_MAX_CS_PROCEDURES;
  sl_btctrl_init_cs(&cs_config);
#endif // SL_CATALOG_BLUETOOTH_FEATURE_CS_PRESENT or SL_CATALOG_BLUETOOTH_FEATURE_CS_TEST_PRESENT

  sl_btctrl_init_basic(SL_BT_CONFIG_MAX_CONNECTIONS + SL_BT_COMPONENT_CONNECTIONS,
                       SL_BT_CONFIG_USER_ADVERTISERS + SL_BT_COMPONENT_ADVERTISERS,
                       SL_BT_CONFIG_ACCEPT_LIST_SIZE);

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_RESOLVING_LIST_PRESENT)
  sl_btctrl_init_privacy();
  status = sl_btctrl_allocate_resolving_list_memory(SL_BT_CONFIG_RESOLVING_LIST_SIZE);
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_AFH_PRESENT)
  sl_btctrl_init_afh(1);
#endif // SL_CATALOG_BLUETOOTH_FEATURE_AFH_PRESENT

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_HIGH_POWER_PRESENT)
  sl_btctrl_init_highpower();
#endif // SL_CATALOG_BLUETOOTH_FEATURE_HIGH_POWER_PRESENT

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_PRESENT)
  sl_btctrl_configure_completed_packets_reporting(SL_BT_CONTROLLER_COMPLETED_PACKETS_THRESHOLD,
                                                  SL_BT_CONTROLLER_COMPLETED_PACKETS_EVENTS_TIMEOUT);
#endif // SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_PRESENT

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SCANNER_PRESENT)
  sl_btctrl_configure_max_queued_adv_reports(SL_BT_CONFIG_MAX_QUEUED_ADV_REPORTS);
#endif // SL_CATALOG_BLUETOOTH_FEATURE_SCANNER_PRESENT

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_HCI_DEBUG_PRESENT)
  hci_debugEnable();
#endif // SL_CATALOG_BLUETOOTH_FEATURE_HCI_DEBUG_PRESENT

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_HCI_TEST_COMMANDS_PRESENT)
  hci_enableVendorSpecificDebugging();
#endif // SL_CATALOG_BLUETOOTH_FEATURE_HCI_TEST_COMMANDS_PRESENT

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_HCI_TEST_COMMANDS_PRESENT) && (SL_RAIL_LIB_MULTIPROTOCOL_SUPPORT == 1)
  test_protocol_init();
#endif // SL_CATALOG_BLUETOOTH_FEATURE_HCI_TEST_COMMANDS_PRESENT && SL_RAIL_LIB_MULTIPROTOCOL_SUPPORT == 1

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CIS_PRESENT)
  sl_btctrl_init_cis();
#endif // SL_CATALOG_BLUETOOTH_FEATURE_CIS_PRESENT

  return status;
}

sl_status_t sl_bt_ll_deinit();

void sl_bt_controller_deinit(void)
{
#if defined(SL_CATALOG_BLUETOOTH_CONTROLLER_ONLY_PRESENT)
  if (sl_btctrl_is_initialized() == false) {
    return;
  }
#endif // SL_CATALOG_BLUETOOTH_CONTROLLER_ONLY_PRESENT

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_ANALYZER_PRESENT)
  sl_btctrl_deinit_sniff();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_RESOLVING_LIST_PRESENT)
  sl_btctrl_allocate_resolving_list_memory(0);
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SYNC_PRESENT)
  sl_btctrl_alloc_periodic_scan(0);
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PAWR_ADVERTISER_PRESENT)
  struct sl_btctrl_pawr_advertiser_config pawr_config = {
    .max_pawr_sets = 0,
    .max_advertised_data_length_hint = 0,
    .subevent_data_request_count = 0,
    .subevent_data_request_advance = 0,
  };
  (void) sl_btctrl_pawr_advertiser_configure(&pawr_config);
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PAWR_SYNC_PRESENT)
  struct sl_btctrl_pawr_synchronizer_config pawr_sync_config = {
    .max_pawr_sets = 0,
  };
  (void) sl_btctrl_pawr_synchronizer_configure(&pawr_sync_config);
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PERIODIC_ADVERTISER_PRESENT)
  (void) sl_btctrl_alloc_periodic_adv(0);
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_SUBRATING_PRESENT)
  sl_btctrl_allocate_conn_subrate_memory(0);
#endif

  sl_bt_ll_deinit();

#if defined(SL_CATALOG_BLUETOOTH_CONTROLLER_ONLY_PRESENT)
  sli_btctrl_deinit_mem();
#endif // SL_CATALOG_BLUETOOTH_CONTROLLER_ONLY_PRESENT
}
