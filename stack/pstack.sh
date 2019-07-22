#!/bin/bash
STACKFILE="pstack"$RANDOM

get_stack(){
	if [ ! -f "/proc/"$2"/stack" ];then
		echo "/proc/"$2"/stack Doesn't exist"
		exit
	fi
	touch $STACKFILE
	if [ -z "$?" ];then
		echo "Failed to create "$STACKFILE
		return
	fi
	rm $STACKFILE

	for i in {0..100}
	do
		for j in `ls "/proc/"$2"/task/"`;do
			cat "/proc/"$2"/task/"$j"/stack" >>$STACKFILE"-"$j
		done
		sleep 0.1
	done
}
clean_stack(){
	rm "$STACKFILE"*
}
stat_stack() {
	for file in `ls $STACKFILE"-"*`;do
		echo "PID:"`echo $file|awk 'BEGIN{FS="-"} {print $2}'`" Stack statistics"
		awk '{ret = gsub("+.*", "",$2);if (ret > 0)array[$2]++;};END{for(k in array){print "\t" array[k],k;}}' $file|sort
	done
}
Usage(){
	echo "\t"$0" pid\n"
	exit
}

main() {
	if [ "$#" -lt 2 ];then
		Usage
	fi
	
	get_stack $0 $2
	
	if [ -z "$?" ];then
		echo "Get "$1" Stack Failed"
		exit
	fi

	stat_stack

	clean_stack
}
main $0 $1
