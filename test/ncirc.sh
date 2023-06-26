#!/bin/bash

./proxy/simple-tcp-proxy 1025 | tee output_pipe | tee output_file &
PID=$?

function exit_properly()
{
	if [ -e input ]
	then
		rm -f input_pipe
	fi

	if [ -e output ] 
	then
		rm -f output_pipe
	fi

	kill -9 $1
	exit
}

function autopong()
{
	while true
	do
		read -r line
		if [[ $line == "PING "* ]]
		then
			echo "PING DETECTED"
			ping_code="${line#PING }"
			response="PONG $ping_code"
			echo "$response" > input_pipe
		fi
	done < output_pipe
}


trap "exit_properly $PID" SIGINT

trap -p SIGINT

if [ ! -e input_pipe ]
then
	mkfifo input_pipe
fi

if [ ! -e output_pipe ]
then
	mkfifo output_pipe
fi

sleep 1


nc localhost 1025 < input_pipe &> /dev/null &

autopong &

echo "nbr of argument $#"

if [ $# -gt 0 ]
then
	cat "$1" - > input_pipe
else
	cat > input_pipe
fi
