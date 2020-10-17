#ifndef __LIST_FIFO_H__
#define __LIST_FIFO_H__

#define _LIST_FIFO_HEAD(name, type, qual)\
struct name {\
    qual type *lfh_first;\
    qual type *qual *lfh_last;\
}
#define LIST_FIFO_HEAD(name, type) _LIST_FIFO_HEAD(name, struct type,)

#define LIST_FIFO_HEAD_INITIALIZER(head)\
    { NULL, &(head).lfh_first }

#define _LIST_FIFO_ENTRY(type, qual)\
struct {\
    qual type *lfe_next;\
}
#define LIST_FIFO_ENTRY(type) _LIST_FIFO_ENTRY(struct type,)

#define LIST_FIFO_INIT(head) do {\
    (head)->lfh_first = NULL;\
    (head)->lfh_last = &((head)->lfh_first);\
} while (/*CONSTCOND*/0)

#define LIST_FIFO_PUSH(head, elm, field) do {\
    (elm)->field.lfe_next = NULL;\
    typeof(elm)*__tmp__ = __sync_lock_test_and_set(&((head)->lfh_last), &((elm)->field.lfe_next));\
    *__tmp__ = (elm);\
} while (/*CONSTCOND*/0)

#define LIST_FIFO_POP(head, field) ({\
    typeof((head)->lfh_first) __tmp__ = NULL;\
    if ((head)->lfh_first) {\
        __tmp__ = (head)->lfh_first;\
        if (!__tmp__->field.lfe_next) {\
            (head)->lfh_first = NULL;\
            typeof((head)->lfh_last) __tmp2__ = &__tmp__->field.lfe_next;\
            if (!__sync_bool_compare_and_swap(&((head)->lfh_last), __tmp2__, &((head)->lfh_first))) {\
                while (!*(void*volatile*)&__tmp__->field.lfe_next) {\
                     /* sched_yield(); */\
                }\
                (head)->lfh_first = __tmp__->field.lfe_next;\
            }\
        } else {\
            (head)->lfh_first = __tmp__->field.lfe_next;\
        }\
    }\
    __tmp__;\
})

#define LIST_FIFO_EMPTY(head) ((head)->lfh_first == NULL)

#endif /* __LIST_FIFO_H__ */

