.data
string_0: .asciiz " "
string_1: .asciiz " n<0,data error!"
string_2: .asciiz " in1>=in2!"
string_3: .asciiz " j != 4 "
string_4: .asciiz " wrong!"
string_5: .asciiz " this is temp:"
.text
move $gp $sp
li $t8 10
sw $t8 0($sp)
li $t8 15
sw $t8 -4($sp)
li $t8 43
sw $t8 -8($sp)
li $t8 45
sw $t8 -12($sp)
li $t8 42
sw $t8 -16($sp)
li $t8 47
sw $t8 -20($sp)
subi $sp $sp 112
#~~~变量声明部分结束~~~~
j main
ptans:
subi $sp $sp 40
#~~~变量声明部分结束~~~~
lw $t8 -8($fp)
li $t9 43
seq $t8 $t8 $t9
beq $t8 0 label_0
lw $t8 -0($fp)
lw $t9 -4($fp)
add $t0 $t8 $t9
sw $t0 -20($fp)
label_0:
lw $t8 -8($fp)
li $t9 45
seq $t8 $t8 $t9
beq $t8 0 label_1
lw $t8 -0($fp)
lw $t9 -4($fp)
sub $t1 $t8 $t9
sw $t1 -20($fp)
label_1:
lw $t8 -8($fp)
li $t9 42
seq $t8 $t8 $t9
beq $t8 0 label_2
lw $t8 -0($fp)
lw $t9 -4($fp)
mult $t8 $t9
mflo $t2
sw $t2 -20($fp)
label_2:
lw $t8 -8($fp)
li $t9 47
seq $t8 $t8 $t9
beq $t8 0 label_3
lw $t8 -0($fp)
lw $t9 -4($fp)
div $t8 $t9
mflo $t3
sw $t3 -20($fp)
label_3:
la $a0 string_0
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -20($fp)
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
jr $ra
fac1:
subi $sp $sp 28
#~~~变量声明部分结束~~~~
lw $t8 -0($fp)
li $t9 0
slt $t8 $t8 $t9
beq $t8 0 label_4
la $a0 string_1
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
li $t9 -1
sw $t9 -12($fp)
label_4:
lw $t8 -0($fp)
li $t9 0
seq $t8 $t8 $t9
beq $t8 0 label_5
li $t9 1
sw $t9 -12($fp)
label_5:
lw $t8 -0($fp)
li $t9 1
seq $t8 $t8 $t9
beq $t8 0 label_6
li $t9 1
sw $t9 -12($fp)
label_6:
lw $t8 -0($fp)
li $t9 1
sgt $t8 $t8 $t9
beq $t8 0 label_7
lw $t8 -0($fp)
li $t9 1
sub $t0 $t8 $t9
sw $t0 0($sp)
sw $t0 -16($fp)
sw $fp -4($sp)
sw $ra -8($sp)
move $fp $sp
jal fac1
move $sp $fp
lw $fp -4($sp)
lw $ra -8($sp)
move $t0 $v0
lw $t9 -0($fp)
mult $t0 $t9
mflo $t1
sw $t1 -12($fp)
label_7:
lw $t8 -12($fp)
move $v0 $t8
jr $ra
fac2:
subi $sp $sp 28
#~~~变量声明部分结束~~~~
li $t9 0
sw $t9 -16($fp)
lw $t8 -0($fp)
sw $t8 0($sp)
lw $t8 -4($fp)
sw $t8 -4($sp)
lw $t8 -8($gp)
sw $t8 -8($sp)
sw $fp -12($sp)
sw $ra -16($sp)
move $fp $sp
jal ptans
move $sp $fp
lw $fp -12($sp)
lw $ra -16($sp)
lw $t8 -0($fp)
sw $t8 0($sp)
lw $t8 -4($fp)
sw $t8 -4($sp)
lw $t8 -12($gp)
sw $t8 -8($sp)
sw $fp -12($sp)
sw $ra -16($sp)
move $fp $sp
jal ptans
move $sp $fp
lw $fp -12($sp)
lw $ra -16($sp)
lw $t8 -0($fp)
sw $t8 0($sp)
lw $t8 -4($fp)
sw $t8 -4($sp)
li $t8 42
sw $t8 -8($sp)
sw $fp -12($sp)
sw $ra -16($sp)
move $fp $sp
jal ptans
move $sp $fp
lw $fp -12($sp)
lw $ra -16($sp)
lw $t8 -0($fp)
sw $t8 0($sp)
lw $t8 -4($fp)
sw $t8 -4($sp)
li $t8 47
sw $t8 -8($sp)
sw $fp -12($sp)
sw $ra -16($sp)
move $fp $sp
jal ptans
move $sp $fp
lw $fp -12($sp)
lw $ra -16($sp)
label_8:
lw $t8 -0($fp)
lw $t9 -4($fp)
slt $t8 $t8 $t9
beq $t8 0 label_9
j label_10
label_9:
la $a0 string_2
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
label_10:
lw $t8 -0($fp)
li $t9 1
add $t0 $t8 $t9
sw $t0 -0($fp)
lw $t8 -16($fp)
li $t9 1
add $t1 $t8 $t9
sw $t1 -16($fp)
lw $t8 -16($fp)
li $t9 5
slt $t8 $t8 $t9
beq $t8 1 label_8
jr $ra
fac3:
subi $sp $sp 36
#~~~变量声明部分结束~~~~
li $t9 0
sw $t9 -20($fp)
li $t9 0
sw $t9 -12($fp)
label_11:
lw $t8 -12($fp)
lw $t9 -0($fp)
slt $t8 $t8 $t9
beq $t8 0 label_12
lw $t8 -12($fp)
sw $t8 0($sp)
sw $fp -4($sp)
sw $ra -8($sp)
move $fp $sp
jal fac1
move $sp $fp
lw $fp -4($sp)
lw $ra -8($sp)
move $t0 $v0
sw $t0 -16($fp)
lw $t8 -20($fp)
lw $t9 -16($fp)
add $t1 $t8 $t9
sw $t1 -20($fp)
lw $t8 -12($fp)
li $t9 1
add $t2 $t8 $t9
sw $t2 -12($fp)
j label_11
label_12:
lw $t8 -20($fp)
move $v0 $t8
jr $ra
fac4:
li $t8 5
sw $t8 -8($fp)
subi $sp $sp 36
#~~~变量声明部分结束~~~~
li $t9 9
sw $t9 -12($fp)
label_13:
lw $t8 -12($fp)
li $t9 0
sge $t8 $t8 $t9
beq $t8 0 label_14
lw $t8 -28($gp)
lw $t9 -24($gp)
add $t0 $t8 $t9
lw $t8 -8($fp)
mult $t8 $t0
mflo $t1
lw $t9 -12($fp)
add $t2 $t1 $t9
lw $t8 -12($fp)
sll $t8 $t8 2
sub $t8 $gp $t8
sw $t2 -32($t8)
lw $t8 -12($fp)
li $t9 1
sub $t3 $t8 $t9
sw $t3 -12($fp)
j label_13
label_14:
lw $t4 -36($gp)
move $v0 $t4
jr $ra
main:
subi $sp $sp 80
#~~~变量声明部分结束~~~~
li $v0 5
syscall
sw $v0 -12($fp)
li $v0 5
syscall
sw $v0 -24($gp)
li $v0 5
syscall
sw $v0 -28($gp)
lw $t8 -12($fp)
li $t9 4
sne $t8 $t8 $t9
beq $t8 0 label_15
la $a0 string_3
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
label_15:
lw $t8 -12($fp)
li $t9 1
add $t0 $t8 $t9
sw $t0 0($sp)
sw $t0 -32($fp)
sw $fp -4($sp)
sw $ra -8($sp)
move $fp $sp
jal fac3
move $sp $fp
lw $fp -4($sp)
lw $ra -8($sp)
move $t0 $v0
sw $t0 -28($fp)
lw $t8 -28($fp)
li $t9 0
sne $t8 $t8 $t9
beq $t8 0 label_16
lw $t8 -28($fp)
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
j label_17
label_16:
la $a0 string_4
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
label_17:
lw $t8 -0($gp)
lw $t9 -24($gp)
add $t1 $t8 $t9
sw $t1 -20($fp)
lw $t8 -4($gp)
lw $t9 -28($gp)
add $t2 $t8 $t9
sw $t2 -24($fp)
lw $t8 -20($fp)
sw $t8 0($sp)
lw $t8 -24($fp)
sw $t8 -4($sp)
sw $t0 -36($fp)
sw $t1 -40($fp)
sw $t2 -44($fp)
sw $fp -8($sp)
sw $ra -12($sp)
move $fp $sp
jal fac2
move $sp $fp
lw $fp -8($sp)
lw $ra -12($sp)
sw $fp 0($sp)
sw $ra -4($sp)
move $fp $sp
jal fac4
move $sp $fp
lw $fp 0($sp)
lw $ra -4($sp)
move $t0 $v0
li $t8 -1
mult $t8 $t0
mflo $t1
lw $t8 -28($fp)
li $t9 2
mult $t8 $t9
mflo $t2
add $t3 $t1 $t2
lw $t8 -24($gp)
lw $t9 -20($gp)
add $t4 $t8 $t9
li $t9 43
sub $t5 $t4 $t9
lw $t8 -28($gp)
mult $t8 $t5
mflo $t6
sub $t7 $t3 $t6
sw $t7 -16($fp)
la $a0 string_5
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -16($fp)
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
