  Procedure: main
  Intermediate Code Statements

label2: 
 iLoad:	v0 <- 4
 store:	a <- v0
 iLoad:	v0 <- 3
 store:	b <- v0
 iLoad:	v0 <- 2
 store:	c <- v0
 iLoad:	v0 <- 10
 store:	d <- v0
 load:	v0 <- b
 load:	t0 <- c
 sgt: t1 <- v0 , t0
 load:	v0 <- d
 sgt: t0 <- t1 , v0
 bne: t0 , zero : goto label3
 goto label4

label3: 
 iLoad:	v0 <- 10
 store:	iftmp0 <- v0
 goto label5

label4: 
 iLoad:	v0 <- 11
 store:	iftmp0 <- v0

label5: 
 load:	v0 <- iftmp0
 store:	a <- v0

	.text 			# The .text assembler directive indicates
	.globl main		# The following is the code (as oppose to data)
main:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $fp, 0($sp)		# Save the frame pointer
	sub $fp, $sp, 4		# Update the frame pointer

	sub $sp, $sp, 20		# Make space for the locals
# Prologue ends


label2: 
	li, $v0, 4
	sw, $v0, 12($fp)
	li, $v0, 3
	sw, $v0, 8($fp)
	li, $v0, 2
	sw, $v0, 4($fp)
	li, $v0, 10
	sw, $v0, 0($fp)
	lw, $v0, 8($fp)
	lw, $t0, 4($fp)
	sgt $t1, $v0, $t0
	lw, $v0, 0($fp)
	sgt $t0, $t1, $v0
	bne $t0, $zero, label3 
	j label4

label3: 
	li, $v0, 10
	sw, $v0, 16($fp)
	j label5

label4: 
	li, $v0, 11
	sw, $v0, 16($fp)

label5: 
	lw, $v0, 16($fp)
	sw, $v0, 12($fp)

# Epilogue Begins
	add $sp, $sp, 20
	lw $fp, 0($sp)  
	jr        $31		# Jump back to the operating system.
# Epilogue Ends

