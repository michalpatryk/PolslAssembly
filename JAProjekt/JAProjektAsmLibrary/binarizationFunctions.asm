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
ALIGN 16
; Data below is ordered in an order easy to multiply by mulps command
blueCol				REAL4	0.0			; contains value of blue in pixel
greenCol			REAL4	0.0			; contains value of green in pixel
redCol				REAL4	0.0			; contains value of red in pixel
fill1				REAL4	0.0			; fill to allign memory to use in mulps 
blueMult			REAL4	0.144		; contains multiplier for blue in pixel
greenMult			REAL4	0.587		; contains multiplier for green in pixel
redMult				REAL4	0.299		; contains multiplier for red in pixel
fill2				REAL4	0.0			; fill to allign memory to use in mulps 
resultBlueMult		REAL4	0.0			; empty place to store result of mulps
resultGreenMult		REAL4	0.0			; empty place to store result of mulps
resultRedMult		REAL4	0.0			; empty place to store result of mulps
fill3				REAL4	0.0			; fill to allign memory to use in mulps 
const256			REAL4	256.0		; one byte
const0Byte			BYTE	0
const255Byte		BYTE	255
.code 
;add comment
; parametry wejsciowe (i ich zakresy) nie sa sprawdzane
; par wyj
; uzywane rejestry - nie niszczy pozostalych rejestrow
; zawsze zwracac 0 w eax
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
	mulss xmm3, const256	; multiply treshold by 256
	movss treshold, xmm3	; move value of treshold to local variable treshold
	

	mov rax, rcx	; move rcx to rax
	add rax, 3		; add 3 to rax
	cmp rax, rdx	; check if rax (current address + 3) is smaller than rdx (end address)
	jg @endWhile
@while:		; Beginning of the while loop


	xorps xmm0, xmm0		; zeroing xmm0 register for safeguarding next operations
	mov al, [rcx]			; load single byte of colour blue
	movzx eax, al			; Zero extend al into eax so we can pass it into xmm0 for sp float conversion
	cvtsi2ss xmm0, eax		; Save value to float register to pass it to blueCol
	movss blueCol, xmm0		; Save value to variable for future mulps (blueCol * blueMult)
	
	xorps xmm0, xmm0	; zeroing xmm0 register for safeguarding next operations
	mov al, [rcx + 1]	; load single byte of colour green
	movzx eax, al		; Zero extend al into eax so we can pass it into xmm0 for sp float conversion
	cvtsi2ss xmm0, eax	; Save value to float register to pass it to greenCol
	movss greenCol, xmm0; Save value to variable for future mulps (greenCol * greenMult)

	xorps xmm0, xmm0	; zeroing xmm0 register for safeguarding next operations
	mov al, [rcx + 2]	; load single byte of colour red
	movzx eax, al		; Zero extend al into eax so we can pass it into xmm0 for sp float conversion
	cvtsi2ss xmm0, eax	; Save value to float register to pass it to redCol
	movss redCol, xmm0	; Save value to variable for future mulps (redCol * redMult)

	xorps xmm0, xmm0	; cleaning xmm0 register for safeguarding next operations
	xorps xmm1, xmm1	; cleaning xmm1 register for safeguarding next operations
	xorps xmm2, xmm2	; cleaning xmm2 register for safeguarding next operations
	
	movaps xmm0, [blueCol]	; move memory (blueCol, greenCol, redCol, fill1) location to xmm0
	;movss result, xmm0
	movaps xmm1, [blueMult]	; move memory (blueMul, greenMul, redMul, fill1) location to xmm1
	vmulps	xmm2, xmm1, xmm0	; multiply 4 sp floats and store result in xmm2
	haddps	xmm2, xmm2			; horizontal add	
								; dest[127:96] = source[127:96] + source[95:64]
								; dest[95:64] = source[63:32] + source[31:0]
								; dest[63:32] = dest[127:96] + dest[95:64]
								; dest[31:0] = dest[63:32] + dest[31:0]
								; so we "merge" 127:0 of source to 63:32, and with
								; next instruction we merge 63:0 to 31:0 and extract
								; 31:0 with movss
	haddps	xmm2, xmm2			; horizontal add like the one above
	movss result, xmm2
	; now check if result > treshold
	comiss	xmm3, xmm2
	jl @belowTreshold
;	push xmm3
	;cmpss xmm3, xmm2, 1
	;movss result, xmm2
	;mov green, al
	;mov al, [rcx+1]
	;mov blue, al
	;mov al, [rcx + 2]
	;mov red, al

@aboveTreshold:
	mov al, 255
	mov [rcx], al	
	mov [rcx + 1], al	
	mov [rcx + 2], al	
	jmp @endOfRowCheck
@belowTreshold:
	mov al, 0
	mov [rcx], al
	mov [rcx + 1], al
	mov [rcx + 2], al
	jmp @endOfRowCheck
	; add red and blue move
@endOfRowCheck:
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
