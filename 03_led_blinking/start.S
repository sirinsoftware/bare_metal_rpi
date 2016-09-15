.section ".text.startup"

.global _start

_start:
	mov	sp, #(128 * 1024)
	bl	_cstartup
	
