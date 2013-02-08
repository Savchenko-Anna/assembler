        org	100h

        ;; for SUB
        mov	di, arg1       
        mov	cx, numsize
        call	read_long

        mov	di, arg2
        mov	cx, numsize
        call	read_long

        
        mov	si, arg1
        xchg    di, si
        mov	cx, numsize
        call	sub_2long
        call	print_long
        ;; end SUB

exit:   
        mov	ax, 4c00h
        int	21h
        
include 'forall.inc'
include 'sub.inc'
        
result          rw      numsize
        