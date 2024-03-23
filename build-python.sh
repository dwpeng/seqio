#!/bin/bash

set -ex
python3 -m pip wheel .
if [ -d dist ]; then
    rm -rf dist
fi
mkdir dist
mv *.whl dist/
rm -rf build
rm -rf python/src/*.egg-info
