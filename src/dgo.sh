# Usage: ./dgo.sh [file] [args]

make && ./gocompiler < $1 > dgo # compile
lli dgo $2 $3 $4 $5 $6 # run. up to 5 arguments
rm dgo # delete
