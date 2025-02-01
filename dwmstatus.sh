gluc=$(cat /tmp/bgain-latest)
date=$(date +'%a %b %d %H:%M')
stat=" $gluc | $date "

batt=$(cat /sys/class/power_supply/BAT1/capacity)

if [ $batt -ne 100 ]
then stat="$stat[$batt %]"
fi

if [ $(date +%H) -gt 21 ]
then shutdown now
fi

xsetroot -name "$stat"
