	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 15, 0	sdk_version 15, 5
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
Lfunc_begin0:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception0
; %bb.0:
	sub	sp, sp, #64
	stp	x22, x21, [sp, #16]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #32]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #48]             ; 16-byte Folded Spill
	add	x29, sp, #48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	mov	w0, #33792                      ; =0x8400
	movk	w0, #6103, lsl #16
	bl	__Znwm
	mov	x19, x0
Lloh0:
	adrp	x1, l_.memset_pattern@PAGE
Lloh1:
	add	x1, x1, l_.memset_pattern@PAGEOFF
	mov	w2, #33792                      ; =0x8400
	movk	w2, #6103, lsl #16
	bl	_memset_pattern16
	str	wzr, [sp, #12]
	bl	__ZNSt3__16chrono12steady_clock3nowEv
	mov	x20, x0
	add	x8, x19, #32
	movi	d1, #0000000000000000
	mov	w9, #57600                      ; =0xe100
	movk	w9, #1525, lsl #16
	mov	w10, #1116340224                ; =0x428a0000
	dup.4s	v0, w10
LBB0_1:                                 ; =>This Inner Loop Header: Depth=1
	ldp	q2, q3, [x8, #-32]
	ldp	q4, q5, [x8], #64
	fadd.4s	v2, v2, v0
	mov	s6, v2[3]
	mov	s7, v2[2]
	mov	s16, v2[1]
	fadd.4s	v3, v3, v0
	mov	s17, v3[3]
	mov	s18, v3[2]
	mov	s19, v3[1]
	fadd.4s	v4, v4, v0
	mov	s20, v4[3]
	mov	s21, v4[2]
	mov	s22, v4[1]
	fadd.4s	v5, v5, v0
	mov	s23, v5[3]
	mov	s24, v5[2]
	mov	s25, v5[1]
	fadd	s1, s1, s2
	fadd	s1, s1, s16
	fadd	s1, s1, s7
	fadd	s1, s1, s6
	fadd	s1, s1, s3
	fadd	s1, s1, s19
	fadd	s1, s1, s18
	fadd	s1, s1, s17
	fadd	s1, s1, s4
	fadd	s1, s1, s22
	fadd	s1, s1, s21
	fadd	s1, s1, s20
	fadd	s1, s1, s5
	fadd	s1, s1, s25
	fadd	s1, s1, s24
	fadd	s1, s1, s23
	subs	x9, x9, #16
	b.ne	LBB0_1
; %bb.2:
	str	s1, [sp, #12]
	bl	__ZNSt3__16chrono12steady_clock3nowEv
	mov	x21, x0
Ltmp0:
Lloh2:
	adrp	x0, __ZNSt3__14coutE@GOTPAGE
Lloh3:
	ldr	x0, [x0, __ZNSt3__14coutE@GOTPAGEOFF]
Lloh4:
	adrp	x1, l_.str@PAGE
Lloh5:
	add	x1, x1, l_.str@PAGEOFF
	mov	w2, #15                         ; =0xf
	bl	__ZNSt3__124__put_character_sequenceB8ne190102IcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
Ltmp1:
; %bb.3:
	sub	x8, x21, x20
	mov	x22, #13531                     ; =0x34db
	movk	x22, #55222, lsl #16
	movk	x22, #56962, lsl #32
	movk	x22, #17179, lsl #48
	smulh	x8, x8, x22
	asr	x9, x8, #18
	add	x1, x9, x8, lsr #63
Ltmp3:
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEx
Ltmp4:
; %bb.4:
Ltmp5:
Lloh6:
	adrp	x1, l_.str.1@PAGE
Lloh7:
	add	x1, x1, l_.str.1@PAGEOFF
	mov	w2, #4                          ; =0x4
	bl	__ZNSt3__124__put_character_sequenceB8ne190102IcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
Ltmp6:
; %bb.5:
	bl	__ZNSt3__16chrono12steady_clock3nowEv
	mov	x20, x0
	add	x8, x19, #64
	movi	d2, #0000000000000000
	mov	w9, #30784                      ; =0x7840
	movk	w9, #381, lsl #16
	movi	d3, #0000000000000000
	movi	d1, #0000000000000000
	movi	d0, #0000000000000000
LBB0_6:                                 ; =>This Inner Loop Header: Depth=1
	sub	x10, x8, #64
	sub	x11, x8, #32
	ld4.2s	{ v4, v5, v6, v7 }, [x10]
	ld4.2s	{ v16, v17, v18, v19 }, [x11]
	mov	s20, v4[1]
	mov	s21, v5[1]
	mov	s22, v6[1]
	fadd	s2, s2, s7
	fadd	s3, s3, s6
	fadd	s1, s1, s5
	fadd	s0, s0, s4
	mov	s4, v7[1]
	fadd	s2, s2, s4
	mov	s4, v16[1]
	fadd	s3, s3, s22
	mov	s5, v17[1]
	fadd	s1, s1, s21
	mov	s6, v18[1]
	fadd	s0, s0, s20
	mov	s7, v19[1]
	mov	x10, x8
	fadd	s2, s2, s19
	fadd	s3, s3, s18
	fadd	s1, s1, s17
	fadd	s0, s0, s16
	ld4.2s	{ v16, v17, v18, v19 }, [x10], #32
	fadd	s2, s2, s7
	mov	s7, v16[1]
	fadd	s3, s3, s6
	mov	s6, v17[1]
	fadd	s1, s1, s5
	mov	s5, v18[1]
	fadd	s0, s0, s4
	mov	s4, v19[1]
	fadd	s2, s2, s19
	fadd	s3, s3, s18
	fadd	s1, s1, s17
	fadd	s0, s0, s16
	ld4.2s	{ v16, v17, v18, v19 }, [x10]
	fadd	s2, s2, s4
	fadd	s3, s3, s5
	mov	s4, v16[1]
	fadd	s1, s1, s6
	mov	s5, v17[1]
	fadd	s0, s0, s7
	mov	s6, v18[1]
	fadd	s2, s2, s19
	fadd	s3, s3, s18
	fadd	s1, s1, s17
	fadd	s0, s0, s16
	mov	s7, v19[1]
	fadd	s2, s2, s7
	fadd	s3, s3, s6
	fadd	s1, s1, s5
	fadd	s0, s0, s4
	add	x8, x8, #128
	subs	x9, x9, #8
	b.ne	LBB0_6
; %bb.7:
	fadd	s0, s0, s1
	fadd	s0, s0, s3
	fadd	s0, s0, s2
	str	s0, [sp, #12]
	bl	__ZNSt3__16chrono12steady_clock3nowEv
	mov	x21, x0
Ltmp8:
Lloh8:
	adrp	x0, __ZNSt3__14coutE@GOTPAGE
Lloh9:
	ldr	x0, [x0, __ZNSt3__14coutE@GOTPAGEOFF]
Lloh10:
	adrp	x1, l_.str.2@PAGE
Lloh11:
	add	x1, x1, l_.str.2@PAGEOFF
	mov	w2, #15                         ; =0xf
	bl	__ZNSt3__124__put_character_sequenceB8ne190102IcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
Ltmp9:
; %bb.8:
	sub	x8, x21, x20
	smulh	x8, x8, x22
	asr	x9, x8, #18
	add	x1, x9, x8, lsr #63
Ltmp11:
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEx
Ltmp12:
; %bb.9:
Ltmp13:
Lloh12:
	adrp	x1, l_.str.1@PAGE
Lloh13:
	add	x1, x1, l_.str.1@PAGEOFF
	mov	w2, #4                          ; =0x4
	bl	__ZNSt3__124__put_character_sequenceB8ne190102IcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
Ltmp14:
; %bb.10:
	mov	x0, x19
	bl	__ZdlPv
	mov	w0, #0                          ; =0x0
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #64
	ret
LBB0_11:
Ltmp10:
	mov	x20, x0
	mov	x0, x19
	bl	__ZdlPv
	mov	x0, x20
	bl	__Unwind_Resume
LBB0_12:
Ltmp2:
	mov	x20, x0
	mov	x0, x19
	bl	__ZdlPv
	mov	x0, x20
	bl	__Unwind_Resume
LBB0_13:
Ltmp15:
	mov	x20, x0
	mov	x0, x19
	bl	__ZdlPv
	mov	x0, x20
	bl	__Unwind_Resume
LBB0_14:
Ltmp7:
	mov	x20, x0
	mov	x0, x19
	bl	__ZdlPv
	mov	x0, x20
	bl	__Unwind_Resume
	.loh AdrpAdd	Lloh0, Lloh1
	.loh AdrpAdd	Lloh4, Lloh5
	.loh AdrpLdrGot	Lloh2, Lloh3
	.loh AdrpAdd	Lloh6, Lloh7
	.loh AdrpAdd	Lloh10, Lloh11
	.loh AdrpLdrGot	Lloh8, Lloh9
	.loh AdrpAdd	Lloh12, Lloh13
Lfunc_end0:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2, 0x0
GCC_except_table0:
Lexception0:
	.byte	255                             ; @LPStart Encoding = omit
	.byte	255                             ; @TType Encoding = omit
	.byte	1                               ; Call site Encoding = uleb128
	.uleb128 Lcst_end0-Lcst_begin0
Lcst_begin0:
	.uleb128 Lfunc_begin0-Lfunc_begin0      ; >> Call Site 1 <<
	.uleb128 Ltmp0-Lfunc_begin0             ;   Call between Lfunc_begin0 and Ltmp0
	.byte	0                               ;     has no landing pad
	.byte	0                               ;   On action: cleanup
	.uleb128 Ltmp0-Lfunc_begin0             ; >> Call Site 2 <<
	.uleb128 Ltmp1-Ltmp0                    ;   Call between Ltmp0 and Ltmp1
	.uleb128 Ltmp2-Lfunc_begin0             ;     jumps to Ltmp2
	.byte	0                               ;   On action: cleanup
	.uleb128 Ltmp3-Lfunc_begin0             ; >> Call Site 3 <<
	.uleb128 Ltmp6-Ltmp3                    ;   Call between Ltmp3 and Ltmp6
	.uleb128 Ltmp7-Lfunc_begin0             ;     jumps to Ltmp7
	.byte	0                               ;   On action: cleanup
	.uleb128 Ltmp8-Lfunc_begin0             ; >> Call Site 4 <<
	.uleb128 Ltmp9-Ltmp8                    ;   Call between Ltmp8 and Ltmp9
	.uleb128 Ltmp10-Lfunc_begin0            ;     jumps to Ltmp10
	.byte	0                               ;   On action: cleanup
	.uleb128 Ltmp11-Lfunc_begin0            ; >> Call Site 5 <<
	.uleb128 Ltmp14-Ltmp11                  ;   Call between Ltmp11 and Ltmp14
	.uleb128 Ltmp15-Lfunc_begin0            ;     jumps to Ltmp15
	.byte	0                               ;   On action: cleanup
	.uleb128 Ltmp14-Lfunc_begin0            ; >> Call Site 6 <<
	.uleb128 Lfunc_end0-Ltmp14              ;   Call between Ltmp14 and Lfunc_end0
	.byte	0                               ;     has no landing pad
	.byte	0                               ;   On action: cleanup
Lcst_end0:
	.p2align	2, 0x0
                                        ; -- End function
	.section	__TEXT,__text,regular,pure_instructions
	.private_extern	___clang_call_terminate ; -- Begin function __clang_call_terminate
	.globl	___clang_call_terminate
	.weak_def_can_be_hidden	___clang_call_terminate
	.p2align	2
___clang_call_terminate:                ; @__clang_call_terminate
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	bl	___cxa_begin_catch
	bl	__ZSt9terminatev
	.cfi_endproc
                                        ; -- End function
	.private_extern	__ZNSt3__120__throw_length_errorB8ne190102EPKc ; -- Begin function _ZNSt3__120__throw_length_errorB8ne190102EPKc
	.globl	__ZNSt3__120__throw_length_errorB8ne190102EPKc
	.weak_def_can_be_hidden	__ZNSt3__120__throw_length_errorB8ne190102EPKc
	.p2align	2
__ZNSt3__120__throw_length_errorB8ne190102EPKc: ; @_ZNSt3__120__throw_length_errorB8ne190102EPKc
Lfunc_begin1:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception1
; %bb.0:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x20, x0
	mov	w0, #16                         ; =0x10
	bl	___cxa_allocate_exception
	mov	x19, x0
Ltmp16:
	mov	x1, x20
	bl	__ZNSt12length_errorC1B8ne190102EPKc
Ltmp17:
; %bb.1:
Lloh14:
	adrp	x1, __ZTISt12length_error@GOTPAGE
Lloh15:
	ldr	x1, [x1, __ZTISt12length_error@GOTPAGEOFF]
Lloh16:
	adrp	x2, __ZNSt12length_errorD1Ev@GOTPAGE
Lloh17:
	ldr	x2, [x2, __ZNSt12length_errorD1Ev@GOTPAGEOFF]
	mov	x0, x19
	bl	___cxa_throw
LBB2_2:
Ltmp18:
	mov	x20, x0
	mov	x0, x19
	bl	___cxa_free_exception
	mov	x0, x20
	bl	__Unwind_Resume
	.loh AdrpLdrGot	Lloh16, Lloh17
	.loh AdrpLdrGot	Lloh14, Lloh15
Lfunc_end1:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2, 0x0
GCC_except_table2:
Lexception1:
	.byte	255                             ; @LPStart Encoding = omit
	.byte	255                             ; @TType Encoding = omit
	.byte	1                               ; Call site Encoding = uleb128
	.uleb128 Lcst_end1-Lcst_begin1
Lcst_begin1:
	.uleb128 Lfunc_begin1-Lfunc_begin1      ; >> Call Site 1 <<
	.uleb128 Ltmp16-Lfunc_begin1            ;   Call between Lfunc_begin1 and Ltmp16
	.byte	0                               ;     has no landing pad
	.byte	0                               ;   On action: cleanup
	.uleb128 Ltmp16-Lfunc_begin1            ; >> Call Site 2 <<
	.uleb128 Ltmp17-Ltmp16                  ;   Call between Ltmp16 and Ltmp17
	.uleb128 Ltmp18-Lfunc_begin1            ;     jumps to Ltmp18
	.byte	0                               ;   On action: cleanup
	.uleb128 Ltmp17-Lfunc_begin1            ; >> Call Site 3 <<
	.uleb128 Lfunc_end1-Ltmp17              ;   Call between Ltmp17 and Lfunc_end1
	.byte	0                               ;     has no landing pad
	.byte	0                               ;   On action: cleanup
Lcst_end1:
	.p2align	2, 0x0
                                        ; -- End function
	.section	__TEXT,__text,regular,pure_instructions
	.private_extern	__ZNSt12length_errorC1B8ne190102EPKc ; -- Begin function _ZNSt12length_errorC1B8ne190102EPKc
	.globl	__ZNSt12length_errorC1B8ne190102EPKc
	.weak_def_can_be_hidden	__ZNSt12length_errorC1B8ne190102EPKc
	.p2align	2
__ZNSt12length_errorC1B8ne190102EPKc:   ; @_ZNSt12length_errorC1B8ne190102EPKc
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	bl	__ZNSt11logic_errorC2EPKc
Lloh18:
	adrp	x8, __ZTVSt12length_error@GOTPAGE
Lloh19:
	ldr	x8, [x8, __ZTVSt12length_error@GOTPAGEOFF]
	add	x8, x8, #16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpLdrGot	Lloh18, Lloh19
	.cfi_endproc
                                        ; -- End function
	.private_extern	__ZNSt3__124__put_character_sequenceB8ne190102IcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m ; -- Begin function _ZNSt3__124__put_character_sequenceB8ne190102IcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
	.globl	__ZNSt3__124__put_character_sequenceB8ne190102IcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
	.weak_def_can_be_hidden	__ZNSt3__124__put_character_sequenceB8ne190102IcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
	.p2align	2
__ZNSt3__124__put_character_sequenceB8ne190102IcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m: ; @_ZNSt3__124__put_character_sequenceB8ne190102IcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
Lfunc_begin2:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception2
; %bb.0:
	sub	sp, sp, #112
	stp	x26, x25, [sp, #32]             ; 16-byte Folded Spill
	stp	x24, x23, [sp, #48]             ; 16-byte Folded Spill
	stp	x22, x21, [sp, #64]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #80]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #96]             ; 16-byte Folded Spill
	add	x29, sp, #96
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset w25, -72
	.cfi_offset w26, -80
	mov	x21, x2
	mov	x20, x1
	mov	x19, x0
Ltmp19:
	add	x0, sp, #8
	mov	x1, x19
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE6sentryC1ERS3_
Ltmp20:
; %bb.1:
	ldrb	w8, [sp, #8]
	cmp	w8, #1
	b.ne	LBB4_10
; %bb.2:
	ldr	x8, [x19]
	ldur	x8, [x8, #-24]
	add	x4, x19, x8
	ldr	x22, [x4, #40]
	ldr	w24, [x4, #8]
	ldr	w23, [x4, #144]
	cmn	w23, #1
	b.ne	LBB4_7
; %bb.3:
Ltmp22:
	add	x8, sp, #24
	mov	x25, x4
	mov	x0, x4
	bl	__ZNKSt3__18ios_base6getlocEv
Ltmp23:
; %bb.4:
Ltmp24:
Lloh20:
	adrp	x1, __ZNSt3__15ctypeIcE2idE@GOTPAGE
Lloh21:
	ldr	x1, [x1, __ZNSt3__15ctypeIcE2idE@GOTPAGEOFF]
	add	x0, sp, #24
	bl	__ZNKSt3__16locale9use_facetERNS0_2idE
Ltmp25:
; %bb.5:
	ldr	x8, [x0]
	ldr	x8, [x8, #56]
Ltmp26:
	mov	w1, #32                         ; =0x20
	blr	x8
Ltmp27:
; %bb.6:
	mov	x23, x0
	add	x0, sp, #24
	bl	__ZNSt3__16localeD1Ev
	mov	x4, x25
	str	w23, [x25, #144]
LBB4_7:
	mov	w8, #176                        ; =0xb0
	and	w8, w24, w8
	add	x3, x20, x21
	cmp	w8, #32
	csel	x2, x3, x20, eq
Ltmp29:
	sxtb	w5, w23
	mov	x0, x22
	mov	x1, x20
	bl	__ZNSt3__116__pad_and_outputB8ne190102IcNS_11char_traitsIcEEEENS_19ostreambuf_iteratorIT_T0_EES6_PKS4_S8_S8_RNS_8ios_baseES4_
Ltmp30:
; %bb.8:
	cbnz	x0, LBB4_10
; %bb.9:
	ldr	x8, [x19]
	ldur	x8, [x8, #-24]
	add	x0, x19, x8
	ldr	w8, [x0, #32]
	mov	w9, #5                          ; =0x5
Ltmp32:
	orr	w1, w8, w9
	bl	__ZNSt3__18ios_base5clearEj
Ltmp33:
LBB4_10:
	add	x0, sp, #8
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE6sentryD1Ev
LBB4_11:
	mov	x0, x19
	ldp	x29, x30, [sp, #96]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #80]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #64]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #48]             ; 16-byte Folded Reload
	ldp	x26, x25, [sp, #32]             ; 16-byte Folded Reload
	add	sp, sp, #112
	ret
LBB4_12:
Ltmp34:
	b	LBB4_15
LBB4_13:
Ltmp28:
	mov	x20, x0
	add	x0, sp, #24
	bl	__ZNSt3__16localeD1Ev
	b	LBB4_16
LBB4_14:
Ltmp31:
LBB4_15:
	mov	x20, x0
LBB4_16:
	add	x0, sp, #8
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE6sentryD1Ev
	b	LBB4_18
LBB4_17:
Ltmp21:
	mov	x20, x0
LBB4_18:
	mov	x0, x20
	bl	___cxa_begin_catch
	ldr	x8, [x19]
	ldur	x8, [x8, #-24]
Ltmp35:
	add	x0, x19, x8
	bl	__ZNSt3__18ios_base33__set_badbit_and_consider_rethrowEv
Ltmp36:
; %bb.19:
	bl	___cxa_end_catch
	b	LBB4_11
LBB4_20:
Ltmp37:
	mov	x19, x0
Ltmp38:
	bl	___cxa_end_catch
Ltmp39:
; %bb.21:
	mov	x0, x19
	bl	__Unwind_Resume
LBB4_22:
Ltmp40:
	bl	___clang_call_terminate
	.loh AdrpLdrGot	Lloh20, Lloh21
Lfunc_end2:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2, 0x0
GCC_except_table4:
Lexception2:
	.byte	255                             ; @LPStart Encoding = omit
	.byte	155                             ; @TType Encoding = indirect pcrel sdata4
	.uleb128 Lttbase0-Lttbaseref0
Lttbaseref0:
	.byte	1                               ; Call site Encoding = uleb128
	.uleb128 Lcst_end2-Lcst_begin2
Lcst_begin2:
	.uleb128 Ltmp19-Lfunc_begin2            ; >> Call Site 1 <<
	.uleb128 Ltmp20-Ltmp19                  ;   Call between Ltmp19 and Ltmp20
	.uleb128 Ltmp21-Lfunc_begin2            ;     jumps to Ltmp21
	.byte	1                               ;   On action: 1
	.uleb128 Ltmp22-Lfunc_begin2            ; >> Call Site 2 <<
	.uleb128 Ltmp23-Ltmp22                  ;   Call between Ltmp22 and Ltmp23
	.uleb128 Ltmp31-Lfunc_begin2            ;     jumps to Ltmp31
	.byte	1                               ;   On action: 1
	.uleb128 Ltmp24-Lfunc_begin2            ; >> Call Site 3 <<
	.uleb128 Ltmp27-Ltmp24                  ;   Call between Ltmp24 and Ltmp27
	.uleb128 Ltmp28-Lfunc_begin2            ;     jumps to Ltmp28
	.byte	1                               ;   On action: 1
	.uleb128 Ltmp29-Lfunc_begin2            ; >> Call Site 4 <<
	.uleb128 Ltmp30-Ltmp29                  ;   Call between Ltmp29 and Ltmp30
	.uleb128 Ltmp31-Lfunc_begin2            ;     jumps to Ltmp31
	.byte	1                               ;   On action: 1
	.uleb128 Ltmp32-Lfunc_begin2            ; >> Call Site 5 <<
	.uleb128 Ltmp33-Ltmp32                  ;   Call between Ltmp32 and Ltmp33
	.uleb128 Ltmp34-Lfunc_begin2            ;     jumps to Ltmp34
	.byte	1                               ;   On action: 1
	.uleb128 Ltmp33-Lfunc_begin2            ; >> Call Site 6 <<
	.uleb128 Ltmp35-Ltmp33                  ;   Call between Ltmp33 and Ltmp35
	.byte	0                               ;     has no landing pad
	.byte	0                               ;   On action: cleanup
	.uleb128 Ltmp35-Lfunc_begin2            ; >> Call Site 7 <<
	.uleb128 Ltmp36-Ltmp35                  ;   Call between Ltmp35 and Ltmp36
	.uleb128 Ltmp37-Lfunc_begin2            ;     jumps to Ltmp37
	.byte	0                               ;   On action: cleanup
	.uleb128 Ltmp36-Lfunc_begin2            ; >> Call Site 8 <<
	.uleb128 Ltmp38-Ltmp36                  ;   Call between Ltmp36 and Ltmp38
	.byte	0                               ;     has no landing pad
	.byte	0                               ;   On action: cleanup
	.uleb128 Ltmp38-Lfunc_begin2            ; >> Call Site 9 <<
	.uleb128 Ltmp39-Ltmp38                  ;   Call between Ltmp38 and Ltmp39
	.uleb128 Ltmp40-Lfunc_begin2            ;     jumps to Ltmp40
	.byte	1                               ;   On action: 1
	.uleb128 Ltmp39-Lfunc_begin2            ; >> Call Site 10 <<
	.uleb128 Lfunc_end2-Ltmp39              ;   Call between Ltmp39 and Lfunc_end2
	.byte	0                               ;     has no landing pad
	.byte	0                               ;   On action: cleanup
Lcst_end2:
	.byte	1                               ; >> Action Record 1 <<
                                        ;   Catch TypeInfo 1
	.byte	0                               ;   No further actions
	.p2align	2, 0x0
                                        ; >> Catch TypeInfos <<
	.long	0                               ; TypeInfo 1
Lttbase0:
	.p2align	2, 0x0
                                        ; -- End function
	.section	__TEXT,__text,regular,pure_instructions
	.private_extern	__ZNSt3__116__pad_and_outputB8ne190102IcNS_11char_traitsIcEEEENS_19ostreambuf_iteratorIT_T0_EES6_PKS4_S8_S8_RNS_8ios_baseES4_ ; -- Begin function _ZNSt3__116__pad_and_outputB8ne190102IcNS_11char_traitsIcEEEENS_19ostreambuf_iteratorIT_T0_EES6_PKS4_S8_S8_RNS_8ios_baseES4_
	.globl	__ZNSt3__116__pad_and_outputB8ne190102IcNS_11char_traitsIcEEEENS_19ostreambuf_iteratorIT_T0_EES6_PKS4_S8_S8_RNS_8ios_baseES4_
	.weak_def_can_be_hidden	__ZNSt3__116__pad_and_outputB8ne190102IcNS_11char_traitsIcEEEENS_19ostreambuf_iteratorIT_T0_EES6_PKS4_S8_S8_RNS_8ios_baseES4_
	.p2align	2
__ZNSt3__116__pad_and_outputB8ne190102IcNS_11char_traitsIcEEEENS_19ostreambuf_iteratorIT_T0_EES6_PKS4_S8_S8_RNS_8ios_baseES4_: ; @_ZNSt3__116__pad_and_outputB8ne190102IcNS_11char_traitsIcEEEENS_19ostreambuf_iteratorIT_T0_EES6_PKS4_S8_S8_RNS_8ios_baseES4_
Lfunc_begin3:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception3
; %bb.0:
	sub	sp, sp, #112
	stp	x26, x25, [sp, #32]             ; 16-byte Folded Spill
	stp	x24, x23, [sp, #48]             ; 16-byte Folded Spill
	stp	x22, x21, [sp, #64]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #80]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #96]             ; 16-byte Folded Spill
	add	x29, sp, #96
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset w25, -72
	.cfi_offset w26, -80
	mov	x19, x0
	cbz	x0, LBB5_16
; %bb.1:
	mov	x24, x5
	mov	x20, x4
	mov	x22, x3
	mov	x21, x2
	ldr	x8, [x4, #24]
	sub	x9, x3, x1
	subs	x8, x8, x9
	csel	x23, x8, xzr, gt
	sub	x25, x2, x1
	cmp	x25, #1
	b.lt	LBB5_3
; %bb.2:
	ldr	x8, [x19]
	ldr	x8, [x8, #96]
	mov	x0, x19
	mov	x2, x25
	blr	x8
	cmp	x0, x25
	b.ne	LBB5_15
LBB5_3:
	cmp	x23, #1
	b.lt	LBB5_12
; %bb.4:
	mov	x8, #9223372036854775800        ; =0x7ffffffffffffff8
	cmp	x23, x8
	b.hs	LBB5_17
; %bb.5:
	cmp	x23, #23
	b.hs	LBB5_7
; %bb.6:
	strb	w23, [sp, #31]
	add	x25, sp, #8
	b	LBB5_8
LBB5_7:
	orr	x8, x23, #0x7
	cmp	x8, #23
	mov	w9, #25                         ; =0x19
	csinc	x26, x9, x8, eq
	mov	x0, x26
	bl	__Znwm
	mov	x25, x0
	orr	x8, x26, #0x8000000000000000
	stp	x23, x8, [sp, #16]
	str	x0, [sp, #8]
LBB5_8:
	mov	x0, x25
	mov	x1, x24
	mov	x2, x23
	bl	_memset
	strb	wzr, [x25, x23]
	ldrsb	w8, [sp, #31]
	ldr	x9, [sp, #8]
	cmp	w8, #0
	add	x8, sp, #8
	csel	x1, x9, x8, lt
	ldr	x8, [x19]
	ldr	x8, [x8, #96]
Ltmp41:
	mov	x0, x19
	mov	x2, x23
	blr	x8
Ltmp42:
; %bb.9:
	ldrsb	w8, [sp, #31]
	tbnz	w8, #31, LBB5_11
; %bb.10:
	cmp	x0, x23
	b.ne	LBB5_15
	b	LBB5_12
LBB5_11:
	ldr	x8, [sp, #8]
	mov	x24, x0
	mov	x0, x8
	bl	__ZdlPv
	cmp	x24, x23
	b.ne	LBB5_15
LBB5_12:
	sub	x22, x22, x21
	cmp	x22, #1
	b.lt	LBB5_14
; %bb.13:
	ldr	x8, [x19]
	ldr	x8, [x8, #96]
	mov	x0, x19
	mov	x1, x21
	mov	x2, x22
	blr	x8
	cmp	x0, x22
	b.ne	LBB5_15
LBB5_14:
	str	xzr, [x20, #24]
	b	LBB5_16
LBB5_15:
	mov	x19, #0                         ; =0x0
LBB5_16:
	mov	x0, x19
	ldp	x29, x30, [sp, #96]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #80]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #64]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #48]             ; 16-byte Folded Reload
	ldp	x26, x25, [sp, #32]             ; 16-byte Folded Reload
	add	sp, sp, #112
	ret
LBB5_17:
	add	x0, sp, #8
	bl	__ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE20__throw_length_errorB8ne190102Ev
LBB5_18:
Ltmp43:
	mov	x19, x0
	ldrsb	w8, [sp, #31]
	tbz	w8, #31, LBB5_20
; %bb.19:
	ldr	x0, [sp, #8]
	bl	__ZdlPv
LBB5_20:
	mov	x0, x19
	bl	__Unwind_Resume
Lfunc_end3:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2, 0x0
GCC_except_table5:
Lexception3:
	.byte	255                             ; @LPStart Encoding = omit
	.byte	255                             ; @TType Encoding = omit
	.byte	1                               ; Call site Encoding = uleb128
	.uleb128 Lcst_end3-Lcst_begin3
Lcst_begin3:
	.uleb128 Lfunc_begin3-Lfunc_begin3      ; >> Call Site 1 <<
	.uleb128 Ltmp41-Lfunc_begin3            ;   Call between Lfunc_begin3 and Ltmp41
	.byte	0                               ;     has no landing pad
	.byte	0                               ;   On action: cleanup
	.uleb128 Ltmp41-Lfunc_begin3            ; >> Call Site 2 <<
	.uleb128 Ltmp42-Ltmp41                  ;   Call between Ltmp41 and Ltmp42
	.uleb128 Ltmp43-Lfunc_begin3            ;     jumps to Ltmp43
	.byte	0                               ;   On action: cleanup
	.uleb128 Ltmp42-Lfunc_begin3            ; >> Call Site 3 <<
	.uleb128 Lfunc_end3-Ltmp42              ;   Call between Ltmp42 and Lfunc_end3
	.byte	0                               ;     has no landing pad
	.byte	0                               ;   On action: cleanup
Lcst_end3:
	.p2align	2, 0x0
                                        ; -- End function
	.section	__TEXT,__text,regular,pure_instructions
	.private_extern	__ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE20__throw_length_errorB8ne190102Ev ; -- Begin function _ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE20__throw_length_errorB8ne190102Ev
	.globl	__ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE20__throw_length_errorB8ne190102Ev
	.weak_def_can_be_hidden	__ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE20__throw_length_errorB8ne190102Ev
	.p2align	2
__ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE20__throw_length_errorB8ne190102Ev: ; @_ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE20__throw_length_errorB8ne190102Ev
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
Lloh22:
	adrp	x0, l_.str.4@PAGE
Lloh23:
	add	x0, x0, l_.str.4@PAGEOFF
	bl	__ZNSt3__120__throw_length_errorB8ne190102EPKc
	.loh AdrpAdd	Lloh22, Lloh23
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"Standard Loop: "

l_.str.1:                               ; @.str.1
	.asciz	" ms\n"

l_.str.2:                               ; @.str.2
	.asciz	"Unrolled (4x): "

l_.str.4:                               ; @.str.4
	.asciz	"basic_string"

	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ; @.memset_pattern
l_.memset_pattern:
	.long	0x3f8ccccd                      ; float 1.10000002
	.long	0x3f8ccccd                      ; float 1.10000002
	.long	0x3f8ccccd                      ; float 1.10000002
	.long	0x3f8ccccd                      ; float 1.10000002

.subsections_via_symbols
