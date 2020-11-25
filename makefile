.SUFFIXES:
all: lex scanner preprocesador

scanner: Proy1.h Proy1.c lex.yy.c
	cc lex.yy.c Proy1.c -o scanner

lex: FlexScanner.l
	lex FlexScanner.l > lex.yy.c

preprocesador: preprocesador.c
	cc preprocesador.c -o preprocesador

clean:
	rm lex.yy.c
	rm scanner
