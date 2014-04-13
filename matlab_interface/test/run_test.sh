#!/bin/bash

cd "$(dirname "$BASH_SOURCE")"
../scripts/matlab_console.sh -r 'test(); exit();'
