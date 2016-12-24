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
	.file	"CServer.c"
	.section	.text.startup,"ax",%progbits
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
	sub	sp, sp, #24
	bl	printIP(PLT)
	movw	r0, #7777
	mov	r1, #1024
	mov	r2, #5
	bl	initServer(PLT)
	ldr	r9, .L15
.LPIC4:
	add	r9, pc, r9
	cmn	r0, #1
	mov	r4, r0
	beq	.L10
	cmn	r0, #2
	beq	.L11
	cmn	r0, #3
	beq	.L12
.L3:
	ldr	r0, .L15+4
	add	r6, sp, #8
	add	r5, sp, #4
	mov	r8, #16
.LPIC3:
	add	r0, pc, r0
	bl	puts(PLT)
	b	.L7
.L14:
	str	r8, [sp, #4]
	bl	accept(PLT)
	cmp	r0, #0
	bgt	.L13
.L7:
	bl	kbhit(PLT)
	mov	r1, r6
	mov	r2, r5
	subs	r7, r0, #0
	mov	r0, r4
	beq	.L14
	ldr	r0, .L15+8
.LPIC5:
	add	r0, pc, r0
	bl	puts(PLT)
	ldr	r3, .L15+12
	mov	r0, r4
	mov	r2, #0
	ldr	r3, [r9, r3]
	str	r2, [r3]
	bl	close(PLT)
	add	sp, sp, #24
	@ sp needed
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, pc}
.L13:
	ldr	r2, .L15+16
	mov	r3, r0
	mov	r1, r7
	mov	r0, sp
	ldr	r2, [r9, r2]
	bl	pthread_create(PLT)
	b	.L7
.L10:
	ldr	r0, .L15+20
.LPIC0:
	add	r0, pc, r0
	bl	err_quit(PLT)
	b	.L3
.L11:
	ldr	r0, .L15+24
.LPIC1:
	add	r0, pc, r0
	bl	err_quit(PLT)
	b	.L3
.L12:
	ldr	r0, .L15+28
.LPIC2:
	add	r0, pc, r0
	bl	err_quit(PLT)
	b	.L3
.L16:
	.align	2
.L15:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC4+8)
	.word	.LC4-(.LPIC3+8)
	.word	.LC5-(.LPIC5+8)
	.word	program_run(GOT)
	.word	recvThread(GOT)
	.word	.LC1-(.LPIC0+8)
	.word	.LC2-(.LPIC1+8)
	.word	.LC3-(.LPIC2+8)
	.size	main, .-main
	.text
	.align	2
	.global	printPacketInfo
	.type	printPacketInfo, %function
printPacketInfo:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, lr}
	sub	sp, sp, #12
	ldrb	r1, [r0, #3]	@ zero_extendqisi2
	mov	r4, #0
	ldrb	r7, [r0, #4]	@ zero_extendqisi2
	mov	ip, #2
	ldrb	r2, [r0, #5]	@ zero_extendqisi2
	ldrb	r3, [r0, #6]	@ zero_extendqisi2
	ldrb	r9, [r0, #7]	@ zero_extendqisi2
	ldrb	r8, [r0, #8]	@ zero_extendqisi2
	ldrb	r6, [r0, #14]	@ zero_extendqisi2
.L19:
	ldrb	r5, [r0, ip]	@ zero_extendqisi2
	add	ip, ip, #1
	cmp	ip, #14
	add	r4, r4, r5
	uxtb	r4, r4
	bne	.L19
	cmp	r6, r4
	beq	.L24
	ldr	r0, .L25
.LPIC15:
	add	r0, pc, r0
	add	sp, sp, #12
	@ sp needed
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, lr}
	b	printf(PLT)
.L24:
	ldr	r0, .L25+4
	orr	r2, r2, r7, asl #8
	str	r9, [sp]
	str	r8, [sp, #4]
.LPIC14:
	add	r0, pc, r0
	bl	printf(PLT)
	add	sp, sp, #12
	@ sp needed
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, pc}
.L26:
	.align	2
.L25:
	.word	.LC7-(.LPIC15+8)
	.word	.LC6-(.LPIC14+8)
	.size	printPacketInfo, .-printPacketInfo
	.align	2
	.global	recvThread
	.type	recvThread, %function
recvThread:
	@ args = 0, pretend = 0, frame = 1064
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	sub	sp, sp, #1056
	ldr	r6, .L49
	sub	sp, sp, #12
	ldr	r3, .L49+4
.LPIC16:
	add	r6, pc, r6
	str	r0, [sp, #4]
	ldr	r0, .L49+8
	ldr	r3, [r6, r3]
.LPIC17:
	add	r0, pc, r0
	ldr	r1, [r3]
	str	r3, [sp, #12]
	add	r1, r1, #1
	str	r1, [r3]
	bl	printf(PLT)
	mov	r0, #1
	bl	sleep(PLT)
	ldr	r1, .L49+12
	ldr	r0, [sp, #4]
	mov	r2, #17
	mov	r3, #0
	ldr	r1, [r6, r1]
	bl	send(PLT)
	cmp	r0, #0
	blt	.L28
	ldr	r3, .L49+16
	add	fp, sp, #20
	ldr	r7, .L49+20
	mov	r5, #0
	str	fp, [sp, #8]
	add	fp, sp, #40
	ldr	r9, [r6, r3]
.LPIC21:
	add	r7, pc, r7
	ldr	r6, .L49+24
	mov	r4, r5
.LPIC22:
	add	r6, pc, r6
.L32:
	ldr	r3, [r9]
	cmp	r3, #0
	beq	.L41
	ldr	r0, [sp, #4]
	mov	r1, fp
	movw	r2, #1023
	mov	r3, #0
	bl	recv(PLT)
	subs	r8, r0, #0
	blt	.L32
	beq	.L41
	mov	r2, r4
	mov	r10, #0
	b	.L39
.L48:
	ldrb	r2, [r2, #-1045]	@ zero_extendqisi2
	cmp	r2, #10
	beq	.L46
.L35:
	cmp	r4, #19
	bgt	.L47
.L36:
	add	r10, r10, #1
	cmp	r10, r8
	bge	.L32
	mov	r2, r4
.L39:
	add	r3, sp, #1056
	ldrb	r0, [fp, r10]	@ zero_extendqisi2
	add	r3, r3, #8
	cmp	r5, #0
	add	r2, r3, r2
	add	r4, r4, #1
	strb	r0, [r2, #-1044]
	bne	.L48
	cmp	r4, #1
	beq	.L36
	ldrb	r2, [sp, #20]	@ zero_extendqisi2
	cmp	r2, #160
	bne	.L37
	ldrb	r2, [sp, #21]	@ zero_extendqisi2
	cmp	r2, #10
	moveq	r5, #1
	beq	.L36
.L37:
	mov	r0, r6
	mov	r4, #0
	bl	puts(PLT)
	b	.L36
.L47:
	mov	r0, r7
	mov	r5, #0
	bl	puts(PLT)
	mov	r4, r5
	b	.L36
.L46:
	cmp	r0, #160
	bne	.L35
	cmp	r4, #17
	bne	.L35
	ldr	r0, [sp, #8]
	mov	r5, #0
	bl	printPacketInfo(PLT)
	mov	r4, r5
	b	.L36
.L41:
	ldr	r2, [sp, #12]
	ldr	r0, [sp, #4]
	ldr	r3, [r2]
	sub	r3, r3, #1
	str	r3, [r2]
	bl	close(PLT)
	ldr	r0, .L49+28
.LPIC20:
	add	r0, pc, r0
	bl	puts(PLT)
	mov	r0, #0
	add	sp, sp, #1056
	add	sp, sp, #12
	@ sp needed
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L28:
	ldr	r0, .L49+32
.LPIC18:
	add	r0, pc, r0
	bl	puts(PLT)
	ldr	r0, .L49+36
.LPIC19:
	add	r0, pc, r0
	bl	puts(PLT)
	ldr	r0, [sp, #4]
	bl	close(PLT)
	ldr	r2, [sp, #12]
	mov	r0, #0
	ldr	r3, [r2]
	sub	r3, r3, #1
	str	r3, [r2]
	add	sp, sp, #1056
	add	sp, sp, #12
	@ sp needed
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L50:
	.align	2
.L49:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC16+8)
	.word	client_cnt(GOT)
	.word	.LC8-(.LPIC17+8)
	.word	SENSOR_REQ_PACKET(GOT)
	.word	program_run(GOT)
	.word	.LC11-(.LPIC21+8)
	.word	.LC12-(.LPIC22+8)
	.word	.LC10-(.LPIC20+8)
	.word	.LC9-(.LPIC18+8)
	.word	.LC10-(.LPIC19+8)
	.size	recvThread, .-recvThread
	.align	2
	.global	sendPacket
	.type	sendPacket, %function
sendPacket:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	bx	lr
	.size	sendPacket, .-sendPacket
	.global	client_cnt
	.global	program_run
	.global	SENSOR_REQ_PACKET
	.data
	.align	2
	.type	program_run, %object
	.size	program_run, 4
program_run:
	.word	1
	.type	SENSOR_REQ_PACKET, %object
	.size	SENSOR_REQ_PACKET, 17
SENSOR_REQ_PACKET:
	.byte	-96
	.byte	10
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	10
	.byte	-96
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC1:
	.ascii	"socket() error\000"
	.space	1
.LC2:
	.ascii	"bind() error\000"
	.space	3
.LC3:
	.ascii	"listen() error\000"
	.space	1
.LC4:
	.ascii	"Server Started\000"
	.space	1
.LC5:
	.ascii	"Server Closed\000"
	.space	2
.LC6:
	.ascii	"[%03d] cds:%d, pir:%d, reed_sw:%d, temp:%d\012\000"
.LC7:
	.ascii	"[%03d] Checksum error\012\000"
	.space	1
.LC8:
	.ascii	"Client connected (Total:%d)\012\000"
	.space	3
.LC9:
	.ascii	"send() ERROR\000"
	.space	3
.LC10:
	.ascii	"Client disconnected\000"
.LC11:
	.ascii	"BUFFER OVERFLOW ERROR\000"
	.space	2
.LC12:
	.ascii	"START PACKET ERROR\000"
	.bss
	.align	2
	.type	client_cnt, %object
	.size	client_cnt, 4
client_cnt:
	.space	4
	.ident	"GCC: (GNU) 4.8"
	.section	.note.GNU-stack,"",%progbits
