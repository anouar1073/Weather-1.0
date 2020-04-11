FILE=./httpGet
if test ! -f "$FILE" ; then
    make
fi

./linkGet &
./httpGet &
./main

