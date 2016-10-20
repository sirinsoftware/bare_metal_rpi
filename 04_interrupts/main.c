#include "arm_timer.h"
#include "interrupt_controller.h"

#define BASE_IO_ADDRESS     0x20000000
#define GPIO_ADDRESS        (BASE_IO_ADDRESS + 0x200000)
#define SYSTIMER_ADDRESS    (BASE_IO_ADDRESS + 0x3000)

#define SELECT_GPIO16_REG   (GPIO_ADDRESS + 0x4)
#define SET_GPIO16_REG      (GPIO_ADDRESS + 0x1C)
#define CLEAR_GPIO16_REG    (GPIO_ADDRESS + 0x28)

#define SELECT_GPIO47_REG   (GPIO_ADDRESS + 0xC)
#define SET_GPIO47_REG      (GPIO_ADDRESS + 0x20)
#define CLEAR_GPIO47_REG    (GPIO_ADDRESS + 0x2C)

#define ARMTIMER_ADDRESS    (BASE_IO_ADDRESS + 0xB400)
#define INTERRUPT_CONTROLLER_ADDRESS (BASE_IO_ADDRESS+ 0xB200)


volatile unsigned int* select_reg = (unsigned int*) SELECT_GPIO16_REG;
volatile unsigned int* set_reg = (unsigned int*) SET_GPIO16_REG;
volatile unsigned int* clear_reg = (unsigned int*) CLEAR_GPIO16_REG;

static ARM_TIMER_T* arm_timer = (ARM_TIMER_T*) ARMTIMER_ADDRESS;
static INTERRUPT_CONTROLLER_T* interrupt_controller = (INTERRUPT_CONTROLLER_T*) INTERRUPT_CONTROLLER_ADDRESS;

extern int __bss_start__;
extern int __bss_end__;

int main(void);

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
	// set FSEL16 to state 001 (output)
    *select_reg |= (unsigned int) (1 << 18);

    // Enable ARM TIMER interrups
    interrupt_controller->Enable_Basic_IRQs = INTERRUPT_CTRL_ARM_TIMER_IRQ;

    // set start delay
    arm_timer->Load = 2000;
    // set mode, 256 prescale and enable timer and its interrupts
    arm_timer->Control = 
                    ARMTIMER_23_BIT_MODE |
                    ARMTIMER_256_PRESCALER |
                    ARMTIMER_CTRL_IRQ_ENABLE |
                    ARMTIMER_CTLR_ENABLE;

    //enable all interupts
    _enable_interrupts();

    return 0;
}

static char on = 1;
/*
 * interrupt handler
 */
void __attribute__((interrupt("IRQ"))) interrupt_vector(void)
{
    // check whether the source is TIMER
    if (interrupt_controller->IRQ_basic_pending & INTERRUPT_CTRL_ARM_TIMER_IRQ)
    {
        // switch LED
        if (on)
        {
            *clear_reg = (unsigned int) (1 << 16);
        }
        else
        {
            *set_reg = (unsigned int) (1 << 16);
        }

        on = !on;
        // clear pending pit for timer
        arm_timer->IRQ_Clear = 1;
    }
}