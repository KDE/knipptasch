#! /bin/sh

$EXTRACTRC `find . -name '*.ui' \
            -o -name '*.rc' \
            -o -name '*.kcfg' | grep -v '/qsint/test/' | grep -v '/qsint/examples/'` >> rc.cpp
$XGETTEXT gui/main.cpp rc.cpp -o $podir/knipptasch.pot
$XGETTEXT_QT `find . -name '*.cpp' -a ! -name main.cpp` -o $podir/knipptasch_qt.pot
