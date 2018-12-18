.data
.text
move $gp $sp
subi $sp $sp 0
#~~~变量声明部分结束~~~~
j main
test:
subi $sp $sp 8
#~~~变量声明部分结束~~~~
li $t8 1
move $v0 $t8
jr $ra
main:
subi $sp $sp 8
#~~~变量声明部分结束~~~~
sw $fp 0($sp)
sw $ra -4($sp)
move $fp $sp
jal test
move $sp $fp
lw $fp 0($sp)
lw $ra -4($sp)
