
	.data
c:	.word 0
b:	.word 0
a:	.word 0

	.text 			# The .text assembler directive indicates
	.globl main		# The following is the code (as oppose to data)
main:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $fp, 0($sp)		# Save the frame pointer
	sub $fp, $sp, 4		# Update the frame pointer

	sub $sp, $sp, 28		# Make space for the locals
# Prologue ends


label2: 
	li $v0, 3
	sw $v0, -20($fp)
	li $t0, 2
	sw $t0, c
	lw $t1, 0($fp)
	sw $t1, -8($fp)
	li $t2, 4
	sw $t2, -16($fp)
	sw $v0, -16($fp)
	sw $v0, -20($fp)
	lw $t2, 0($fp)
	sw $t2, -20($fp)
	sw $v0, -4($fp)
	sw $t2, -12($fp)
	li $t3, 4
	sw $t3, -12($fp)
	li $t3, 4
	sw $t3, -12($fp)
	sw $t3, -16($fp)
	sw $t2, -16($fp)
	sw $t2, -12($fp)

# Epilogue Begins
	add $sp, $sp, 28
	lw $fp, 0($sp)  
	jr        $31		# Jump back to the operating system.
# Epilogue Ends

