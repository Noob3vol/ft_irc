#!/bin/bash

NAME=ircserv

function exit_properly()
{
	if [ -e input_pipe ]
	then
		rm -f input_pipe
	fi

	if [ -e output_pipe ] 
	then
		rm -f output_pipe
	fi

	kill -9 $1
	kill -9 $2
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



trap -p SIGINT

if [ ! -e ../${NAME} ]
then
	make -C ../
fi

../ircserv "ircd" "123" &> /dev/null &
PID_SERV=$?
./proxy/myproxy 1025 | tee output_pipe | tee output_file &
PID_CLIENT=$?

trap "exit_properly ${PID_SERV} ${PID_CLIENT}" SIGINT

if [ ! -e input_pipe ]
then
	mkfifo input_pipe
fi

if [ ! -e output_pipe ]
then
	mkfifo output_pipe
fi

sleep 1

nc -C localhost 1025 < input_pipe &> /dev/null &

autopong &

echo "nbr of argument $#"

find Input -type f | cat <(echo " -") | xargs cat > input_pipe

#if [ $# -gt 0 ]
#then
#	cat "$1" - > input_pipe
#else
#	cat > input_pipe
#fi
