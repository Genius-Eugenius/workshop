/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief NDN
 *
 * Declarations of ASN.1 types for NDN of RTE flow
 *
 *
 */

#ifndef __TE_NDN_RTE_FLOW_H__
#define __TE_NDN_RTE_FLOW_H__

#include "te_stdint.h"
#include "asn_usr.h"
#include "ndn.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NDN_FLOW_ATTR,
    NDN_FLOW_ACTION_TYPES,
    NDN_FLOW_ACTION,
    NDN_FLOW_ITEM,
    NDN_FLOW_ITEM_TYPES,
    NDN_FLOW_RULE,
} ndn_rte_flow_tags_t;

typedef enum {
    NDN_FLOW_ATTR_GROUP,
    NDN_FLOW_ATTR_PRIORITY,
    NDN_FLOW_ATTR_INGRESS,
    NDN_FLOW_ATTR_EGRESS,
    NDN_FLOW_ATTR_TRANSFER,
} ndn_rte_flow_attr_t;

typedef enum {
    NDN_FLOW_ACTION_TYPE_VOID,
    NDN_FLOW_ACTION_TYPE_QUEUE,
    NDN_FLOW_ACTION_TYPE_RSS,
    NDN_FLOW_ACTION_TYPE_DROP,
    NDN_FLOW_ACTION_TYPE_FLAG,
    NDN_FLOW_ACTION_TYPE_MARK,
    NDN_FLOW_ACTION_TYPE_COUNT,
    NDN_FLOW_ACTION_TYPE_VXLAN_ENCAP,
    NDN_FLOW_ACTION_TYPE_VXLAN_DECAP,
    NDN_FLOW_ACTION_TYPE_OF_POP_VLAN,
    NDN_FLOW_ACTION_TYPE_OF_PUSH_VLAN,
    NDN_FLOW_ACTION_TYPE_OF_SET_VLAN_VID,
    NDN_FLOW_ACTION_TYPE_PORT_REPRESENTOR,
    NDN_FLOW_ACTION_TYPE_REPRESENTED_PORT,
    NDN_FLOW_ACTION_TYPE_JUMP,
    NDN_FLOW_ACTION_TYPE_DEC_TTL,
} ndn_rte_flow_action_type_t;

typedef enum {
    NDN_FLOW_ITEM_TYPE_PORT_REPRESENTOR,
    NDN_FLOW_ITEM_TYPE_REPRESENTED_PORT,
} ndn_rte_flow_item_type_t;

typedef enum {
    NDN_FLOW_ACTION_CONF_COUNT,
    NDN_FLOW_ACTION_CONF_COUNT_ID,
    NDN_FLOW_ACTION_CONF_RSS,
    NDN_FLOW_ACTION_CONF_RSS_QUEUE,
    NDN_FLOW_ACTION_CONF_RSS_OPT,
    NDN_FLOW_ACTION_CONF_RSS_OPT_KEY,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_IPV4,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_FRAG_IPV4,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_NONFRAG_IPV4_TCP,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_NONFRAG_IPV4_UDP,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_NONFRAG_IPV4_SCTP,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_NONFRAG_IPV4_OTHER,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_IPV6,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_FRAG_IPV6,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_NONFRAG_IPV6_TCP,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_NONFRAG_IPV6_UDP,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_NONFRAG_IPV6_SCTP,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_NONFRAG_IPV6_OTHER,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_L2_PAYLOAD,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_IPV6_EX,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_IPV6_TCP_EX,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_IPV6_UDP_EX,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_PORT,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_VXLAN,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_GENEVE,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_NVGRE,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_IP,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_TCP,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_UDP,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_SCTP,
    NDN_FLOW_ACTION_CONF_RSS_OPT_HF_TUNNEL,
} ndn_rte_flow_action_conf_t;

typedef enum {
    NDN_FLOW_ACTION_TYPE,
    NDN_FLOW_ACTION_CONF,
    NDN_FLOW_ACTION_QID,
    NDN_FLOW_ACTION_MARK_ID,
    NDN_FLOW_ACTION_ENCAP_HDR,
    NDN_FLOW_ACTION_OF_PUSH_VLAN_ETHERTYPE,
    NDN_FLOW_ACTION_OF_SET_VLAN_VID,
    NDN_FLOW_ACTION_ETHDEV_PORT_ID,
    NDN_FLOW_ACTION_GROUP,
} ndn_rte_flow_action_t;

typedef enum {
    NDN_FLOW_ITEM_CONF_ETHDEV_PORT_ID,
} ndn_rte_flow_item_conf_t;

typedef enum {
    NDN_FLOW_ITEM_TYPE,
    NDN_FLOW_ITEM_CONF,
} ndn_rte_flow_item_t;

typedef enum {
    NDN_FLOW_RULE_ATTR,
    NDN_FLOW_RULE_PATTERN,
    NDN_FLOW_RULE_ACTIONS,
} ndn_rte_flow_rule_t;

extern const asn_type * const ndn_rte_flow_attr;
extern const asn_type * const ndn_rte_flow_pattern;
extern const asn_type * const ndn_rte_flow_actions;
extern const asn_type * const ndn_rte_flow_rule;
extern const asn_type * const ndn_rte_flow_action;
extern const asn_type * const ndn_rte_flow_action_conf_rss;
extern const asn_type * const ndn_rte_flow_action_conf_rss_queue;
extern const asn_type * const ndn_rte_flow_action_conf_rss_opt;
extern const asn_type * const ndn_rte_flow_action_conf_rss_opt_hf;
extern const asn_type * const ndn_rte_flow_item;
extern const asn_type * const ndn_rte_flow_items;
extern const asn_type * const ndn_rte_flow_item_conf;

extern asn_type ndn_rte_flow_rule_s;

#define NDN_RTE_FLOW_FIELD_NAME_MAX_LEN 128
#define NDN_RTE_FLOW_IPV4_FRAG_OFST_FIELD_LEN 13
#define NDN_RTE_FLOW_IPV4_FRAG_OFST_FIELD_OFFSET 0
#define NDN_RTE_FLOW_IPV4_FLAG_FIELD_LEN 1
#define NDN_RTE_FLOW_IPV4_FLAG_MORE_FRAG_FIELD_OFST 13
#define NDN_RTE_FLOW_IPV4_FLAG_DONT_FRAG_FIELD_OFST 14

/**
 * Get values of spec, mask and last of requested field with specified name
 * based on size of the value (in bits) and offset of the value (in bits)
 *
 * @param pdu    ASN.1 value
 * @param name   Name of the field
 * @param size   Size of the requested filed
 * @param offset Offset of the requested filed
 * @param spec_p Pointer to the spec
 * @param mask_p Pointer to the mask
 * @param last_p Pointer to the last
 *
 * @return Status code
*/
extern te_errno ndn_rte_flow_read_with_offset(const asn_value *pdu,
                                              const char *name,
                                              size_t size, uint32_t offset,
                                              uint32_t *spec_p,
                                              uint32_t *mask_p,
                                              uint32_t *last_p);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __TE_NDN_RTE_FLOW_H__ */
