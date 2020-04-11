all: main httpGet linkGet
main: main.c 
	gcc -o main main.c 
linkGet: linkGet.c
	gcc -o linkGet linkGet.c
httpGet: httpGet.c
	gcc -o httpGet httpGet.c -lcurl
