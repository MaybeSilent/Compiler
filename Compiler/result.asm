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
sw $fp -4($sp)
sw $ra -8($sp)
move $fp $sp
jal factorial
move $sp $fp
lw $fp -4($sp)
lw $ra -8($sp)
move $t1 $v0
lw $t8 -0($fp)
mult $t8 $t1
mflo $t2
move $v0 $t2
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
subiu $sp $sp 624
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
lw $s0 -524($fp)
lw $s1 -532($fp)
lw $s2 -528($fp)
lw $s3 -536($fp)
lw $s4 -540($fp)
label_3:
div $s2 $s0
mflo $t0
mult $t0 $s0
mflo $s4
sw $s2 0($sp)
sw $s0 -4($sp)
sw $fp -8($sp)
sw $ra -12($sp)
move $fp $sp
jal mod
move $sp $fp
lw $fp -8($sp)
lw $ra -12($sp)
move $t1 $v0
li $t9 0
seq $t8 $t1 $t9
beq $t8 0 label_6
li $t9 1
addu $s1 $s1 $t9
subu $s3 $s3 $s0
li $t9 128
sge $t8 $s1 $t9
beq $t8 0 label_4
la $a0 string_4
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
j label_5
label_4:
sll $t8 $s1 2
subu $t8 $fp $t8
sw $s0 -12($t8)
label_5:
j label_7
label_6:
label_7:
li $t9 1
addu $s0 $s0 $t9
slt $t8 $s0 $s2
beq $t8 1 label_3
sw $s0 -524($fp)
sw $s1 -532($fp)
sw $s2 -528($fp)
sw $s3 -536($fp)
sw $s4 -540($fp)
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
lw $s0 -524($fp)
lw $s1 -532($fp)
label_8:
la $a0 string_6
li $v0 4
syscall
li $a0 '\n'
li $v0 11
syscall
sll $t8 $s0 2
subu $t8 $fp $t8
lw $t2 -12($t8)
move $a0 $t2
li $v0 1
syscall
li $a0 '\n'
li $v0 11
syscall
li $t9 1
addu $s0 $s0 $t9
sle $t8 $s0 $s1
beq $t8 1 label_8
sw $s0 -524($fp)
sw $s1 -532($fp)
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
addu $t3 $t8 $t9
sw $t3 -528($fp)
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
lw $s0 -524($fp)
lw $s1 -564($fp)
lw $s2 -572($fp)
lw $s3 -568($fp)
lw $s4 -532($fp)
lw $s5 -0($gp)
lw $s6 -528($fp)
label_11:
mult $s1 $s0
mflo $s3
mult $s6 $s4
mflo $s2
mult $s1 $s0
mflo $s1
addu $t4 $s1 $s3
addu $s1 $t4 $s2
li $t9 1
addu $s0 $s0 $t9
slt $t8 $s0 $s5
beq $t8 1 label_11
sw $s0 -524($fp)
sw $s1 -564($fp)
sw $s2 -572($fp)
sw $s3 -568($fp)
sw $s4 -532($fp)
sw $s5 -0($gp)
sw $s6 -528($fp)
lw $t9 -564($fp)
sw $t9 -568($fp)
li $t9 0
sw $t9 -524($fp)
lw $s0 -524($fp)
lw $s1 -568($fp)
lw $s2 -0($gp)
label_12:
addu $s1 $s1 $s0
li $t9 1
addu $s0 $s0 $t9
slt $t8 $s0 $s2
beq $t8 1 label_12
sw $s0 -524($fp)
sw $s1 -568($fp)
sw $s2 -0($gp)
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
mflo $t5
sw $t5 -548($fp)
li $t9 2
sw $t9 -524($fp)
lw $s0 -524($fp)
lw $s1 -544($fp)
lw $s2 -556($fp)
lw $s3 -548($fp)
lw $s4 -560($fp)
label_14:
div $s1 $s0
mflo $t6
mult $t6 $s0
mflo $s4
sw $s1 0($sp)
sw $s0 -4($sp)
sw $fp -8($sp)
sw $ra -12($sp)
move $fp $sp
jal mod
move $sp $fp
lw $fp -8($sp)
lw $ra -12($sp)
move $t7 $v0
li $t9 0
seq $t8 $t7 $t9
beq $t8 0 label_15
li $t9 0
move $s2 $t9
j label_16
label_15:
label_16:
li $t9 1
addu $s0 $s0 $t9
sle $t8 $s0 $s3
beq $t8 1 label_14
sw $s0 -524($fp)
sw $s1 -544($fp)
sw $s2 -556($fp)
sw $s3 -548($fp)
sw $s4 -560($fp)
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
sw $t0 -576($fp)
addu $t0 $t8 $t9
sw $t0 -552($fp)
lw $t8 -552($fp)
li $t9 10
div $t8 $t9
sw $t1 -580($fp)
mflo $t1
li $t9 10
mult $t1 $t9
sw $t2 -584($fp)
mflo $t2
sw $t2 -560($fp)
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
sw $t3 -588($fp)
addu $t3 $t8 $t9
sw $t3 -544($fp)
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
