.SUFFIXES:
all: lex scanner

scanner: Proy1.h Proy1.c lex.yy.c
	cc lex.yy.c Proy1.c -o scanner

lex: FlexScanner.l
	lex FlexScanner.l > lex.yy.c

clean:
	rm lex.yy.c
	rm scanner
