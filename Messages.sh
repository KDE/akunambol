#! /usr/bin/env bash
$EXTRACTRC `find . -name \*.ui -o -name \*.rc -o -name \*.kcfg` >> rc.cpp
$XGETTEXT rc.cpp qtgui/*.cpp library/*.cpp plugins/*/*.cpp -o $podir/akunambol.pot
rm -f rc.cpp

