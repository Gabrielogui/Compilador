#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "analex.h"

#define TAM_LEXEMA 50
#define TAM_NUM 20
#define TAM_STR 50

#define QTD_PV 31

/* ANALIZADOR LÉXICO */

// |=======| DECLARAÇÃO DO CONTADOR DE LINHA |=======|
contLinhas = 1;

// |=======| PALAVRAS RESERVADAS |=======|
const char *palavrasReservadas[QTD_PV] = {
    "const",
    "prot",
    "def",
    "init",
    "endp",
    "char",
    "int",
    "real",
    "bool",
    "do",
    "while",
    "endw",
    "var",
    "from",
    "dt",
    "to",
    "by",
    "endv",
    "if",
    "elif",
    "else",
    "endi",
    "getint",
    "getout",
    "getchar",
    "getreal",
    "getstr",
    "putint",
    "putchar",
    "putreal",
    "putstr"
};

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
        switch(estado){
            case 0:
                if(c == ' '){
                    estado = 0;
                }else if(c == '\t'){
                    estado = 0;
                }else if(c == '\n'){
                    estado = 0;
                    contLinhas++;
                }else if(c == '\0'){
                    estado = 0;
                }else if((c >= 'a' && c <= 'z') ||(c >= 'A' && c <= 'Z')){
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
                }else if(c == '"'){ // USAR O 'strig[]' PARA GUARDA-LA
                    estado = 11;
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
                    return t;
                }else if(c == '-'){
                    estado = 38;
                    t.cat = SN;
                    t.codigo = SUBTRACAO;
                    return t;
                }else if(c == '*'){
                    estado = 39;
                    t.cat = SN;
                    t.codigo = MULTIPLICACAO;
                    return t;
                }else if(c == '/'){
                    estado = 40;
                }else if(c == EOF){
                    //printf("\n Final do arquivo");
                    t.cat = FIM_ARQ;
                    return t;
                }else if(c == '\''){
                    estado = 44;
                }else{
                    printf("\n[ERRO] - Estado no %d \n", estado);
                }
                break;
            case 1:
                if((c == '_') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')){
                    estado = 1;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }else{ // OUTRO*
                    estado = 3;
                    ungetc(c, fd);
                    if(verificarPV(lexema) == 1){
                        t.cat = PVR;
                        strcpy(t.lexema, lexema);
                        return t;
                    }else{
                        t.cat = ID;
                        strcpy(t.lexema, lexema);
                        return t;
                    }
                }
                break;
            case 2:
                if(c == '_'){
                    estado = 2;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }else if((c >= 'a' && c <= 'z')||(c >= 'A' && c <= 'Z')){
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
               /* if((c != '"') && (c != '\n')){
                    estado = 11;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }*/
                break;
            case 10:
                break;
            case 11:
                if((c != '"')&&(c != '\n')){
                    estado = 11;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }else if(c == '"'){
                    estado = 12;
                    strcpy(t.lexema, lexema);
                    t.cat = CT_S;
                    return t;
                }else{
                    printf("[ERRO] - Estado no %d \n", estado);
                }
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
                    ungetc(c, fd);
                    t.cat = SN;
                    t.codigo = NEGACAO;
                    return t;
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
                    ungetc(c, fd);
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
                    ungetc(c, fd);
                    t.cat = SN;
                    t.codigo = MAIOR_QUE;
                    return t;
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
                    ungetc(c, fd);
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
                    ungetc(c, fd);
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
                if(c == '/'){
                    estado = 42;
                }else{
                    estado = 41;
                    ungetc(c, fd);
                    t.cat = SN;
                    t.codigo = DIVISAO;
                    return t;
                }
                break;
            case 41: // *ESTADO DE ACEITAÇÃO* - DIVISÃO
                break;
            case 42:
                if(c != '\n'){
                    estado = 42;
                }else{
                    estado = 0;
                    contLinhas++;
                }
                break;
            case 43:
                break;
            case 44:
                if(c == '\\'){
                    estado = 45;
                   // lexema[tamL] = c;
                   // lexema[++tamL] = '\0';
                }else if((isprint(c) != '\0') && (c != '\"') && (c != '\n')){
                    estado = 50;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }else{
                    printf("\n[ERRO] - Estado no %d \n", estado);
                }
                break;
            case 45:
                if(c == 'n'){
                    estado = 47;
                }else if(c == '0'){
                    estado = 46;
                }else{
                    printf("\n[ERRO] - Estado no %d \n", estado);
                }
                break;
            case 46:
                if(c == '\''){
                    estado = 48;
                    t.cat = CT_C;
                    t.c = '\0';
                    return t;
                }else{
                    printf("\n[ERRO] - Estado no %d \n", estado);
                }
                break;
            case 47:
                if(c == '\''){
                    estado = 49;
                    t.cat = CT_C;
                    t.c = '\n';
                    return t;
                }else{
                    printf("\n[ERRO] - Estado no %d \n", estado);
                }
                break;
            case 48: // *ESTADO DE ACEITAÇÃO* - CHAR_CON
                break;
            case 49: // *ESTADO DE ACEITAÇÃO* - CHAR_CON
                break;
            case 50:
                if(c == '\''){
                    estado = 51;
                    t.cat = CT_C;
                    t.c = lexema[0];
                    return t;
                }else{
                    printf("\n[ERRO] - Estado no %d \n", estado);
                }
                break;
            case 51: // *ESTADO DE ACEITAÇÃO* - CHAR_CON
                break;
            default:
                printf("\n[ERRO] - Erro ao ler o estado");
                break;
        }
    }
}

// =======| VERIFICAÇÃO DAS PALAVRAS RESERVADAS |=======
int verificarPV(char lexema[]){
    int i;
    for(i = 0 ; i < QTD_PV ; i++){
        if(strcmp(lexema, palavrasReservadas[i]) == 0){
            return 1;
        }
    }
    return -1; // RETORNA SE NÃO ACHAR A PALAVRA RESERVADA
}

