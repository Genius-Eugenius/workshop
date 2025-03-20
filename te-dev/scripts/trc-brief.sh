#! /bin/bash
# SPDX-License-Identifier: Apache-2.0

`dirname \`which $0\``/trc.sh --html=trc-brief.html --no-packages-only \
    --no-total --no-unspec --no-expected --no-stats-not-run $@
