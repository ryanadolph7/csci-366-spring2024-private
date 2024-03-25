.text 

main: 
	li $a0, 100
	jal sum_to
	
	move $a0, $v0 #save the print value
	li $v0, 1 # print an integer
	syscall
	
	li $v0, 10 #exit
	syscall
	
sum_to:
	subi $sp, $sp, 8 # subtract 8 from the stack pointer
	move $t0, $a0 # move our a0 register val into our temp0 register 
	blez $a0, done # break if a0 register is 0
	subi $a0, $a0, 1 # if its not 0, subtract 1 from a0
	
	sw $t0, 4($sp) #save the temp value on the stack 
	sw $ra, 0($sp) #save the return address on the stack 
	
	jal sum_to # recurse back to sum_to "function"
	
	lw $t0, 4($sp) # restore the temp value from the stack with offsets on the stack pointer / current value into t0 with reg offset 
	lw $ra, 0($sp) #restore the return address from the stack
	
	lw $t0, 4($sp) # load the original value 
	add $t0, $t0, $v0 # add the return value (in v0) to t0
	
done: 
	move $v0, $t0 # move our temporary reg (sum) to v0
	addi $sp, $sp, 8 
	jr $ra # jump to whatever address is in ra
