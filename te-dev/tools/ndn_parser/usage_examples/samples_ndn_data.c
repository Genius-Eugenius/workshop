/* SPDX-License-Identifier: Apache-2.0 */
/*****************************************************************
 *
 * Test Environment. Research document.
 * API samples for template, patterns, and packets handling.
 *
 *
 ****************************************************************/


/*
============================================
Some general notes.

There are two pricipally differ approaches to NDS hanling in C code.
All the other ones (which author may imagine) are either combinations and
variations of these, or obviously bad.
They may be called as "universal value handling approach" and
"Protocol-specific value handling".

So this document contains descritions of both approaches.
Each description is devided to three sections:
    pre-written declarations;
    declarations which should be generated by NDN parser;
    usage examples.

All samples will be done for "Ethernet-Header" ASN.1 type.

Certainly, all functions declared in 'pre-written' declarations will have
(in case of that approach will be chosed for implementation) pre-written
definitions.

============================================
*/

#include "asn_value.h"



/*
 * This structure and the following functions are common to both approaches.
 * They should be generated by NDN parser for all ASN types respective
 * to protocol data. Set of this types will be taken from definition of
 * ASN.1 type "Generic-PDU".
 *
 * Usage of these structure and functions is clear.
 * Return code: zero on success, otherwise - error code.
 */

struct Ethernet_Header_Packet {
    unsigned char dst_addr[6];
    unsigned char src_addr[6];
    unsigned char eth_type[2];
};

int ASN_value_to_Ethernet_Header_Packet(ASN_value_p eth_header,
                                        struct Ethernet_Header_Packet *pkt);
int Ethernet_Header_Packet_to_ASN_value(ASN_value_p *eth_header_p,
                                        struct Ethernet_Header_Packet *pkt);

/* This functions finds Ethernet Header PDU in passed Raw-Packet ASN value,
 * and puts it into structure pointed by pkt.
 * Return code: zero on success, otherwise - error code.
 */
int Find_Ethernet_Header_in_Raw_Packet(ASN_value_p raw_packet,
                                       struct Ethernet_Header_Packet *pkt);

/* ============== Universal Value handling Approach ==================== */


/* ---------------- pre-written declarations ---------------- */

/*
 * DATA-UNIT operating functions.
 * These funcion has common prefix
 * "NDN_data_unit_{write/read}_<choice-name>"
 * where <choice-name> is name of choice in ASN DATA-UNIT type.
 * some choices with same data syntax are joined and denoted as 'string'
 * pseudo-choice-name.
 *
 * 'plain' choice has no special function for access, use common
 * ASN_value operating API for usage this choice.
 *
 * Parameters:
 *      - pointer to ASN_value. this value may be either sequence of
 *        DATA-UNIT fields, or itself DATA-UNIT type.
 *      - field_name is string with name of field in ASN_value (passed in
 *        first argument) having DATA-UNIT type. If there is no field with
 *        such name or it has type differ than DATA-UNIT, error will be
 *        returned. If this parameter is NULL or empty string, ASN_value
 *        itself is expected to be DATA-UNIT.
 *      - some data, depending of respective choice.
 * Functions NDN_data_unit_{write/read}_string has additional argument,
 * specifying wanted choice.
 */

typedef enum  {DU_script,DU_env, DU_function } ASN_DU_string_choices;

extern int NDN_data_unit_write_string(ASN_value_p, const char *field_name,
                                      ASN_DU_string_choice,
                                      const char *user_data);
extern int NDN_data_unit_read_string(ASN_value_p, const char *field_name,
                                      ASN_DU_string_choice,
                                      int *len, char *user_data);

extern int NDN_data_unit_write_enum(ASN_value_p, const char *field_name,
                                    int len, const int *user_data);
extern int NDN_data_unit_read_enum(ASN_value_p, const char *field_name,
                                    int *len, int *user_data);

extern int NDN_data_unit_write_mask(ASN_value_p, const char *field_name,
                                      int pr_len,  const char *product,
                                      int pat_len, const char *pattern);
extern int NDN_data_unit_read_mask(ASN_value_p, const char *field_name,
                                      int *pr_len,  const char *product,
                                      int *pat_len, const char *pattern);

typedef struct {
    int min;
    int max;
} NDN_DU_interval;

extern int NDN_data_unit_write_intervals(ASN_value_p,
                                         const char *field_name,
                                         int len,
                                         NDN_DU_interval *user_data);
extern int NDN_data_unit_read_intervals(ASN_value_p,
                                        const char *field_name,
                                        int *len,
                                        NDN_DU_interval *user_data);

/* declarations of globals with common NDN ASN.1 types specifications. */

extern ASN_type_p ndn_Traffic_Template;
extern ASN_type_p ndn_Template_Parameter;

extern ASN_type_p ndn_Traffic_Pattern;

extern ASN_type_p ndn_Raw_Packet;

/*
 * Function NDN_parse_packet
 *      Parses NDS of packet, received from TA.
 * Parameters:
 *      filename - name of file where RCF saved NDS attachment.
 * Return code:
 *      pointer ot ASN_value structure with type "Raw-Packet"
 *      (see NDN ASN.1 definitions about it) or zero if error occurred.
 */

ASN_value_p NDN_parse_packet(const char *filename);






/* ---------- generated (by NDN parser) declarations -------- */

extern ASN_type_p ndn_Ethernet_Header;



/* ------------------- usage example ------------------------ */

void
universal_handling_usage_example(void)
{
    int eth_pdu_index, i;
    ASN_value_p pattern, pdus, eth_header = -1;

    char mask_pattern[6] = {0xff,0xff,0xff,0xfa,0x00,0x00};
    char mask_product[6] = {0x6a,0x84,0x01,0x11,0x00,0x00};

    pattern = ASN_parse_file(ndn_Traffic_Pattern, "eht_test_pattern.asn");
    ASN_read_component_value(pattern, &pdus, "pdus");
    for (i = 0; i < ASN_get_length(pdus); i++)
    {
        ASN_value_p pdu = ASN_read_indexed (pdus, i);
        if (strcmp(ASN_get_type(pdu), "Ethernet-Header") == 0)
        {
            eth_pdu_index = i;
            eth_header = pdu;
        }
        else ASN_free_value(pdu);
    }

    NDN_data_unit_write_mask(eth_header, "dst-addr",
                                6, mask_product,
                                6, mask_pattern)
    {
    NDN_DU_interval eth_type_interval = {2, 8};

    NDN_data_unit_write_intervals (eth_header, "eth-type",
                                   1, &eth_type_interval);
    }
    ASN_write_indexed(pdus, eth_header, eth_pdu_index);
    ASN_write_component_value(pattern, pdus, "pdus");

    ASN_save_to_file(pattern, "eth_pattern_prepared.asn");
    ASN_free_value(eth_header);
    ASN_free_value(pdus);
    ASN_free_value(pattern);

    /* Pass file with prepared pattern to RCF */

    /* Receive response to trrecv_stop. It is supposed that only one packet
     * contained in every message from TA */
    {
        Ethernet_Header_Packet eth_pkt;
        ASN_value_p raw_packet;
        ASN_value_p tmpl;

        raw_packet = NDN_parse_packet ("received_packet.asn");
        Find_Ethernet_Header_in_Raw_Packet(raw_packet, &eth_pkt);

        /* use eth_pkt, for example to construct new Traffic-Template */

        ASN_value_p tmpl = ASN_parse_file(ndn_Traffic_Pattern,
                                          "eth_template.asn");
        eth_header = ASN_init_value(ndn_Ethernet_Header);
        ASN_write_value_field(eth_header, 6, eth_pkt.src_addr,
                                "dst-addr", "plain");
        ASN_write_value_field(eth_header, 6, eth_pkt.dst_addr,
                                "src-addr", "plain");
        ASN_write_value_field(eth_header, 2, eth_pkt.eth_type,
                                "eth-type", "plain");
        ASN_push_value (tmpl, eth_header, "pdus");
    }
}




