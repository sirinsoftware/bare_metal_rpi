.section ".text.startup"

.global _start
.global _enable_interrupts


// See ARM section A2.2 (Processor Modes)
.equ    CPSR_MODE_IRQ,          0x12
.equ    CPSR_MODE_SVR,          0x13

// See ARM section A2.5 (Program status registers)
.equ    CPSR_IRQ_INHIBIT,       0x80
.equ    CPSR_FIQ_INHIBIT,       0x40
.equ    CPSR_THUMB,             0x20

// lable that placed to 0x8000 address and 
// it''s where rpi starts its execution
// vector table with addresses of exception handlers
// 
_start:
	ldr pc, _reset_v
	ldr pc, _reset_v
	ldr pc, _reset_v
	ldr pc, _reset_v
	ldr pc, _reset_v
	ldr pc, _reset_v
	ldr pc, _interrupt_vector_v
	ldr pc, _reset_v

// all labels are relative to PC
// so as we going to move vecor table to 0x0000
// relativly should be moved constans that fill the table
_reset_v:	 		.word _reset_
_interrupt_vector_v:.word interrupt_vector

// moved vector table to 0x0000 location
_reset_:
	mov r0, #0x8000
	mov r1, #0x0000
	ldmia r0!, {r2, r3, r4, r5, r6, r7, r8, r9, r10, r11}
	stmia r1!, {r2, r3, r4, r5, r6, r7, r8, r9, r10, r11}

	mov	sp, #(128 * 1024)
	bl	_cstartup

_enable_interrupts:
	mrs r0, cpsr
	bic r0, r0, #CPSR_IRQ_INHIBIT
	msr cpsr_c, r0

	mov pc, lr
