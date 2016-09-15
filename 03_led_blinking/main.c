#include "system_timer.h"

#define BASE_IO_ADDRESS     0x20000000
#define GPIO_ADDRESS        (BASE_IO_ADDRESS + 0x200000)
#define SYSTIMER_ADDRESS    (BASE_IO_ADDRESS + 0x3000)

#define SELECT_GPIO16_REG   (GPIO_ADDRESS + 0x4)
#define SET_GPIO16_REG      (GPIO_ADDRESS + 0x1C)
#define CLEAR_GPIO16_REG    (GPIO_ADDRESS + 0x28)

// timer frequency 1 Mhz
#define DELAY               500000

volatile unsigned int* select_reg = (unsigned int*) SELECT_GPIO16_REG;
volatile unsigned int* set_reg = (unsigned int*) SET_GPIO16_REG;
volatile unsigned int* clear_reg = (unsigned int*) CLEAR_GPIO16_REG;

static sys_timer_t* sys_timer = (sys_timer_t*) SYSTIMER_ADDRESS;

extern int __bss_start__;
extern int __bss_end__;

int main(void);
void wait_usec(unsigned int usec);

/*
 * Function from C library that automaticaly called before main
 */
void _cstartup(unsigned int r0, unsigned int r1, unsigned int r2)
{
    // clear bss (static uninizialized data)
    int* bss = &__bss_start__;
    while (bss < &__bss_end__)
    {
        *bss++ = 0;
    }

    main();

    for (;;)
    {
    }
}


int main(void)
{
	// set FSEL16 001 output
    *select_reg |= (unsigned int) (1 << 18);

    for (;;)
    {

        // Turn on ACK LED
        *clear_reg = (unsigned int) (1 << 16);

        wait_usec(DELAY);

        // Turn off ACK LED
        *set_reg = (unsigned int) (1 << 16);

        wait_usec(DELAY);
    }
    return 0;
}

// wait function using polling approach
void wait_usec(unsigned int usec)
{
    volatile unsigned int start_time = sys_timer->counter_lo;

    while((sys_timer->counter_lo - start_time) < usec)
        ;
}


