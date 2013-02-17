use32
clear_screen:
        mov   dword [cursor], 320
        
        push   eax
        push   ecx
        push   edi

        xor    ecx, ecx
        mov    cx,1500    ; symbols
        xor    edi, edi
        mov    al,32
        mov    ah,07h
        rep stosw

        mov   dword [cursor], 320

        pop     edi
        pop     ecx
        pop     eax

        ret


print_timer:
	push eax
	push ebx
	push ecx
	push edx

	mov ebx, 10
	mov ecx, 1
	xor edx, edx
@@:
	div ebx
	mov dh, 07h
	add dl, 30h
	mov [buffer+2*ecx], dx
	inc ecx
	xor edx, edx
	or eax, eax
	jnz @b	
	
	xor ebx, ebx
	dec ecx

@@:
	mov ax, [buffer+2*ecx]
	mov [es:2*ebx], ax
	dec ecx
	inc ebx
	or ecx, ecx
	jnz @b

@@:
	pop edx
	pop ecx
	pop ebx
	pop eax
	ret

