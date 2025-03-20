# /bin/sh
# SPDX-License-Identifier: Apache-2.0

. `dirname \`which $0\``/guess.sh

rgt-conv --no-cntrl-msg --mode=live -f ${TE_LOG_RAW}
