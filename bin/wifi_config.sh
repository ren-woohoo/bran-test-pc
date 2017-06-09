#!/bin/sh
grepFlag='wpa_supplicant'

ret=`ps | grep "$grepFlag" | grep -v grep | wc -l`
echo $ret
if [ $ret -eq 0 ];then
	echo "WPA_SUPPLICANT IS'T RAN!!!"
	ifconfig wlan0 up		
	echo ctrl_interface=/var/run/wpa_supplicant>/etc/wpa_supplicant.conf
	wpa_supplicant -B -i wlan0 -D nl80211 -c /etc/wpa_supplicant.conf	
fi
wpa_cli -i wlan0 scan
echo 1 > /dev/Boost_En



