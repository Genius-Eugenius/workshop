/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief RPC Test Suite
 *
 * Test Suite prologue.
 *
 */

/** Logging subsystem entity name */
#define TE_TEST_NAME    "rpc_server_prologue"

#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>

#include "tapi_test.h"
#include "tapi_cfg_net.h"


int
main(int argc, char **argv)
{
    TEST_START;

    CHECK_RC(tapi_cfg_net_all_up(false));
    CHECK_RC(tapi_cfg_net_all_assign_ip(AF_INET));

    TEST_SUCCESS;
cleanup:
    TEST_END;
}
