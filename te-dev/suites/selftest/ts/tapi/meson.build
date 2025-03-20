# SPDX-License-Identifier: Apache-2.0

packages = [
    'cache',
    'file',
    'fio',
    'job',
    'jobopt',
    'tool',
    'dns',
]

install_data([ 'package.xml' ], install_dir: package_dir)

tapi_mydir = package_dir
foreach package : packages
    package_dir = join_paths(tapi_mydir, package)
    package_tests_c = []
    subdir(package)
endforeach
