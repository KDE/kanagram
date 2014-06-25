#! /usr/bin/env bash
$EXTRACTRC `find . -name "*.kcfg"` >> rc.cpp
$XGETTEXT `find . -name "*.cpp" -o -name "*.h"` -o $podir/kanagram.pot
$XGETTEXT `find . -name \*.qml` -L Java -o $podir/kanagram.temp.pot
$XGETTEXT `find src/ -name "*.cpp" -o -name "*.h"` -o $podir/kanagram.pot
$MSGCAT $podir/kanagram.temp.pot $podir/kanagram.pot -o $podir/kanagram.pot
rm -f $podir/kanagram.temp.pot

