#! /bin/bash
# SPDX-License-Identifier: Apache-2.0
#
# Script to generate html test log.
#

HTML_OPTION=false EXEC_NAME=$0 `dirname \`which $0\``/log.sh --html "$@"
