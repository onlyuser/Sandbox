/* Permission to reproduce portions of this document is given provided the web
 * site listed below is referenced. No additional restrictions apply. Source
 * code, when part of a software project, may be used freely without reference
 * to the author.
 *
 * Tom Niemann
 * Portland, Oregon
 * epaperpress.com
 */

%option outfile="lex.calc2.c"

    /* calculator #1 */
%{
    #include "calc2.tab.h"
    #include <stdlib.h>
    void yyerror(char *);
%}

%%

[a-z]       {
                yylval = *yytext - 'a';
                return VARIABLE;
                }

[0-9]+      {
                yylval = atoi(yytext);
                return INTEGER;
            }

[-+()=/*\n]     { return *yytext; }

[ \t]   ;       /* skip whitespace */

.               yyerror("Unknown character");

%%

int yywrap(void) {
    return 1;
}
