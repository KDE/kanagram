#! /usr/bin/env bash
$EXTRACTRC `find . -name "*.kcfg"` >> rc.cpp
$XGETTEXT `find . -name "*.cpp" -o -name "*.h"` -o $podir/kanagram-engine.pot