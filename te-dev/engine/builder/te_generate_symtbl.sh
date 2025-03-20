#!/bin/sh
# SPDX-License-Identifier: Apache-2.0
#
# Shell wrapper to generate symbol table.
#

MYDIR="$(dirname "$0")"

NM="$1" ; shift

${NM} --format=sysv "$@" |
    awk --posix -vTABLE_NAME=generated_table -f "${MYDIR}"/te_generate_symtbl
