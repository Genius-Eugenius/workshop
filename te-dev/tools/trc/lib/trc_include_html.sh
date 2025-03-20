#!/bin/sh
# SPDX-License-Identifier: Apache-2.0
include_path="$(dirname "$(which "$0")")"
cat $include_path/$1
