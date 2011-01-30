#!/bin/bash
SW_BUNDLE="`echo "$0" | sed -e 's/\/Contents\/MacOS\/SnailWail//'`"
SW_RESOURCES="$SW_BUNDLE/Contents/Resources"

cd $SW_RESOURCES
exec "./bin/snail-wail" #$SW_RESOURCES/bin/snail-wail"