date=$(date +'%a %b %d %H:%M')
batt=$(cat /sys/class/power_supply/BAT1/capacity)
stat=" $date [$batt%]"
xsetroot -name "$stat"
