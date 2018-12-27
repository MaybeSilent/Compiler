.data
string_0: .asciiz " x = "
string_1: .asciiz " y = "
string_2: .asciiz " SWAP x = "
string_3: .asciiz " SWAP y = "
string_4: .asciiz " OVERFLOW!          "
string_5: .asciiz " complete number: "
string_6: .asciiz "  "
string_7: .asciiz "          "
string_8: .asciiz " ---------------------------------------------------------------          "
string_9: .asciiz " b = "
string_10: .asciiz " "
string_11: .asciiz "          The total is "
string_12: .asciiz " 10! = "
.text
move $gp $sp
li $t8 1024
sw $t8 0($sp)
subiu $sp $sp 4
#~~~变量声明部分结束~~~~
j main
factorial:
subiu $sp $sp 24
#~~~变量声明部分结束~~~~
lw $t8 -0($fp)
li $t9 1
sle $t8 $t8 $t9
beq $t8 0 label_0
li $t8 1
move $v0 $t8
jr $ra
j label_1
label_0:
lw $t8 -0($fp)
li $t9 1
subu $t0 $t8 $t9
sw $t0 0($sp)
sw $t0 -12($fp)
sw $fp -4($sp)
sw $ra -8($sp)
move $fp $sp
jal factorial
move $sp $fp
lw $fp -4($sp)
lw $ra -8($sp)
move $t0 $v0
lw $t8 -0($fp)
mult $t8 $t0
mflo $t1
move $v0 $t1
jr $ra
label_1:
jr $ra
mod:
subiu $sp $sp 28
#~~~变量声明部分结束~~~~
lw $t8 -0($fp)
lw $t9 -4($fp)
div $t8 $t9
mflo $t0
lw $t9 -4($fp)
mult $t0 $t9
mflo $t1
lw $t8 -0($fp)
subu $t2 $t8 $t1
sw $t2 -0($fp)
lw $t8 -0($fp)
move $v0 $t8
jr $ra
swap:
subiu $sp $sp 20
#~~~变量声明部分结束~~~~
la $a0 string_0
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -0($fp)
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
la $a0 string_1
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -4($fp)
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t9 -0($fp)
sw $t9 -16($fp)
lw $t9 -4($fp)
sw $t9 -0($fp)
lw $t9 -16($fp)
sw $t9 -4($fp)
la $a0 string_2
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -0($fp)
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
la $a0 string_3
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -4($fp)
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
jr $ra
complete_num:
subiu $sp $sp 680
#~~~变量声明部分结束~~~~
lw $t9 -0($fp)
sw $t9 -528($fp)
label_2:
li $t9 -1
sw $t9 -532($fp)
lw $t9 -528($fp)
sw $t9 -536($fp)
li $t9 1
sw $t9 -524($fp)
label_3:
lw $t8 -528($fp)
lw $t9 -524($fp)
div $t8 $t9
mflo $t0
lw $t9 -524($fp)
mult $t0 $t9
mflo $t1
sw $t1 -540($fp)
lw $t8 -528($fp)
sw $t8 0($sp)
lw $t8 -524($fp)
sw $t8 -4($sp)
sw $t0 -576($fp)
sw $t1 -580($fp)
sw $fp -8($sp)
sw $ra -12($sp)
move $fp $sp
jal mod
move $sp $fp
lw $fp -8($sp)
lw $ra -12($sp)
move $t0 $v0
li $t9 0
seq $t8 $t0 $t9
beq $t8 0 label_6
lw $t8 -532($fp)
li $t9 1
addu $t1 $t8 $t9
sw $t1 -532($fp)
lw $t8 -536($fp)
lw $t9 -524($fp)
subu $t2 $t8 $t9
sw $t2 -536($fp)
lw $t8 -532($fp)
li $t9 128
sge $t8 $t8 $t9
beq $t8 0 label_4
la $a0 string_4
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
j label_5
label_4:
lw $t8 -532($fp)
sll $t8 $t8 2
subu $t8 $fp $t8
lw $t9 -524($fp)
sw $t9 -12($t8)
label_5:
j label_7
label_6:
label_7:
lw $t8 -524($fp)
li $t9 1
addu $t3 $t8 $t9
sw $t3 -524($fp)
lw $t8 -524($fp)
lw $t9 -528($fp)
slt $t8 $t8 $t9
beq $t8 1 label_3
lw $t8 -536($fp)
li $t9 0
seq $t8 $t8 $t9
beq $t8 0 label_9
la $a0 string_5
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -528($fp)
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
li $t9 0
sw $t9 -524($fp)
label_8:
la $a0 string_6
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -524($fp)
sll $t8 $t8 2
subu $t8 $fp $t8
lw $t8 -524($fp)
sll $t8 $t8 2
subu $t8 $fp $t8
lw $t4 -12($t8)
move $a0 $t4
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -524($fp)
li $t9 1
addu $t5 $t8 $t9
sw $t5 -524($fp)
lw $t8 -524($fp)
lw $t9 -532($fp)
sle $t8 $t8 $t9
beq $t8 1 label_8
la $a0 string_7
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
j label_10
label_9:
label_10:
lw $t8 -528($fp)
li $t9 1
addu $t6 $t8 $t9
sw $t6 -528($fp)
lw $t8 -528($fp)
lw $t9 -0($gp)
slt $t8 $t8 $t9
beq $t8 1 label_2
la $a0 string_8
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
li $t9 0
sw $t9 -564($fp)
li $t9 0
sw $t9 -524($fp)
label_11:
lw $t8 -564($fp)
lw $t9 -524($fp)
mult $t8 $t9
mflo $t7
sw $t7 -568($fp)
lw $t8 -528($fp)
lw $t9 -532($fp)
mult $t8 $t9
sw $t0 -584($fp)
mflo $t0
sw $t0 -572($fp)
lw $t8 -564($fp)
lw $t9 -524($fp)
mult $t8 $t9
sw $t1 -588($fp)
mflo $t1
sw $t1 -564($fp)
lw $t8 -564($fp)
lw $t9 -568($fp)
sw $t2 -592($fp)
addu $t2 $t8 $t9
lw $t9 -572($fp)
sw $t3 -596($fp)
addu $t3 $t2 $t9
sw $t3 -564($fp)
lw $t8 -524($fp)
li $t9 1
sw $t4 -600($fp)
addu $t4 $t8 $t9
sw $t4 -524($fp)
lw $t8 -524($fp)
lw $t9 -0($gp)
slt $t8 $t8 $t9
beq $t8 1 label_11
lw $t9 -564($fp)
sw $t9 -568($fp)
li $t9 0
sw $t9 -524($fp)
label_12:
lw $t8 -568($fp)
lw $t9 -524($fp)
sw $t5 -604($fp)
addu $t5 $t8 $t9
sw $t5 -568($fp)
lw $t8 -524($fp)
li $t9 1
sw $t6 -608($fp)
addu $t6 $t8 $t9
sw $t6 -524($fp)
lw $t8 -524($fp)
lw $t9 -0($gp)
slt $t8 $t8 $t9
beq $t8 1 label_12
la $a0 string_9
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -568($fp)
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
la $a0 string_8
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
li $t9 0
sw $t9 -552($fp)
li $t9 1
sw $t9 -556($fp)
li $t9 2
sw $t9 -544($fp)
label_13:
lw $t8 -544($fp)
li $t9 2
div $t8 $t9
sw $t0 -616($fp)
mflo $t0
sw $t0 -548($fp)
li $t9 2
sw $t9 -524($fp)
label_14:
lw $t8 -544($fp)
lw $t9 -524($fp)
div $t8 $t9
sw $t1 -620($fp)
mflo $t1
lw $t9 -524($fp)
mult $t1 $t9
sw $t2 -624($fp)
mflo $t2
sw $t2 -560($fp)
lw $t8 -544($fp)
sw $t8 0($sp)
lw $t8 -524($fp)
sw $t8 -4($sp)
sw $t0 -644($fp)
sw $t1 -648($fp)
sw $t2 -652($fp)
sw $t3 -628($fp)
sw $t4 -632($fp)
sw $t5 -636($fp)
sw $t6 -640($fp)
sw $t7 -612($fp)
sw $fp -8($sp)
sw $ra -12($sp)
move $fp $sp
jal mod
move $sp $fp
lw $fp -8($sp)
lw $ra -12($sp)
move $t0 $v0
li $t9 0
seq $t8 $t0 $t9
beq $t8 0 label_15
li $t9 0
sw $t9 -556($fp)
j label_16
label_15:
label_16:
lw $t8 -524($fp)
li $t9 1
addu $t1 $t8 $t9
sw $t1 -524($fp)
lw $t8 -524($fp)
lw $t9 -548($fp)
sle $t8 $t8 $t9
beq $t8 1 label_14
lw $t8 -556($fp)
li $t9 1
seq $t8 $t8 $t9
beq $t8 0 label_19
la $a0 string_10
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -544($fp)
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -552($fp)
li $t9 1
addu $t2 $t8 $t9
sw $t2 -552($fp)
lw $t8 -552($fp)
li $t9 10
div $t8 $t9
mflo $t3
li $t9 10
mult $t3 $t9
mflo $t4
sw $t4 -560($fp)
lw $t8 -560($fp)
lw $t9 -552($fp)
seq $t8 $t8 $t9
beq $t8 0 label_17
la $a0 string_7
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
j label_18
label_17:
label_18:
j label_20
label_19:
label_20:
li $t9 1
sw $t9 -556($fp)
lw $t8 -544($fp)
li $t9 1
addu $t5 $t8 $t9
sw $t5 -544($fp)
lw $t8 -544($fp)
lw $t9 -0($gp)
sle $t8 $t8 $t9
beq $t8 1 label_13
la $a0 string_11
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -552($fp)
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
jr $ra
main:
subiu $sp $sp 16
#~~~变量声明部分结束~~~~
li $t8 10
sw $t8 0($sp)
sw $fp -4($sp)
sw $ra -8($sp)
move $fp $sp
jal factorial
move $sp $fp
lw $fp -4($sp)
lw $ra -8($sp)
move $t0 $v0
sw $t0 -8($fp)
la $a0 string_12
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
lw $t8 -8($fp)
move $a0 $t8
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
li $t8 5
sw $t8 0($sp)
li $t8 10
sw $t8 -4($sp)
sw $t0 -12($fp)
sw $fp -8($sp)
sw $ra -12($sp)
move $fp $sp
jal swap
move $sp $fp
lw $fp -8($sp)
lw $ra -12($sp)
li $t8 2
sw $t8 0($sp)
sw $fp -4($sp)
sw $ra -8($sp)
move $fp $sp
jal complete_num
move $sp $fp
lw $fp -4($sp)
lw $ra -8($sp)
