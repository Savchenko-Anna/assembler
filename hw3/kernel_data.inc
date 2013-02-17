GDT:
	 dd  0,0
;           [ LIMIT    |      BASE    | PDLSTYPE   GD0ALIMT | BASE ]
	 db  0FFh, 0FFh, 00h, 00h, 01h, 10011010b, 11001111b, 00	; code	(selector = 8h == 000000000001 0 00b)
	 db  0FFh, 0FFh, 00h, 00h, 01h, 10010010b, 11001111b, 00	; data	(selector = 10h == 000000000010 0 00b)
	 db  0FFh, 0FFh, 00h, 80h, 0Bh, 10010010b, 01000000b, 00	; video	(selector = 18h == 000000000011 0 00b))

TASK0_DESC:	;selector == 000000000100 0 00 == 20h
	 db  067h, 00h	;limit
	 dw  TASK0_TSS	;base
	 db  01h	;base
	 db  10001001b	;PDLSTYPE
	 db  01000000b	;GD0ALIMT
	 db  00h	;limit
TASK1_DESC:	;selector == 000000000101 0 00 == 28h
	 db  067h, 00h	;limit
	 dw  TASK1_TSS	;base
	 db  01h	;base
	 db  10001001b	;PDLSTYPE
	 db  01000000b	;GD0ALIMT
	 db  00h	;limit

TASK2_DESC:	;selector == 000000000101 0 00 == 30h
	 db  067h, 00h	;limit
	 dw  TASK2_TSS	;base
	 db  01h	;base
	 db  10001001b	;PDLSTYPE
	 db  01000000b	;GD0ALIMT
	 db  00h	;limit

GDT_size	equ $-GDT
GDTR:	 
	 dw GDT_size-1
	 dd GDT

TASK0_TSS	db	68h dup(0)

TASK1_TSS:
	dd	0, 0, 0, 0, 0, 0, 0, 0
	dd	task1
	dd	0, 0, 0, 0, 0, 0x8000, 0, 0, 0
	dd	18h, 8h, 10h, 10h, 0, 0, 0, 0

TASK2_TSS:
	dd	0, 0, 0, 0, 0, 0, 0, 0
	dd	task2
	dd	0, 0, 0, 0, 0, 0xD000, 0, 0, 0
	dd	18h, 8h, 10h, 10h, 0, 0, 0, 0


IDT:
	 dd 0,0 ; 0
	 dw syscall_handler, 08h, 1000111000000000b, 0	 ; 1
	 dd 0,0 ; 2
	 dd 0,0 ; 3
	 dd 0,0 ; 4
	 dd 0,0 ; 5
	 dd 0,0 ; 6
	 dd 0,0 ; 7
	 dw int8_handler, 08h, 1000111000000000b, 0   ; 8  (IRQ 0 - timer)
	 dw int9_handler, 08h, 1000111000000000b, 0   ; 9  (IRQ 1 - keyboard)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 10  (IRQ 2 - slave interrupt controller)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 11  (IRQ 3 - COM2)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 12  (IRQ 4 - COM1)
	 dw exGP_handler, 08h, 1000111000000000b, 0   ; 13  #GP - panic
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 14  (IRQ 6 - FDD)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 15  (IRQ 7 - LPT1)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 16 (8)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 17 (9)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 18 (10)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 19 (11)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 20 (12)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 21 (13)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 22 (14)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 23 (15)
IDT_size	equ $-IDT
IDTR:	
	dw IDT_size-1
	dd IDT

; Scan-codes to ASCII
ascii	db 0,'1234567890-+',0,0,'QWERTYUIOP[]',0,0,'ASDFGHJKL;',"'`",0,0,'ZXCVBNM,./',0,'*',0,' ',0, 0,0,0,0,0,0,0,0,0,0, 0,0, '789-456+1230.', 0,0

; Misc data
gp db 'GENERAL PROTECTION FAULT',0
cursor	dd 0
timer	dd 0
buffer	rw 100
color	dd 00h

