#!/bin/bash

set -ex
cd python && python3 setup.py sdist bdist_wheel
rm -rf build
rm -rf src/*.egg-info
rm -rf seqio.c

