#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.h"
#include "anasint.h"

#define TAM_TIPOS 4
#define pass (void)0
/* ANALISADOR SINTÁTICO */

// |=======| TABELA DE SÍMBOLOS |=======|
TABELA_SIMBOLO ts;

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

    ts.Linhas[ts.topo].escopo = LOCAL;

    printf("\nproc ou def\n");
}

// ======= DECL_LIST_VAR =======
void decl_list_var(){

    ts.Linhas[ts.topo].escopo    = GLOBAL;
    ts.Linhas[ts.topo].categoria = VAR_GLOBAL;
    ts.Linhas[ts.topo].passagem  = NA_PASSAGEM;
    ts.Linhas[ts.topo].zumbi     = NA_ZUMBI;

    if(strcmp(tk.lexema, "const") == 0){
        ts.Linhas[ts.topo].eh_const = SIM;
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

    // DECLARAÇÃO DE VARIÁVEIS:


    if(strcmp(tk.lexema, "int") == 0){
        ts.Linhas[ts.topo].tipo = INT_TIPO;

    }else if(strcmp(tk.lexema, "real") == 0){
        ts.Linhas[ts.topo].tipo = REAL_TIPO;

    }else if(strcmp(tk.lexema, "char") == 0){
        ts.Linhas[ts.topo].tipo = CHAR_TIPO;

    }else if(strcmp(tk.lexema, "bool") == 0){
        ts.Linhas[ts.topo].tipo = BOOL_TIPO;

    }else{
        error("Tipo esperado! ");
    }

    tk = analise_lexica(fd);

    if(tk.cat != ID){
        error("Tem que passar identificador após tipo!");
    }

    strcpy(ts.Linhas[ts.topo].lexema, tk.lexema); // COLOCAR IDENTIFICADOR NA TABELA DE SÍMBOLO

    tk = analise_lexica(fd);
    if(tk.cat == SN && tk.codigo == ATRIBUICAO){

        tk = analise_lexica(fd);
        if(ts.Linhas[ts.topo].tipo == INT_TIPO && tk.cat == CT_I){
            if(ts.Linhas[ts.topo].eh_const == SIM){
                ts.Linhas[ts.topo].constInt = tk.valor_i;
            }
        }else if(ts.Linhas[ts.topo].tipo == REAL_TIPO && tk.cat == CT_R){
            if(ts.Linhas[ts.topo].eh_const == SIM){
                ts.Linhas[ts.topo].constReal = tk.valor_r;
            }
        }else if(ts.Linhas[ts.topo].tipo == CHAR_TIPO && tk.cat == CT_C){
            if(ts.Linhas[ts.topo].eh_const == SIM){
                ts.Linhas[ts.topo].constChar = tk.c;
            }
        }else if(ts.Linhas[ts.topo].tipo == CHAR_TIPO && tk.cat == CT_S){
            if(ts.Linhas[ts.topo].eh_const == SIM){
                strcpy(ts.Linhas[ts.topo].constString, tk.lexema);
            }
        }else{
            error("Tipo incompativel com atribuicao");
        }
    }else if(tk.cat == SN && tk.codigo == ABRE_COLCHETES){
        tk = analise_lexica(fd);

        if(tk.cat == ID){
            while(ts.topo != 0){

            }
        }

        if(tk.cat == CT_I || (tk.cat == ID && ts.Linhas[ts.topo].eh_const == SIM)){ // PRECISA USAR TABELA DE SIMBOLO - SABER SE O ID É UMA CONSTANTE
            tk = analise_lexica(fd);
            if(tk.cat == SN && tk.codigo == FECHA_COLCHETES){
                tk = analise_lexica(fd);

                if(tk.cat == SN && tk.codigo == ABRE_COLCHETES){
                    tk = analise_lexica(fd);
                    printf("\n %s", tk.lexema);
                    if(tk.cat == CT_I || (tk.cat == ID && ts.Linhas[ts.topo].eh_const == SIM)){ // PRECISA USAR TABELA DE SIMBOLO - SABER SE O ID É UMA CONSTANTE

                        if(ts.Linhas[ts.topo].eh_const == SIM){
                            /*while(ts.topo != 0){

                                if()

                                ts.topo--;
                            }*/
                        }else{
                            ts.Linhas[ts.topo].dim01 = tk.valor_i; // PASSAR CERTO
                        }


                        tk = analise_lexica(fd);

                        if(tk.cat == SN && tk.codigo == FECHA_COLCHETES){

                            ts.Linhas[ts.topo].isArray = MATRIZ;
                            tk = analise_lexica(fd);

                        }else{
                            error("Tem que fechar o colchetes apos abri-lo");
                        }
                    }else{
                        error("Tem que ser inteiro ou constante apos '['");
                    }
                }else{
                    ts.Linhas[ts.topo].isArray = VETOR;
                }


                if(tk.cat == SN && tk.codigo == ATRIBUICAO){
                    tk = analise_lexica(fd);

                    if(tk.cat == SN && tk.codigo == ABRE_CHAVES){
                        tk = analise_lexica(fd);

                        if(!((tk.cat == CT_I ) || (tk.cat == CT_C ) || (tk.cat == CT_R ))){
                            error("Valor esperado! ");
                        }


                        while(1){
                            tk = analise_lexica(fd);

                            if((tk.cat == SN) && (tk.codigo == VIRGULA)){
                                tk = analise_lexica(fd);
                                if(!((tk.cat == CT_I ) || (tk.cat == CT_C ) || (tk.cat == CT_R ))){
                                    error("Valor esperado! ");
                                }
                            }else if((tk.cat == SN) && (tk.codigo == FECHA_CHAVES)){
                                break;
                            }else{
                                error("Fecha chaves ou virgula esperadao");
                            }


                        }



                    }else{
                        error("Tem que vir '{' apos atribuicao");
                    }
                }

            }else{
                error("Tem que fechar o colchetes apos abri-lo");
            }
        }else{
            error("Tem que ser inteiro ou constante apos '['");
        }

    }
    if((ts.Linhas[ts.topo].isArray != VETOR) && (ts.Linhas[ts.topo].isArray != MATRIZ)){
        ts.Linhas[ts.topo].isArray = ESCALAR;
    }


}
