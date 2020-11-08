# Hw03Parser
Part 3 in the compiler project


These files:
	lex.c
	lex.h
	parser.c
	parser.h
	codegen.c
	codegen.h
	vm.c
	vm.h
	driver.c
are an example of having multiple files for your homework 3. They are just
a suggestion. You should definitely edit them for your own implementation,
especially driver.c. driver.c contains some code for implementing command
line arguments which you can use for compiler directives.

To compile: $ gcc driver.c parser.c codegen.c vm.c lex.c
To run: 	$ ./a.out input.txt
