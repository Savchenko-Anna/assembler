use32
task_switcher:
	; disable interrupts
	cli
	in al, 70h
	or al, 80h
	out 70h, al	

	push ax

	mov word ax, [TASK1_DESC + 40]
	test ax, 00000010b
	jnz to2

to1:
	pop ax
    	and word [TASK2_DESC + 40], 11111101b
	jmp task1

to2:
	pop ax
	and word [TASK1_DESC + 40], 11111101b	
	jmp task2


task1:
	in al, 70h
	and al, 7Fh
	out 70h, al
	sti
	call clear_screen

@@:	
	mov dword [cursor], 260
	or word [TASK1_DESC + 40],2	
   
	mov edi, dword [cursor]
	shl edi, 1
	mov byte [es:edi], 'a'
	inc edi
	inc byte [es:edi]

	jmp @b


task2:
	in al, 70h
	and al, 7Fh
	out 70h, al
	sti
	call clear_screen

@@:
	mov dword [cursor], 280
	or word [TASK2_DESC + 40],2

	mov edi, dword [cursor]
	shl edi, 1
	mov byte [es:edi], 'b'
	inc edi
	inc byte [es:edi]
    
	jmp @b


