*a je $s1
*b je $s2
*c je $s3
len je $s4

add $t0, $0, $0

; loop: 
;     beq $t0, $s4, done
;     lw  $t1, 0($s1)  
;     lw  $t2, 0($s2)  
;     sgt $t3, $t1, $t2
;     bne $t3, $0, tacno
; netacno:
;     add $t4, $t1, $t2
;     j   save 
; tacno:
;     sub $t4, $t1, $t2
; save:
;     sw  $t4, 0($s3)
;     addi $s1, $s1, 4
;     addi $s2, $s2, 4
;     addi $s3, $s3, 4
;     addi $t0, $t0, 1
;     j   loop
; done:
;

loop: 
    beq $t0, $s4, done
    lw  $t1, 0($s1)     ; t1 = 8
    lw  $t2, 0($s2)     ; t2 = 8
    sub $t3, $t1, $t2   ; t3 = 0
    sgt $t4, $t1, $t2
    bne $t4, vece 
    add $t3, $t1, $t2   ; t3 = 3+5=8
vece:
    sw  $t3, 0($s3)
    addi $s1, $s1, 4
    addi $s2, $s2, 4
    addi $s3, $s3, 4
    addi $t0, $t0, 1

    lw  $t5, 0($s1)     ; t1 = 8
    lw  $t6, 0($s2)     ; t2 = 8
    sub $t7, $t5, $t6   ; t3 = 0
    sgt $t8, $t5, $t6
    bne $t8, vece 
    add $t7, $t5, $t6   ; t3 = 3+5=8
vece:
    sw  $t3, 0($s3)
    addi $s1, $s1, 4
    addi $s2, $s2, 4
    addi $s3, $s3, 4
    addi $t0, $t0, 1
    j   loop
    beq $t0, $s4, done
    lw  $t1, 0($s1)     ; t1 = 8
    lw  $t2, 0($s2)     ; t2 = 8
    sub $t3, $t1, $t2   ; t3 = 0
    sgt $t4, $t1, $t2
    bne $t4, vece 
    add $t3, $t1, $t2   ; t3 = 3+5=8
vece:
    sw  $t3, 0($s3)
    addi $s1, $s1, 4
    addi $s2, $s2, 4
    addi $s3, $s3, 4
    addi $t0, $t0, 1
    j   loop
    beq $t0, $s4, done
    lw  $t1, 0($s1)     ; t1 = 8
    lw  $t2, 0($s2)     ; t2 = 8
    sub $t3, $t1, $t2   ; t3 = 0
    sgt $t4, $t1, $t2
    bne $t4, vece 
    add $t3, $t1, $t2   ; t3 = 3+5=8
vece:
    sw  $t3, 0($s3)
    addi $s1, $s1, 4
    addi $s2, $s2, 4
    addi $s3, $s3, 4
    addi $t0, $t0, 1
    j   loop
done:









