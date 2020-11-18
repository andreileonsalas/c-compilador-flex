#ifndef PRECOMPILER_H_INCLUDED
#define PRECOMPILER_H_INCLUDED
#define linesize 512
#define simbolsize 500

char SimbolTable[simbolsize][3][linesize];//simb,val,args
int SimbolStats[simbolsize];//arg
char PathSearch[5][linesize];//arg

int posLast;
char def[linesize];
char inc[linesize];

void newEntry(char* simb,char* val,char* args,int arg);
int tipoLinea(char *in,char* arg1,char *arg2);
int Comments (FILE *In,FILE* Out);
int token(char*s,char*resp,int i);
int exchangeToken(char* ele,char *sust,char*in,char*out);
int insertTable(int pos,char *in,char argTable[][2][linesize]);
int exchange(char*in,char*out);
int verificarArgs(char*s,char *out,char *args);
int Preproses (FILE *In,FILE* Out,char *filein);
int Preprosesar(char* input,char* output);

#endif // PRECOMPILER_H_INCLUDED
