;Bitmap binarization, version 0.0
;Author: Michał Urbańczyk
;Subject: Jezyki Assemblerowe
;Year: 2020/2021
;Compiler: MASM
;File containing asmBinarization algorithms.
;The main idea of the algorith is: take every pixels RGB values, and perform an operation on provided array:
;result = R * 0.299 + G * 0.587 + B * 0.114
;if(result) > threshold [R,G,B] = [255, 255, 255]
;else [R, G, B] = [0, 0, 0]

;Naming convention is asmBinarization{version number}
.data
;currentByteLoc DWORD 0
; beginptr: QWORD, endptr: PTR BYTE, biWidth: QWORD, treshold: real4
blueMult	REAL4	0.144
greenMult	REAL4	0.587
redMult		REAL4	0.299
.code 
asmBinarization1 proc 
;rcx - pointer to the beginning of the array
;rdx - pointer to the end of the array
;r8d - value of row width
;xmm3 - value of threshold
	;LOCAL beginptr: PTR BYTE	
	;LOCAL endptr: PTR BYTE	
	LOCAL currByteLoc: DWORD
	;LOCAL biWidth: DWORD	
	LOCAL treshold: REAL4
	LOCAL red: BYTE
	LOCAL fred: REAL4
	LOCAL green: BYTE
	LOCAL fgreen: REAL4
	LOCAL blue: BYTE
	LOCAL fblue: REAL4	
	LOCAL result: REAL4
	push rbp  ; Save address of previous stack frame
	push rdi  ; Save register destination index
	mov currByteLoc, 0	; Initialize currByteLoc with 0

	movss treshold, xmm3	; move value of treshold to local variable treshold
	

	mov rax, rcx	; move rcx to rax
	add rax, 3		; add 3 to rax
	cmp rax, rdx	; check if rax (current address + 3) is smaller than rdx (end address)
	jg @endWhile
@while:		; Beginning of the while loop
	mov al, [rcx]			; load simngle byte of colour blue
	movzx eax, al			; Zero extend al into eax
	cvtsi2ss xmm0, eax		; Save value to float register
	;PSLLD	 xmm0, 8
	movss xmm1, blueMult	; insert blue multiplier value ro register

	mov al, [rcx + 1]	; load simngle byte of colour green
	movzx eax, al	; Zero extend al into eax
	cvtsi2ss xmm2, eax	; Save value to float register
	movss xmm3, greenMult	; insert green multiplier value ro register

	mov al, [rcx + 2]	; load simngle byte of colour red
	movzx eax, al	; Zero extend al into eax
	cvtsi2ss xmm4, eax	; Save value to float register
	movss xmm5, greenMult	; insert red multiplier value ro register

	xorps xmm6, xmm6	; set all bits to 0
	xorpd xmm7, xmm7	; set all bits to 0



	;mov green, al
	;mov al, [rcx+1]
	;mov blue, al
	;mov al, [rcx + 2]
	;mov red, al

@aboveTreshold:
@belowTreshold:
	; add red and blue move
	mov eax, currByteLoc	; move currByteLoc to eax
	add eax, 3				; add 3 to eax
	cmp eax, r8d			; check if eax (currByteLoc + 3) is greater than r8d (row width)
	jg @jumpToNewRow
@continueCurrentRow:
	add rcx, 3	; currPos += 3;
	add currByteLoc, 3	; currByteLoc += 3;
	jmp @checkWhile
@jumpToNewRow:
	mov r9d, r8d	; cast biWidth here
	sub r9d, currByteLoc	;biWidth - currByteLoc
	add rcx, r9	; currPos += (biWidth - currByteLoc);
	mov currByteLoc, 0	; move 0 to currByteLoc
	cmp rcx, rdx	; 
	jge @checkWhile		; if previous condition returned greater or equal, jmp to @while
@checkWhile:
	mov rax, rcx	; move rcx (current address) to rax
	add rax, 3		; add 3 to rax
	cmp rax, rdx	; check if rax (current address + 3) is smaller than rdx (end address)
	jl @while
@endWhile:
	pop rdi	; Return rdi from stack 
	pop rbp	; Return rbp from stack
	ret		; Return from function
asmBinarization1 endp
end
	;char* begin, char* end, long biWidth, float treshold



;	xor eax, eax
;	mov eax, x
;	mov ecx, y
;	ror ecx, 1
;	shld eax,ecx,2
;	jnc ET1
;	mul y
;	ret
;ET1: 
;	mul x
;	neg y
;	ret
	
;asmBinarization1 endp
