.data
	emptyline: .ascii "\n" # for formatting output 

.text 
	move $t0, $0 # put 0 into register t0, this is our current number
	li $t1, 11 # load 10 into t2 register, this is our goal number
	addi $t0, $t0, 1 # instead of starting at 0, we add 1 to t0 so we start at value 1
	
start:
	li $v0, 4 # load the print string syscall into v0
	la $a0, emptyline # load the emptyline into a0
	syscall # print the new line characteer
	
	beq $t0, $t1, exit # loop until reg t0 and t1 are even
	
	la $a0, ($t0) # load our current value into a0 from t0
	li $v0, 1 # load the print int syscall into v0
	syscall # print the integer
	
	addi $t0, $t0, 1 # add 1 to our current value
	
	j start # infinitely jump back to the start of our "program"
	
	
exit: 
	li $v0, 10 #system call for exit 
	syscall # exit the "program"