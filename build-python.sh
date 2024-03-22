#!/bin/bash

set -ex
python3 setup.py sdist bdist_wheel
rm -rf build
rm -rf python/src/*.egg-info
