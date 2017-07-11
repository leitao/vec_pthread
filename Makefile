all:
	gcc -O0 -g vector.c -lpthread
	gcc -O0 -g fp.c  -o fp -lpthread
