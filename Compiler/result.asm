.data
.text
move $gp $sp
subiu $sp $sp 0
#~~~变量声明部分结束~~~~
j main
main:
subiu $sp $sp 16
#~~~变量声明部分结束~~~~
li $t9 10
sw $t9 -8($fp)
label_0:
lw $t8 -8($fp)
li $t9 1
subu $t0 $t8 $t9
sw $t0 -8($fp)
lw $t8 -8($fp)
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -8($fp)
li $t9 0
sne $t8 $t8 $t9
beq $t8 1 label_0
