ogma: main.c
	gcc -o ogma -g main.c token.c token_list.c ../lib/nc_string.c scanner.c ast.c
