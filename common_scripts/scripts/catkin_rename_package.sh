#!/bin/bash
set -e -u
shopt -s xpg_echo

# @todo Robustify to spaces in the paths
# @todo Figure out better way to implode flags?
# @todo Robustify `find` with -not -path? -type f?

bin=$(basename $0)
LONG_USAGE="Rename a package from <from> to <to>.
This will first find and replace the package name (whole world) in source files, then\
rename any directories exactly named <from> to <to>.
It is highly recommended that you version control your source code before and after\
these changes!"

OPTIONS_SPEC="\
usage: $bin [options] <dir> <from> <to>

	-p, --pause  Pause to show renames
    -h, --help   Show help

$LONG_USAGE
"

usage()
{
	echo "$OPTIONS_SPEC" >&2
	exit ${1-0}
}

pause=
do-pause() {
	if [[ -n "$pause" ]]; then
		echo "Press ENTER..."
		read
	fi
}

while [[ $# -gt 0 ]]
do
	case "$1" in
	-h|--help)
		usage
		;;
	-p|--pause)
		pause=1
		;;
	-*)
		echo "error: Invalid option: $1"
		usage 1
		;;
	*)
		break
		;;
	esac
	shift
done

[[ $# -ne 3 ]] && { echo "error: Invalid arguments: $@" >&2; usage 1; }

dir="$1"
from="$2"
to="$3"

# Ensure that everything is first staged in git (if you are doing this outside of git, then shame on you!)
# What about subprojects? ...
# git add -A :/

# First find all text files (ignoring .git and build/) and replace the package name

# File type / name flags - implementing implode functionality :/
# python -c 'print " -o ".join(map(lambda x: "-name " + x, ["*.cpp", "*.hpp"]))'
patterns=('*.cpp' '*.hpp' '*.cc' '*.h' '*.c' '*.m' '*.py' 'package.xml' 'CMakeLists.txt' '*.cmake' '*.sh' 'BUILD' '*.bzl')
name_flags=()
for pattern in "${patterns[@]}"
do
	flag=(-name "$pattern")
	echo "${flag[@]}"
	[[ ${#name_flags[@]} -eq 0 ]] && name_flags=("${flag[@]}") || name_flags=("${name_flags[@]}" -o "${flag[@]}")
done

# Prune flags
prunes='*/build */.git'
prune_flags=""
for prune in $prunes
do
	flag="-path $prune -prune"
	[[ -z "$prune_flags" ]] && prune_flags="$flag" || prune_flags="$prune_flags -o $flag"
done

regex_flags=-regex\ ".*[\b_]?$from[\b_]?.*"

echo "[ Rename Package: '$from' -> '$to' ]"
# http://stackoverflow.com/questions/4210042/exclude-directory-from-find-command
# @note This works with -prune because our names are typically files types
# @todo http://stackoverflow.com/questions/8677546/bash-for-in-looping-on-null-delimited-string-variable
files="$(find "$dir" $prune_flags -o \( "${name_flags[@]}" \) -print)"
echo "[ Text Replacement for Patterns: $patterns ]"
echo $files
do-pause
sed -i "s#\b$from\b#$to#g" $files

# Next rename directories
echo "[ Rename Directories ]"
srcs="$(find "$dir" -name "$from" -type d | tac)"
echo $srcs
do-pause
for src in $srcs
do
	dest="$(dirname $src)/$to"
	mv "$src" "$dest"
done

echo "[ Rename Files ]"
srcs="$(find "$dir" $regex_flags -type f)"
echo $srcs
do-pause
for src in $srcs; do
	rename "s#([\b_]?)${from}([\b_]?)#\$1${to}\$2#g" $src
done
