#! /usr/bin/env bash
$EXTRACTRC `find . -name "*.kcfg" -o -name "*.ui"` >> rc.cpp
$XGETTEXT `find . -name "*.cpp" -o -name "*.h" -o -name "*.qml"` -o $podir/kanagram.pot
