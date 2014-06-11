#!/bin/bash

mkcd () 
{ 
    mkdir "$@" && cd "${!#}"
}

test-target-low()
{ (
    set -e -u
    label=$1
    type=$2
    flags="$3"
    dir=build/${label}
    mkcd -p ${dir}
    indent='    '
    echo -e "-   label: ${label}\n    type: ${type}\n    flags: ${flags}\n    output: |-"
    {
        cmake ../.. -DCMAKE_BUILD_TYPE=${type} $flags
        make -j4 all && make -j4 tests
    } > "/tmp/eigen/log-${label}.txt" 2>&1
    ./devel/lib/eigen_utilities/eigen_utilities-test | sed "s#^#${indent}${indent}#g"
) }

test-target()
{
    type=$1
    test-target-low "type-${type}" "${type}" ""
}

mkdir -p /tmp/test

echo "results:"
test-target Release
test-target MinSizeRel 
test-target RelWithDebInfo 
test-target Debug 
test-target "" 
