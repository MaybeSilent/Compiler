.data
string_0: .asciiz "var_a < var_b"
string_1: .asciiz "var_a <= var_b"
string_2: .asciiz "var_a > var_b"
string_3: .asciiz "var_a >= var_b"
string_4: .asciiz "var_a != var_b"
string_5: .asciiz "var_a == var_b"
string_6: .asciiz "var_a"
string_7: .asciiz "not var_a"
.text
move $gp $sp
li $t8 10
sw $t8 0($sp)
li $t8 98
sw $t8 -4($sp)
li $t8 -20
sw $t8 -8($sp)
li $t8 25
sw $t8 -12($sp)
subiu $sp $sp 440
#~~~变量声明部分结束~~~~
j main
compareTwo:
subiu $sp $sp 32
#~~~变量声明部分结束~~~~
lw $t8 -16($gp)
li $t9 0
addu $t0 $t8 $t9
lw $t9 -20($gp)
slt $t8 $t0 $t9
beq $t8 0 label_0
la $a0 string_0
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
label_0:
lw $t8 -16($gp)
li $t9 0
addu $t1 $t8 $t9
lw $t9 -20($gp)
sle $t8 $t1 $t9
beq $t8 0 label_1
la $a0 string_1
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
label_1:
lw $t8 -16($gp)
li $t9 0
addu $t2 $t8 $t9
lw $t9 -20($gp)
sgt $t8 $t2 $t9
beq $t8 0 label_2
la $a0 string_2
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
label_2:
lw $t8 -16($gp)
li $t9 0
addu $t3 $t8 $t9
lw $t9 -20($gp)
sge $t8 $t3 $t9
beq $t8 0 label_3
la $a0 string_3
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
label_3:
lw $t8 -16($gp)
li $t9 0
addu $t4 $t8 $t9
lw $t9 -20($gp)
sne $t8 $t4 $t9
beq $t8 0 label_4
la $a0 string_4
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
label_4:
lw $t8 -16($gp)
li $t9 0
addu $t5 $t8 $t9
lw $t9 -20($gp)
seq $t8 $t5 $t9
beq $t8 0 label_5
la $a0 string_5
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
label_5:
jr $ra
testIf:
subiu $sp $sp 12
#~~~变量声明部分结束~~~~
lw $t8 -16($gp)
li $t9 0
addu $t0 $t8 $t9
li $t9 0
sne $t8 $t0 $t9
beq $t8 0 label_6
la $a0 string_6
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
j label_7
label_6:
la $a0 string_7
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
label_7:
jr $ra
getMult:
subiu $sp $sp 24
#~~~变量声明部分结束~~~~
lw $t8 -0($fp)
li $t9 1
seq $t8 $t8 $t9
beq $t8 0 label_8
li $t8 1
move $v0 $t8
jr $ra
label_8:
lw $t8 -0($fp)
li $t9 1
subu $t0 $t8 $t9
sw $t0 0($sp)
sw $fp -4($sp)
sw $ra -8($sp)
move $fp $sp
jal getMult
move $sp $fp
lw $fp -4($sp)
lw $ra -8($sp)
move $t1 $v0
sw $t1 -16($fp)
lw $t8 -0($fp)
lw $t1 -16($fp)
mult $t8 $t1
mflo $t2
move $v0 $t2
jr $ra
test_do_while:
subiu $sp $sp 16
#~~~变量声明部分结束~~~~
li $t9 0
sw $t9 -8($fp)
lw $s0 -8($fp)
label_9:
li $t9 1
addu $s0 $s0 $t9
move $a0 $s0
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
li $t9 10
slt $t8 $s0 $t9
beq $t8 1 label_9
sw $s0 -8($fp)
jr $ra
test_expression:
subiu $sp $sp 60
#~~~变量声明部分结束~~~~
lw $t8 -0($fp)
lw $t9 -4($fp)
mult $t8 $t9
mflo $t0
li $t8 -1
mult $t8 $t0
mflo $t1
move $a0 $t1
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -0($fp)
lw $t9 -4($fp)
mult $t8 $t9
mflo $t2
move $a0 $t2
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -0($fp)
lw $t9 -4($fp)
div $t8 $t9
mflo $t3
move $a0 $t3
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -0($fp)
lw $t9 -0($fp)
mult $t8 $t9
mflo $t4
lw $t8 -4($fp)
lw $t9 -4($fp)
mult $t8 $t9
mflo $t5
addu $t6 $t4 $t5
move $a0 $t6
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -0($fp)
li $t9 2
mult $t8 $t9
mflo $t7
li $t9 100
sw $t3 -28($fp)
addu $t3 $t7 $t9
move $a0 $t3
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -0($fp)
li $t9 97
sw $t4 -32($fp)
addu $t4 $t8 $t9
move $a0 $t4
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -4($fp)
li $t9 98
sw $t5 -36($fp)
addu $t5 $t8 $t9
move $a0 $t5
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
jr $ra
test_for:
subiu $sp $sp 20
#~~~变量声明部分结束~~~~
li $t9 1
sw $t9 -8($fp)
li $t9 10
sw $t9 -12($fp)
lw $t9 -8($fp)
sw $t9 -16($fp)
lw $s0 -16($fp)
lw $s1 -12($fp)
label_10:
sgt $t8 $s0 $s1
beq $t8 0 label_11
move $a0 $s0
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
li $t9 2
subu $s0 $s0 $t9
j label_10
label_11:
sw $s0 -16($fp)
sw $s1 -12($fp)
lw $t9 -8($fp)
sw $t9 -16($fp)
lw $s0 -16($fp)
lw $s1 -12($fp)
label_12:
slt $t8 $s0 $s1
beq $t8 0 label_13
move $a0 $s0
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
li $t9 1
addu $s0 $s0 $t9
j label_12
label_13:
sw $s0 -16($fp)
sw $s1 -12($fp)
lw $t9 -8($fp)
sw $t9 -16($fp)
lw $s0 -16($fp)
lw $s1 -12($fp)
label_14:
slt $t8 $s0 $s1
beq $t8 0 label_15
move $a0 $s0
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
li $t9 2
addu $s0 $s0 $t9
j label_14
label_15:
sw $s0 -16($fp)
sw $s1 -12($fp)
jr $ra
feb:
subiu $sp $sp 32
#~~~变量声明部分结束~~~~
lw $t8 -0($fp)
li $t9 0
seq $t8 $t8 $t9
beq $t8 0 label_16
li $t8 0
move $v0 $t8
jr $ra
label_16:
lw $t8 -0($fp)
li $t9 1
seq $t8 $t8 $t9
beq $t8 0 label_17
li $t8 1
move $v0 $t8
jr $ra
label_17:
lw $t8 -0($fp)
li $t9 2
sge $t8 $t8 $t9
beq $t8 0 label_18
lw $t8 -0($fp)
li $t9 1
subu $t0 $t8 $t9
sw $t0 0($sp)
sw $fp -4($sp)
sw $ra -8($sp)
move $fp $sp
jal feb
move $sp $fp
lw $fp -4($sp)
lw $ra -8($sp)
move $t1 $v0
sw $t1 -16($fp)
lw $t8 -0($fp)
li $t9 2
subu $t1 $t8 $t9
sw $t1 0($sp)
sw $fp -4($sp)
sw $ra -8($sp)
move $fp $sp
jal feb
move $sp $fp
lw $fp -4($sp)
lw $ra -8($sp)
move $t2 $v0
sw $t2 -24($fp)
lw $t2 -16($fp)
lw $t3 -24($fp)
addu $t4 $t2 $t3
move $v0 $t4
jr $ra
label_18:
jr $ra
main:
subiu $sp $sp 28
#~~~变量声明部分结束~~~~
li $t9 1
sw $t9 -16($gp)
li $t9 0
sw $t9 -20($gp)
sw $fp 0($sp)
sw $ra -4($sp)
move $fp $sp
jal compareTwo
move $sp $fp
lw $fp 0($sp)
lw $ra -4($sp)
li $t9 0
sw $t9 -16($gp)
li $t9 1
sw $t9 -20($gp)
sw $fp 0($sp)
sw $ra -4($sp)
move $fp $sp
jal compareTwo
move $sp $fp
lw $fp 0($sp)
lw $ra -4($sp)
li $t9 1
sw $t9 -16($gp)
li $t9 1
sw $t9 -20($gp)
sw $fp 0($sp)
sw $ra -4($sp)
move $fp $sp
jal compareTwo
move $sp $fp
lw $fp 0($sp)
lw $ra -4($sp)
li $t8 3
sw $t8 0($sp)
sw $fp -4($sp)
sw $ra -8($sp)
move $fp $sp
jal getMult
move $sp $fp
lw $fp -4($sp)
lw $ra -8($sp)
move $t0 $v0
sw $t0 -20($fp)
lw $t0 -20($fp)
sw $t0 -16($gp)
lw $t8 -16($gp)
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
li $t9 1
sw $t9 -16($gp)
sw $fp 0($sp)
sw $ra -4($sp)
move $fp $sp
jal testIf
move $sp $fp
lw $fp 0($sp)
lw $ra -4($sp)
li $t9 0
sw $t9 -16($gp)
sw $fp 0($sp)
sw $ra -4($sp)
move $fp $sp
jal testIf
move $sp $fp
lw $fp 0($sp)
lw $ra -4($sp)
li $t9 3
sw $t9 -16($gp)
li $t9 3
sw $t9 -20($gp)
lw $t8 -16($gp)
sw $t8 0($sp)
lw $t8 -20($gp)
sw $t8 -4($sp)
sw $fp -8($sp)
sw $ra -12($sp)
move $fp $sp
jal test_expression
move $sp $fp
lw $fp -8($sp)
lw $ra -12($sp)
sw $fp 0($sp)
sw $ra -4($sp)
move $fp $sp
jal test_do_while
move $sp $fp
lw $fp 0($sp)
lw $ra -4($sp)
sw $fp 0($sp)
sw $ra -4($sp)
move $fp $sp
jal test_for
move $sp $fp
lw $fp 0($sp)
lw $ra -4($sp)
li $t8 10
sw $t8 0($sp)
sw $fp -4($sp)
sw $ra -8($sp)
move $fp $sp
jal feb
move $sp $fp
lw $fp -4($sp)
lw $ra -8($sp)
move $t1 $v0
sw $t1 -24($fp)
lw $t1 -24($fp)
sw $t1 -16($gp)
lw $t8 -16($gp)
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
