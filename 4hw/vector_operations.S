#include "vector_operations.h"

#define SSE3_BIT 0x1
#define SSE4_BIT 0x80000

.text
.globl add
.globl sub
.globl what_sse
.globl mul_SSE3
.globl mul_SSE41

// x - in eax
// y - in ebx
// size - in esi
//output - in x 
.type add, @function 
add: 
	pushl %eax
	pushl %ebx
	pushl %esi
	movl 16(%esp), %eax
	movl 20(%esp), %ebx
	movl 24(%esp),%esi 
	
	xorps %xmm1, %xmm1
	xorps %xmm2, %xmm2
a_1: 
	cmpl $4, %esi

	jb ea_1

	movups (%eax), %xmm1
	movups (%ebx), %xmm2
	addps %xmm2, %xmm1
	movups %xmm1, (%eax) 
	
	addl $16, %eax
	addl $16, %ebx 
	subl $4, %esi	
	jmp a_1
ea_1:
	test %esi, %esi
	jz end_add
	movss (%eax), %xmm1
	movss (%ebx), %xmm2
	addss %xmm2, %xmm1
	movss %xmm1, (%eax) 
	addl $4, %eax
	addl $4, %ebx 
	decl %esi	
	jmp ea_1	
end_add:
	pop %esi
	pop %ebx
	pop %esi
	ret
				
// x - in eax
// y - in ebx
// size - in esi
//output - in x  
.type sub, @function
sub: 
	pushl %eax
	pushl %ebx
	pushl %esi
	
	movl 16(%esp), %eax
	movl 20(%esp), %ebx
	movl 24(%esp),%esi

	xorps %xmm1, %xmm1
	xorps %xmm2, %xmm2
s_1: 
	cmpl $4, %esi

	jb es_1

	movups (%eax), %xmm1
	movups (%ebx), %xmm2
	subps %xmm2, %xmm1
	movups %xmm1, (%eax) 
	
	addl $16, %eax
	addl $16, %ebx 
	subl $4, %esi	
	jmp s_1

es_1:
	test %esi, %esi
	jz end_sub
	movss (%eax), %xmm1
	movss (%ebx), %xmm2
	subss %xmm2, %xmm1
	movss %xmm1, (%eax) 
	addl $4, %eax
	addl $4, %ebx 
	decl %esi	
	jmp es_1	
end_sub:
	pop %esi
	pop %ebx
	pop %esi
	ret

//output - in eax
.type what_sse, @function
what_sse:
	pushl %ecx
	pushl %edx
	pushl %ebx

	movl $1, %eax
	cpuid
1: 
	test $SSE3_BIT, %ecx 
	jnz 1f
	movl $NONE_SUPPORTED, %eax
	jmp end_sse
1: 	
	test $SSE4_BIT, %ecx	
	jnz 1f
	movl $SSE3_SUPPORTED, %eax
	jmp end_sse	
1:
	movl $SSE41_SUPPORTED, %eax
end_sse: 
	popl %ebx
	popl %edx
	popl %ecx
	ret

// x - in eax
// y - in ebx
// size - in esi
//output - in st(0)  
.type mul_SSE3, @function
mul_SSE3:
	pushl %eax
	pushl %edi
	pushl %edx
	movl 16(%esp), %eax
	movl 20(%esp), %edi
	movl 24(%esp), %edx
 
	xorps %xmm1, %xmm1
	xorps %xmm2, %xmm2
	xorps %xmm0, %xmm0	
m3_1: 
	test %edx, %edx
	jz em3_1

	movups (%eax), %xmm1
	movups (%edi), %xmm2

	mulps %xmm2, %xmm1
	haddps %xmm1, %xmm1 // a0 + a1 | a3 + a2 | a0 + a1 | a2 + a3 | 
	haddps %xmm1, %xmm1 // a0 + a1 + a3 + a2| ..  | .. | .. |
	addps %xmm1, %xmm0	

	addl $16, %eax
	addl $16, %edi 
	subl $4, %edx	
	jmp m3_1
em3_1:  
	movss %xmm0, (temp)
	fld (temp)
	popl %edx
	popl %edi
	popl %eax
	ret

// x - in eax
// y - in ebx
// size - in esi
//output - in st(0) 
.type mul_SSE41, @function 
mul_SSE41: 
	xorps %xmm1, %xmm1
	xorps %xmm2, %xmm2
	xorps %xmm0, %xmm0	
m41_1: 
	test %edx, %edx
	jz em41_1

	movups (%eax), %xmm1
	movups (%edi), %xmm2

	dpps $0b11110001, %xmm2, %xmm1
	addps %xmm1, %xmm0	

	addl $16, %eax
	addl $16, %edi 
	subl $4, %edx	
	jmp m41_1
em41_1: 
	movss %xmm0, (temp)
	fld (temp)
	ret

.data
temp:
	.space 4 

