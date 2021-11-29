	.text
	
JT:	.word Default
	.word case1
	.word case2
	.word case3
	.word case4

	.arch msp430g2553
	.p2align 1,0

	.global argument_for_update_shape1

argument_for_update_shape1:
	cmp #5, R12
	jhs Default

	add R12, R12
	mov JT(R12), R0

case1:
	mov #1, R12
	call #argument_for_update_shape2
	jmp exit

case2:
	mov #2, R12
	call #argument_for_update_shape2
	jmp exit

case3:
	mov #3, R12
	call #argument_for_update_shape2
	jmp exit

case4:
	mov #4, R12
	call #argument_for_update_shape2
	jmp exit

Default:
	jmp exit

exit:	pop r0
