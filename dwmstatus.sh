gluc=$(cat /tmp/my-bget-latest)
date=$(date +'%a %b %d %H:%M')
batt=$(cat /sys/class/power_supply/BAT1/capacity)
stat=" $gluc | $date "

if [ $batt -ne 100 ]
then stat="$stat[$batt%]"
fi

if [ $(date +%H) -lt 5 ]
then shutdown now
fi

xsetroot -name "$stat"
