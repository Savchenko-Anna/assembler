        org	100h

        ;;  for MUL
        mov	di, arg1
        mov	cx, numsize
        call	read_long

        mov     di, arg2
        mov     cx, numsize
        call    read_long

        mov     si, arg1
        mov     di, result
        mov     bp, arg2
        mov     cx, numsize
        call    mul_2long
                
        mov     di, result
        mov     cx, numsize*2
        call    print_long
        ;; end MUL

exit:   
        mov	ax, 4c00h
        int	21h
        
include 'forall.inc'
include 'sub.inc'
include 'mul.inc'        
        
result          rw      numsize*2
        