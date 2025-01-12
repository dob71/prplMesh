///////////////////////////////////////
// AUTO GENERATED FILE - DO NOT EDIT //
///////////////////////////////////////

/* SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 * Copyright (c) 2016-2019 Intel Corporation
 *
 * This code is subject to the terms of the BSD+Patent license.
 * See LICENSE file for more details.
 */

#include <tlvf/wfa_map/tlvHigherLayerData.h>
#include <tlvf/tlvflogging.h>

using namespace wfa_map;

tlvHigherLayerData::tlvHigherLayerData(uint8_t* buff, size_t buff_len, bool parse, bool swap_needed) :
    BaseClass(buff, buff_len, parse, swap_needed) {
    m_init_succeeded = init();
}
tlvHigherLayerData::tlvHigherLayerData(std::shared_ptr<BaseClass> base, bool parse, bool swap_needed) :
BaseClass(base->getBuffPtr(), base->getBuffRemainingBytes(), parse, swap_needed){
    m_init_succeeded = init();
}
tlvHigherLayerData::~tlvHigherLayerData() {
}
const eTlvTypeMap& tlvHigherLayerData::type() {
    return (const eTlvTypeMap&)(*m_type);
}

const uint16_t& tlvHigherLayerData::length() {
    return (const uint16_t&)(*m_length);
}

tlvHigherLayerData::eProtocol& tlvHigherLayerData::protocol() {
    return (eProtocol&)(*m_protocol);
}

uint8_t* tlvHigherLayerData::payload(size_t idx) {
    if ( (m_payload_idx__ <= 0) || (m_payload_idx__ <= idx) ) {
        TLVF_LOG(ERROR) << "Requested index is greater than the number of available entries";
        return nullptr;
    }
    return &(m_payload[idx]);
}

bool tlvHigherLayerData::alloc_payload(size_t count) {
    if (m_lock_order_counter__ > 0) {;
        TLVF_LOG(ERROR) << "Out of order allocation for variable length list payload, abort!";
        return false;
    }
    if (count == 0) {
        TLVF_LOG(WARNING) << "can't allocate 0 bytes";
        return false;
    }
    size_t len = sizeof(uint8_t) * count;
    if(getBuffRemainingBytes() < len )  {
        TLVF_LOG(ERROR) << "Not enough available space on buffer - can't allocate";
        return false;
    }
    m_lock_order_counter__ = 0;
    uint8_t *src = (uint8_t *)m_payload;
    uint8_t *dst = src + len;
    if (!m_parse__) {
        size_t move_length = getBuffRemainingBytes(src) - len;
        std::copy_n(src, move_length, dst);
    }
    m_payload_idx__ += count;
    m_buff_ptr__ += len;
    if(m_length){ (*m_length) += len; }
    return true;
}

void tlvHigherLayerData::class_swap()
{
    tlvf_swap(16, reinterpret_cast<uint8_t*>(m_length));
}

size_t tlvHigherLayerData::get_initial_size()
{
    size_t class_size = 0;
    class_size += sizeof(eTlvTypeMap); // type
    class_size += sizeof(uint16_t); // length
    class_size += sizeof(eProtocol); // protocol
    return class_size;
}

bool tlvHigherLayerData::init()
{
    if (getBuffRemainingBytes() < kMinimumLength) {
        TLVF_LOG(ERROR) << "Not enough available space on buffer. Class init failed";
        return false;
    }
    m_type = (eTlvTypeMap*)m_buff_ptr__;
    if (!m_parse__) *m_type = eTlvTypeMap::TLV_HIGHER_LAYER_DATA;
    m_buff_ptr__ += sizeof(eTlvTypeMap) * 1;
    m_length = (uint16_t*)m_buff_ptr__;
    if (!m_parse__) *m_length = 0;
    m_buff_ptr__ += sizeof(uint16_t) * 1;
    m_protocol = (eProtocol*)m_buff_ptr__;
    m_buff_ptr__ += sizeof(eProtocol) * 1;
    if(m_length && !m_parse__){ (*m_length) += sizeof(eProtocol); }
    m_payload = (uint8_t*)m_buff_ptr__;
    if (m_length && m_parse__) {
        size_t len = *m_length;
        if (m_swap__) { tlvf_swap(16, reinterpret_cast<uint8_t*>(&len)); }
        len -= (m_buff_ptr__ - sizeof(*m_type) - sizeof(*m_length) - m_buff__);
        m_payload_idx__ = len/sizeof(uint8_t);
        m_buff_ptr__ += len;
    }
    if (m_buff_ptr__ - m_buff__ > ssize_t(m_buff_len__)) {
        TLVF_LOG(ERROR) << "Not enough available space on buffer. Class init failed";
        return false;
    }
    if (m_parse__ && m_swap__) { class_swap(); }
    if (m_parse__) {
        if (*m_type != eTlvTypeMap::TLV_HIGHER_LAYER_DATA) {
            TLVF_LOG(ERROR) << "TLV type mismatch. Expected value: " << int(eTlvTypeMap::TLV_HIGHER_LAYER_DATA) << ", received value: " << int(*m_type);
            return false;
        }
    }
    return true;
}


