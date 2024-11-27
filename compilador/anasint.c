#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.h"
#include "anasint.h"

#define TAM_TIPOS 4
#define pass (void)0
/* ANALISADOR SINTÁTICO */

// |=======| TABELA DE TIPOS |=======|
const char *tipos[TAM_TIPOS] = {
    "int",
    "real",
    "char",
    "bool"
};

int verificarTipo(char lexema[]){
    int i;
    for(i = 0 ; i < TAM_TIPOS ; i++){
        if((strcmp(lexema, tipos[i]) == 0)){
            return 0; // FOI UM TIPO
        }
    }
    return -1; // NÃO FOI UM TIPO
}

// |=======| FUNÇÕES |=======|

// ======= ERRO =======
void error(char msg[]) {
    printf("\n%s na Linha: %d\n", msg, contLinhas);
    exit(1);
}

// ======= PROG =======
void prog(){

    if((strcmp(tk.lexema, "proc") == 0) || (strcmp(tk.lexema, "def") == 0)){
        decl_def_proc();
    } else if((strcmp(tk.lexema, "const") == 0) || (verificarTipo(tk.lexema) == 0)){
        decl_list_var();
    }else{
        error("Nao iniciou o procedimento!");
    }
}

// ======= DECL_DEF_PROC =======
void decl_def_proc(){
    printf("\nproc ou def\n");
}

// ======= DECL_LIST_VAR =======
void decl_list_var(){

    if(strcmp(tk.lexema, "const") == 0){
        tk = analise_lexica(fd);
        if((verificarTipo(tk.lexema)) == 0){
            pass;
        }else{
            error("Tipo esperado! ");
        }
    }
    decl_var();

    tk = analise_lexica(fd);
    while(tk.cat == SN && tk.codigo == VIRGULA){
        decl_var();
    }
}

// ======= DECL_VAR =======
void decl_var(){
    printf("\n declaracao de variavel");
}
