#! /usr/bin/env bash
$EXTRACTRC *.ui >> rc.cpp
$XGETTEXT *.cpp -o $podir/plasma_wallpaper_mandelbrot.pot
