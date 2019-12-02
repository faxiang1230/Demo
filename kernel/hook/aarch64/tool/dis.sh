#!/bin/bash
ins=$1
TMPFILE=a.out
ins1=${ins:0:2}
ins2=${ins:2:2}
ins3=${ins:4:2}
ins4=${ins:6:2}
do_exit()
{
	rm ${TMPFILE}
	exit
}
:<<!
echo ${ins1} $ins2 $ins3 $ins4
echo -e -n "\x${ins1}\x${ins2}\x${ins3}\x${ins4}" > ${TMPFILE}
objdump -D -b binary -m aarch64 ${TMPFILE} >/dev/null && objdump -D -b binary -m aarch64 ${TMPFILE} && do_exit
!

echo -e -n "\x${ins4}\x${ins3}\x${ins2}\x${ins1}" > ${TMPFILE} 
objdump -D -b binary -m aarch64 ${TMPFILE} >/dev/null && objdump -D -b binary -m aarch64 ${TMPFILE} && do_exit
echo "failed dis"

