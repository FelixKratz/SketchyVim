#!/usr/bin/env sh

# This script is executed when either the mode changes,
# or the commandline changes
# This is just an example using sketchybar:
# The variables $MODE and $CMDLINE hold the 
# current editor and cmdline info

# sketchybar --set svim.mode icon="[$MODE]" \
#            --set svim.cmdline label="$CMDLINE"
#
# if [ "$CMDLINE" != "" ]; then
#   sketchybar --set center popup.drawing=on
# else
#   sketchybar --set center popup.drawing=off
# fi

