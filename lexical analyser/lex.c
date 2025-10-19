#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main() {
    FILE *input, *output;
    char ch, word[20];
    int line = 1, token = 1, i, isKeyword;

    // List of simple C keywords
    char keywords[10][10] = {"int", "main", "if", "else", "do", "while"};

    // Open input and output files
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");

    // Header for output table
    fprintf(output, "Line No.\tToken No.\tToken Type\tLexeme\n\n");

    // Read each character from input file
    while ((ch = fgetc(input)) != EOF) {

        // Skip spaces and tabs
        if (ch == ' ' || ch == '\t')
            continue;

        // Move to next line if newline character
        if (ch == '\n') {
            line++;
            continue;
        }

        // ✅ Operators check using strchr()
        if (strchr("+-*/", ch)) {
            fprintf(output, "%d\t\t%d\t\tOperator\t%c\n", line, token++, ch);
            continue;
        }

        // ✅ Special symbols check using strchr()
        if (strchr(";{}()%!", ch)) {
            fprintf(output, "%d\t\t%d\t\tSpecial\t\t%c\n", line, token++, ch);
            continue;
        }

        // ✅ Digits
        if (isdigit(ch)) {
            fprintf(output, "%d\t\t%d\t\tDigit\t\t%c\n", line, token++, ch);
            continue;
        }

        // ✅ Identifiers or Keywords
        if (isalpha(ch)) {
            i = 0;
            word[i++] = ch;

            // Continue reading till alphanumeric or underscore
            while ((ch = fgetc(input)) != EOF && (isalnum(ch) || ch == '_'))
                word[i++] = ch;

            word[i] = '\0'; // end the string
            isKeyword = 0;

            // Check if the word is a keyword
            for (int k = 0; k < 6; k++) {
                if (strcmp(word, keywords[k]) == 0) {
                    isKeyword = 1;
                    break;
                }
            }

            // Print type of token
            if (isKeyword)
                fprintf(output, "%d\t\t%d\t\tKeyword\t\t%s\n", line, token++, word);
            else
                fprintf(output, "%d\t\t%d\t\tIdentifier\t%s\n", line, token++, word);

            // Go back one character if extra read
            if (ch != EOF)
                fseek(input, -1, SEEK_CUR);
        }
    }

    fclose(input);
    fclose(output);

    printf("✅ Lexical analysis completed. Check output.txt for results.\n");
    return 0;
}
