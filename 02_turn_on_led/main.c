#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BASE_IO_ADDRESS     0x20000000
#define GPIO_ADDRESS        (BASE_IO_ADDRESS + 0x200000)

#define SELECT_GPIO16_REG     (GPIO_ADDRESS + 0x4)
#define SET_GPIO16_REG         (GPIO_ADDRESS + 0x1C)
#define CLEAR_GPIO16_REG    (GPIO_ADDRESS + 0x28)

#define SELECT_GPIO47_REG   (GPIO_ADDRESS + 0xC)
#define SET_GPIO47_REG      (GPIO_ADDRESS + 0x20)
#define CLEAR_GPIO47_REG    (GPIO_ADDRESS + 0x2C)


volatile unsigned int* select_reg = (unsigned int*) SELECT_GPIO16_REG;
volatile unsigned int* set_reg = (unsigned int*) SET_GPIO16_REG;
volatile unsigned int* clear_reg = (unsigned int*) CLEAR_GPIO16_REG;

extern int __bss_start__;
extern int __bss_end__;

extern char _end; // start of the heap

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
	// set FSEL16 001 output
    *select_reg |= (unsigned int) (1 << 18);

    // Turn on ACK LED
    *clear_reg = (unsigned int) (1 << 16);

    size_t mem_size = 20;
    char *mem = malloc(mem_size);
    if (mem)
    {   
        strncpy(mem, "Hello world", mem_size);
        free(mem);
    }

    for (;;)
    	;

    return 0;
}

void * _sbrk (int incr)
{
    static char *heap_head = &_end;
    char* stack_pointer = 0;
    asm("mov %0, sp" : "=r" (stack_pointer));

    if (heap_head + incr > stack_pointer)
    {
        errno = ENOMEM;
        return NULL;
    }
    else
    {
        char *allocated = heap_head;
        heap_head += incr;
        return (void*) allocated;
    }


}