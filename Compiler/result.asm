.data
string_0: .asciiz "**********************\\\\\\0\\n\\ddd"
string_1: .asciiz "Full permutation, please input a number(<100)"
string_2: .asciiz "toLowerAscii:"
string_3: .asciiz "Three integers"
string_4: .asciiz "Bye"
string_5: .asciiz "I am not main function!"
string_6: .asciiz "Please input e/r/n/v/t instead of "
.text
move $gp $sp
li $t8 100
sw $t8 0($sp)
li $t8 20
sw $t8 -4($sp)
li $t8 121
sw $t8 -8($sp)
subi $sp $sp 836
#~~~变量声明部分结束~~~~
j main
sqrt:
subi $sp $sp 16
#~~~变量声明部分结束~~~~
lw $t8 -0($fp)
lw $t9 -0($fp)
mult $t8 $t9
mflo $t0
move $v0 $t0
jr $ra
output:
subi $sp $sp 24
#~~~变量声明部分结束~~~~
li $t9 0
sw $t9 -8($fp)
label_0:
lw $t8 -8($fp)
sll $t8 $t8 2
sub $t8 $gp $t8
lw $t8 -8($fp)
sll $t8 $t8 2
sub $t8 $gp $t8
lw $t0 -412($t8)
li $t9 1
add $t1 $t0 $t9
move $a0 $t1
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -8($fp)
li $t9 1
add $t2 $t8 $t9
sw $t2 -8($fp)
lw $t8 -8($fp)
lw $t9 -832($gp)
slt $t8 $t8 $t9
beq $t8 1 label_0
la $a0 string_0
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
jr $ra
arrange:
subi $sp $sp 28
#~~~变量声明部分结束~~~~
li $t9 0
sw $t9 -12($fp)
lw $t8 -0($fp)
lw $t9 -832($gp)
seq $t8 $t8 $t9
beq $t8 0 label_1
sw $fp 0($sp)
sw $ra -4($sp)
move $fp $sp
jal output
move $sp $fp
lw $fp 0($sp)
lw $ra -4($sp)
j label_2
label_1:
label_2:
label_3:
lw $t8 -12($fp)
sll $t8 $t8 2
sub $t8 $gp $t8
lw $t8 -12($fp)
sll $t8 $t8 2
sub $t8 $gp $t8
lw $t0 -12($t8)
li $t9 0
seq $t8 $t0 $t9
beq $t8 0 label_4
lw $t8 -12($fp)
sll $t8 $t8 2
sub $t8 $gp $t8
li $t9 1
sw $t9 -12($t8)
lw $t8 -0($fp)
sll $t8 $t8 2
sub $t8 $gp $t8
lw $t9 -12($fp)
sw $t9 -412($t8)
lw $t8 -0($fp)
li $t9 1
add $t1 $t8 $t9
sw $t1 0($sp)
sw $t0 -16($fp)
sw $t1 -20($fp)
sw $fp -4($sp)
sw $ra -8($sp)
move $fp $sp
jal arrange
move $sp $fp
lw $fp -4($sp)
lw $ra -8($sp)
lw $t8 -12($fp)
sll $t8 $t8 2
sub $t8 $gp $t8
li $t9 0
sw $t9 -12($t8)
j label_5
label_4:
label_5:
lw $t8 -12($fp)
li $t9 1
add $t0 $t8 $t9
sw $t0 -12($fp)
lw $t8 -12($fp)
lw $t9 -832($gp)
slt $t8 $t8 $t9
beq $t8 1 label_3
jr $ra
foo_recursion:
subi $sp $sp 16
#~~~变量声明部分结束~~~~
li $t9 0
sw $t9 -8($fp)
la $a0 string_1
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
li $v0 5
syscall
sw $v0 -832($gp)
label_6:
lw $t8 -8($fp)
sll $t8 $t8 2
sub $t8 $gp $t8
li $t9 0
sw $t9 -12($t8)
lw $t8 -8($fp)
li $t9 1
add $t0 $t8 $t9
sw $t0 -8($fp)
lw $t8 -8($fp)
lw $t9 -832($gp)
slt $t8 $t8 $t9
beq $t8 1 label_6
li $t8 0
sw $t8 0($sp)
sw $t0 -12($fp)
sw $fp -4($sp)
sw $ra -8($sp)
move $fp $sp
jal arrange
move $sp $fp
lw $fp -4($sp)
lw $ra -8($sp)
jr $ra
foo_trans:
subi $sp $sp 28
#~~~变量声明部分结束~~~~
li $v0 12
syscall
sw $v0 -8($fp)
lw $t8 -8($fp)
li $t9 0
add $t0 $t8 $t9
sw $t0 -12($fp)
lw $t8 -8($fp)
li $t9 65
sge $t8 $t8 $t9
beq $t8 0 label_9
lw $t8 -8($fp)
li $t9 90
sle $t8 $t8 $t9
beq $t8 0 label_7
lw $t8 -8($fp)
li $t9 65
sub $t1 $t8 $t9
li $t9 97
add $t2 $t1 $t9
sw $t2 -12($fp)
j label_8
label_7:
label_8:
j label_10
label_9:
label_10:
la $a0 string_2
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -12($fp)
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
jr $ra
foo_expr:
subi $sp $sp 64
#~~~变量声明部分结束~~~~
li $t9 0
sw $t9 -20($fp)
la $a0 string_3
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
li $v0 5
syscall
sw $v0 -8($fp)
li $v0 5
syscall
sw $v0 -12($fp)
li $v0 5
syscall
sw $v0 -16($fp)
label_11:
lw $t8 -20($fp)
sll $t8 $t8 2
sub $t8 $gp $t8
lw $t9 -20($fp)
sw $t9 -412($t8)
lw $t8 -20($fp)
li $t9 1
add $t0 $t8 $t9
sw $t0 -20($fp)
lw $t8 -20($fp)
lw $t9 -0($gp)
slt $t8 $t8 $t9
beq $t8 1 label_11
lw $t8 -12($fp)
sll $t8 $t8 2
sub $t8 $gp $t8
lw $t8 -12($fp)
sll $t8 $t8 2
sub $t8 $gp $t8
lw $t1 -412($t8)
lw $t8 -8($fp)
mult $t8 $t1
mflo $t2
li $t8 -1
mult $t8 $t2
mflo $t3
li $t8 -2
li $t9 99
mult $t8 $t9
mflo $t4
add $t5 $t3 $t4
lw $t8 -16($fp)
sw $t8 0($sp)
sw $t0 -24($fp)
sw $t1 -28($fp)
sw $t2 -32($fp)
sw $t3 -36($fp)
sw $t4 -40($fp)
sw $t5 -44($fp)
sw $fp -4($sp)
sw $ra -8($sp)
move $fp $sp
jal sqrt
move $sp $fp
lw $fp -4($sp)
lw $ra -8($sp)
move $t0 $v0
lw $t1 -44($fp)
add $t2 $t1 $t0
li $t8 2
li $t9 1
mult $t8 $t9
mflo $t3
sub $t4 $t2 $t3
move $v0 $t4
jr $ra
do_nothing:
subi $sp $sp 8
#~~~变量声明部分结束~~~~
la $a0 string_4
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
jr $ra
value:
li $t8 110
sw $t8 -8($fp)
subi $sp $sp 12
#~~~变量声明部分结束~~~~
lw $t8 -8($fp)
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
li $t8 98
move $a0 $t8
li $v0 11
syscall
li $a0 '\n'
li $v0 11
syscall
li $t8 98
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
li $t8 98
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
jr $ra
main_mirror:
subi $sp $sp 8
#~~~变量声明部分结束~~~~
la $a0 string_5
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
jr $ra
main:
subi $sp $sp 16
#~~~变量声明部分结束~~~~
li $v0 12
syscall
sw $v0 -8($fp)
lw $t8 -8($fp)
li $t9 101
seq $t8 $t8 $t9
beq $t8 0 label_12
sw $fp 0($sp)
sw $ra -4($sp)
move $fp $sp
jal foo_expr
move $sp $fp
lw $fp 0($sp)
lw $ra -4($sp)
move $t0 $v0
move $a0 $t0
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
j label_21
label_12:
lw $t8 -8($fp)
li $t9 114
seq $t8 $t8 $t9
beq $t8 0 label_13
sw $t0 -12($fp)
sw $fp 0($sp)
sw $ra -4($sp)
move $fp $sp
jal foo_recursion
move $sp $fp
lw $fp 0($sp)
lw $ra -4($sp)
j label_20
label_13:
lw $t8 -8($fp)
li $t9 110
seq $t8 $t8 $t9
beq $t8 0 label_14
sw $fp 0($sp)
sw $ra -4($sp)
move $fp $sp
jal do_nothing
move $sp $fp
lw $fp 0($sp)
lw $ra -4($sp)
j label_19
label_14:
lw $t8 -8($fp)
li $t9 118
seq $t8 $t8 $t9
beq $t8 0 label_15
sw $fp 0($sp)
sw $ra -4($sp)
move $fp $sp
jal value
move $sp $fp
lw $fp 0($sp)
lw $ra -4($sp)
j label_18
label_15:
lw $t8 -8($fp)
li $t9 116
seq $t8 $t8 $t9
beq $t8 0 label_16
sw $fp 0($sp)
sw $ra -4($sp)
move $fp $sp
jal foo_trans
move $sp $fp
lw $fp 0($sp)
lw $ra -4($sp)
j label_17
label_16:
la $a0 string_6
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -8($fp)
move $a0 $t8
li $v0 11
syscall
li $a0 '\n'
li $v0 11
syscall
label_17:
label_18:
label_19:
label_20:
label_21:
