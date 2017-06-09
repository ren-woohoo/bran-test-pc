#!/bin/sh

echo "START THE SHELL FOR DEVICE INIT"
dinfo_file="/bin/qtapp/device.conf"
did=$1
key=$2
mac=$3
chmod 777 $dinfo_file
echo "file path:$dinfo_file"
echo "DID:$did"
echo "KEY:$key"
echo "MAC:$mac"
sed -i "/did=/c did=$did" $dinfo_file
sed -i "/key=/c key=$key" $dinfo_file
sed -i "/mac=/c mac=$mac" $dinfo_file

dinfo_did=`cat $dinfo_file | grep -v ^# | grep did= | tail -1 | cut -d '=' -f 2`
dinfo_key=`cat $dinfo_file | grep -v ^# | grep key= | tail -1 | cut -d '=' -f 2`
dinfo_mac=`cat $dinfo_file | grep -v ^# | grep mac= | tail -1 | cut -d '=' -f 2`

echo "dinfo_did:$dinfo_did"
echo "dinfo_key:$dinfo_key"
echo "dinfo_mac:$dinfo_mac"

if [ $did == $dinfo_did ] && [ $key == $dinfo_key ] && [ $mac == $dinfo_mac ];then
	echo "SET MIIO SUCCESS"
else
	echo "SET MIIO FAILED"
fi
