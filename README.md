# Hw03Parser
Part 3 in the compiler project, parser and code generation

The files of the project:
	lex.c
	lex.h
	parser.c
	parser.h
	codegen.c
	codegen.h
	vm.c
	vm.h
	driver.c

For normal compilation and running:
To compile: $ gcc driver.c parser.c codegen.c vm.c lex.c
To run: 	$ ./a.out input.txt

For testing:
To compile: $ gcc driver.c parser.c codegen.c vm.c lex.c
To run: $ bash test#.sh
To push output to file: $ bash test#.sh > outputfilename.txt


All credit given to @theshinymew#4122 and @IpodKid20Konkichi#4674 on discord for the test case file and scripts (everything in the 'test' file and test1-3.sh files)
