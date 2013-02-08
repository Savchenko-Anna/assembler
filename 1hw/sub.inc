; ds:di - long number 1
; ds:si - long number 2
; cx    - length in words
; result is stored in ds:di
; function: [di] = [di] - [si]
sub_2long:
        push    di
        push    cx
        push    si

        xchg    di, si        
        call    neg_long
        xchg    di, si

        call    add_2long

        xchg    di, si
        call    neg_long

        pop     si
        pop     cx
        pop     di
        ret

; ds:di - long number
; cx - length in words
neg_long:
        push    di
        push    cx
        push    dx
        push    ax
        push    bx

        mov     dx,1                ;overflow flag
@@:
        or      cx,cx
        jz      @f
        
        xor     bx,bx

        
        mov     ax,[di]
        not     ax
        add     ax,dx
        adc     bx,0
        
        mov     dx,bx
        mov     [di],ax
        
        add     di,2
        dec     cx
        jmp     @b
@@:
        pop     bx
        pop     ax
        pop     dx
        pop     cx
        pop     di

        ret

        