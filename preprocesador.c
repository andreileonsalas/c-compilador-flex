#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXARCHIVOS 10
#define MAXCHARS 1024


typedef struct nodo nodo;

struct nodo{
	char nombre[100];
	char valor[100];
	char tipo[100];
    nodo* siguiente;
};

//Variables de preprocesador

nodo* primero = NULL;
nodo* ultimo = NULL;
int validateInput = 1;
FILE** archivos;
FILE *outputFile;
int indexPila = -1;
char buffer[MAXCHARS];
const char* compilar= "make";//comando 1
const char* ejecutar = " ./scanner < output.c";//comando 2




//Funciones del preprocesador
void createOutput();
void closeOutput();
void inputValidation(int argc, char** argv);
void reset();
void addFile(char* fileName);
void cleanBuffer();
void includeValidation(char* tira);
void defineValidation(char* tira);
void addFile(char* fileName);
void addDefine(char* nombre, char* valor);
void printInfo();
void analizeBuffer(char* linea);
void readFile(FILE* archivo1);
int checkExistence(nodo* nodo1);
char *replaceWord(char *fila, char *encontrar, char *reemplazo);
char *findDefine(char *linea);
char *findValue(char *linea);
char *findName(char *linea);
void tableChange(char *nombre, char *valor);





//implementacion de funciones-------------

void createOutput(){
	outputFile = fopen("output.c","w"); //w+
}

void closeOutput(){
	fclose(outputFile);
}




//funcion que setea variables a valores predefinidos
void reset(){
	archivos = malloc(sizeof(FILE*) * MAXARCHIVOS);
	validateInput = 1;
	indexPila = -1;
	
}

//funcion que valida que se este ingresando un archivo.c de entrada estandar
void inputValidation(int argc, char** argv){
	reset();
	
	if (argc != 2){
		printf("Error, se debe pasar exactamente el nombre de un archivo seguido de .c\n");
	}else{
		char* fileName = argv[argc-1];
		int largo_nombre_archivo = strlen(fileName);
		
		//revisamos si archivo termina en .c
		if (fileName[largo_nombre_archivo-1] == 'c'  && fileName[largo_nombre_archivo-2] == '.'){
				validateInput = 0;
				addFile(fileName);
				readFile(archivos[0]);
				fclose(archivos[0]);
		}else{
				printf("Debe ingresar un archivo con extension .c \n");
		}
	}
}




//funcion para abrir includes
void addFile(char* fileName){
	FILE* archivo;
	archivo = fopen(fileName,"r");
	if(archivo){
		archivos[++indexPila] = archivo;
		int num_indice = indexPila;
		//printf("#include %s \n",fileName); 
		fprintf(outputFile,"//#include %s \n",fileName);
		readFile(archivo); 
		printf("Indice: %d \n",num_indice);
		fclose(archivos[num_indice]);
		}
	else{
		printf("Error en el include %s \n",fileName);
		}	
}

void addFile(char* fileName){

	archivos[++indexPila] = fopen(fileName,"r");
	//printf("Archivo agregado %s \n",fileName);
	
}



//funcion que limpia buffer
void cleanBuffer(){
	int i;
	for (i = 0; i < MAXCHARS; i++){
		buffer[i] = 0;
	}
}


//funcion que realizar lectura de archivos
void readFile(FILE* archivo_actual){	
	int indice_buffer = 0;
	char c;
	cleanBuffer();
	
		while(1){
				c = getc(archivo_actual);
				
				if (c == '\n'){
                          
						analizeBuffer(buffer);
						cleanBuffer();
						indice_buffer = 0;
						
				}else if(c == EOF){
						break;
				}
				buffer[indice_buffer++] = c;
		}

}


void includeValidation(char* tira){
	 int n = 0; 
     while(1){
		 if (isspace(tira[n])){
			 n++;
			}else{
			break;
			}								 
	} if(tira[n] == '#'){n=n+9;}
	 int indice =0;
	 char nombre[MAXCHARS];
	 //limpiar el nombre
	 int p;
	 for (p = 0; p < MAXCHARS; p++){
		nombre[p] = 0;
 	 } 
	 while(1){
		 if (isspace(tira[n])){
			 n++;
		 }else{
			if (tira[n] == '/' && tira[n+1] == '*'){
				n++;
				n++;
				while (tira[n] != '*' && tira[n+1] != '/'){
					n++;
				}
				n++;
				n++;

			}else{
				break;
			}								 
		 }
	}

	
	
	if((tira[n] != '"') ){ if((tira[n] == '<') ){ fprintf(outputFile,"%s \n",tira); //printf("%s \n",tira); //aqui poner en el archivo
		} else {printf("Error en el include \n");}}
	else{
   
		n++;
		 while(1){
			 if(tira[n] == 0){printf("Error en el include \n"); break;}
			 if ( (tira[n] == '"') ){
				 addFile(nombre);  //Aqui trata de abrir con el nombre y si esta incorrecto no lo guardamos
				 break;			 
				}else{
					
				nombre[indice++] = tira[n];}
			n++;
		}}
	 
	}
	
void defineValidation(char* tira){
	int n = 0; 
     while(1){
		 if (isspace(tira[n])){
			 n++;

		 }else{
			if (tira[n] == '/' && tira[n+1] == '*'){
				n++;
				n++;
				while (tira[n] != '*' && tira[n+1] != '/'){
					n++;
				}
				n++;
				n++;

			}else{
				break;
			}								 
		 }
   								 
	} if(tira[n] == '#'){n=n+7;}
	 int indice =0;
	 int indice2 =0;
	 char nombre[MAXCHARS];
	 char nombre2[MAXCHARS];
	 //limpiar los nombres
	 int p;
	 for (p = 0; p < MAXCHARS; p++){
		nombre[p] = 0;
		nombre2[p] = 0;
 	 } 
	 while(1){
		 if (isspace(tira[n])){
			 n++;
			}else{
			break;
			}								 
	}
	if(! isalpha(tira[n]) ){ printf("Error en el define \n"); }
	else{	   
		 while(1){
			 if (isspace(tira[n])){
				
				 break;			 
				}else{					
				nombre[indice++] = tira[n];}
			n++;
		 } 
		 	    if(tira[n] == 0){ printf("Error en el define \n");}
		 else{	
		 while(1){
			 if (isspace(tira[n])){
				 n++;
				 }
				 else{ break;}			 
			 } 
		while(1){
			if(tira[n] == 0){
				 addDefine(nombre,nombre2);
				 break;
				}
			else{
				nombre2[indice2++] = tira[n];
				n++;
				}
			}		 
		}}
}
	
	
void addDefine(char* nombre, char* valor){
	nodo* nuevo = (nodo*) malloc(sizeof(nodo));
	strcpy(nuevo->nombre,nombre);
	strcpy(nuevo->tipo,"define");
	//ver si el valor es un define previo 
	char *nuevo_valor;
		nuevo_valor = findValue(valor);
		if(strcmp(nuevo_valor,"")){
			char *nombre2 = findName(valor);
			  nuevo_valor = replaceWord(valor,nombre2,nuevo_valor);
			  strcpy(nuevo->valor,nuevo_valor);
			  fprintf(outputFile,"//#define %s %s \n",nombre,nuevo->valor);
			}else{	
				nodo* actual = primero;
				int distinto =1;
				while(actual != NULL && strcmp(actual->tipo,"define") == 0){
					
				if(strcmp(valor,actual->nombre) == 0){ //son iguales
					  strcpy(nuevo->valor,actual->valor);
					  fprintf(outputFile,"//#define  %s %s \n",nombre,nuevo->valor);
					  distinto = 0;
					  break;
					}						
				actual = actual->siguiente;		
				}
				if(distinto == 1){ //no se habian definido antes
						strcpy(nuevo->valor,valor);
						fprintf(outputFile,"//#define  %s %s \n",nombre,nuevo->valor);}
				}	
	nuevo->siguiente = NULL;
	if( checkExistence(nuevo) == 1){
		if(primero == NULL){ primero = nuevo;}
		else{ 
			nodo* nodo_actual = primero; 
			while(nodo_actual->siguiente != NULL){
				nodo_actual = nodo_actual->siguiente;
			}
			nodo_actual->siguiente = nuevo;
		}
		
	}else{tableChange(nombre,nuevo->valor); printf("Warning : Ya existe un define llamado %s \n",nombre);}
}



void tableChange(char *nombre, char *valor) {
	nodo* actual = primero;
	while(actual != NULL){
		if(strcmp(actual->nombre,nombre) == 0){
			strcpy(actual->valor,valor);
			//printf("Cambie  %s por el nuevo valor %s \n",actual->nombre,actual->valor);
		} 
		actual = actual->siguiente;		
		}	

	
}




void printInfo(){
	nodo* actual = primero;
	while(actual != NULL){
		printf(" tipo, nombre y valor: %s %s %s \n",actual->tipo,actual->nombre,actual->valor); 
		actual = actual->siguiente;		
		}	
}

int checkExistence(nodo* nodo1){ //0 for true, 1 for false (not exist)
	nodo* actual = primero;
	while(actual != NULL){
		if(strcmp(actual->nombre,nodo1->nombre) == 0){
			return 0;}
		actual = actual->siguiente;		
		}	
	return 1;	
}

void analizeBuffer(char* linea){
	int n=0;
	//Ver si hay espacios al inicio
	while(1){
		if (isspace(linea[n])){
			 n++;
		}else{
			break;
		}								 
	}
	//Ver si es un #include o un #define
	if(linea[n] == '#'){
		if(linea[n+1] == 'd' && linea[n+2] == 'e' && linea[n+3] == 'f' && linea[n+4] == 'i' && linea[n+5] == 'n' && linea[n+6] == 'e' && linea[n+7] == ' '){
			defineValidation(linea);}
		if(linea[n+1] == 'i' && linea[n+2] == 'n' && linea[n+3] == 'c' && linea[n+4] == 'l' && linea[n+5] == 'u' && linea[n+6] == 'd' && linea[n+7] == 'e' && linea[n+8] == ' '){
			includeValidation(linea);}
	}else{
			char *nuevo_valor;
			nuevo_valor = findValue(linea);
			//no encontro valor
			if(strcmp(nuevo_valor,"") == 0){
					fprintf(outputFile,"%s\n",linea);
			}	
			else{ //si encontro valor 
				  char *nombre = findName(linea);
				  while(strcmp(nombre,"")){				 
					  linea = replaceWord(linea,nombre,nuevo_valor);
					  nuevo_valor = findValue(linea);
					  if(strcmp(nuevo_valor,"")){// encontro otro valor	
						nombre = findName(linea);	
					  }else{nombre = "";}	//ya no encontro valor, salir del while	  
					  
					}
				   fprintf(outputFile,"%s \n",linea);
				  
				}
			}
	}

	

char *findDefine(char *linea){
	nodo* actual = primero;
	char *result = linea;
	while(actual != NULL && strcmp(actual->tipo,"define") == 0){
		result = replaceWord(result,actual->nombre,actual->valor);
		actual = actual->siguiente;		
		}

	return 	result;
	}
	

char *findValue(char *linea){
	nodo* actual = primero;
	char* temporal = NULL;
	while(actual != NULL && strcmp(actual->tipo,"define") == 0){
		//+
		asprintf(&temporal, "%s%s", actual->nombre,"+");
		if (strstr(linea, temporal) != NULL) {
			return actual->valor;
		}
		//-
		asprintf(&temporal, "%s%s", actual->nombre,"-");
		if (strstr(linea, temporal) != NULL) {
			return actual->valor;
		}
		//*
		asprintf(&temporal, "%s%s", actual->nombre,"*");
		if (strstr(linea, temporal) != NULL) {
			return actual->valor;
		}
		//  /
		asprintf(&temporal, "%s%s", actual->nombre,"/");
		if (strstr(linea, temporal) != NULL) {
			return actual->valor;
		}
		// =
		asprintf(&temporal, "%s%s", actual->nombre,"=");
		if (strstr(linea, temporal) != NULL) { 
			printf("Error , no se puede redefinir el define %s \n",actual->nombre);
		}
		// espacio
		asprintf(&temporal, "%s%c", actual->nombre,' ');		
		if (strstr(linea, temporal) != NULL) { 
			return actual->valor;
		}
		// espacio 2
		asprintf(&temporal, "%c%s",' ', actual->nombre);		
		if (strstr(linea, temporal) != NULL) { 
			return actual->valor;
		}
		// espacio 3
		asprintf(&temporal, "%c%s%c",' ', actual->nombre,' ');		
		if (strstr(linea, temporal) != NULL) { 
			return actual->valor;
		}
		// ;
		asprintf(&temporal, "%s%s", actual->nombre,";");
		if (strstr(linea, temporal) != NULL) { 
			return actual->valor;
		}
		//si es exactamente igual
		//printf("linea:%s? nombreactual:%s? soniguales: %d \n",linea,actual->nombre,strcmp(linea, actual->nombre));
		if (strcmp(linea, actual->nombre) == 0) { 
			return actual->valor;
		}
		//Salto de linea antes
		asprintf(&temporal, "%c%s",'\n', actual->nombre);
		if(strcmp(linea, temporal) == 0){
			return actual->valor;
		}
		actual = actual->siguiente;		
		}
	return 	"";
	}
	
char *findName(char *linea){
	nodo* actual = primero;
	char* temporal = NULL;
	while(actual != NULL && strcmp(actual->tipo,"define") == 0){
		//+
		asprintf(&temporal, "%s%s", actual->nombre,"+");
		if (strstr(linea, temporal) != NULL) {
			return actual->nombre;
		}
		//-
		asprintf(&temporal, "%s%s", actual->nombre,"-");
		if (strstr(linea, temporal) != NULL) {
			return actual->nombre;
		}
		//*
		asprintf(&temporal, "%s%s", actual->nombre,"*");
		if (strstr(linea, temporal) != NULL) {
			return actual->nombre;
		}
		//  /
		asprintf(&temporal, "%s%s", actual->nombre,"/");
		if (strstr(linea, temporal) != NULL) {
			return actual->nombre;
		}
		// =
		asprintf(&temporal, "%s%s", actual->nombre,"=");
		if (strstr(linea, temporal) != NULL) { 
			printf("Error , no se puede redefinir el define %s \n",actual->nombre);
		}
		// espacio
		asprintf(&temporal, "%s%c", actual->nombre,' ');		
		if (strstr(linea, temporal) != NULL) { 
			return actual->nombre;
		}
		// espacio 2
		asprintf(&temporal, "%c%s",' ', actual->nombre);		
		if (strstr(linea, temporal) != NULL) { 
			return actual->nombre;
		}
		// espacio 3
		asprintf(&temporal, "%c%s%c",' ', actual->nombre,' ');		
		if (strstr(linea, temporal) != NULL) { 
			return actual->nombre;
		}
		// ;
		asprintf(&temporal, "%s%s", actual->nombre,";");
		if (strstr(linea, temporal) != NULL) { 
			return actual->nombre;
		}
		// si es exactamente igual
		if (strcmp(linea, actual->nombre) == 0) { 
			return actual->nombre;
		}
		//Salto de linea antes
		asprintf(&temporal, "%c%s",'\n', actual->nombre);
		if(strcmp(linea, temporal) == 0){
			return actual->nombre;
		}
		actual = actual->siguiente;		
	
	}
		

	return 	"";
	}



char *replaceWord(char *fila, char *encontrar, char *reemplazo)
{
    char *cambio = malloc (strlen(fila)-strlen(encontrar)+strlen(reemplazo)+1);
    char *ptr;

    strcpy (cambio, fila);

    ptr = strstr (cambio, encontrar);
    if (ptr)
    {
        memmove (ptr+strlen(reemplazo), ptr+strlen(encontrar), strlen(ptr+strlen(encontrar))+1);
        strncpy (ptr, reemplazo, strlen(reemplazo));
    }
    return cambio;
}
	
	
	


int main(int argc, char** argv){
	createOutput();

	inputValidation(argc, argv);

	// char ch;
	// do 
    // {
    //     /* Read single character from file */
    //     ch = fgetc(archivo_salida);

    //     /* Print character read on console */
    //     putchar(archivo_salida);

    // } while(ch != EOF); /* Repeat this if last read character is not EOF */
	//free(archivos);
	
	
	printInfo();
	
	closeOutput();
	printf("Ejecutando Scanner \n");
	system(compilar);
	
	system(ejecutar);

	return 0;
}