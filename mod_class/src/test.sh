#!/bin/sh
# test modular inverses
TXTFILE=primes-100008.txt
INCR=20	# number of arguments; must be even
PRIMES=19999	# index of primes to test

function print_ij() {
# print j'th field (column) of the i'th line
	awk -v i=$1 -v j=$2 'FNR == i {print $j}' $TXTFILE
}

for i in $(seq 0 $INCR $PRIMES); do
	args=""
	for j in $(seq $i $(($i+20))); do
		modulus=`print_ij $((1 + $j / 9)) $((1 + $j % 9))`
		val=$(($RANDOM % $modulus))
		if [ $val -eq 0 ]; then val=1; fi
		args=$args" $val $modulus"
	done
	echo $args
done | parallel --colsep ' ' ./mod
