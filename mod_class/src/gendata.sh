#!/bin/sh
# gendata.sh

function print_ij() {
# print j'th field (column) of the i'th line
	awk -v i=$1 -v j=$2 'FNR == i {print $j}' $3
}

function gendata() {
	n=$(($1 - 1))
	fname=$2
	for i in $(seq 0 $n); do
		modulus=`print_ij $((1 + $i / 9)) $((1 + $i % 9)) $fname`
		val=$(($RANDOM % $modulus))
		if [ $val -eq 0 ]; then val=1; fi
		echo $val $modulus
	done | parallel echo {} > output.txt
}

gendata $1 $2
