/* Permission to reproduce portions of this document is given provided the web
 * site listed below is referenced. No additional restrictions apply. Source
 * code, when part of a software project, may be used freely without reference
 * to the author.
 *
 * Tom Niemann
 * Portland, Oregon
 * epaperpress.com
 */

%option outfile="lex.calc3.c"

%{
#include <stdlib.h>
#include "calc3.h"
#include "calc3.tab.h"
void yyerror(char *);
%}

%%

[a-z]       { 
                yylval.sIndex = *yytext - 'a';
                return VARIABLE;
            }

0           {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

[1-9][0-9]* {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

[-()<>=+*/;{}.] {
                return *yytext;
             }

">="            return GE;
"<="            return LE;
"=="            return EQ;
"!="            return NE;
"while"         return WHILE;
"if"            return IF;
"else"          return ELSE;
"print"         return PRINT;

[ \t\n]+        ;       /* ignore whitespace */

.               yyerror("Unknown character");
%%
int yywrap(void) {
    return 1;
}
