	.arch armv7-a
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"utility.c"
	.text
	.align	2
	.global	initServer
	.type	initServer, %function
initServer:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, lr}
	mov	r1, #1
	sub	sp, sp, #36
	mov	r6, r0
	mov	r7, r2
	mov	r0, #2
	mov	r2, #0
	bl	socket(PLT)
	cmn	r0, #1
	mov	r5, r0
	beq	.L2
	mov	r3, #8
	mov	r1, #1
	str	r3, [sp]
	mov	r2, #20
	add	r3, sp, r3
	mov	r4, #0
	movw	ip, #34464
	str	r4, [sp, #8]
	movt	ip, 1
	str	ip, [sp, #12]
	bl	setsockopt(PLT)
	str	r4, [sp, #16]
	mov	r2, #2
	mov	r3, r6	@ movhi
	str	r4, [sp, #20]
	str	r4, [sp, #24]
	str	r4, [sp, #28]
	strh	r2, [sp, #16]	@ movhi
#APP
@ 20 "utility.c" 1
	rev16 r3, r3
@ 0 "" 2
	mov	r2, #16
	mov	r0, r5
	add	r1, sp, r2
	strh	r3, [sp, #18]	@ movhi
	bl	bind(PLT)
	cmn	r0, #1
	beq	.L4
	mov	r1, r7
	mov	r0, r5
	bl	listen(PLT)
	cmn	r0, #1
	movne	r0, r5
	mvneq	r0, #2
.L2:
	add	sp, sp, #36
	@ sp needed
	ldmfd	sp!, {r4, r5, r6, r7, pc}
.L4:
	mvn	r0, #1
	b	.L2
	.size	initServer, .-initServer
	.align	2
	.global	err_quit
	.type	err_quit, %function
err_quit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L8
	ldr	r2, .L8+4
.LPIC0:
	add	r3, pc, r3
	stmfd	sp!, {r4, lr}
	ldr	r4, [r3, r2]
	add	r4, r4, #168
	mov	r1, r4
	bl	fputs(PLT)
	mov	r1, r4
	mov	r0, #10
	bl	fputc(PLT)
	mov	r0, #1
	bl	exit(PLT)
.L9:
	.align	2
.L8:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC0+8)
	.word	__sF(GOT)
	.size	err_quit, .-err_quit
	.align	2
	.global	kbhit
	.type	kbhit, %function
kbhit:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, lr}
	sub	sp, sp, #32
	add	r5, sp, #12
	mov	r0, #0
	movw	r1, #21509
	mov	r4, r0
	mov	r2, r5
	bl	ioctl(PLT)
	ldrh	r6, [sp, #18]
	mov	r2, r5
	movw	r1, #21510
	mov	r0, r4
	bic	r3, r6, #2
	ldrb	r8, [sp, #27]	@ zero_extendqisi2
	strh	r3, [sp, #18]	@ movhi
	ldrb	r7, [sp, #26]	@ zero_extendqisi2
	strb	r4, [sp, #27]
	strb	r4, [sp, #26]
	bl	ioctl(PLT)
	mov	r0, r4
	mov	r1, sp
	mov	r2, #9
	bl	read(PLT)
	cmp	r0, r4
	ble	.L13
	mov	r0, r4
	mov	r2, r5
	movw	r1, #21510
	strh	r6, [sp, #18]	@ movhi
	strb	r8, [sp, #27]
	strb	r7, [sp, #26]
	bl	ioctl(PLT)
	mov	r0, #1
	add	sp, sp, #32
	@ sp needed
	ldmfd	sp!, {r4, r5, r6, r7, r8, pc}
.L13:
	mov	r2, r5
	mov	r0, r4
	movw	r1, #21510
	strh	r6, [sp, #18]	@ movhi
	strb	r8, [sp, #27]
	strb	r7, [sp, #26]
	bl	ioctl(PLT)
	mov	r0, r4
	add	sp, sp, #32
	@ sp needed
	ldmfd	sp!, {r4, r5, r6, r7, r8, pc}
	.size	kbhit, .-kbhit
	.align	2
	.global	getch
	.type	getch, %function
getch:
	@ args = 0, pretend = 0, frame = 72
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, lr}
	sub	sp, sp, #72
	movw	r1, #21505
	mov	r0, #0
	mov	r2, sp
	ldr	r5, .L15
	bl	ioctl(PLT)
	mov	lr, sp
	ldmia	lr!, {r0, r1, r2, r3}
	add	ip, sp, #36
.LPIC1:
	add	r5, pc, r5
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	ldr	r6, [sp, #48]
	bic	r6, r6, #10
	str	r6, [sp, #48]
	ldr	lr, [lr]
	stmia	ip!, {r0, r1, r2, r3}
	add	r2, sp, #36
	movw	r1, #21506
	mov	r0, #0
	str	lr, [ip]
	bl	ioctl(PLT)
	ldr	r3, .L15+4
	ldr	r0, [r5, r3]
	bl	getc(PLT)
	mov	r2, sp
	movw	r1, #21506
	mov	r5, r0
	mov	r0, #0
	bl	ioctl(PLT)
	mov	r0, r5
	add	sp, sp, #72
	@ sp needed
	ldmfd	sp!, {r4, r5, r6, pc}
.L16:
	.align	2
.L15:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC1+8)
	.word	__sF(GOT)
	.size	getch, .-getch
	.align	2
	.global	printIP
	.type	printIP, %function
printIP:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	mov	r0, #2
	mov	r2, #0
	stmfd	sp!, {r4, r5, r6, r7, lr}
	mov	r1, r0
	sub	sp, sp, #12
	bl	socket(PLT)
	mov	r3, #960
	mov	r2, #0
	str	r3, [sp]
	str	r2, [sp, #4]
	mov	r7, r0
	mov	r0, r3
	bl	malloc(PLT)
	mov	r2, sp
	movw	r1, #35090
	str	r0, [sp, #4]
	mov	r0, r7
	bl	ioctl(PLT)
	cmp	r0, #0
	blt	.L23
.L18:
	ldmia	sp, {r3, r5}
	mov	r4, #0
	ldr	r6, .L24
	cmp	r3, #0
.LPIC3:
	add	r6, pc, r6
	ble	.L20
.L21:
	add	r3, r5, r4
	add	r4, r4, #32
	ldr	r0, [r3, #20]
	bl	inet_ntoa(PLT)
	mov	r1, r0
	mov	r0, r6
	bl	printf(PLT)
	ldr	r3, [sp]
	cmp	r3, r4
	bgt	.L21
.L20:
	mov	r0, r7
	bl	close(PLT)
	add	sp, sp, #12
	@ sp needed
	ldmfd	sp!, {r4, r5, r6, r7, pc}
.L23:
	ldr	r0, .L24+4
.LPIC2:
	add	r0, pc, r0
	bl	perror(PLT)
	b	.L18
.L25:
	.align	2
.L24:
	.word	.LC1-(.LPIC3+8)
	.word	.LC0-(.LPIC2+8)
	.size	printIP, .-printIP
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"SIOCGIFCONF \000"
	.space	3
.LC1:
	.ascii	"IP    %s\012\000"
	.ident	"GCC: (GNU) 4.8"
	.section	.note.GNU-stack,"",%progbits
