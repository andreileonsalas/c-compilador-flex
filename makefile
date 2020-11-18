.SUFFIXES:
all: lex scanner

scanner: Proy1.h preprocessor.h Proy1.c lex.yy.c
	cc lex.yy.c Proy1.c preprocessor.c -o scanner -w

lex: FlexScanner.l
	lex FlexScanner.l > lex.yy.c

clean:
	rm lex.yy.c
	rm scanner
