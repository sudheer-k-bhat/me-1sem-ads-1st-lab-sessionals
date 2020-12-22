#!/bin/sh
rm -rf bin/out
gcc -D QUEUE_CONTENT_TYPE="void*" -D SLIST_CONTENT_TYPE="void*" \
-I headers/ \
sources/queue.c sources/slist.c \
sources/trading.c sources/main.c -o bin/out
bin/out