#!/usr/bin/env sh

# This script is executed when either the mode changes,
# or the commandline changes
# This is just an example using sketchybar:
# The variables $MODE and $CMDLINE hold the 
# current editor and cmdline info

# COLOR=0xff9dd274
# if [ "$MODE" = "" ]; then
#   COLOR=0xffff6578
# fi
#
# DRAW_CMD="off"
# if [ "$CMDLINE" != "" ]; then
#   DRAW_CMD="on"
# fi
#
# sketchybar --set svim.mode label="[$MODE]" \
#                            label.drawing=$(if [ "$MODE" = "" ]; then echo "off"; else echo "on"; fi) \
#                            icon.color=$COLOR \
#                            popup.drawing=$DRAW_CMD \
#            --set svim.cmdline label="$CMDLINE"
#
# This is the setup command needed for sketchybar in sketchybarrc:
#
# sketchybar --add item svim.mode right \
#            --set svim.mode popup.align=right \
#                            icon= \
#                            icon.font="Hack Nerd Font Mono:Bold:28.0" \
#                            label.font="Hack Nerd Font Mono:Bold:13.0" \
#                            icon.color=0xffff6578 \
#                            script="sketchybar --set svim.mode popup.drawing=off" \
#            --subscribe svim.mode front_app_switched window_focus \
#            --add item svim.cmdline popup.svim.mode \
#            --set svim.cmdline icon="Command: "
#
