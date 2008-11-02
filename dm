# Swap the commented out lines if you want to run via ssh

#export DISPLAY=:0
#xrandr -display :0 -s 240x320 -o 1
xrandr -s 240x320 -o 1
/usr/games/opendoom -config ~/.opendoom/opendoom.cfg
#xrandr -display :0 -s 480x640 -o 0
xrandr -s 480x640 -o 0
