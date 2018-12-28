.data
string_0: .asciiz "ENTERED AT FIRST"
string_1: .asciiz "ENTERED AT SECOND"
.text
move $gp $sp
subiu $sp $sp 4
#~~~变量声明部分结束~~~~
j main
first:
subiu $sp $sp 8
#~~~变量声明部分结束~~~~
li $t9 1
sw $t9 -0($gp)
li $t8 0
move $v0 $t8
jr $ra
second:
subiu $sp $sp 8
#~~~变量声明部分结束~~~~
li $t9 2
sw $t9 -0($gp)
li $t8 2
move $v0 $t8
jr $ra
main:
subiu $sp $sp 24
#~~~变量声明部分结束~~~~
li $t9 0
sw $t9 -0($gp)
lw $t8 -0($gp)
li $t9 0
addu $t0 $t8 $t9
sw $t0 -8($fp)
sw $fp 0($sp)
sw $ra -4($sp)
move $fp $sp
jal first
move $sp $fp
lw $fp 0($sp)
lw $ra -4($sp)
move $t0 $v0
lw $t1 -8($fp)
seq $t8 $t1 $t0
beq $t8 0 label_0
la $a0 string_0
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
label_0:
lw $t8 -0($gp)
li $t9 0
addu $t2 $t8 $t9
sw $t0 -12($fp)
sw $t1 -8($fp)
sw $t2 -16($fp)
sw $fp 0($sp)
sw $ra -4($sp)
move $fp $sp
jal second
move $sp $fp
lw $fp 0($sp)
lw $ra -4($sp)
move $t0 $v0
lw $t1 -16($fp)
seq $t8 $t1 $t0
beq $t8 0 label_1
la $a0 string_1
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
label_1:
