getFirstGreaterOrZero:
    addi $t0, $0, $0
loop:
    slt $t1, $t0, $a1
    beq $t1, $0, dalje 
    sll $t1, $t0, 2
    addi $t1, $t1, $a0
    lw $t1, $0($t1)
    sgt $t2, $t1, $a2
    beq $t2, $0, L1
    addi $v0, $t1, $0
    jr $ra
L1: 
    addi $t0, $t0, 1
    j loop
dalje:
    addi $v0, $0, $0
    jr $ra

outerFn:
    
