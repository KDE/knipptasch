#!/bin/sh

echo "Applying astyle rules..."
astyle \
--indent=spaces=4 \
--brackets=linux \
--indent-classes \
--indent-switches \
--indent-cases \
--indent-namespaces \
--indent-preprocessor \
--indent-labels \
--pad=oper \
--pad=paren-in \
--unpad=paren \
--one-line=keep-statements \
--convert-tabs \
`find src/ -type f -name '*.cpp'` `find src/ -type f -name '*.h'`

echo "Removing .orig files..."
find src/ -type f -name '*.orig' -exec rm {} \;

echo -e "\n [done]\n"

