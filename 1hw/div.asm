        org	100h

        ;; for DIV
        mov     di, arg1
        mov     cx, numsize
        call    read_long

        mov     di, arg2
        mov     cx, numsize
        call    read_long

        mov     di, arg1
        mov     si, arg2
        mov     bx, result
        mov     cx, numsize
        call    div_2long

        call    print_long

        mov     di, result
        mov     cx, numsize
        call	print_long
        ;; end DIV
exit:   
        mov	ax, 4c00h
        int	21h
        
        
        ;; ds:di - dividend
        ;; ds:si - divisor
        ;; bx - quotient
        ;; cx - length in words
        ;; returns remainder in di
div_2long:

        ;; this uses following algorithm:
        ;; t - dividend
        ;; p - divisor
        ;; q - quotient
        ;; if ( t < p ) return {0,t}
	;; i = 0
	;; while(t > p){
	;;         p <<= 4;
	;;         i++;
	;; }
	;; while(i > 0){
	;;         p >>= 4;
	;;         n = find_max_factor(t,p);
	;;         if (n){
	;;            p *= n;
	;;            t -= p;
	;;            p /= n;
	;;         }   
	;;         q <<= 4;
	;;         q += n;
	;; }
        push    di
        push    bp
        mov     bp, sp
        
        pusha                   
        
        mov     di, bx
        mov     ax, ds
        mov     es, ax
        call    fill_zeros
        mov     di, [bp+2]

        ;; if (t < p) return
        call    cmp_long
        ja      div_end

        call    find_max_shift
@@:
        or      ax, ax
        jz      @f

        ;; p >>= 4
        mov     di, si
        push    word 4
        call    shr_long
        add     sp, 2
        mov     di, [bp+2]

        call    find_max_factor ;result in dx
        or      dx, dx
        jz      div_no_sub

        ;; p *= n
        xchg    bx, dx
        mov     di, si
        call    mul_long
        xchg    bx, dx

        ;; t -= p
        mov     di, [bp+2]
        call    sub_2long

        ;; p /= n
        xchg    di, si
        xchg    bx, dx
        push    dx
        call    div_long
        pop     dx
        xchg    bx, dx
        xchg    di, si
        
div_no_sub:     
        ;; q >>= 4
        mov     di, bx
        push    word 4
        call    shl_long
        add     sp, 2

        ;; q += n
        xchg    bx, dx
        call    add_long
        xchg    bx, dx

        dec     ax
        jmp     @b
@@:
div_end:
        popa
        pop     bp
        pop     di
        ret
        
        ;; ds:di - dividend
        ;; ds:si - divisor
        ;; cx
        ;; returns ax
find_max_shift:
        push dx
        
        xor     ax,ax
@@:
        call    cmp_long
        ja      @f

        xchg    di, si
        push    word 4
        call    shl_long
        add     sp, 2
        xchg    di, si

        inc     ax
        jmp     @b
@@:
        pop     dx
        ret

;; ds:di - dividend
;; ds:si - divisor
;; cx
;; returns dx        
find_max_factor:
        push    ax
        push    bx

        xor     dx, dx          ;down
        mov     ax, 10h          ;up
        
        call    cmp_long
        ja      end_find_max_factor
                
@@:
        mov     bx, ax
        dec     bx
        cmp     bx, dx
        jbe     @f

        mov     bx, dx
        add     bx, ax
        shr     bx, 1

        xchg    di, si
        call    mul_long
        xchg    di, si

        call    cmp_long        ;cmp si,di
        jz      mid_finded
        ja      mid_is_bigger
        
        mov     dx, bx
        jmp     end_switch
mid_is_bigger:    
        mov     ax, bx
        jmp     end_switch
mid_finded:
        mov     ax, bx
        mov     dx, ax
end_switch:
        xchg    di, si
        push    dx
        call    div_long
        pop     dx
        xchg    di, si
        
        jmp     @b
@@:
end_find_max_factor:    
        pop     bx
        pop     ax
        ret
        


;; ds:di - long number
;; cx    - length
;; first argument on the stack - counter
shl_long:
        push    bp
        mov     bp, sp
        
        push    dx
        
        mov     dx, [bp+4]
@@:                
        or      dx, dx
        jz      @f

        call    shl_long_once

        dec     dx
        jmp     @b
@@:        
        pop     dx
        pop     bp
        ret
        
;; ds:di - long number
;; cx    - length
shl_long_once:
        push    di
        push    cx
        push    dx
        push    ax

        
        xor     dx,dx
@@:        
        or      cx,cx
        jz      @f

        xor     ax,ax
        shl     word [di], 1
        adc     ax, 0
        add     [di], dx
        mov     dx, ax

        add     di, 2
        dec     cx
        jmp     @b
@@:
        pop     ax
        pop     dx
        pop     cx
        pop     di
        ret
        
;; ds:di - long number
;; cx    - length
;; first argument on the stack - counter
shr_long:
        push    bp
        mov     bp, sp
        
        push    dx
        
        mov     dx, [bp+4]
@@:                
        or      dx, dx
        jz      @f

        call    shr_long_once

        dec     dx
        jmp     @b
@@:        
        pop     dx
        pop     bp
        ret

;; ds:di - long number
;; cx    - length
shr_long_once:
        push    di
        push    cx
        push    ax
        push    dx
        
        shl     cx, 1
        add     di, cx
        sub     di, 2
        shr     cx, 1

        xor     dx, dx
 @@:
        xor     ax, ax
        
        ;; or      cx, cx
        ;; jz      @f
        jcxz    @f

        shr     word [di], 1
        adc     ax, 0
        add     [di], dx
        
        shl     ax, 15        
        mov     dx, ax

        dec     cx
        sub     di, 2
        
        jmp     @b
@@:
        pop     dx
        pop     ax
        pop     cx
        pop     di        
        ret
        
;; ds:di
;; ds:si
;; cx
cmp_long:
        push    di
        push    si
        push    es
        push    cx
        
        push    ds
        pop     es

        add     di, numsize*2-2
        add     si, numsize*2-2
        std
        repe    cmpsw

        pop     cx
        pop     es
        pop     si
        pop     di
        ret
        
include 'forall.inc'
include 'sub.inc'
include 'mul.inc'        
        
result          rw      numsize