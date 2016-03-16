# CS 61C Su14 Homework 3
# Name: Kevin Chau
# Login: cs61c-gq
	
	.data
mem: 	.space 80 # 4 bytes/word * 20 words
testAB: .asciiz "Testing linked lists A and B (should be 0): "
testAC: .asciiz "Testing linked lists A and C (should be 1): "
testAD:	.asciiz "Testing linked lists A and D (should be 0): "
nl: 	.asciiz "\n"

# Linked lists for testing:
# A: 1,0,2,2,9,5,NULL
# B: 1,0,2,2,9,1,NULL
# C: 2,9,0,1,5,2,NULL
# D: 1,0,2,2,9,NULL
A5:	.word 5	0
A4: .word 9 A5
A3: .word 2 A4
A2: .word 2 A3
A1: .word 0 A2
A0: .word 1 A1

B5:	.word 1 0
B4:	.word 9 B5
B3:	.word 2 B4
B2:	.word 2 B3
B1:	.word 0 B2
B0:	.word 1 B1

C5:	.word 2 0
C4:	.word 5 C5
C3:	.word 1 C4
C2:	.word 0 C3
C1:	.word 9 C2
C0:	.word 2 C1

D4:	.word 9 0
D3:	.word 2 D4
D2:	.word 2 D3
D1:	.word 0 D2
D0:	.word 1 D1

	.text
	.globl main 

# Function that sets the values of a block of memory to zero.
# Do NOT modify this function.
# Params:
#  $a0 = beginning of block of memory
#  $a1 = number of words to set to zero
zeroMem:
	beq $a1,$zero,done
	sw $zero,0($a0)
	addiu $a0,$a0,4
	addi $a1,$a1,-1
	j zeroMem
done:
	jr $ra
	
# Function that traverses through a linked list whose values contain
# the numbers 0 to 9 and stores the count of each number into an array.
# Params:
#  $a0 = beginning of the linked list
#  $a1 = a pointer to memory for the array
#	You may assume that the memory block contains enough space for 10 elements

countList:
# YOUR CODE HERE
	addiu $sp, $sp, -4	#push stack
	sw $ra, 0($sp)	#save return address in stack
countLoop:	
	lw $t0, 0($a0)	 #load node's value into t0
	addi $t2, $0, 4	#t2=4 for mult
	mult $t0, $t2	#set offset amount by multiplying node value by 4, stored in Lo
	mflo $t3	#t3=Lo=offset
	add $t4, $a1, $t3 #t4= pointer to array spot for node value
	lw $t5, 0($t4)	#read digit's array count
	addi $t5, $t5, 1	#increment array count
	sw $t5, 0($t4)		#update array with new count
	lw $t1, 4($a0) #load next pointer into t1
	beq $t1, $0, doneCount #last pointer in list is null/zero
	move $a0, $t1	#move linked list pointer to beginning of next node
	j countLoop
doneCount:
	lw $ra, 0($sp)
	addiu $sp, $sp, 4
	jr $ra
	
# Function that checks if the numbers (0-9) stored in two linked lists are 
# anagrams of each other (ie. if two lists contains the same number 
# of each int). Each linked list will be at least 1 element long.
# You should first traverse each linked list and store the count of each digit into
# an array (memory for it is given to you), and then check if the two arrays
# are equal.
# Params:
#  $a0 = first element of linked list 1
#  $a1 = first element of linked list 2
#  $a2 = memory segment containing space for 2 arrays of 10 ints each.
#	 Note that the memory segment may contain garbage values.
#
# Return (put in $v0): 1 if linked lists are anagrams, 0 if not

isAnagram:
# YOUR CODE HERE
addiu $sp, $sp, -16
sw $ra, 12($sp)		#save ra on stack
sw $a2, 8($sp)		#save a2 on stack
sw $a1, 4($sp)		#save a1 on stack
sw $a0, 0($sp)		#save a0 on stack
# Perhaps prepare the block of memory somehow?
lw $a0, 8($sp)	#pass pointer to memory block to zeromem
addi $a1, $0, 20	#pass word number to zero
jal zeroMem
# Counts first list
lw $a0, 0($sp)
lw $a1, 8($sp)
jal countList
# Counts second list
lw $a0, 4($sp)
lw $a1, 8($sp)
addi $a1, $a1, 40
jal countList 
# Check arrays for equality
addi $v0, $0, 1		#set return to true initially
lw $a2, 8($sp)		#a2 points to arrays again
move $t0, $a2		#t0 is pointer to first array
addi $t1, $a2, 40	#t1 is pointer to second array
move $t7, $t1
anagramLoop:
beq $v0, $0, anagramDone	#break loop if not equal lists
beq $t0, $t7, anagramDone	#break when t0 goes to second array
lw $t2, 0($t0)
lw $t3, 0($t1)
beq $t2, $t3, trueCase		#continue loop if true
move $v0, $0				#else set value to false
trueCase:				
addi $t0,$t0,4				#move pointers
addi $t1, $t1,4
j anagramLoop				
anagramDone:
lw $ra, 12($sp)		#recover ra
addiu $sp, $sp, 16	#pop stack
jr $ra
# Program starts here. Do NOT modify anything below this line.
main:
	la $a0,testAB	# Test LL A and B
	li $v0,4
	syscall
	la $a0,A0
	la $a1,B0
	la $a2,mem
	jal isAnagram
	move $a0,$v0
	li $v0,1
	syscall
	la $a0,nl
	li $v0,4
	syscall 
	
	la $a0,testAC	# Test LL A and C
	li $v0,4
	syscall
	la $a0,A0
	la $a1,C0
	la $a2,mem
	jal isAnagram
	move $a0,$v0
	li $v0,1
	syscall
	la $a0,nl
	li $v0,4
	syscall

	la $a0,testAD	# Test LL A and D
	li $v0,4
	syscall
	la $a0,A0
	la $a1,D0
	la $a2,mem
	jal isAnagram
	move $a0,$v0
	li $v0,1
	syscall
	li $v0 10	# Exit
	syscall
