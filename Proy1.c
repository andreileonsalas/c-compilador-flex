#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include "Proy1.h"


#define NEWFRM 250
#define HISTOGRAM_LINES 50
#define HSTGRM_SIZE 10
#define DEBUG 0


extern int yylex();

// Line number of read file
extern int yylineno;

// Lexeme of current token
extern char* yytext;

int yylinecounter = NEWFRM;
int yylinelastcount = 1;
int yylinecounter_H = HISTOGRAM_LINES;
int yylinelastcount_H = 1;

char* tokens[TOTALTOKENS] = {"PREPROCESSOR","COMMENT","KEYWORD","IDENTIFIER","CONSTANTLITERAL","CONSTANTCHAR","CONSTANTSTRING","OPERATOR","PUNCTUATOR","BLANK","ERROR"};
char* colors[TOTALTOKENS] = {"OliveDrab3","Azure4","Turquoise3","Tomato2","HotPink1","Tan2","IndianRed1","DarkOrchid3","SeaGreen3","White","OrangeRed3"};

char * commands[]={"gnuplot \"histogram_script.gnu\"","pdflatex beamer.tex","okular beamer.pdf --presentation"};

FILE* file;
char* latexFile = "source.tex";


Row getToken(void)
{
	Row myRow;
	int token = yylex();
	myRow.token = token;

	myRow.lexeme = yytext;
	// printf("%s\n", myRow.lexeme);
	return myRow;
}

void addHistogramExtension()
{
	#if DEBUG
	for (int i=0; i<TOTALTOKENS; i++)
		printf(" n:%d ",histogram[current_Hist_i-1].token_count[i]);
	#endif
	fragment_info *tmp;
	current_Hist_s=current_Hist_s+HSTGRM_SIZE;
	tmp = realloc(histogram, current_Hist_s*sizeof(*histogram));
	if (!tmp)
	{
		printf("failed to realloc");
		exit(9); //failed to realloc
	}
	else
	{
		#if DEBUG
		printf("extended hist to %d\n",current_Hist_s);
		for (int i=0; i<TOTALTOKENS; i++)
			printf(" n:%d ",histogram[current_Hist_i].token_count[i]);
		printf("^ new line of extended hist\n");
		#endif
		histogram = tmp;
		//zero out the rows
		for(int a=0; a<HSTGRM_SIZE; a++)
			for(int i=i+0; i<TOTALTOKENS; i++)
				histogram[a+current_Hist_i].token_count[i]=0;
		#if DEBUG
		printf("extended hist to %d\n",current_Hist_s);
		for (int i=0; i<TOTALTOKENS; i++)
			printf(" n:%d ",histogram[current_Hist_i].token_count[i]);
		printf("^ new line of extended hist\n");
		#endif
	}
}

void writeLatexFileStart(FILE* file)
{
	fprintf(file,"\\begin{frame}[fragile,allowframebreaks]{Análisis sintáctico por colores}~");
}

void writeLatexFileEnd(FILE* file)
{
	fprintf(file,"\n\\end{frame}\n");
}

void writeTokenToLatexFile(Row rowToken,FILE* file)
{
	if(yylineno >= (yylinelastcount + yylinecounter)) // Break beamer frames
	{
		fprintf(file,"\n\\end{frame}\n\\begin{frame}[fragile,allowframebreaks]{Análisis sintáctico por colores}~");
		yylinelastcount+=yylinecounter;
	}
	if(yylineno >= (yylinelastcount_H + yylinecounter_H)) // Histogram array index and realloc
	{
		current_Hist_i++;
		#if DEBUG
		printf("hist index ++:%d, yylineno:%d\n",current_Hist_i,yylineno);
		#endif
		if(current_Hist_i % HSTGRM_SIZE == 0) // Histogram index is multiple of HSGRM_SIZE (10)
		{
			addHistogramExtension();
		}
		yylinelastcount_H = yylinelastcount_H + yylinecounter_H;
	}
	if(rowToken.token==BLANK)
	{
		if(rowToken.lexeme[0]==0x9)
			fprintf(file,"\\tab");
		if(rowToken.lexeme[0]==0xa)
			fprintf(file,"\\newline");
		if(rowToken.lexeme[0]==' ')
			fprintf(file," ");
	}
	if(rowToken.token==COMMENT)
	{
		fprintf(file,"\\color{%s}\\begin{verbatim}%s\\end{verbatim}\\leavevmode", colors[rowToken.token-1], rowToken.lexeme);
	}
	if(rowToken.token!=COMMENT&&rowToken.token!=BLANK)
	{
		fprintf(file,"\\color{%s}\\verb$%s$", colors[rowToken.token-1],rowToken.lexeme);
	}
	/* Increase the token count in histogram index */
	histogram[current_Hist_i].token_count[rowToken.token-1]++;
}

void writeTokensToDatafile(void)
{
	file = fopen("datafile.dat","w");
	fprintf(file, "x ");
	for(int i=0; i<TOTALTOKENS-1; i++)
		fprintf(file,"%s ",tokens[i]);
	fprintf(file, "\n");
	for(int i=0; i<=current_Hist_i; i++)
	{
		fprintf(file,"%d-%d ",i*HISTOGRAM_LINES,i*HISTOGRAM_LINES+HISTOGRAM_LINES);
		for(int a=0; a<=TOTALTOKENS-2; a++)
		{
			fprintf(file,"%d ",histogram[i].token_count[a]);
		}
		fprintf(file,"\n");
	}
	fclose(file);
}




int main(int argc, char *argv[])
{
//preprocesador
	char*filename=(char*)malloc(256 * sizeof(char));
	sprintf (filename,"%s",argv[1]);
	printf(argv[1]);
	//Preprosesar(filename,"TSource.in");
	//gcc -E -C %s > andrei.in",filename
	char*temp=(char*)malloc(256 * sizeof(char));
	strcpy(temp,"gcc -E -C ");
	strcat(temp,filename);
	strcat(temp," > andrei.in");
	system(temp);
	stdin = freopen("andrei.in", "r", stdin); 
//preprocesador

	
	histogram = malloc (HSTGRM_SIZE * sizeof(*histogram));
	current_Hist_s = HSTGRM_SIZE;
	#if DEBUG
	for (int i=0; i<TOTALTOKENS; i++)
		printf(" n:%d ",histogram[0].token_count[i]);
	#endif
	file = fopen(latexFile,"w");

	Row rowToken;
	rowToken = getToken();
	writeLatexFileStart(file);
	while(rowToken.token)
	{
		writeTokenToLatexFile(rowToken,file);
		rowToken = getToken();
		#if DEBUG
		for (int i=0; i<TOTALTOKENS; i++)
			printf(" n:%d ",histogram[current_Hist_i].token_count[i]);
		printf(" end line:%d\n", current_Hist_i);
		#endif
	}
	writeLatexFileEnd(file);
	fclose(file);
	writeTokensToDatafile();
	// system(commands[0]); //gnuplot
	// system(commands[1]); //pdflatex
	// system(commands[2]); //okular
	return 0;
}

