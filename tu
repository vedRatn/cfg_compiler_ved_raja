
	.text 			# The .text assembler directive indicates
	.globl main		# The following is the code (as oppose to data)
main:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $fp, 0($sp)		# Save the frame pointer
	sub $fp, $sp, 4		# Update the frame pointer

	sub $sp, $sp, 16		# Make space for the locals
# Prologue ends


label2: 
	li $v0, 4
	sw $v0, -8($fp)
	li $t0, 3
	sw $t0, -4($fp)
	li $t1, 10
	sw $t1, 0($fp)
	li $t2, 0
	seq $t3, $t0, $t2
	sne $t2, $t3, $v0
	bne $t2, $zero, label3 
	j label4

label3: 
	lw $v0, -8($fp)
	lw $t0, -4($fp)
	slt $t1, $v0, $t0
	lw $v0, 0($fp)
	sge $t0, $t1, $v0
	sw $t0, -8($fp)
	j label5

label4: 
	lw $v0, -8($fp)
	lw $t0, -4($fp)
	slt $t1, $v0, $t0
	lw $v0, 0($fp)
	sge $t0, $t1, $v0
	sw $t0, -4($fp)

label5: 

# Epilogue Begins
	add $sp, $sp, 16
	lw $fp, 0($sp)  
	jr        $31		# Jump back to the operating system.
# Epilogue Ends

