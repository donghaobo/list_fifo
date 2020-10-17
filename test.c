#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "list_fifo.h"

int stop = 1;
enum {
    thread_cnt = 5,
};
struct fifo_entry {
    LIST_FIFO_ENTRY(fifo_entry) l;
};
LIST_FIFO_HEAD(lh, fifo_entry) head[thread_cnt];

void *fifo_push(void* data)
{
    struct lh *lh = data;
    while (stop) sched_yield();
    while (!stop) {
        struct fifo_entry *e = NULL;
        e = LIST_FIFO_POP(lh, l);
        if (!e) {
            continue;
        }
        LIST_FIFO_PUSH(&head[0], e, l);
    }
    return NULL;
}

void *fifo_pop(void* data)
{
    int i = 0;
    while (stop) sched_yield();
    while (!stop) {
        struct fifo_entry *e = NULL;
        e = LIST_FIFO_POP(&head[0], l);
        if (!e) {
            continue;
        }
        LIST_FIFO_PUSH(&head[i % thread_cnt], e, l);
        i++;
    }
}

struct fifo_entry ge[20];
int main(int argc, char *argv[])
{
    pthread_t tid;
    int i = 0;
    LIST_FIFO_INIT(&head[0]);
    for (i = 0; i < 20; i++) {
        LIST_FIFO_PUSH(&head[0], &ge[i], l);
    }
    
    pthread_create(&tid, NULL, fifo_pop, NULL);
    for (i = 1; i < thread_cnt;i++) {
        LIST_FIFO_INIT(&head[i]);
        pthread_create(&tid, NULL, fifo_push, &head[i]);
    }

    stop = 0;
    usleep(1000*1000*10);
    stop = 1;
    usleep(10000);
    int total = 0;
    for (i = 0; i < thread_cnt; i++) {
        int cnt = 0;
        while (LIST_FIFO_POP(&head[i], l)) {
            cnt++;
        }
        total += cnt;
        printf("%d:%d ", i, cnt);
    }
    printf("\n total %d\n", total);
    return 0;
}

