#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include "preprocessor.h"


void newEntry(char* simb,char* val,char* args,int arg){
    memset(SimbolTable[posLast][0], '\0', linesize);
    memset(SimbolTable[posLast][1], '\0', linesize);
    strcpy(SimbolTable[posLast][0], simb);
    strcpy(SimbolTable[posLast][1], val);
    if (arg){
        memset(SimbolTable[posLast][2], '\0', linesize);
        strcpy(SimbolTable[posLast][2], args);
    }
    SimbolStats[posLast]=arg;
    posLast++;
}

int tipoLinea(char *in,char* arg1,char *arg2){
    int pos=0;
    int resp=0;
    char arg[linesize];
    memset(arg1, '\0', linesize);
    memset(arg2, '\0', linesize);
    while(in[pos]==' '){
        pos++;
    }
    if (in[pos]=='#')
        pos++;
    else
        return 0;
    if(!(strncmp((in+pos),def,6))){
        resp=1;
        pos+=6;
    }else if(!(strncmp((in+pos),inc,7))){
        resp=2;
        pos+=7;
    }else
        return 0;
    while(in[pos]==' '){
        pos++;
    }
    memset(arg, '\0', linesize);
    if (resp==2){
        if (in[pos]=='"'){
            pos++;
            int i=0;
            while(in[pos]!='"'){
                arg[i]=in[pos];
                i++;
                pos++;
            }
            if (i!=0){
                strcpy(arg1,arg);
                return resp;
            }else
                return -1;
        }else if(in[pos]=='<'){
            resp++;
            pos++;
            int i=0;
            while(in[pos]!='>'){
                arg[i]=in[pos];
                i++;
                pos++;
            }
            if (i!=0){
                strcpy(arg1,arg);
                return resp;
            }else
                return -1;
        }else
            return 0;
    }
    else{
        int i=0;
        char stop = ' ';
        while(in[pos]!=stop && in[pos]!='\n'){
            arg[i]=in[pos];
            if (arg[i]=='(')
                stop=')';
            i++;
            pos++;
        }
        if((in[pos]==')')){
            arg[i]=')';
            i++;
            pos++;
        }
        if (i!=0){
            strcpy(arg1,arg);
            memset(arg, '\0', linesize);
        }else
            return -1;
        if(in[pos]=='\0'||in[pos]=='\n'){
			strcpy(arg2,"");
			return resp;
        }
        while(in[pos]==' '){
            pos++;
        }
        i=0;
        while(in[pos]!='\n' && in[pos]!='\0'){
            arg[i]=in[pos];
            i++;
            pos++;
        }
        strcpy(arg2,arg);
        return resp;
    }
}

int Comments (FILE *In,FILE* Out){
    char act;
    char temp;
    int estate=0;
    int cambiar=0;
    while((act = fgetc(In)) != EOF) {
        if (act == '/') {
                temp = fgetc(In);
                if (temp == '/'){
                    while ((estate != EOF) && (act!='\n')){
                        estate=(act = fgetc(In));
                    }
                    if (estate == EOF)
                        return 1;
                    else
                        putc(act, Out);
                }else if (temp == '*'){
					temp = ' ';
                    while (estate != EOF){
                        estate=(act = fgetc(In));
                        if ((act=='/') && (temp =='*'))
                            break;
                        temp = act;
                    }
                    if (estate == EOF)
                        return 0;
                }else{
                    putc(act, Out);
                    putc(temp, Out);
                    }
        }
        else if(act=='\r'){
            act ='\n';
            putc(act, Out);
        }
        else if(act=='\t'){
            act =' ';
            putc(act, Out);
        }
        else if(act =='\\'){
            cambiar=1;
        }
        else{
                if (cambiar){
                    putc(' ', Out);
                    cambiar=0;
                    }
                else
                    putc(act, Out);
        }
    }
    return 1;
}

int token(char*s,char*resp,int i){
    int comp=i;
    if(s[i]==' ')
        return 1+i;
    if (s[i]=='_'||isalnum(s[i])){
        while (s[i]!='\0'&&s[i]!=' '&&(s[i]=='_'||isalnum(s[i]))){
            resp[i-comp]=s[i];
            i++;
        }
    }else{
        while (s[i]!='\0'&&s[i]!=' '&&!(s[i]=='_'||isalnum(s[i]))){
            resp[i-comp]=s[i];
            i++;
        }
    }
    return i;
}

int exchangeToken(char* ele,char *sust,char*in,char*out){
    int n,k,m,resp;
    char tok[linesize]="";
    m=n=resp=0;
    k=-1;
    if(!(strcmp(tok,sust)))
		return resp;
    while(k<m){
        k=m;
        memset(tok, '\0', linesize);
        m=token(in,tok,k);
        if (k<m&&tok[0]=='\0'){
            out[n]=' ';
            n++;
        }else{
            if(!(strcmp(tok,ele))){//iguales
                memset(tok, '\0', linesize);
                strcpy(tok,sust);
                resp=1;
            }
            strcpy((out+n),tok);
            n+=strlen(tok);
        }
    }
    return resp;
}

int insertTable(int pos,char *in,char argTable[][2][linesize]){
    int k,m,n;
    char tok[linesize]="";
    m=n=0;
    k=-1;
    while(k<m){
        k=m;
        memset(tok, '\0', linesize);
        m=token(in,tok,k);
        if((strcmp(tok,","))&&(strcmp(tok,""))&&(strcmp(tok,"("))&&(strcmp(tok,")"))){//no iguales
            memset(argTable[n][pos], '\0', linesize);
            strcpy(argTable[n][pos],tok);
            n++;
        }
    }
    return n;
}

int exchange(char*in,char*out){
    char TOK[linesize]="";
    char Temp[linesize]="";
    char aux[linesize]="";
    int next,av,ant,resp,j,total;
    ant=-1;
    resp=next=av=0;
    while (ant<av){
        memset(TOK, '\0', linesize);
        ant=av;
        av=token(in,TOK,ant);
        if (ant<av&&TOK[0]=='\0'){
            Temp[next]=' ';
            next++;
        }else{
            for(int i=posLast-1;i>=0;i--){
                memset(aux, '\0', linesize);
                if(exchangeToken(SimbolTable[i][0],SimbolTable[i][1],TOK,aux)){
                    resp=1;
                    if(SimbolStats[i]){
                        char argTable[100][2][linesize];
                        char acc[linesize];
                        memset(TOK, '\0', linesize);
                        memset(acc, '\0', linesize);
                        j=0;
                        total = insertTable(0,SimbolTable[i][2],argTable);
                        while(strcmp(TOK,")")&&ant<av){
                            ant=av;
                            memset(TOK, '\0', linesize);
                            av=token(in,TOK,ant);
                            strcpy((acc+j),TOK);
                            j+=strlen(TOK);
                        }
                        insertTable(1,acc,argTable);
                        for(j=0;j<total;j++){
                            memset(TOK, '\0', linesize);
                            exchangeToken(argTable[j][0],argTable[j][1],aux,TOK);
                            strcpy(aux,TOK);
                        }
                    }
                }
                memset(TOK, '\0', linesize);
                strcpy(TOK,aux);
            }
            strcpy((Temp+next),TOK);
            next+=strlen(TOK);
        }
    }
    if(Temp[0]!='\0')
        strcpy(out,Temp);
    else
        strcpy(out,in);
    return resp;
}

int verificarArgs(char*s,char *out,char *args){
    int i,pos;
    i=pos=0;
    char stop = '\0';

    while(s[pos]!=stop){
        if (s[pos]=='('){
            stop=')';
            break;
        }
        out[pos]=s[pos];
        pos++;
    }
    if(stop=='\0'){
        return 0;
    }
    pos++;
    while(s[pos]!=stop){
        if(s[pos]!=' '){
            args[i]=s[pos];
            i++;
        }
        pos++;
    }
    return 1;
}

int Preproses (FILE *In,FILE* Out,char *filein){
    char act[linesize];
    char arg1[linesize]="";
    char arg2[linesize]="";
    char arg3[linesize]="";
    char salida[linesize]="";
    int tipo,cambio;
    FILE * Aux = tmpfile();
    if(Aux==NULL){
        printf("Error: No se pudo crear archivo temporal\n");
        fclose(Aux);
        return 0;
    }
    Comments (In,Aux);
    rewind(Aux);
    while(fgets(act, linesize, Aux)!= NULL) {
        tipo=tipoLinea(act,arg1,arg2);
        if(tipo==-1)
            printf("Advertencia: linea <%s> en archivo %s, linea no ejecutada.\n",act,filein);
        else if(tipo==1){
			if(arg2[0]!='\0'){
				cambio=1;
				while (cambio){
					memset(salida, '\0', linesize);
					cambio=exchange(arg2,salida);
					memset(arg2, '\0', linesize);
					strcpy(arg2,salida);
				}
				memset(arg2, '\0', linesize);
				memset(arg3, '\0', linesize);
				cambio=verificarArgs(arg1,arg2,arg3);
			}
            newEntry(arg2,salida,arg3,cambio);
        }
        else if (tipo==2||tipo==3){
            FILE * Temp;
            int i;
            char FileName[linesize];
            memset(FileName, '\0', linesize);
            if(tipo == 2){
                strcpy(FileName,arg1);
                Temp = fopen(FileName,"r");
                i=0;
                while(Temp==NULL&&i<5){
                    memset(FileName, '\0', linesize);
                    sprintf(FileName,PathSearch[i],arg1);
                    Temp = fopen(FileName,"r");
                    i++;
                }
            }
            else{
                sprintf(FileName,PathSearch[0],arg1);
                Temp = fopen(FileName,"r");
                i=1;
                while(Temp==NULL&&i<5){
                    memset(FileName, '\0', linesize);
                    sprintf(FileName,PathSearch[i],arg1);
                    Temp = fopen(FileName,"r");
                    i++;
                }
            }
            if(Temp!= NULL){
                int correct=Preproses (Temp,Out,FileName);
                fclose(Temp);
                if (!(correct)){
                    fclose(Aux);
                    return 0;
                    }
            }else{
                printf("Error: linea <%s> en archivo %s, Archivo %s no encontrado.\n",act,filein,arg1);
                fclose(Aux);
                return 0;
            }
        }
        else{
            cambio=1;
            tipo=0;
            while (cambio){
                tipo++;
                memset(salida, '\0', linesize);
                cambio=exchange(act,salida);
                memset(act, '\0', linesize);
                strcpy(act,salida);
                if (tipo == 20){
                    printf("Advertencia: definicion de linea actualizada multiples veces <%s>.\n",act);
                }else if(tipo == 100){
                    printf("Error: Posible definicion recursiva de linea <%s>.\n",act);
                    fclose(Aux);
                    return 0;
                }
            }
            fputs(salida,Out);
        }
        memset(act, '\0', linesize);
        memset(salida, '\0', linesize);
    }
    fclose(Aux);
    return 1;
}

int Preprosesar(char* input,char* output){
    posLast=0;
    strcpy(def,"define");
    strcpy(inc,"include");
    printf("\n\n%s\n\n",input);
    FILE* Input=fopen(input,"r");
    printf("\n\n%s\n\n",Input);
    if (Input==NULL){
	    printf("Archivo de entrada no encontrado\n");
        printf("El archivo debe tener extensión .c\n");
	    return 0;
    }
    FILE* Output=fopen(output,"w");
    if (Output==NULL){
    printf("Archivo de salida creado incorrectamente\n");
    return 0;
    }

    int res;
    fclose(Output);
    Output=fopen(output,"a");
    if (Output==NULL){
	     printf("Archivo de salida creado incorrectamente\n");
	     return 0;
    }
    res=Preproses(Input,Output,input);
    fclose(Output);
    fclose(Input);
    return res;
}