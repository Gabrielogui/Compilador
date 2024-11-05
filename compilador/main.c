#include <stdio.h>
#include <stdlib.h>
#include "analex.h"

#define TAM_LEXEMA 50
#define TAM_NUM 20

// |=======|  DECLARAÇÃO DAS FUNÇÕES |=======|
TOKEN analise_lexica(FILE*);

// |=======| MAIN |=======|
int main()
{
    printf("oi");
    return 0;
}

// |=======| FUNÇÕES |=======|
TOKEN analise_lexica(FILE *fd){
    // VARIÁVEIS DAS FUNÇÕES:
    int estado = 0;
    char lexema[TAM_LEXEMA] = "";
    int tamL = 0;
    char digitos[TAM_NUM] = "";
    int tamD = 0;

    //TOKEN:
    TOKEN t;

    // WHILE TRUE:
    while(1){
        char c = getc(fd);
        switch(estado){
            case 0:
                if(c == ' '){
                    estado = 0;
                }else if(c == '/t'){
                    estado = 0;
                }else if(c == '/n'){
                    estado = 0;
                }else if(c >= 'a' && c <= 'Z'){
                    estado = 1;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }else if(c == '_'){
                    estado = 31;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }else if(c >= '0' && c <= '9'){
                    estado = 2;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }
                break;
            case 1:
                if((c == '_') || (c >= 'a' && c <= 'Z') || (c >= '0' && c <= '9')){
                    estado = 1;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }else{ // OUTRO*
                    estado = 48;
                }
                break;
            case 2:
                if(c >= '1' && c <= '9'){
                    estado = 2;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }else if(c == '.'){
                    estado = 3;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }else{
                    estado = 47;
                    ungetc(c, fd);
                }
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            case 10:
                break;
            case 11:
                break;
            case 12:
                break;
            case 13:
                break;
            case 14:
                break;
            case 15:
                break;
            case 16:
                break;
            case 17:
                break;
            case 18:
                break;
            case 19:
                break;
            case 20:
                break;
            case 21:
                break;
            case 22:
                break;
            case 23:
                break;
            case 24:
                break;
            case 25:
                break;
            case 26:
                break;
            case 27:
                break;
            case 28:
                break;
            case 29:
                break;
            case 30:
                break;
            case 31:
                if(c == '_'){
                    estado = 31;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }else if(c >= 'a' && c <= 'Z'){
                    estado = 1;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }
                break;
            case 32:
                break;
            case 33:
                break;
            case 34:
                break;
            case 35:
                break;
            case 36:
                break;
            case 37:
                break;
            case 38:
                break;
            case 39:
                break;
            case 40:
                break;
            case 41:
                break;
            case 42:
                break;
            case 43:
                break;
            case 44:
                break;
            case 45:
                break;
            case 46:
                break;
            case 47:
                break;
            case 48:
                break;
        }
    }
}
