#!/bin/bash

ROOT_PATH=$(pwd)
client=$ROOT_PATH/udpClient
conf=$ROOT_PATH/conf/client.conf

function Usage()
{
	printf "Usage: %s [-s(start) | -t(stop) | -rt(restart)]\n" "$0"
}

function StartClient()
{
	pid=$(pidof $client)
	if [ $? -eq 0 ];then
		echo "client is already runing"
	else
		ip=$(awk -F: '/ip/{print $2}' $conf)
		port=$(awk -F: '/port/{print $2}' $conf)
		nick_name=$(awk -F: '/nick_name/{print $2}' $conf)
		school=$(awk -F: '/school/{print $2}' $conf)
		$client $ip $port $nick_name $school
	fi
}

function StopClient()
{
	pid=$(pidof $client)
	if [ $? -eq 0 ];then
		kill -9 $pid
	else
		echo "client is already stop"
	fi
}

function RestartClient()
{
	StopClient
	StartClient
}


if [ $# -ne 1 ];then
	Usage
fi 

case $1 in 
	-s | "start" )
	StartClient
	;;
	-t | "stop" )
	StopClient
	;;
	-rt | "restart" )
	RestartClient
	;;
esac




