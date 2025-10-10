f:                           // int f(int array[], int size)
    addi $t0, $0, $0         //   int cnt = 0;
    addi $t1, $0, $0         //   int i = 0;
    addi $a1, $a1, -1        //   sizeMinus1 = size - 1;
loop:
    slt  $t2, $t1, $a1       //   i < sizeMinus1?
    beq  $t2, $0, kraj       //   ako uslov petlje nije, onda skok na kraj
    sll  $t2, $t1, 2         //   $t2 = 4*i
    addi $t2, $t2, $a0       //   $t3 = array + $t2
    lw   $t3, 0($t2)         //   $t4 = array[$t3]
    addi $t2, $t2, 4         //   $t3 = $t3 + 4
    lw   $t4, 0($t2)         //   $t5 = array[$t3]
    sgt  $t2, $t3, $t4       //   array[i] > array[i+1]
    beq  $t2, $0, jumpnaloop //   ako nije, idi na pocetak petlje
    addi $t1, $t1, 1         //   ako je uslov tacan radi +1 pa na pocetak petlje
jumpnaloop:
    j loop
kraj:
    addi $v0, $t0, $0        //   return cnt
    

