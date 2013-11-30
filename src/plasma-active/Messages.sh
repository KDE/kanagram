#! /usr/bin/env bash
$XGETTEXT `find . -name \*.qml` -L Java -o $podir/kanagram-active.temp.pot
$XGETTEXT `find src/ -name "*.cpp" -o -name "*.h"` -o $podir/kanagram-active.pot
$MSGCAT $podir/kanagram-active.temp.pot $podir/kanagram-active.pot -o $podir/kanagram-active.pot
rm -f $podir/kanagram-active.temp.pot
