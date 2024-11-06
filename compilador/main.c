#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.h"

#define TAM_LEXEMA 50
#define TAM_NUM 20

// |=======|  DECLARAÇÃO DAS FUNÇÕES |=======|
TOKEN analise_lexica(FILE*);

// |=======| MAIN |=======|
int main()
{
    FILE *fd;
    TOKEN tk;

    if((fd = fopen("codigo.txt", "r")) == NULL){
        printf("\n[ERRO] - ERRO ao abrir o arquivo\n");
    }

    printf("\nLinha: %d", contLinhas);

    /*
    char c = fgetc(fd);
    tk.cat = ID;

    printf("\nc = %c ; tk.cat = %d ", c, tk.cat);
    */
    while(1){
        printf("\nEntrou no while do main");
        tk = analise_lexica(fd);
        printf("\n-> %d ", tk.cat);
        switch(tk.cat){
            case ID:
                printf("\n <ID, %s >", tk.lexema);
                break;
            case CT_I:
                printf("\n <CT_I, %d >", tk.valor_i);
                break;
            case CT_R:
                printf("\n <CT_R, %f >", tk.valor_r);
                break;
            case CT_C:
                printf("\n <CT_C, >"); // TERMINAR
                break;
            case LT:
                printf("\n <LT, >"); // TERMINAR
                break;
            case SN:
                break;
            case FIM_ARQ:
                printf("\nArquivo lido com sucesso!\n");
                break;

        }
        if(tk.cat == FIM_ARQ){
            break;
        }
    }

    return 0;
}

// |=======| FUNÇÕES |=======|
// =======| ÁNALISE LÉXICA |=======
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
       // printf("\nentrou no while do analisador lexico");
        char c = fgetc(fd);
        printf("\n -> %c \n", c);
        system("pause");
        switch(estado){
            case 0:
                printf("\n0");
                if(c == ' '){
                    estado = 0;
                }else if(c == '/t'){
                    estado = 0;
                }else if(c == '/n'){
                    estado = 0;
                    contLinhas++;
                }else if(c == '/0'){
                    estado = 0;
                }else if(c >= 'a' && c <= 'z'){
                    //printf("\nentrou no 1");
                    estado = 1;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }else if(c == '_'){
                    estado = 2;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }else if(c >= '0' && c <= '9'){
                    estado = 4;
                    digitos[tamD] = c;
                    digitos[++tamD] = '\0';
                }else if(c == '"'){
                    estado = 9;
                }else if(c == '!'){
                    estado = 13;
                }else if(c == '&'){
                    estado = 16;
                }else if(c == '|'){
                    estado = 19;
                }else if(c == '('){
                    estado = 21;
                    t.cat = SN;
                    t.codigo = ABRE_PAR;
                    return t;
                }else if(c == ')'){
                    estado = 22;
                    t.cat = SN;
                    t.codigo = FECHA_PAR;
                    return t;
                }else if(c == '['){
                    estado = 23;
                    t.cat = SN;
                    t.codigo = ABRE_COLCHETES;
                    return t;
                }else if(c == ']'){
                    estado = 24;
                    t.cat = SN;
                    t.codigo = FECHA_COLCHETES;
                    return t;
                }else if(c == '{'){
                    estado = 25;
                    t.cat = SN;
                    t.codigo = ABRE_CHAVES;
                    return t;
                }else if(c == '}'){
                    estado = 26;
                    t.cat = SN;
                    t.codigo = FECHA_CHAVES;
                    return t;
                }else if(c == ','){
                    estado = 27;
                    t.cat = SN;
                    t.codigo = VIRGULA;
                    return t;
                }else if(c == '>'){
                    estado = 28;
                }else if(c == '<'){
                    estado = 31;
                }else if(c == '='){
                    estado = 34;
                }else if(c == '+'){
                    estado = 37;
                    t.cat = SN;
                    t.codigo = ADICAO;
                }else if(c == '-'){
                    estado = 38;
                    t.cat = SN;
                    t.codigo = SUBTRACAO;
                }else if(c == '*'){
                    estado = 39;
                    t.cat = SN;
                    t.codigo = MULTIPLICACAO;
                }else if(c == '/'){
                    estado = 40;
                }else if(c == EOF){
                    printf("\n Final do arquivo");
                    t.cat = FIM_ARQ;
                    return t;
                }else{
                    printf("\n[ERRO] - Estado no %d \n", estado);
                }
                break;
            case 1:
                printf("\n1");
                if((c == '_') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')){
                    estado = 1;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }else{ // OUTRO*
                    printf("\n3");
                    estado = 3;
                    ungetc(c, fd);
                    t.cat = ID;
                    strcpy(t.lexema, lexema);
                    return t;
                }
                break;
            case 2:
                printf("\n2");
                if(c == '_'){
                    estado = 2;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }else if(c >= 'a' && c <= 'z'){
                    estado = 1;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }else{
                    printf("\n[ERRO] - Estado no %d \n", estado);
                }
                break;
            case 3: // *ESTADO DE ACEITAÇÃO* - ID
                break;
            case 4:
                printf("\n4");
                if(c >= '0' && c <= '9'){
                    estado = 4;
                    digitos[tamD] = c;
                    digitos[++tamD] = '\0';
                }else if(c == '.'){
                    estado = 6;
                    digitos[tamD] = c;
                    digitos[++tamD] = '\0';
                }else{
                    estado = 5;
                    ungetc(c, fd);
                    t.cat = CT_I;
                    t.valor_i = atoi(digitos);
                    return t;
                }
                break;
            case 5: // *ESTADO DE ACEITAÇÃO* - CT_I
                break;
            case 6:
                printf("\n6");
                if(c >= '0' && c <= '9'){
                    estado = 7;
                    digitos[tamD] = c;
                    digitos[++tamD] = '/0';
                }else{
                    printf("\n[ERRO] - Estado no %d \n", estado);
                }
                break;
            case 7:
                if(c >= '0' && c <= '9'){
                    estado = 7;
                    digitos[tamD] = c;
                    digitos[++tamD] = '/0';
                }else{
                    estado = 8;
                    ungetc(c, fd);
                    t.cat = CT_R;
                    t.valor_r = atof(digitos);
                    return t;
                }
                break;
            case 8: // *ESTADO DE ACEITAÇÃO* - CT_F
                break;
            case 9:
                if(c != '"'){
                    estado = 11;
                }
                // SE VIER '\' IR PARA ESTADO 10
                break;
            case 10:
                // SE VIER 'n' IR PARA ESTADO 11
                break;
            case 11:
                if(c != '"'){
                    estado = 11;
                }else{
                    estado = 12;
                    ungetc(c, fd);
                    t.cat = LT;
                    return t;
                }
                // SE VIER CARACTER CONTINUAR NO ESTADO 11
                // SE VIER '"' IR PARA ESTADO 12
                break;
            case 12: // *ESTADO DE ACEITAÇÃO* - STRINGCON
                break;
            case 13:
                if(c == '='){ // DIFERENTE
                    estado = 15;
                    t.cat = SN;
                    t.codigo = DIFERENTE;
                    return t;
                }else{
                    estado = 14; // NEGAÇÃO
                    t.cat = SN;
                    t.codigo = NEGACAO;
                }
                break;
            case 14: // *ESTADO DE ACEITAÇÃO* - NEGAÇÃO
                break;
            case 15: // *ESTADO DE ACEITAÇÃO* - DIFERENÇA
                break;
            case 16:
                if(c == '&'){
                    estado = 18;
                    t.cat = SN;
                    t.codigo = AND;
                    return t;
                }else{
                    estado = 17;
                    t.cat = SN;
                    t.codigo = E_COMERCIAL;
                    return t;
                }
                break;
            case 17: // *ESTADO DE ACEITAÇÃO* - E_COMERCIAL (&)
                break;
            case 18: // *ESTADO DE ACEITAÇÃO* - AND
                break;
            case 19:
                if(c == '|'){
                    estado = 20;
                    t.cat = SN;
                    t.codigo = OR;
                    return t;
                }else{
                    printf("\n[ERRO] - Estado no %d \n", estado);
                }
                break;
            case 20: // *ESTADO DE ACEITAÇÃO* - OR
                break;
            case 21: // *ESTADO DE ACEITAÇÃO* - ABRE PARÊNTESE
                break;
            case 22: // *ESTADO DE ACEITAÇÃO* - FECHA PARÊNTESE
                break;
            case 23: // *ESTADO DE ACEITAÇÃO* - ABRE COLCHETE
                break;
            case 24: // *ESTADO DE ACEITAÇÃO* - FECHA COLCHETE
                break;
            case 25: // *ESTADO DE ACEITAÇÃO* - ABRE CHAVES
                break;
            case 26: // *ESTADO DE ACEITAÇÃO* - FECHA CHAVES
                break;
            case 27: // *ESTADO DE ACEITAÇÃO* - VÍRGULA
                break;
            case 28:
                if(c == '='){
                    estado = 29;
                    t.cat = SN;
                    t.codigo = MAIOR_OU_IGUAL;
                    return t;
                }else{
                    estado = 30;
                    t.cat = SN;
                    t.codigo = MAIOR_QUE;
                }
                break;
            case 29: // *ESTADO DE ACEITAÇÃO* - MAIOR OU IGUAL
                break;
            case 30: // *ESTADO DE ACEITAÇÃO* - MAIOR QUE
                break;
            case 31:
                if(c == '='){
                    estado = 33;
                    t.cat = SN;
                    t.codigo = MENOR_OU_IGUAL;
                    return t;
                }else{
                    estado = 32;
                    t.cat = SN;
                    t.codigo = MENOR_QUE;
                    return t;
                }
                break;
            case 32: // *ESTADO DE ACEITAÇÃO* - MENOR QUE
                break;
            case 33: // *ESTADO DE ACEITAÇÃO* - MENOR OU IGUAL
                break;
            case 34:
                if(c == '='){
                    estado = 36;
                    t.cat = SN;
                    t.codigo = COMPARACAO;
                    return t;
                }else{
                    estado = 35;
                    t.cat = SN;
                    t.codigo = ATRIBUICAO;
                    return t;
                }
                break;
            case 35: // *ESTADO DE ACEITAÇÃO* - ATRIBUIÇÃO
                break;
            case 36: // *ESTADO DE ACEITAÇÃO* - COMPARAÇÃO
                break;
            case 37: // *ESTADO DE ACEITAÇÃO* - ADIÇÃO
                break;
            case 38: // *ESTADO DE ACEITAÇÃO* - SUBTRAÇÃO
                break;
            case 39: // *ESTADO DE ACEITAÇÃO* - MULTIPLICAÇÃO
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
