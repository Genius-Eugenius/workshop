# SPDX-License-Identifier: Apache-2.0

packages = [
    'nginx',
]

install_data([ 'package.xml' ], install_dir: package_dir)

apps_mydir = package_dir
foreach package : packages
    package_dir = join_paths(apps_mydir, package)
    package_tests_c = []
    subdir(package)
endforeach
