#!/bin/sh
rm -rf bin/out
gcc -D QUEUE_CONTENT_TYPE="void*" -D SLIST_CONTENT_TYPE="void*" \
-I headers/ -I lib/sds -I lib/q -I lib/list \
lib/q/queue.c lib/list/slist.c lib/sds/sds.c \
sources/trading.c sources/main.c -o bin/out
bin/out