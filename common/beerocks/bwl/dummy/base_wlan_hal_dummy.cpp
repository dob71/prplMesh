/* SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 * Copyright (c) 2016-2019 Intel Corporation
 *
 * This code is subject to the terms of the BSD+Patent license.
 * See LICENSE file for more details.
 */

#include "base_wlan_hal_dummy.h"

#include <beerocks/bcl/beerocks_string_utils.h>
#include <beerocks/bcl/beerocks_utils.h>
#include <beerocks/bcl/network/network_utils.h>
#include <beerocks/bcl/son/son_wireless_utils.h>

#include <easylogging++.h>

#define UNHANDLED_EVENTS_LOGS 20

namespace bwl {
namespace dummy {

//////////////////////////////////////////////////////////////////////////////
////////////////////////// Local Module Functions ////////////////////////////
//////////////////////////////////////////////////////////////////////////////

std::ostream &operator<<(std::ostream &out, const dummy_fsm_state &value)
{
    switch (value) {
    case dummy_fsm_state::Delay:
        out << "Delay";
        break;
    case dummy_fsm_state::Init:
        out << "Init";
        break;
    case dummy_fsm_state::GetRadioInfo:
        out << "GetRadioInfo";
        break;
    case dummy_fsm_state::Attach:
        out << "Attach";
        break;
    case dummy_fsm_state::Operational:
        out << "Operational";
        break;
    case dummy_fsm_state::Detach:
        out << "Detach";
        break;
    }
    return out;
}

std::ostream &operator<<(std::ostream &out, const dummy_fsm_event &value)
{
    switch (value) {
    case dummy_fsm_event::Attach:
        out << "Attach";
        break;
    case dummy_fsm_event::Detach:
        out << "Detach";
        break;
    }
    return out;
}

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Implementation ///////////////////////////////
//////////////////////////////////////////////////////////////////////////////

base_wlan_hal_dummy::base_wlan_hal_dummy(HALType type, std::string iface_name, bool acs_enabled,
                                         hal_event_cb_t callback, int wpa_ctrl_buffer_size)
    : base_wlan_hal(type, iface_name, IfaceType::Intel, acs_enabled, callback),
      beerocks::beerocks_fsm<dummy_fsm_state, dummy_fsm_event>(dummy_fsm_state::Delay),
      m_wpa_ctrl_buffer_size(wpa_ctrl_buffer_size)
{

    // Allocate wpa_ctrl buffer
    if (m_wpa_ctrl_buffer_size) {
        m_wpa_ctrl_buffer = std::shared_ptr<char>(new char[m_wpa_ctrl_buffer_size], [](char *obj) {
            if (obj)
                delete[] obj;
        });
    }

    m_fd_ext_events = 0;

    // Initialize the FSM
    fsm_setup();
}

base_wlan_hal_dummy::~base_wlan_hal_dummy() { detach(); }

bool base_wlan_hal_dummy::fsm_setup() { return true; }

HALState base_wlan_hal_dummy::attach(bool block)
{
    refresh_radio_info();
    return (m_hal_state = HALState::Operational);
}

bool base_wlan_hal_dummy::detach() { return true; }

bool base_wlan_hal_dummy::ping() { return true; }

bool base_wlan_hal_dummy::dummy_send_cmd(const std::string &cmd) { return false; }

bool base_wlan_hal_dummy::dummy_send_cmd(const std::string &cmd, char **reply) { return false; }

bool base_wlan_hal_dummy::refresh_radio_info()
{
    if (get_iface_name() == "wlan2") {
        m_radio_info.is_5ghz = true;
    }
    return true;
}

bool base_wlan_hal_dummy::refresh_vap_info(int vap_id) { return true; }

bool base_wlan_hal_dummy::refresh_vaps_info(int id) { return true; }

/**
 * @brief process simulated events
 *        events are expected to be simulated by writing the event
 *        string to the event pipe (/tmp/$BEEROCKS_TMP_PATH/bwl-event-wlanX).
 *        For example, simulating client connected event:
 *        echo "STA_CONNECTED,11:22:33:44:55:66" > /tmp/$BEEROCKS_TMP_PATH/bwl-event-wlanX
 *
 * @return true on success
 * @return false on failure
 */
bool base_wlan_hal_dummy::process_ext_events()
{
    if (m_fd_ext_events <= 0)
        return true;

    char buf[1024];
    size_t len = read(m_fd_ext_events, buf, sizeof(buf));

    LOG(DEBUG) << "Received event, len: " << len << ", data: " << std::string(buf, len);

    // TODO Handle event
    return true;
}

std::string base_wlan_hal_dummy::get_radio_mac()
{
    std::string mac;
    if (!beerocks::net::network_utils::linux_iface_get_mac(m_radio_info.iface_name, mac)) {
        LOG(ERROR) << "Failed to get radio mac from ifname " << m_radio_info.iface_name;
    }
    return mac;
}

} // namespace dummy
} // namespace bwl
