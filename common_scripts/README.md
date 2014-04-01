# common_scripts

Very general scripts for use in a catkin workspace.

## Installation

This is meant for develspace for the time being. To install, the easiest way is to just build with `catkin`, which will use `env-hooks` to add the scripts to `$PATH`.

## `catkin_rename_package.sh`

The invocation of this scripts is simply:

	catkin_rename_package ${dir} ${from} ${to}

where `${dir}` is your source dirctory containing the package itself (and its dependencies), `${from}` is the original package name, and `${to}` is the new package name.

1.	Find all files matching a source-type pattern (i.e., C++, Python, CMake, shell) and replaces `${from}` by `${to}`
2.	Finds all directories matching `${from}` and renames them to `${to}` (in reverse order so that top-level renames happen last)

## Todo

*	Make more specific
*	This script currently adds scripts via `env-hooks`. Would be better to use `${DEVELSPACE}/bin`...
