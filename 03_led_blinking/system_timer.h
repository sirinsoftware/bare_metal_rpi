#ifndef SYSTEM_TIMER_H
#define SYSTEM_TIMER_H


typedef struct {
    volatile unsigned int control_status;
    volatile unsigned int counter_lo;
    volatile unsigned int counter_hi;
    volatile unsigned int compare0;
    volatile unsigned int compare1;
    volatile unsigned int compare2;
    volatile unsigned int compare3;
} sys_timer_t;

#endif