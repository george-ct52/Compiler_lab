%{
#include <stdio.h>
%}

%%

"int"           { printf("Keyword: %s\n", yytext); }
"float"         { printf("Keyword: %s\n", yytext); }
"char"          { printf("Keyword: %s\n", yytext); }
"if"            { printf("Keyword: %s\n", yytext); }
"else"          { printf("Keyword: %s\n", yytext); }
"for"           { printf("Keyword: %s\n", yytext); }
"while"         { printf("Keyword: %s\n", yytext); }
"return"        { printf("Keyword: %s\n", yytext); }
"do"            { printf("Keyword: %s\n", yytext); }
"break"         { printf("Keyword: %s\n", yytext); }
"continue"      { printf("Keyword: %s\n", yytext); }
"double"        { printf("Keyword: %s\n", yytext); }
"long"          { printf("Keyword: %s\n", yytext); }
"short"         { printf("Keyword: %s\n", yytext); }
"const"         { printf("Keyword: %s\n", yytext); }
"auto"          { printf("Keyword: %s\n", yytext); }

[0-9]+          { printf("Number: %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]*  { printf("Identifier: %s\n", yytext); }
[+\-*/%=<>]     { printf("Operator: %s\n", yytext); }
[{}()\[\];,]    { printf("Delimiter: %s\n", yytext); }
[ \t\n]+        ;   /* ignore whitespace */
.               { printf("Unidentified: %s\n", yytext); }

%%
int yywrap() 
{ return 1; }

int main() {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        printf("Cannot open input.txt\n");
        return 1;
    }

    yyin = fp;
    yylex();
    fclose(fp);
    return 0;
}
