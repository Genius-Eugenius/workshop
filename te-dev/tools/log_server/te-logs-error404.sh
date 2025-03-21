#! /bin/bash
# SPDX-License-Identifier: Apache-2.0
#
# CGI script to handle error 404 in log storage directories and
# generate requested file on fly.
#
# Uses the following CGI environment varialbes:
#   - REQUEST_URI
#
# Uses the following environment variables:
#   - TE_INSTALL (mandatory) to find TE log processing tools
#   - SHARED_URL (optional) shared static files of HTML logs to speed up
#   - TMPDIR (optoinal) location for temporarily unpacked log files
#
# Multi-page HTML logs have static part which is the same for instances.
# It could be stored on logs server in one place and referenced from
# all generated logs. It allows to use browser cache efficiently.
#
# Required size for TMPDIR depends on size of your logs. Typically 10 Gb
# is sufficient.

#############################
# Exit with "Not Found" message.
# Arguments:
#   None
# Outputs:
#   HTTP response
#############################
function not_found_exit() {
    printf "Content-Type: text/plain\r\n"
    printf "\r\n"
    printf "Not Found\r\n"
    printf "$*\r\n"
    exit 0
}

[[ -n "${TE_INSTALL}" ]] || not_found_exit "TE_INSTALL is not set"

TE_PATH="${TE_INSTALL}/default/bin/"

case "${REQUEST_URI}" in
    # FIXME Put your logs URL to location mapping here
    /logs/* )
        root_dir_uri="/logs"
        root_dir="/srv/logs"
        # FIXME Set docs_url to test suite documentation html root to
        # have links to the test documentation in HTML and JSON logs.
        ;;
    * )
        not_found_exit
        ;;
esac

# Requested file
request_file="${REQUEST_URI/$root_dir_uri/$root_dir}"
# Substitute %20->'space'
request_file="${request_file//%20/ }"
# Substitute %3A->':'
request_file="${request_file//%3A/:}"

get_item_cmd=()
# It is recommended to run it under nice since really many requests
# could be generated via Web server
get_item_cmd+=(nice)
get_item_cmd+=("${TE_PATH}"/rgt-log-get-item)
[[ -n "${SHARED_URL}" ]] && get_item_cmd+=(--shared-url="${SHARED_URL}/")
[[ -n "${docs_url}" ]] && get_item_cmd+=(--docs-url="${docs_url}")
# Try to fix generated files permissions
get_item_cmd+=(--fix-permissions)
# Try to generate requested log file
status=$("${get_item_cmd[@]}" --req-path="${request_file}" 2>&1)
result=$?

if [[ -r "${request_file}" ]] ; then
    # Now requested file exists, redirect
    printf "Status: 302 Moved\r\n"
    printf "Location: %s\r\n" "${REQUEST_URI}"
    printf "\r\n"
elif [[ x"$result" = x"2" ]] ; then
    # Internal error
    printf "Content-Type: text/plain\r\n"
    printf "\r\n"
    printf "ERROR: $status\r\n"
else
    not_found_exit
fi
