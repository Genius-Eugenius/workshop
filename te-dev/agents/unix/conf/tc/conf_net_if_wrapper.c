/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief Wrapper for net/if.h
 *
 * Wrapper for net/if.h need for resolve conflict with linux/if.h and net/if.h
 *
 */

#include "te_config.h"
#include "conf_net_if_wrapper.h"

#ifdef HAVE_NET_IF_H
#include <net/if.h>
#endif

int
conf_net_if_wrapper_if_nametoindex(const char *if_name)
{
    return if_nametoindex(if_name);
}
