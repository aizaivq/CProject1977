main:main.c
	gcc main.c -o main  -Iinclude   -lavcodec  -lavformat -lavcodec -lavutil -lavdevice -lavfilter -lswresample  -Wl,-Bdynamic  -lm -lz -lpthread  -lbz2

