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
const0Byte			BYTE	0			; constant byte of value 0
const255Byte		BYTE	255			; constant byte of value 255
.code 
; parametry wejsciowe (i ich zakresy) nie sa sprawdzane
; par wyjsciowe to zwrot 0 na eax, poniewaz funkcja zawsze sie wykona. Jedyna mozliwoscia na niewykonanie
; funkcji jest podanie zlych danych wejsciowych i wywolanie przerwania systemowego
; uzywane rejestry - nie niszczy pozostalych rejestrow
asmBinarization1 proc 
;rcx - pointer to the beginning of the array
;rdx - pointer to the end of the array
;r8d - value of row width
;xmm3 - value of threshold]
	ALIGN 16

	LOCAL blueColLocal: REAL4
	LOCAL greenColLocal: REAL4
	LOCAL redColLocal: REAL4
	LOCAL fill1Local: REAL4
	LOCAL resultBlueMultLocal: REAL4
	LOCAL resultGreenMultLocal: REAL4
	LOCAL resultRedMultLocal: REAL4
	LOCAL fill3Local: REAL4
	LOCAL currByteLoc: DWORD 	; local double word holding data about current row location
	LOCAL treshold: REAL4		; local real4 holding data about cutoff treshold used by binarization filter
	mov eax, blueCol
	mov blueColLocal, eax
	mov greenColLocal, eax
	mov redColLocal, eax
	mov fill1Local, eax
	mov resultBlueMultLocal, eax
	mov resultGreenMultLocal, eax
	mov resultRedMultLocal, eax
	mov fill3Local, eax
	

	push rbp					; Save address of previous stack frame
	push rdi					; Save register destination index
	push rsp					; Save stack pointer
	movaps xmm4, [blueMult]		; move data to xmm4 so we can save ourselves from multiple memory loading
	mov currByteLoc, 0			; Initialize currByteLoc with 0
	mulss xmm3, const256		; multiply treshold by 256
	movss treshold, xmm3		; move value of treshold to local variable treshold
	mov rax, rcx				; move rcx to rax
	add rax, 3					; add 3 to rax
	cmp rax, rdx				; check if rax (current address + 3) is smaller than rdx (end address)
	jg @endWhile
@while:							; Beginning of the while loop


	xorps xmm0, xmm0			; zeroing xmm0 register for safeguarding next operations
	mov al, [rcx]				; load single byte of colour blue
	movzx eax, al				; Zero extend al into eax so we can pass it into xmm0 for sp float conversion
	cvtsi2ss xmm1, eax			; Convert dword from eax (color byte) to float, store it in xmm1 without changing upper three doublewords
	; Now we will be slowly loading data to xmm1 register.
	; Data in xmm1 register: 0 0 0 blue
	pshufd xmm1, xmm1, 00000000b; blue blue blue blue

	xorps xmm0, xmm0			; zeroing xmm0 register for safeguarding next operations
	mov al, [rcx + 1]			; load single byte of colour green
	movzx eax, al				; Zero extend al into eax so we can pass it into xmm0 for sp float conversion
	cvtsi2ss xmm1, eax			; Save value to float register to pass it to greenCol
	
	;movss xmm1, xmm0			; blue blue blue green
	pshufd xmm1, xmm1, 11100000b; blue blue green green

	xorps xmm0, xmm0			; zeroing xmm0 register for safeguarding next operations
	mov al, [rcx + 2]			; load single byte of colour red
	movzx eax, al				; Zero extend al into eax so we can pass it into xmm0 for sp float conversion
	cvtsi2ss xmm1, eax			; Save value to float register to pass it to redCol
	
	;movss xmm1, xmm0			; blue blue green red
	pshufd xmm1, xmm1, 11000100b	; blue red green red
	xorps xmm2, xmm2			; 0 0 0 0
	movss xmm1, xmm2			; blue red green 0
	pshufd xmm1, xmm1, 00100111b; 0 red green blue
	;Starting register cleaning
	xorps xmm0, xmm0			; cleaning xmm0 register for safeguarding next operations
								; we do not clean xmm1, because it contains 0 rgb data
	xorps xmm2, xmm2			; cleaning xmm2 register for safeguarding next operations

	movaps xmm0, xmm1			; move memory (0, red, green, blue) to xmm0
	movaps xmm1, xmm4	; move memory (0, 0.298 (redMult), 0.587 (greenMult), 0.144 (blueMult)) location to xmm1
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
	comiss	xmm2, xmm3			; now check if result > treshold
	jb @belowTreshold

@aboveTreshold:					; result of haddps is above treshold, so we place 255 in all 3 bytes
	mov al, 255					; move 0xFF into al
	mov [rcx], al				; move 0xFF into memory address of blue
	mov [rcx + 1], al			; move 0xFF into memory address of green
	mov [rcx + 2], al			; move 0xFF into memory address of red
	jmp @endOfRowCheck
@belowTreshold:					; result of haddps is below treshold, so we place 255 in all 3 bytes
	mov al, 0					; move 0x00 into al
	mov [rcx], al				; move 0x00 into memory address of blue
	mov [rcx + 1], al			; move 0x00 into memory address of green
	mov [rcx + 2], al			; move 0x00 into memory address of red
	jmp @endOfRowCheck
@endOfRowCheck:
	mov eax, currByteLoc		; move currByteLoc to eax
	add eax, 3					; add 3 to eax
	cmp eax, r8d				; check if eax (currByteLoc + 3) is greater than r8d (row width)
	jg @jumpToNewRow
@continueCurrentRow:
	add rcx, 3					; currPos += 3;
	add currByteLoc, 3			; currByteLoc += 3;
	jmp @checkWhile
@jumpToNewRow:
	mov r9d, r8d				; cast biWidth (r8d) to r9d, so we can substract and add 
	sub r9d, currByteLoc		; bitmapWidth (originaly r8d) - currByteLoc
	add rcx, r9					; currPos (pixel pointer) += (biWidth - currByteLoc) <- we are doing this to skip padding
	mov currByteLoc, 0			; move 0 to currByteLoc because we are at the beginning of the current row
	cmp rcx, rdx				; check if we end our while loop (beginning pointer greater than end pointer)
	jge @checkWhile				; if previous condition returned greater or equal, jmp to @while
@checkWhile:
	mov rax, rcx				; move rcx (current address) to rax (temp unused register)
	add rax, 3					; add 3 to rax
	cmp rax, rdx				; check if rax (current address + 3) is smaller than rdx (end address) so we can continue the while loop
	jl @while
@endWhile:
	pop rsp						; Return rsp from stack
	pop rdi						; Return rdi from stack 
	pop rbp						; Return rbp from stack
	mov eax, 0					; Function always works
	ret							; Return from function
asmBinarization1 endp
end
	;char* begin, char* end, long biWidth, float treshold

