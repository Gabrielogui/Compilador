#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.h"
#include "anasint.h"

#define TAM_TIPOS 4
#define pass (void)0
/* ANALISADOR SINT�TICO */

// |=======| TABELA DE S�MBOLOS |=======|
TABELA_SIMBOLO ts;

int var_virg_aux = 0;

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
    return -1; // N�O FOI UM TIPO
}

// |=======| FUN��ES |=======|

// ======= ERRO =======
void error(char msg[]) {
    printf("\n%s na Linha: %d\n", msg, contLinhas);
    exit(1);
}

// ======= PROG =======
void prog(){

    printf("\n%s", tk.lexema);

    if(tk.cat == FIM_ARQ){
        printf("\nArquivo encerrado\n");
    }
    else if((strcmp(tk.lexema, "proc") == 0) || (strcmp(tk.lexema, "def") == 0)){
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

    if(strcmp(tk.lexema, "prot") == 0){

    }else if(strcmp(tk.lexema, "def") == 0){

    }
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
    else{
        ts.Linhas[ts.topo].eh_const = NAO;
    }
    decl_var();

    //lido = 1;
    if (!(tk.cat == SN && tk.codigo == VIRGULA))
    {
        tk.processado = 1;
    }

    while(tk.cat == SN && tk.codigo == VIRGULA){
        tk = analise_lexica(fd);
        var_virg_aux = 1;
        decl_var();
    }
    if (!(tk.cat == SN && tk.codigo == VIRGULA))
    {
        tk.processado = 1;
    }
}

// ======= DECL_VAR =======
void decl_var(){
    // DECLARA��O DE VARI�VEIS:
    if(var_virg_aux == 1){
        int topoAux_virg = ts.topo - 1;
        ts.Linhas[ts.topo].tipo = ts.Linhas[var_virg_aux].tipo;
        var_virg_aux = 0;
    }else if(strcmp(tk.lexema, "int") == 0){
        ts.Linhas[ts.topo].tipo = INT_TIPO;
        tk = analise_lexica(fd);
    }else if(strcmp(tk.lexema, "real") == 0){
        ts.Linhas[ts.topo].tipo = REAL_TIPO;
        tk = analise_lexica(fd);
    }else if(strcmp(tk.lexema, "char") == 0){
        ts.Linhas[ts.topo].tipo = CHAR_TIPO;
        tk = analise_lexica(fd);
    }else if(strcmp(tk.lexema, "bool") == 0){
        ts.Linhas[ts.topo].tipo = BOOL_TIPO;
        tk = analise_lexica(fd);
    }else{
        error("Tipo esperado! ");
    }

    if(tk.cat != ID){
        error("Tem que passar identificador ap�s tipo!");
    }

    strcpy(ts.Linhas[ts.topo].lexema, tk.lexema); // COLOCAR IDENTIFICADOR NA TABELA DE S�MBOLO

    tk = analise_lexica(fd);
    if(tk.cat == SN && tk.codigo == ATRIBUICAO){
        tk = analise_lexica(fd);
        //printf("\nO char eh: %d",tk.cat);
        if(ts.Linhas[ts.topo].tipo == INT_TIPO && tk.cat == CT_I){
            if(ts.Linhas[ts.topo].eh_const == SIM){
                ts.Linhas[ts.topo].constInt = tk.valor_i;
            }
            tk = analise_lexica(fd);
        }else if(ts.Linhas[ts.topo].tipo == REAL_TIPO && tk.cat == CT_R){
            if(ts.Linhas[ts.topo].eh_const == SIM){
                ts.Linhas[ts.topo].constReal = tk.valor_r;
            }
            tk = analise_lexica(fd);
        }else if(ts.Linhas[ts.topo].tipo == CHAR_TIPO && tk.cat == CT_C){
            if(ts.Linhas[ts.topo].eh_const == SIM){
                ts.Linhas[ts.topo].constChar = tk.c;
            }
            tk = analise_lexica(fd);
        }else if(ts.Linhas[ts.topo].tipo == CHAR_TIPO && tk.cat == CT_S){
            if(ts.Linhas[ts.topo].eh_const == SIM){
                strcpy(ts.Linhas[ts.topo].constString, tk.lexema);
            }
            tk = analise_lexica(fd);
        }else{
            error("Tipo incompativel com atribuicao");
        }
    }else if(tk.cat == SN && tk.codigo == ABRE_COLCHETES){
        tk = analise_lexica(fd);

       /* if(tk.cat == ID){ // OLHAR ESSA BUSCA
            while(ts.topo != 0){

            }
        } */

        if(tk.cat == CT_I || (tk.cat == ID)){ // PRECISA USAR TABELA DE SIMBOLO - SABER SE O ID � UMA CONSTANTE

            if(tk.cat == ID){ // BUSCA DO DIM01 CASO SEJA CONSTANTE
                int topoAux1 = ts.topo;
                int strcmp_aux1 = 0;
                while(topoAux1 >= 0){
                    if(strcmp(ts.Linhas[topoAux1].lexema, tk.lexema) == 0){
                        strcmp_aux1 = 1;
                        if(ts.Linhas[topoAux1].eh_const == SIM){
                            if(ts.Linhas[topoAux1].tipo == INT_TIPO){
                                ts.Linhas[ts.topo].dim01 = ts.Linhas[topoAux1].constInt;
                                break;
                            }else{
                                error("A constante nao eh compativel");
                            }
                        }else{
                            error("A constante nao eh compativel");
                        }
                    }
                    topoAux1--;
                }
                if(topoAux1 < 0 && strcmp_aux1 == 0){
                    error("A constante nao eh compativel");
                }
            }else{
                ts.Linhas[ts.topo].dim01 = tk.valor_i;
            }


            tk = analise_lexica(fd);
            if(tk.cat == SN && tk.codigo == FECHA_COLCHETES){
                tk = analise_lexica(fd);

                if(tk.cat == SN && tk.codigo == ABRE_COLCHETES){
                    tk = analise_lexica(fd);
                    //printf("\n %s", tk.lexema);
                    if(tk.cat == CT_I || (tk.cat == ID)){ // PRECISA USAR TABELA DE SIMBOLO - SABER SE O ID � UMA CONSTANTE

                        if(tk.cat == ID){ // BUSCA DO DIM01 CASO SEJA CONSTANTE
                            int topoAux2 = ts.topo;
                            int strcmp_aux2 = 0;
                            while(topoAux2 >= 0){
                                if(strcmp(ts.Linhas[topoAux2].lexema, tk.lexema) == 0){
                                    strcmp_aux2 = 1;
                                    if(ts.Linhas[topoAux2].eh_const == SIM){
                                        if(ts.Linhas[topoAux2].tipo == INT_TIPO){
                                            ts.Linhas[ts.topo].dim02 = ts.Linhas[topoAux2].constInt;
                                            break;
                                        }else{
                                            error("A constante nao eh compativel");
                                        }
                                    }else{
                                        error("A constante nao eh compativel");
                                    }

                                }
                                topoAux2--;
                            }
                            if(topoAux2 < 0 && strcmp_aux2 == 0){
                                error("A constante nao eh compativel");
                            }
                        }else{
                            ts.Linhas[ts.topo].dim02 = tk.valor_i; // PASSAR CERTO
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
                    tk.processado = 1;
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
    //printf("\n%d", tk.cat);
    ts.topo++; // INCREMENTA��O DO TOPO


}
