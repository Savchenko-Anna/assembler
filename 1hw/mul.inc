; ds:si - long number
; ds:di - result
; cx    - length in words
; bx    - multiplier
; dx    - shift in words
mul_long_shift:
        pusha
        mov     bp, dx
        shl     bp, 1
        ;; TODO: nulling if necessary
@@:
        test    cx,cx
        jz      @f

        mov     ax, [si]
        mul     bx
        add     ax, [di+bp]
        adc     dx, [di+bp+2]
        mov     [di+bp], ax
        mov     [di+bp+2], dx

        add     si, 2
        add     di, 2

        dec     cx
        jmp     @b
 @@:
        popa
        ret
        
        
;; si - factor 1
;; bp - factor 2
;; di - product
;; cx - length in words
mul_2long:
        pusha

        shl     cx, 1
        
        push    ds
        pop     es
        call    fill_zeros
        shr     cx, 1
                
        xor     dx, dx
@@:
        ;; while(dx < cx)
        cmp     dx,cx
        jae     @f

        mov     bx, [bp]
        call    mul_long_shift

        inc     dx
        add     bp, 2
        jmp     @b        
@@:      
        popa
        ret
