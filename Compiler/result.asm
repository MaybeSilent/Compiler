.data
.text
move $gp $sp
subiu $sp $sp 24
#~~~变量声明部分结束~~~~
j main
test:
subiu $sp $sp 56
#~~~变量声明部分结束~~~~
lw $t8 -0($fp)
lw $t9 -4($fp)
addu $t0 $t8 $t9
lw $t9 -8($fp)
addu $t1 $t0 $t9
lw $t9 -12($fp)
addu $t2 $t1 $t9
lw $t9 -16($fp)
addu $t3 $t2 $t9
lw $t9 -20($fp)
addu $t4 $t3 $t9
sw $t4 -32($fp)
lw $t8 -32($fp)
move $v0 $t8
jr $ra
main:
subiu $sp $sp 28
#~~~变量声明部分结束~~~~
li $t9 1
sw $t9 -8($fp)
li $t9 2
sw $t9 -12($fp)
lw $t8 -8($fp)
lw $t9 -12($fp)
addu $t0 $t8 $t9
sw $t0 0($sp)
lw $t8 -8($fp)
lw $t9 -12($fp)
subu $t1 $t8 $t9
sw $t1 -4($sp)
li $t8 0
sw $t8 -8($sp)
li $t8 0
sw $t8 -12($sp)
li $t8 100
sw $t8 -16($sp)
li $t8 0
sw $t8 -20($sp)
sw $t0 -16($fp)
sw $t1 -20($fp)
sw $fp -24($sp)
sw $ra -28($sp)
move $fp $sp
jal test
move $sp $fp
lw $fp -24($sp)
lw $ra -28($sp)
move $t0 $v0
move $a0 $t0
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
