#!/bin/sh

echo "Applying astyle rules..."
astyle --indent=spaces=4 --brackets=linux --indent-classes --indent-switches --indent-cases --indent-namespaces --indent-preprocessor --indent-labels --pad=oper --pad=paren-in --unpad=paren --one-line=keep-statements --convert-tabs --add-brackets --align-pointer=name --suffix=none --exclude=build --exclude=3rdparty --lineend=linux --recursive --verbose *.h *.cpp
