#!/bin/bash

cd "$(dirname "$BASH_SOURCE")"
../scripts/matlab_console.sh -r "try; test(); catch e; disp(getReport(e, 'extended')); end; exit();"
