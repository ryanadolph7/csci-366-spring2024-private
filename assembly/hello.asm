.data
	hello: .ascii "hello there"

.text
	la $a0, hello
	li $v0, 4 # load the word into v0
	syscall
	
	li $v0, 10 #system call for exit 
	syscall # exit!