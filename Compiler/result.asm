.data
.text
move $gp $sp
subi $sp $sp 0
#~~~�����������ֽ���~~~~
j main
test:
subi $sp $sp 8
#~~~�����������ֽ���~~~~
li $t8 1
move $v0 $t8
jr $ra
main:
subi $sp $sp 8
#~~~�����������ֽ���~~~~
sw $fp 0($sp)
sw $ra -4($sp)
move $fp $sp
jal test
move $sp $fp
lw $fp 0($sp)
lw $ra -4($sp)
