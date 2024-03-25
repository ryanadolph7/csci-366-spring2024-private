//==========================================================================================
// PROBLEM 1: Counting Up From 1 to 10 in Little Man Assembly
//==========================================================================================

// You can develop your code via the online emulator and then paste it in here
// Please use a jump in your solution
// Please use the OUT instruction to print values

        LDA current
loop    OUT
        SUB ten
        BRP exit
        LDA current
        ADD one
        STA current
        BRA loop
exit    HLT
current DAT 1
ten     DAT 10
one     DAT 1


//==========================================================================================
// PROBLEM 2: Counting Up From 1 to 10 in MIPS Assembly
//==========================================================================================

// You can develop your code via the MARS emulator and then paste it in here
// Please use a jump in your solution
// Please use sys-call in your solution
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


