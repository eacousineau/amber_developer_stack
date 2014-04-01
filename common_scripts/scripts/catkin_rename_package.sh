#!/bin/bash
set -e -u

dir="$1"
from="$2"
to="$3"

# Ensure that everything is first staged in git (if you are doing this outside of git, then shame on you!)
# What about subprojects? ...
# git add -A :/

# First find all text files (ignoring .git and build/) and replace the package name

# File type / name flags - implementing implode functionality :/
# python -c 'print " -o ".join(map(lambda x: "-name " + x, ["*.cpp", "*.hpp"]))'
patterns='*.cpp *.hpp *.h *.c *.m *.py package.xml CMakeLists.txt *.cmake *.sh'
name_flags=""
for pattern in $patterns
do
	flag="-name $pattern"
	[[ -z "$name_flags" ]] && name_flags="$flag" || name_flags="$name_flags -o $flag"
done

# Prune flags
prunes='*/build */.git'
prune_flags=""
for prune in $prunes
do
	flag="-path $prune -prune"
	[[ -z "$prune_flags" ]] && prune_flags="$flag" || prune_flags="$prune_flags -o $flag"
done

echo "[ Rename Package: '$from' -> '$to' ]"
# http://stackoverflow.com/questions/8677546/bash-for-in-looping-on-null-delimited-string-variable
files="$(find "$dir" $prune_flags -o \( $name_flags \) -print)"
echo "[ Text Replacement for Patterns: $patterns ]"
sed -i "s#\b$from\b#$to#g" $files

# Next rename directories
echo "[ Rename Directories ]"
srcs="$(find "$dir" -name "$from" -type d | tac)"
for src in $srcs
do
	dest="$(dirname $src)/$to"
	mv "$src" "$dest"
done
