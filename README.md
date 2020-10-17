# list_fifo

* singly linked queue, queue size unlimited
* lockless implementation
* multi/single producer enqueue
* single consumer dequeue

# refers:

* https://www.cnblogs.com/zylthinking/p/5975710.html
* DPDK RTE RING, fixed-size queue, lockless, multi/single producer/producer bulk operate, 
* <sys/queue.h> STAILQ

test with gcc test.c -o test -pthread -O3 && ./test
