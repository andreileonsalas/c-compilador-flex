#define PREPROCESSOR 1
#define COMMENT 2
#define KEYWORD 3
#define IDENTIFIER 4
#define CONSTANTLITERAL 5
#define CONSTANTCHAR 6
#define CONSTANTSTRING 6
#define OPERATOR 8
#define PUNCTUATOR 9
#define BLANK 10
#define ERROR 11

#define TOTALTOKENS 11

typedef struct table_row
{
	int token;
	char* lexeme;
	struct table_row * next;
}
Row;

Row* symbolTable[TOTALTOKENS];

typedef struct
{
	int token_count[TOTALTOKENS];
}
fragment_info;

fragment_info* histogram;
int current_Hist_i; // Histogram index
int current_Hist_s; // Histogram size

Row getToken(void);
void writeTokenToLatexFile(Row rowToken, FILE* file);
void writeLatexFileStart(FILE* file);
void writeLatexFileEnd(FILE* file);
void writeTokensToDatafile(void);
void addHistogramExtension(void);
