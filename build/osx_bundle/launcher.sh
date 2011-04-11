#!/bin/bash
SW_BUNDLE="`echo "$0" | sed -e 's/\/Contents\/MacOS\/tankage//'`"
SW_RESOURCES="$SW_BUNDLE/Contents/Resources"

cd $SW_RESOURCES
exec "./tankage" #$SW_RESOURCES/bin/tankage"
