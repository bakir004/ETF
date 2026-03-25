	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 15, 0	sdk_version 15, 5
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
Lloh0:
	adrp	x0, __ZNSt3__13cinE@GOTPAGE
Lloh1:
	ldr	x0, [x0, __ZNSt3__13cinE@GOTPAGEOFF]
	sub	x1, x29, #4
	bl	__ZNSt3__113basic_istreamIcNS_11char_traitsIcEEErsERj
	ldur	w8, [x29, #-4]
Lloh2:
	adrp	x0, __ZNSt3__14coutE@GOTPAGE
Lloh3:
	ldr	x0, [x0, __ZNSt3__14coutE@GOTPAGEOFF]
	and	w1, w8, #0x1
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEj
	mov	w0, #0                          ; =0x0
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
	.loh AdrpLdrGot	Lloh2, Lloh3
	.loh AdrpLdrGot	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
.subsections_via_symbols
