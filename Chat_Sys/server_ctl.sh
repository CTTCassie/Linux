#!/bin/bash

ROOT_PATH=$(pwd)
server=$ROOT_PATH/udpServer
conf=$ROOT_PATH/conf/server.conf

function Usage()
{
	printf "Usage: %s [-s(start) | -t(stop) | -rt(restart)]\n" "$0"
}
	
function StartServer()
{
	pid=$(pidof $server)
	if [ $? -eq 0 ];then
		echo  "server is already runing"	
	else
		ip=$(awk -F: '/ip/{print $2}' $conf)
		port=$(awk -F: '/port/{print $2}' $conf)
		$server $ip $port
	fi
}

function StopServer()
{	
	pid=$(pidof $server)
	if [ $? -eq 0 ];then
		kill -9 $pid
	else
		echo "server is already stop"
	fi
}

function RestartServer()
{
	StopServer
	StartServer
}


if [ $# -ne 1 ] ; then
	Usage
fi 

case $1 in 
	-s | 'start' )
	StartServer
	;;
	-t | 'stop' )
	StopServer
	;;
	-rt | 'restart')
	RestartServer
	;;
esac
