#!/bin/bash
SW_BUNDLE="`echo "$0" | sed -e 's/\/Contents\/MacOS\/Tankage//'`"
SW_RESOURCES="$SW_BUNDLE/Contents/Resources"

cd $SW_RESOURCES
exec "./bin/tankage" #$SW_RESOURCES/bin/tankage"
