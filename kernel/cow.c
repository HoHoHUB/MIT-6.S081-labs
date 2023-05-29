#include "types.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

struct {
    uint8 ref_cnt;
    struct spinlock lock;
} cow[(PHYSTOP - KERNBASE) >> 12];

void increfcnt(uint64 pa) {
    if (pa < KERNBASE) {
        return;
    }
    pa = (pa - KERNBASE) >> 12;
    acquire(&cow[pa].lock);
    ++cow[pa].ref_cnt;
    release(&cow[pa].lock);
}

uint8 decrefcnt(uint64 pa) {
    uint8 ret;
    if (pa < KERNBASE) {
        return 0;
    }
    pa = (pa - KERNBASE) >> 12;
    acquire(&cow[pa].lock);
    ret = --cow[pa].ref_cnt;
    release(&cow[pa].lock);
    return ret;
}