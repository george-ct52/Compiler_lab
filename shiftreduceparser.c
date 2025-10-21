#include <stdio.h>
#include <string.h>

int z = 0, i = 0, j = 0, c = 0;
char input[50], ac[50], stk[50];
void check();
int main(){
    puts("The Grammar is:");
    puts("E -> E+E");
    puts("E -> E*E");
    puts("E -> (E)");
    puts("E -> id");

    printf("\nEnter Your Expression: ");
    scanf("%s", input);
    c=strlen(input);
    puts("\nStack\t\tInput\t\tAction");
    puts("---------------------------------------------");  
    for (i=0,j=0;j<c;i++,j++)
    {
        if(input[j] == 'i' && input[j+1] == 'd')
        {
            stk[z++] = input[j];
            stk[z++] = input[j+1];
            stk[z] = '\0';
            input[j]=input[j+1]=' ';
            printf("$%-10s%-10sSHIFT -> id\n", stk, input);
            check();
        }
        else if(input[j] != ' ')
        {
             stk[z++] = input[j];
             stk[z] = '\0';
             input[j] = ' ';
             printf("$%-10s%-10sSHIFT -> %c\n", stk, input,stk[z-1]);
             check();
        }
    }
    
    printf("\nParsing Completed.\n");
    return 0;

}
void check()
{   strcpy(ac, "REDUCE TO E");

   for (int p = 0; p < z; p++) {
    if(stk[p] == 'i' && stk[p+1] == 'd')
    {
        stk[p]='E';
        for(int q=p+1; q<z; q++)
            stk[q]=stk[q+2];
        z=z-1;
        
        printf("$%-10s%-10s%s\n", stk, input, ac);
        
    }
     }
     
     for (int p = 0; p < z; p++) {
    if(stk[p] == 'E' && stk[p+1] == '+'&& stk[p+2] == 'E')
    {
        stk[p]='E';
        for(int q=p+1; q < z ; q++)
            stk[q]=stk[q+3];
        z=z-2;
        
        printf("$%-10s%-10s%s\n", stk, input, ac);
        
    }
     }
      for (int p = 0; p < z; p++) {
    if(stk[p] == 'E' && stk[p+1] == '*'&& stk[p+2] == 'E')
    {
        stk[p]='E';
        for(int q=p+1;q<z;q++)
            stk[q]=stk[q+3];
        z=z-2;
        
        printf("$%-10s%-10s%s\n", stk, input, ac);
        
    }
     }
     
     for (int p = 0; p < z; p++) {
    if(stk[p] == '(' && stk[p+1] == 'E'&& stk[p+2] == ')')
    {
        stk[p]='E';
        for(int q=p+1;q<z;q++)
            stk[q]=stk[q+3];
        z=z-2;
        
        printf("$%-10s%-10s%s\n", stk, input, ac);
        
    }
     }
     
}
