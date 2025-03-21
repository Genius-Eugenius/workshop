/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief Test Environment
 *
 * Check TCP/IP4/ETH CSAP data-sending behaviour
 *
 */

/** @page ipstack-ip4_send_tcp Send TCP packets via CSAP
 *        and accept it via STREAM socket.
 *
 * @objective Check that tcp.ip4.eth CSAP can send TCP packets with
 *            user-specified payload length and checksum.
 *
 * @param pco_iut       TA wich will be TCP server
 * @param pco_tst       TA wich will be TCP client
 * @param iut_addr      IUT local IPv4 address
 * @param tst_addr      TST local IPv4 address
 * @param iut_mac       IUT local MAC address
 * @param tst_mac       TST remote MAC address
 *
 * @par Scenario:
 *
 * -# Create TCP socket on @p pco_iut.
 * -# Send TCP init connection packet from @p pco_tst
 * -# Accept TCP connection on @p pco_iut.
 * -# Send TCP packet with random generated payload data from @p pco_tst
 * -# Receive TCP packet on @p pco_iut
 * -# Close connection by sending RST from @ pco_tst.
 *
 */
#ifndef DOXYGEN_TEST_SPEC

#define TE_TEST_NAME "ipstack/ip4_send_tcp"

#define TEST_START_VARS     TEST_START_ENV_VARS
#define TEST_START_SPECIFIC TEST_START_ENV
#define TEST_END_SPECIFIC   TEST_END_ENV

#include "te_config.h"

#if HAVE_STRING_H
#include <string.h>
#endif

#if HAVE_NETINET_IP_H
#include <netinet/ip.h>
#endif

#include "tad_common.h"
#include "rcf_rpc.h"
#include "asn_usr.h"
#include "ndn_ipstack.h"
#include "tapi_ndn.h"
#include "tapi_env.h"
#include "tapi_rpcsock_macros.h"
#include "tapi_test.h"
#include "tapi_rpc_params.h"
#include "tapi_tcp.h"
#include "tapi_sockaddr.h"
#include "tapi_route_gw.h"

#define TCP_FLAG_ACK 0x10

int
main(int argc, char *argv[])
{
    rcf_rpc_server            *iut_pco = NULL;
    rcf_rpc_server            *tst_pco = NULL;

    const struct sockaddr     *iut_addr = NULL;
    const struct sockaddr     *tst_addr = NULL;
    struct sockaddr           *fake_tst_addr = NULL;

    const struct sockaddr     *iut_mac = NULL;
    const struct sockaddr     *tst_mac = NULL;
    uint8_t                   *fake_tst_mac = NULL;

    const struct if_nameindex *iut_if = NULL;
    const struct if_nameindex *tst_if = NULL;

    int                        payload_len;

    const char                *check_sum = NULL;

    int                        iut_tcp_sock = -1;
    int                        tmp_sock = -1;
    tapi_tcp_handler_t         tcp_conn;

    void                      *send_buf = NULL;
    void                      *recv_buf = NULL;

    asn_value                 *template = NULL;
    asn_value                 *pdu = NULL;
    asn_value                 *pdu_ip = NULL;

    bool checksum_correct = true;

    TEST_START;
    TEST_GET_PCO(iut_pco);
    TEST_GET_PCO(tst_pco);
    TEST_GET_ADDR(iut_pco, iut_addr);
    TEST_GET_ADDR(tst_pco, tst_addr);
    TEST_GET_LINK_ADDR(iut_mac);
    TEST_GET_LINK_ADDR(tst_mac);
    TEST_GET_IF(iut_if);
    TEST_GET_IF(tst_if);
    TEST_GET_INT_PARAM(payload_len);
    TEST_GET_STRING_PARAM(check_sum);

    if (payload_len < 1)
        TEST_FAIL("Invalid payload_len parameter %d", payload_len);

    /* Prepate fake ip address. */
    fake_tst_addr = (struct sockaddr *)malloc(sizeof(struct sockaddr));
    memcpy(fake_tst_addr, tst_addr, sizeof(struct sockaddr));
    *((uint8_t *)&(((struct sockaddr_in *)(fake_tst_addr))->sin_addr.s_addr) +
        3) = rand_range(50, 100);

    /* Prepare fake mac address. */
    fake_tst_mac = malloc(sizeof(uint8_t) * 6);
    memcpy(fake_tst_mac, te_sockaddr_get_netaddr(tst_mac), sizeof(uint8_t) * 6);
    do {
        *(fake_tst_mac + rand_range(3,5)) = rand_range(1, 255);
    } while (!memcmp(fake_tst_mac, te_sockaddr_get_netaddr(tst_mac), sizeof(uint8_t) * 6));

    /* Open sock for listen. */
    iut_tcp_sock = rpc_socket(iut_pco, RPC_PF_INET,
                           RPC_SOCK_STREAM, RPC_PROTO_DEF);
    bool optval = true;
    rpc_setsockopt(iut_pco, iut_tcp_sock, RPC_SO_REUSEADDR, &optval);
    rpc_bind(iut_pco, iut_tcp_sock, iut_addr);
    rpc_listen(iut_pco, iut_tcp_sock, 1);

    CHECK_RC(tapi_update_arp(iut_pco->ta, iut_if->if_name, NULL, NULL,
                             fake_tst_addr, fake_tst_mac, false));
    CFG_WAIT_CHANGES;

    /* Establish TCP connection. */
    CHECK_RC(tapi_tcp_init_connection(tst_pco->ta, TAPI_TCP_CLIENT,
                             SA(fake_tst_addr), SA(iut_addr), tst_if->if_name,
                             fake_tst_mac, te_sockaddr_get_netaddr(iut_mac),
                             0, &tcp_conn));

    CHECK_RC(tapi_tcp_wait_open(tcp_conn, 3000));
    tmp_sock = rpc_accept(iut_pco, iut_tcp_sock, NULL, NULL);
    rpc_close(iut_pco, iut_tcp_sock);
    iut_tcp_sock = tmp_sock;

    /* Prepare send and receive buffers. */
    send_buf = te_make_buf_by_len(payload_len);
    recv_buf = malloc(payload_len);

    /* Prepare data to send. */
    CHECK_RC(tapi_tad_tmpl_ptrn_add_layer(&template, false,
                                          ndn_tcp_header, "#tcp", &pdu));
    CHECK_RC(asn_write_int32(pdu, TCP_FLAG_ACK, "flags.#plain"));
    CHECK_RC(asn_write_int32(pdu, tapi_tcp_next_ackn(tcp_conn),
                             "ackn.#plain"));
    CHECK_RC(asn_write_int32(pdu, tapi_tcp_next_seqn(tcp_conn),
                             "seqn.#plain"));
    CHECK_RC(tapi_tad_tmpl_ptrn_set_payload_plain(&template, false,
                                                  send_buf, payload_len));
    if (strcmp(check_sum, "correct"))
    {
        checksum_correct = false;
        CHECK_RC(tapi_ip4_add_pdu(&template, &pdu_ip, false, 0, 0, -1, -1, -1));
        CHECK_RC(asn_write_int32(pdu_ip, rand_range(1, 255),
                                 "pld-checksum.#diff"));
    }

    /* Send data. */
    CHECK_RC(tapi_tcp_send_template(tcp_conn, template, RCF_MODE_BLOCKING));
    CHECK_RC(tapi_tcp_update_sent_seq(tcp_conn, payload_len));

    /* Give data some time to be received. */
    MSLEEP(100);

    /* Receive data. */
    bool sock_ready_for_read;
    RPC_GET_READABILITY(sock_ready_for_read, iut_pco, iut_tcp_sock, 1);

    if (checksum_correct)
    {
        if (sock_ready_for_read)
        {
            if (payload_len != rpc_recv(iut_pco, iut_tcp_sock,
                                    recv_buf, payload_len, 0))
            TEST_FAIL("Number of sended bytes diffes "
                      "with number of received bytes.");

            /* Check received data. */
            if (memcmp(send_buf, recv_buf, payload_len))
                TEST_FAIL("Received data corrupted.");
        }
        else
            TEST_FAIL("Can not receive TCP packet");
    }
    else
        if (sock_ready_for_read)
            TEST_FAIL("TCP packet was received despite of incorrect cehcksum");

    TEST_SUCCESS;
cleanup:
    CLEANUP_CHECK_RC(tapi_tcp_send_rst(tcp_conn));
    CLEANUP_CHECK_RC(tapi_tcp_destroy_connection(tcp_conn));
    CLEANUP_RPC_CLOSE(iut_pco, iut_tcp_sock);

    if (template != NULL)
    {
        asn_free_value(template);
        template = NULL;
        pdu = NULL;
    }

    free(send_buf);
    free(recv_buf);

    TEST_END;
}

#endif /* !DOXYGEN_TEST_SPEC */
