#!/bin/sh

./test $1
/bin/dot -Tpng results.dot -o outgraph.png
