#!/bin/bash

set -ex
c++ -O3 -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) -fpermissive python/seqio.cc seqio.c -lz -I. -o python/_seqio$(python3-config --extension-suffix)
if [ -d python/src/lib ]; then rm -r python/src/lib; fi
mkdir -p python/src/lib
mv python/_seqio$(python3-config --extension-suffix) python/src/lib/_seqio$(python3-config --extension-suffix)
cd python && python3 setup.py sdist bdist_wheel
rm -rf build
rm -rf src/*.egg-info

