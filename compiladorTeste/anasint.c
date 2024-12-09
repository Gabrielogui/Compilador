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
    else if((strcmp(tk.lexema, "prot") == 0) || (strcmp(tk.lexema, "def") == 0)){
        decl_def_proc();
    } else if((strcmp(tk.lexema, "const") == 0) || (verificarTipo(tk.lexema) == 0)){
        ts.Linhas[ts.topo].escopo    = GLOBAL;
        ts.Linhas[ts.topo].categoria = VAR_GLOBAL;
        ts.Linhas[ts.topo].passagem  = NA_PASSAGEM;
        ts.Linhas[ts.topo].zumbi     = NA_ZUMBI;
        decl_list_var();
    }else{
        error("Nao iniciou o procedimento!");
    }
}

// ======= DECL_DEF_PROC =======
void decl_def_proc(){

    /*
        decl_def_proc ::=
            prot idproc ( [[&] tipo { [ ] } { , [&] tipo { [ ] } }]] )
            |
            def ( init | idproc ( [ [&] tipo id1{ [( intcon2 | idconst2 )] } { , [&] tipo id2 { [( intcon2 | idconst2 )] } }] ) ) {decl_list_var} { cmd } endp
    */

    int saida_aux = 0;
    if(strcmp(tk.lexema, "prot") == 0){
        ts.Linhas[ts.topo].categoria = PROTOTIPO;
        ts.Linhas[ts.topo].escopo = GLOBAL;
        ts.Linhas[ts.topo].tipo = NA_TIPO;
        ts.Linhas[ts.topo].passagem = NA_PASSAGEM;
        ts.Linhas[ts.topo].zumbi = NA_ZUMBI;
        ts.Linhas[ts.topo].isArray = NA_ISARRAY;
        ts.Linhas[ts.topo].dim01 = 0;
        ts.Linhas[ts.topo].dim02 = 0;
        ts.Linhas[ts.topo].eh_const = NAO;
        tk = analise_lexica(fd);
        if(tk.cat == ID)
        {
            strcpy(ts.Linhas[ts.topo].lexema, tk.lexema);
            ts.topo++;
            tk = analise_lexica(fd);
            if(tk.cat == SN && tk.codigo == ABRE_PAR)
            {
                tk = analise_lexica(fd);
                if(tk.cat == SN && tk.codigo == E_COMERCIAL)
                {
                    ts.Linhas[ts.topo].passagem = REFERENCIA;
                    tk = analise_lexica(fd);
                }
                else if((verificarTipo(tk.lexema)) == 0)
                {
                    ts.Linhas[ts.topo].passagem = COPIA;
                }
            }
            else
            {
                error("'(' esperado!");
            }
            if(strcmp(tk.lexema, "int") == 0)
            {
                ts.Linhas[ts.topo].tipo = INT_TIPO;
            }
            else if(strcmp(tk.lexema, "real") == 0)
            {
                ts.Linhas[ts.topo].tipo = REAL_TIPO;
            }
            else if(strcmp(tk.lexema, "char") == 0)
            {
                ts.Linhas[ts.topo].tipo = CHAR_TIPO;
            }
            else if(strcmp(tk.lexema, "bool") == 0)
            {
                ts.Linhas[ts.topo].tipo = BOOL_TIPO;
            }
            else
            {
                error("tipo esperado!");
            }
            ts.Linhas[ts.topo].zumbi = NA_ZUMBI;
            ts.Linhas[ts.topo].eh_const = NAO;
            ts.Linhas[ts.topo].escopo = LOCAL;
            ts.Linhas[ts.topo].categoria = PAR_PROCEDIMENTO;
            tk = analise_lexica(fd);
            if(tk.cat == SN && tk.codigo == ABRE_COLCHETES)
            {
                tk = analise_lexica(fd);
                if(tk.cat == SN && tk.codigo == FECHA_COLCHETES)
                {
                    tk = analise_lexica(fd);
                    if(tk.cat == SN && tk.codigo == ABRE_COLCHETES)
                    {
                        tk = analise_lexica(fd);
                        if(tk.cat == SN && tk.codigo == FECHA_COLCHETES)
                        {
                            ts.Linhas[ts.topo].isArray = MATRIZ;
                            ts.topo++;
                            tk = analise_lexica(fd);
                        }
                        else
                        {
                            error("']' esperado!");
                        }
                    }
                    else
                    {
                        ts.Linhas[ts.topo].isArray = VETOR;
                        ts.topo++;
                    }
                }
                else
                {
                    error("']' esperado!");
                }
            }
            else
            {
                ts.Linhas[ts.topo].isArray = ESCALAR;
                ts.topo++;
            }
            if(tk.cat == SN && tk.codigo == FECHA_PAR)
            {
                saida_aux = 1;
                tk.processado = 0;
                pass;
            }
            else if(tk.cat == SN && tk.codigo == VIRGULA)
            {
                while(tk.cat == SN && tk.codigo == VIRGULA)
                {
                    tk = analise_lexica(fd);
                    if(tk.cat == SN && tk.codigo == E_COMERCIAL)
                    {
                        ts.Linhas[ts.topo].passagem = REFERENCIA;
                        ts.Linhas[ts.topo].escopo = LOCAL;
                        ts.Linhas[ts.topo].categoria = PAR_PROCEDIMENTO;
                        tk = analise_lexica(fd);
                    }
                    else if((verificarTipo(tk.lexema)) == 0)
                    {
                        ts.Linhas[ts.topo].passagem = COPIA;
                        ts.Linhas[ts.topo].escopo = LOCAL;
                        ts.Linhas[ts.topo].categoria = PAR_PROCEDIMENTO;
                    }
                    if(strcmp(tk.lexema, "int") == 0)
                    {
                        ts.Linhas[ts.topo].tipo = INT_TIPO;
                    }
                    else if(strcmp(tk.lexema, "real") == 0)
                    {
                        ts.Linhas[ts.topo].tipo = REAL_TIPO;
                    }
                    else if(strcmp(tk.lexema, "char") == 0)
                    {
                        ts.Linhas[ts.topo].tipo = CHAR_TIPO;
                    }
                    else if(strcmp(tk.lexema, "bool") == 0)
                    {
                        ts.Linhas[ts.topo].tipo = BOOL_TIPO;
                    }
                    ts.Linhas[ts.topo].zumbi = NA_ZUMBI;
                    ts.Linhas[ts.topo].eh_const = NAO;
                    tk = analise_lexica(fd);
                    if(tk.cat == SN && tk.codigo == ABRE_COLCHETES)
                    {
                        tk = analise_lexica(fd);
                        if(tk.cat == SN && tk.codigo == FECHA_COLCHETES)
                        {
                            tk = analise_lexica(fd);
                            if(tk.cat == SN && tk.codigo == ABRE_COLCHETES)
                            {
                                tk = analise_lexica(fd);
                                if(tk.cat == SN && tk.codigo == FECHA_COLCHETES)
                                {
                                    ts.Linhas[ts.topo].isArray = MATRIZ;
                                    ts.topo++;
                                    tk = analise_lexica(fd);
                                }
                                else
                                {
                                    error("']' esperado!");
                                }
                            }
                            else
                            {
                                ts.Linhas[ts.topo].isArray = VETOR;
                                ts.topo++;
                            }
                        }
                        else
                        {
                            error("']' esperado!");
                        }
                    }
                    else
                    {
                        ts.Linhas[ts.topo].isArray = ESCALAR;
                        ts.topo++;
                    }
                    if(tk.cat == SN && tk.codigo == FECHA_PAR)
                    {
                        saida_aux = 1;
                        break;
                    }
                }
            }
            else
            {
                error("')' ou ',' esperado!");
            }
            if(saida_aux == 1)
            {
                tk.processado = 0;
                pass;
            }
        }
        else
        {
            error("prototipo sem identificador!");
        }
    }
    else if(strcmp(tk.lexema, "def") == 0)
    {
        ts.Linhas[ts.topo].escopo = GLOBAL;
        ts.Linhas[ts.topo].tipo = NA_TIPO;
        ts.Linhas[ts.topo].categoria = PROCEDIMENTO;
        ts.Linhas[ts.topo].passagem = NA_PASSAGEM;
        ts.Linhas[ts.topo].zumbi = NA_ZUMBI;
        ts.Linhas[ts.topo].isArray = NA_ISARRAY;
        ts.Linhas[ts.topo].dim01 = 0;
        ts.Linhas[ts.topo].dim02 = 0;
        ts.Linhas[ts.topo].eh_const = NAO;

        tk = analise_lexica(fd);
        if(tk.cat == PVR && strcmp(tk.lexema, "init") == 0)
        {
            strcpy(ts.Linhas[ts.topo].lexema, tk.lexema);
            ts.topo++;
            tk = analise_lexica(fd);
        }
        else if(tk.cat == ID)
        {
            strcpy(ts.Linhas[ts.topo].lexema, tk.lexema);
            ts.topo++;
            tk = analise_lexica(fd);
            if(tk.cat == SN && tk.codigo == ABRE_PAR)
            {
                do{
                    tk = analise_lexica(fd);
                    if(tk.cat == SN && tk.codigo == E_COMERCIAL)
                    {
                        ts.Linhas[ts.topo].passagem = REFERENCIA;
                        tk = analise_lexica(fd);
                    }
                    else if((verificarTipo(tk.lexema)) == 0)
                    {
                        ts.Linhas[ts.topo].passagem = COPIA;
                    }
                    else if(tk.cat == SN && tk.codigo == FECHA_PAR)
                    {
                        break;
                    }
                    if(strcmp(tk.lexema, "int") == 0)
                    {
                        ts.Linhas[ts.topo].tipo = INT_TIPO;
                    }
                    else if(strcmp(tk.lexema, "real") == 0)
                    {
                        ts.Linhas[ts.topo].tipo = REAL_TIPO;
                    }
                    else if(strcmp(tk.lexema, "char") == 0)
                    {
                        ts.Linhas[ts.topo].tipo = CHAR_TIPO;
                    }
                    else if(strcmp(tk.lexema, "bool") == 0)
                    {
                        ts.Linhas[ts.topo].tipo = BOOL_TIPO;
                    }
                    else
                    {
                        error("tipo esperado!");
                    }
                    ts.Linhas[ts.topo].zumbi = VIVO;
                    ts.Linhas[ts.topo].eh_const = NAO;
                    ts.Linhas[ts.topo].escopo = LOCAL;
                    ts.Linhas[ts.topo].categoria = PAR_PROCEDIMENTO;
                    tk = analise_lexica(fd);
                    if(tk.cat == ID)
                    {
                        strcpy(ts.Linhas[ts.topo].lexema, tk.lexema);
                        tk = analise_lexica(fd);
                        if(tk.cat == SN && tk.codigo == ABRE_COLCHETES)
                        {
                           tk = analise_lexica(fd);
                           if(tk.cat == ID) // BUSCA DO DIM01 CASO SEJA CONSTANTE
                           {
                                int topoAux1 = ts.topo;
                                int strcmp_aux1 = 0;
                                while(topoAux1 >= 0)
                                {
                                    if(strcmp(ts.Linhas[topoAux1].lexema, tk.lexema) == 0)
                                    {
                                        strcmp_aux1 = 1;
                                        if(ts.Linhas[topoAux1].eh_const == SIM)
                                        {
                                            if(ts.Linhas[topoAux1].tipo == INT_TIPO)
                                            {
                                                ts.Linhas[ts.topo].dim01 = ts.Linhas[topoAux1].constInt;
                                                break;
                                            }
                                            else
                                            {
                                                error("A constante nao eh compativel");
                                            }
                                        }
                                        else
                                        {
                                            error("A constante nao eh compativel");
                                        }
                                    }
                                    topoAux1--;
                                }
                                if(topoAux1 < 0 && strcmp_aux1 == 0)
                                {
                                    error("A constante nao eh compativel");
                                }
                           }
                           else if(tk.cat == CT_I)
                           {
                               ts.Linhas[ts.topo].dim01 = tk.valor_i;
                           }
                           else
                           {
                               error("Array sem dimensao!");
                           }
                           tk = analise_lexica(fd);
                           if(tk.cat == SN && tk.codigo == FECHA_COLCHETES)
                           {
                               tk = analise_lexica(fd);
                               if(tk.cat == SN && tk.codigo == ABRE_COLCHETES)
                               {
                                   tk = analise_lexica(fd);
                                   if(tk.cat == ID) // BUSCA DO DIM02 CASO SEJA CONSTANTE
                                   {
                                        int topoAux2 = ts.topo;
                                        int strcmp_aux2 = 0;
                                        while(topoAux2 >= 0)
                                        {
                                            if(strcmp(ts.Linhas[topoAux2].lexema, tk.lexema) == 0)
                                            {
                                                strcmp_aux2 = 1;
                                                if(ts.Linhas[topoAux2].eh_const == SIM)
                                                {
                                                    if(ts.Linhas[topoAux2].tipo == INT_TIPO)
                                                    {
                                                        ts.Linhas[ts.topo].dim02 = ts.Linhas[topoAux2].constInt;
                                                        break;
                                                    }
                                                    else
                                                    {
                                                        error("A constante nao eh compativel");
                                                    }
                                                }
                                                else
                                                {
                                                    error("A constante nao eh compativel");
                                                }
                                            }
                                            topoAux2--;
                                        }
                                        if(topoAux2 < 0 && strcmp_aux2 == 0)
                                        {
                                            error("A constante nao eh compativel");
                                        }
                                   }
                                   else if(tk.cat == CT_I)
                                   {
                                       ts.Linhas[ts.topo].dim02 = tk.valor_i;
                                   }
                                   else
                                   {
                                       error("Array sem dimensao!");
                                   }
                                   ts.Linhas[ts.topo].isArray = MATRIZ;
                                   tk = analise_lexica(fd);
                                   if(tk.cat == SN && tk.codigo == FECHA_COLCHETES)
                                   {
                                       tk = analise_lexica(fd);
                                   }
                                   else
                                   {
                                       error("']' esperado!");
                                   }
                            }
                            else
                            {
                                ts.Linhas[ts.topo].isArray = VETOR;
                            }
                           }
                           else
                           {
                               error("']' esperado!");
                           }

                        }
                        else
                        {
                            ts.Linhas[ts.topo].isArray = ESCALAR;
                            ts.Linhas[ts.topo].dim01 = 0;
                            ts.Linhas[ts.topo].dim02 = 0;
                        }
                        ts.topo++;
                    }
                    else
                    {
                        error("parametro de procedimento sem identificador!");
                    }
                }while(tk.cat == SN && tk.codigo == VIRGULA);
            }
            if(tk.cat == SN && tk.codigo == FECHA_PAR)
            {
                tk = analise_lexica(fd);
            }
            else
            {
                error("')' esperado!");
            }
        }
        else
        {
            error("procedimento sem identificador!");
        }

        //while do decl list var    negaçao desse if((strcmp(tk.lexema, "const") == 0) || (verificarTipo(tk.lexema) == 0))
        while((strcmp(tk.lexema, "const") == 0) || (verificarTipo(tk.lexema) == 0))
        {
            ts.Linhas[ts.topo].escopo    = LOCAL;
            ts.Linhas[ts.topo].categoria = VAR_LOCAL;
            ts.Linhas[ts.topo].passagem  = NA_PASSAGEM;
            ts.Linhas[ts.topo].zumbi     = NA_ZUMBI;
            decl_list_var();
            if(tk.processado != 1)
            {
                tk = analise_lexica(fd);
                tk.processado = 0;
            }
        }

        while(tk.cat == PVR && strcmp(tk.lexema, "endp") != 0) // verificar depois se esta certo
        {
            cmd();
            if(tk.processado != 1)
            {
                tk = analise_lexica(fd);
                tk.processado = 0;
            }
        }

        // fazer função cmd

        if(tk.cat == PVR && strcmp(tk.lexema, "endp") == 0)
        {
            // limpando a tabela de simbolos
            while(ts.Linhas[ts.topo].categoria == VAR_LOCAL && ts.Linhas[ts.topo].escopo == LOCAL)
            {
                memset(&ts.Linhas[ts.topo], 0, sizeof(REG_TS));
            }

            int topo_auxx = ts.topo;
            while(ts.Linhas[topo_auxx].zumbi == VIVO)
            {
                ts.Linhas[topo_auxx].zumbi = ZUMBI;
                topo_auxx--;
            }
        }
        else
        {
            error("endp esperado!");
        }
    }
}

// ======= DECL_LIST_VAR =======
void decl_list_var(){


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
        int topoAux_virg = ts.topo - 1; // CUIDADO COM ISSO
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
        }else if(ts.Linhas[ts.topo].tipo == BOOL_TIPO && tk.cat == CT_I){
            if(ts.Linhas[ts.topo].eh_const == SIM){
                ts.Linhas[ts.topo].constBool = tk.valor_i;
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
                                }else{
                                    tk.processado = 0;
                                }
                            }else if((tk.cat == SN) && (tk.codigo == FECHA_CHAVES)){
                                tk = analise_lexica(fd);
                                break;
                            }else{
                                error("Fecha chaves ou virgula esperado");
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
        //printf("\nvericiando");
        ts.Linhas[ts.topo].isArray = ESCALAR;
    }
    //printf("\n%d", tk.cat);
    ts.topo++; // INCREMENTA��O DO TOPO


}

void cmd()
{
    if(tk.cat == PVR && strcmp(tk.lexema, "do") == 0)
    {
        tk = analise_lexica(fd);
        if(tk.cat == ID)
        {
            tk = analise_lexica(fd);
            if(tk.cat == SN && tk.codigo == ABRE_PAR)
            {
                do{
                    tk = analise_lexica(fd);
                    //expr();

                }while(tk.cat == SN && tk.codigo == VIRGULA);
                if(tk.cat == SN && tk.codigo == FECHA_PAR)
                {
                    pass; // TALVEZ ALGO AQUI
                }
                else
                {
                    error("')' esperado!");
                }
            }
            else
            {
                error("'(' esperado!");
            }
        }
        else
        {
            error("identificador de procedimento esperado!");
        }
    }
    else if(tk.cat == PVR && strcmp(tk.lexema, "while") == 0)
    {
        tk = analise_lexica(fd);
        if(tk.cat == SN && tk.codigo == ABRE_PAR)
        {
            tk = analise_lexica(fd);
            //expr();
            // TALVEZ tk = analise_lexica(fd);
            if(tk.cat == SN && tk.codigo == FECHA_PAR)
            {
                tk = analise_lexica(fd);
                if(tk.cat == PVR && strcmp(tk.lexema, "endw") == 0)
                {
                    pass; // TALVEZ ALGO AQUI
                }
                else
                {
                    while(strcmp(tk.lexema, "endw") != 0)
                    {
                        cmd();
                        tk = analise_lexica(fd);
                    }
                    if(tk.cat != PVR && strcmp(tk.lexema, "endw") != 0)
                    {
                        error("'endw' esperado!");
                    }
                }
            }
            else
            {
                error("')' esperado!");
            }
        }
        else
        {
            error("'(' esperado!");
        }
    }
    else if(tk.cat == PVR && strcmp(tk.lexema, "var") == 0)
    {
        tk = analise_lexica(fd);
        if(tk.cat == ID)
        {
            tk = analise_lexica(fd);
            if(tk.cat == PVR && strcmp(tk.lexema, "from") == 0)
            {
                tk = analise_lexica(fd); // TALVEZ NAO TENHA
                //expr();
                //tk = analise_lexica(fd); TALVEZ NAO TENHA
                if(tk.cat == PVR && ((strcmp(tk.lexema, "to") == 0) || (strcmp(tk.lexema, "dt") == 0)))
                {
                    tk = analise_lexica(fd); // TALVEZ NAO TENHA
                    //expr();
                    //tk = analise_lexica(fd); TALVEZ NAO TENHA
                    if(tk.cat == PVR && strcmp(tk.lexema, "by") == 0)
                    {
                        tk = analise_lexica(fd);
                        if(tk.cat == CT_I || tk.cat == ID)
                        {
                            if(tk.cat == ID){
                                int topoAux1 = ts.topo;
                                int strcmp_aux1 = 0;
                                while(topoAux1 >= 0){
                                    if(strcmp(ts.Linhas[topoAux1].lexema, tk.lexema) == 0){
                                        strcmp_aux1 = 1;
                                        if(ts.Linhas[topoAux1].eh_const == SIM){
                                            if(ts.Linhas[topoAux1].tipo == INT_TIPO){
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
                            }
                            tk = analise_lexica(fd);
                        }
                        else
                        {
                            error("inteiro ou identificador esperado!");
                        }
                    }
                    while(tk.cat != PVR && strcmp(tk.lexema, "endv") != 0)
                    {
                        cmd();
                        tk = analise_lexica(fd);
                    }
                }
                else
                {
                    error("'to' ou 'dt' esperado");
                }
            }
            else
            {
                error("'from' esperado!");
            }
        }
        else
        {
            error("identificador esperado!");
        }
    }
    else if(tk.cat == PVR && strcmp(tk.lexema, "if") == 0)
    {
        tk = analise_lexica(fd);
        if(tk.cat == SN && tk.codigo == ABRE_PAR)
        {
            tk = analise_lexica(fd);
            //expr();
            //tk = analise_lexica(fd); TALVEZ NAO TENHA
            if(tk.cat == SN && tk.codigo == FECHA_PAR)
            {
                tk = analise_lexica(fd);
                while((strcmp(tk.lexema, "elif") != 0) && (strcmp(tk.lexema, "else") != 0) && (strcmp(tk.lexema, "endi") != 0))
                {
                    cmd();
                    tk = analise_lexica(fd);
                }
                while(tk.cat == PVR && (strcmp(tk.lexema, "elif") == 0))
                {
                    tk = analise_lexica(fd);
                    if(tk.cat == SN && tk.codigo == ABRE_PAR)
                    {
                        tk = analise_lexica(fd);
                        //expr();
                        //tk = analise_lexica(fd); TALVEZ NAO TENHA
                        if(tk.cat == SN && tk.codigo == FECHA_PAR)
                        {
                            tk = analise_lexica(fd);
                            while((strcmp(tk.lexema, "elif") != 0) && (strcmp(tk.lexema, "else") != 0) && (strcmp(tk.lexema, "endi") != 0))
                            {
                                cmd();
                                tk = analise_lexica(fd);
                            }
                        }
                        else
                        {
                            error("')' esperado!");
                        }
                    }
                    else
                    {
                        error("'(' esperado!");
                    }
                }
                if(tk.cat == PVR && strcmp(tk.lexema, "else") == 0)
                {
                    tk = analise_lexica(fd);
                    while(strcmp(tk.lexema, "endi") != 0)
                    {
                        cmd();
                        tk = analise_lexica(fd);
                    }
                }
                if(tk.cat == PVR && strcmp(tk.lexema, "endi") == 0)
                {
                    pass; // TALVEZ ALGO AQUI
                }
                else
                {
                    error("'endi' esperado!");
                }
            }
            else
            {
                error("')' esperado!");
            }


        }
        else
        {
            error("'(' esperado!");
        }
    }
    else if(tk.cat == ID)
    {
        //atrib();
    }
    else if(tk.cat == PVR && strcmp(tk.lexema, "getout") == 0)
    {
        pass; // TALVEZ ALGO AQUI
    }
    else if(tk.cat == PVR && strcmp(tk.lexema, "getint") == 0)
    {
        tk = analise_lexica(fd);
        if(tk.cat == ID)
        {
            pass; // TALVEZ ALGO AQUI
        }
        else
        {
            error("identificador esperado!");
        }
    }
    else if(tk.cat == PVR && strcmp(tk.lexema, "getreal") == 0)
    {
        tk = analise_lexica(fd);
        if(tk.cat == ID)
        {
            pass; // TALVEZ ALGO AQUI
        }
        else
        {
            error("identificador esperado!");
        }
    }
    else if(tk.cat == PVR && strcmp(tk.lexema, "getchar") == 0)
    {
        tk = analise_lexica(fd);
        if(tk.cat == ID)
        {
            pass; // TALVEZ ALGO AQUI
        }
        else
        {
            error("identificador esperado!");
        }
    }
    else if(tk.cat == PVR && strcmp(tk.lexema, "getstr") == 0)
    {
        tk = analise_lexica(fd);
        if(tk.cat == ID)
        {
            pass; // TALVEZ ALGO AQUI
        }
        else
        {
            error("identificador esperado!");
        }
    }
    else if(tk.cat == PVR && strcmp(tk.lexema, "putint") == 0)
    {
        tk = analise_lexica(fd);
        if(tk.cat == ID)
        {
            pass; // TALVEZ ALGO AQUI
        }
        else if(tk.cat == CT_I)
        {
            pass; // TALVEZ ALGO AQUI
        }
        else
        {
            error("identificador ou inteiro esperado!");
        }
    }
    else if(tk.cat == PVR && strcmp(tk.lexema, "putreal") == 0)
    {
        tk = analise_lexica(fd);
        if(tk.cat == ID)
        {
            pass; // TALVEZ ALGO AQUI
        }
        else if(tk.cat == CT_R)
        {
            pass; // TALVEZ ALGO AQUI
        }
        else
        {
            error("identificador ou real esperado!");
        }
    }
    else if(tk.cat == PVR && strcmp(tk.lexema, "putchar") == 0)
    {
        tk = analise_lexica(fd);
        if(tk.cat == ID)
        {
            pass; // TALVEZ ALGO AQUI
        }
        else if(tk.cat == CT_C)
        {
            pass; // TALVEZ ALGO AQUI
        }
        else
        {
            error("identificador ou char esperado!");
        }
    }
    else if(tk.cat == PVR && strcmp(tk.lexema, "putstr") == 0)
    {
        tk = analise_lexica(fd);
        if(tk.cat == ID)
        {
            pass; // TALVEZ ALGO AQUI
        }
        else if(tk.cat == CT_S)
        {
            pass; // TALVEZ ALGO AQUI
        }
        else
        {
            error("identificador ou string esperado!");
        }
    }
    else
    {
        error("comando invalido!");
    }
}

void atrib(){

}

void expr()
{
    expr_simp();

    tk = analise_lexica(fd);




}

void expr_simp(){
    if(tk.cat == SN && (tk.codigo == ADICAO || tk.codigo == SUBTRACAO))
    {
        tk = analise_lexica(fd);
    }

    termo();

}

void termo(){

    fator();
}

void fator(){

    if(tk.cat == ID)
    {
        tk = analise_lexica(fd);
    }
    else
    {
        error("identificador esperado!");
    }

    while(tk.cat == CT_I || tk.cat == CT_R || tk.cat == CT_C || (tk.cat == SN && (tk.codigo == ABRE_PAR || tk.codigo == NEGACAO)))
    {
        if(tk.cat == SN && tk.codigo == ABRE_COLCHETES)
        {
            tk = analise_lexica(fd);

            expr();

            tk = analise_lexica(fd); // TALVEZ SEJA INUTIL

            if(tk.cat == SN && tk.codigo == FECHA_COLCHETES)
            {
                tk = analise_lexica(fd);
            }
            else
            {
                error("']' esperado!");
            }

        }
        else
        {
            error("'[' esperado!");
        }
    }

    if(tk.cat == CT_I)
    {
        pass;
    }
    else if(tk.cat == CT_R)
    {
        pass;
    }
    else if(tk.cat == CT_C)
    {
        pass;
    }
    else if(tk.cat == SN && tk.codigo == ABRE_PAR)
    {
        tk = analise_lexica(fd);

        expr();

        tk = analise_lexica(fd);

        if(tk.cat == SN && tk.codigo == FECHA_PAR)
        {
            pass;
        }
        else
        {
            error("')' esperado! ");
        }

    }
    else if(tk.cat == SN && tk.codigo == NEGACAO)
    {
        tk = analise_lexica(fd);

        fator();
    }
    else
    {
        error("fator invalido!");
    }


}
