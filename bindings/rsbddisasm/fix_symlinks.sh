#!/bin/bash
set -e

cd bddisasm-sys
rm -rf LICENSE
ln -s ../../../LICENSE .

cd csrc
rm -rf bddisasm
ln -s ../../../../bddisasm/ .
rm -rf inc
ln -s ../../../../inc/ .
