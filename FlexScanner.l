%{
#include "Proy1.h"
%}
 /* Asks flex to track line numbers */
%option yylineno

%%

"/*"((("*"[^/])?)|[^*])*"*/" return COMMENT; // Multi-line comment
"//".*									return COMMENT; // Single-line comment

 /* Pre-processor line */
^"#".*									return PREPROCESSOR;

 /* Reserved keywords */
auto										return KEYWORD;
register								return KEYWORD;
typedef									return KEYWORD;
extern									return KEYWORD;
return									return KEYWORD;
default									return KEYWORD;
goto										return KEYWORD;
sizeof									return KEYWORD;
volatile								return KEYWORD;

static									return KEYWORD;
unsigned								return KEYWORD;
signed									return KEYWORD;
void										return KEYWORD;
int											return KEYWORD;
double									return KEYWORD;
long										return KEYWORD;
char										return KEYWORD;
const										return KEYWORD;
float										return KEYWORD;
short										return KEYWORD;
union										return KEYWORD;
enum										return KEYWORD;
struct									return KEYWORD;

if											return KEYWORD;
else										return KEYWORD;
for											return KEYWORD;
while										return KEYWORD;
do											return KEYWORD;
switch									return KEYWORD;
case										return KEYWORD;
break										return KEYWORD;
continue								return KEYWORD;

 /* Constants */
[-+]?[0-9]+							return CONSTANTLITERAL; // Decimal constant
O[0-9]+									return CONSTANTLITERAL; // Octal constant
OX[0-9abcdef]+					return CONSTANTLITERAL; // Hex constant
[-+]?[0-9]+(\.[0-9]+)?	return CONSTANTLITERAL; // Real constant

'([^\r^\n^']|\\0)'			return CONSTANTCHAR; // Single char constant

\"[^\"]*\" 							return CONSTANTSTRING; // String constant

 /* Variable names */
[a-zA-Z_][_a-zA-Z0-9]*	return IDENTIFIER;

 /* Operators */
\+											return OPERATOR;
- 											return OPERATOR;
\* 											return OPERATOR;
\/ 											return OPERATOR;
\%											return OPERATOR;
\+\+										return OPERATOR;
"--"										return OPERATOR;

==											return OPERATOR;
!=											return OPERATOR;
\> 											return OPERATOR;
\< 											return OPERATOR;
\>=											return OPERATOR;
\<=											return OPERATOR;

&&											return OPERATOR;
\|\|										return OPERATOR;
!												return OPERATOR;

& 											return OPERATOR;
\| 											return OPERATOR;
\^ 											return OPERATOR;
~ 											return OPERATOR;
\<\<										return OPERATOR;
\>\>										return OPERATOR;

"?"											return OPERATOR;

 /* Punctuators */
\(											return PUNCTUATOR;
\)											return PUNCTUATOR;
\{											return PUNCTUATOR;
\}											return PUNCTUATOR;
\[											return PUNCTUATOR;
\]											return PUNCTUATOR;
,												return PUNCTUATOR;
:												return PUNCTUATOR;
;												return PUNCTUATOR;
=												return PUNCTUATOR;




[ \t\n]									return BLANK; // New lines and black spaces
.												return ERROR; // If not a rule --> ERROR
%%

int yywrap(void)
{
	return 1;
}
