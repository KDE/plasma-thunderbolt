#! /usr/bin/env bash
$XGETTEXT `find . -name "*.cpp" -o -name "*.qml"` -o $podir/kcm_bolt.pot
$XGETTEXT -j -L JavaScript `find . -name "*.js"` -o $podir/kcm_bolt.pot

