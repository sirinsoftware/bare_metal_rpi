#ifndef ARM_TIMER_H
#define ARM_TIMER_H

#include <stdint.h>

#define ARMTIMER_CTLR_ENABLE    		(1 << 7)
#define ARMTIMER_CTRL_IRQ_ENABLE		(1 << 5)
#define ARMTIMER_256_PRESCALER			(1 << 3)
#define ARMTIMER_23_BIT_MODE    		(1 << 1)


typedef struct 
{
    volatile uint32_t Load;
    volatile uint32_t Value;
    volatile uint32_t Control;
    volatile uint32_t IRQ_Clear;
    volatile uint32_t RAW_IRQ;
    volatile uint32_t Masked_IRQ;
    volatile uint32_t Reload;
    volatile uint32_t Pre_divider;
    volatile uint32_t Free_counter;
} ARM_TIMER_T;

#endif