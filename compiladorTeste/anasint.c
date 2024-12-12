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
int flag_endi = 0;

// |=======| FUNÇÃO TABELA DE SÍMBOLOS |=======|
void inserirTabelaDeSimbolos(){

}

void tirarTabelaDeSimbolos(){

}

int consultaTabelaDeSimbolos(char lexema[TAM_MAX_LEXEMA]){
     // BUSCA DO DIM01 CASO SEJA CONSTANTE
    int topoAux1 = ts.topo;
    int strcmp_aux1 = 0;
    while(topoAux1 >= 0){
        if(strcmp(ts.Linhas[topoAux1].lexema, lexema) == 0){
            strcmp_aux1 = 1;
            return topoAux1;


        }
        topoAux1--;
    }
    if(topoAux1 < 0 && strcmp_aux1 == 0){
        return -1;
    }
}


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
    printf("\n%d", contLinhas);
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
            printf("\n 1 %d | %s", tk.codigo, tk.lexema);
            ts.Linhas[ts.topo].escopo    = LOCAL;
            ts.Linhas[ts.topo].categoria = VAR_LOCAL;
            ts.Linhas[ts.topo].passagem  = NA_PASSAGEM;
            ts.Linhas[ts.topo].zumbi     = NA_ZUMBI;
            decl_list_var();
            printf("\n 2 %d | %s", tk.codigo, tk.lexema);
            if(tk.processado != 1)
            {
                tk = analise_lexica(fd);
                tk.processado = 0;
            }
            printf("\n 3 %d | %s", tk.codigo, tk.lexema);
        }

        printf("\n 4 %d | %s", tk.codigo, tk.lexema);

        while(strcmp(tk.lexema, "endp") != 0) // verificar depois se esta certo
        {
            printf("\n 5 %d | %s", tk.codigo, tk.lexema);
            cmd();
            printf("\n 6 %d | %s", tk.codigo, tk.lexema);
            if(tk.processado != 1)
            {
                tk = analise_lexica(fd);
                tk.processado = 0;
            }
            printf("\n 7 %d | %s", tk.codigo, tk.lexema);
        }



        printf("\n 8 %d | %s", tk.codigo, tk.lexema);
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
            tk.processado = 0; // PEGANDO O PRÓXIMO TOKEN, QUE PODE SER O ENDFILE
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
                int a = consultaTabelaDeSimbolos(tk.lexema);
                if(a == -1) error("Constante nao eh compativel");
                if(ts.Linhas[a].eh_const == SIM){
                    if(ts.Linhas[a].tipo == INT_TIPO){
                        ts.Linhas[ts.topo].dim01 = ts.Linhas[a].constInt;

                    }else{
                        error("A constante nao eh compativel");
                    }
                }else{
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

// ======= CMD =======
void cmd()
{
    printf("\n 9 %d | %s", tk.codigo, tk.lexema);
    if(tk.cat == PVR && strcmp(tk.lexema, "do") == 0) // VOLTAR PARA REVER A PORRA DA VÍRGULA
    {
        int flag_virgula = 0;

        printf("\n 10 %d | %s", tk.codigo, tk.lexema);
        tk = analise_lexica(fd);
        printf("\n 11 %d | %s", tk.codigo, tk.lexema); // do id()
        if(tk.cat == ID)
        {
            printf("\n 12 %d | %s", tk.codigo, tk.lexema);
            tk = analise_lexica(fd);
            printf("\n 13 %d | %s", tk.codigo, tk.lexema);
            if(tk.cat == SN && tk.codigo == ABRE_PAR)
            {
                tk = analise_lexica(fd);
                printf("\n 14 %d | %s", tk.codigo, tk.lexema);
                do{

                    if(tk.cat == SN && tk.codigo == VIRGULA){
                        tk = analise_lexica(fd);
                        flag_virgula = 1;
                    }

                    if(tk.cat == SN && tk.codigo == FECHA_PAR){
                        break;
                    }
                    printf("\n 15 %d | %s", tk.codigo, tk.lexema);
                    flag_virgula = 0;
                    expr();
                    printf("\n 16 %d | %s", tk.codigo, tk.lexema);
                    if(tk.processado != 1)
                    {
                        tk = analise_lexica(fd);
                        tk.processado = 0;
                    }
                    printf("\n 17 %d | %s", tk.codigo, tk.lexema);

                }while(tk.cat == SN && tk.codigo == VIRGULA);


                if(tk.cat == SN && tk.codigo == FECHA_PAR)
                {
                    if(flag_virgula == 1){
                        error("Virgula mal colocada! ");
                    }
                    tk.processado = 0;
                }
                else
                {
                    printf("\n 18 %d | %s", tk.codigo, tk.lexema);
                    error(" -- ')' esperado!");
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
        printf("\n 60 %d | %s", tk.codigo, tk.lexema);
        tk = analise_lexica(fd);
        printf("\n 61 %d | %s", tk.codigo, tk.lexema);
        if(tk.cat == SN && tk.codigo == ABRE_PAR)
        {
            printf("\n 62 %d | %s", tk.codigo, tk.lexema);
            tk = analise_lexica(fd);
            printf("\n 63 %d | %s", tk.codigo, tk.lexema);
            expr();
            printf("\n 64 %d | %s", tk.codigo, tk.lexema);
            if(tk.processado != 1)
            {
                tk = analise_lexica(fd);
                tk.processado = 0;
            }

            printf("\n 65 %d | %s", tk.codigo, tk.lexema);
            if(tk.cat == SN && tk.codigo == FECHA_PAR)
            {
                tk = analise_lexica(fd);
                if(tk.cat == PVR && strcmp(tk.lexema, "endw") == 0)
                {
                    printf("\n 66 %d | %s", tk.codigo, tk.lexema);
                    pass; // TALVEZ ALGO AQUI
                }
                else
                {
                    printf("\n 67 %d | %s", tk.codigo, tk.lexema);
                    while(strcmp(tk.lexema, "endw") != 0)
                    {
                        printf("\n 68 %d | %s", tk.codigo, tk.lexema);
                        cmd();
                        printf("\n 69 %d | %s", tk.codigo, tk.lexema);
                        if(tk.processado != 1)
                        {
                            tk = analise_lexica(fd);
                            tk.processado = 0;
                        }
                        printf("\n 691 %d | %s", tk.codigo, tk.lexema);
                    }
                    if(strcmp(tk.lexema, "endw") != 0)
                    {
                        error("'endw' esperado!");
                    }
                    printf("\n 692 %d | %s", tk.codigo, tk.lexema);
                    tk = analise_lexica(fd); // REVER ESSA PORRA
                    tk.processado = 1;
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
        printf("\n 90 %d | %s", tk.codigo, tk.lexema);
        tk = analise_lexica(fd);
        printf("\n 91 %d | %s", tk.codigo, tk.lexema);
        if(tk.cat == ID)
        {
            printf("\n 911 %d | %s", tk.codigo, tk.lexema);
            tk = analise_lexica(fd);
            printf("\n 92 %d | %s", tk.codigo, tk.lexema);
            if(tk.cat == PVR && strcmp(tk.lexema, "from") == 0)
            {
                printf("\n 921 %d | %s", tk.codigo, tk.lexema);
                tk = analise_lexica(fd); // TALVEZ NAO TENHA
                printf("\n 93 %d | %s", tk.codigo, tk.lexema);
                expr();
                printf("\n 94 %d | %s", tk.codigo, tk.lexema);
                if(tk.processado != 1)
                {
                    tk = analise_lexica(fd);
                    tk.processado = 0;
                }
                printf("\n 95 %d | %s", tk.codigo, tk.lexema);
                if(tk.cat == PVR && ((strcmp(tk.lexema, "to") == 0) || (strcmp(tk.lexema, "dt") == 0)))
                {
                    printf("\n 951 %d | %s", tk.codigo, tk.lexema);
                    tk = analise_lexica(fd); // TALVEZ NAO TENHA
                    printf("\n 96 %d | %s", tk.codigo, tk.lexema);
                    expr();
                    printf("\n 97 %d | %s", tk.codigo, tk.lexema);
                    if(tk.processado != 1)
                    {
                        tk = analise_lexica(fd);
                        tk.processado = 0;
                    }
                    printf("\n 971 %d | %s", tk.codigo, tk.lexema);
                    if(tk.cat == PVR && strcmp(tk.lexema, "by") == 0)
                    {
                        printf("\n 972 %d | %s", tk.codigo, tk.lexema);
                        tk = analise_lexica(fd);
                        printf("\n 98 %d | %s", tk.codigo, tk.lexema);
                        if(tk.cat == CT_I || tk.cat == ID)
                        {
                            printf("\n 981 %d | %s", tk.codigo, tk.lexema);
                            if(tk.cat == ID){
                                printf("\n 99 %d | %s", tk.codigo, tk.lexema);
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
                            printf("\n 100 %d | %s", tk.codigo, tk.lexema);
                            tk = analise_lexica(fd);
                            printf("\n 101 %d | %s", tk.codigo, tk.lexema);
                        }
                        else
                        {
                            error("inteiro ou identificador esperado!");
                        }
                    }
                    while(strcmp(tk.lexema, "endv") != 0)
                    {
                        printf("\n 102 %d | %s", tk.codigo, tk.lexema);
                        cmd();
                        printf("\n 103 %d | %s", tk.codigo, tk.lexema);
                        if(tk.processado != 1){
                            tk = analise_lexica(fd);
                            tk.processado = 0;
                        }
                        printf("\n 104 %d | %s", tk.codigo, tk.lexema);
                    }
                    tk = analise_lexica(fd);
                    tk.processado = 1;
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

        printf("\n 130 %d | %s", tk.codigo, tk.lexema);
        tk = analise_lexica(fd);
        printf("\n 131 %d | %s", tk.codigo, tk.lexema);
        if(tk.cat == SN && tk.codigo == ABRE_PAR)
        {
            printf("\n 1311 %d | %s", tk.codigo, tk.lexema);
            tk = analise_lexica(fd);
            printf("\n 132 %d | %s", tk.codigo, tk.lexema);
            expr();
            printf("\n 133 %d | %s", tk.codigo, tk.lexema);
            if(tk.processado != 1)
            {
                tk = analise_lexica(fd);
                tk.processado = 0;
            }
            printf("\n 134 %d | %s", tk.codigo, tk.lexema);
            if(tk.cat == SN && tk.codigo == FECHA_PAR)
            {
                printf("\n 135 %d | %s", tk.codigo, tk.lexema);
                tk = analise_lexica(fd);
                if(strcmp(tk.lexema, "if") == 0){
                    flag_endi++;
                }
                printf("\n 136 %d | %s", tk.codigo, tk.lexema);

                if(strcmp(tk.lexema, "endi") == 0){
                    flag_endi++;
                }

                while((strcmp(tk.lexema, "elif") != 0) && (strcmp(tk.lexema, "else") != 0) && (strcmp(tk.lexema, "endi") != 0))
                {
                    flag_endi++;
                    printf("\n adicionando mais uma flag do endi %d", flag_endi);
                    printf("\n 137 %d | %s", tk.codigo, tk.lexema);
                    cmd();
                    printf("\n 138 %d | %s", tk.codigo, tk.lexema);
                    if(strcmp(tk.lexema, "endi") != 0){
                        if(tk.processado != 1){
                            tk = analise_lexica(fd);
                            tk.processado = 0;
                        }
                    }else{
                        tk.processado = 0;
                    }

                    printf("\n 139 %d | %s", tk.codigo, tk.lexema);
                }
                while(tk.cat == PVR && (strcmp(tk.lexema, "elif") == 0))
                {
                    printf("\n 140 %d | %s", tk.codigo, tk.lexema);
                    tk = analise_lexica(fd);
                    printf("\n 141 %d | %s", tk.codigo, tk.lexema);
                    if(tk.cat == SN && tk.codigo == ABRE_PAR)
                    {
                        printf("\n 142 %d | %s", tk.codigo, tk.lexema);
                        tk = analise_lexica(fd);
                        printf("\n 143 %d | %s", tk.codigo, tk.lexema);
                        expr();
                        printf("\n 144 %d | %s", tk.codigo, tk.lexema);
                        if(tk.processado != 1)
                        {
                            tk = analise_lexica(fd);
                            tk.processado = 0;
                        }
                        printf("\n 145 %d | %s", tk.codigo, tk.lexema);
                        if(tk.cat == SN && tk.codigo == FECHA_PAR)
                        {
                            printf("\n 146 %d | %s", tk.codigo, tk.lexema);
                            tk = analise_lexica(fd);
                            printf("\n 147 %d | %s", tk.codigo, tk.lexema);
                            while((strcmp(tk.lexema, "elif") != 0) && (strcmp(tk.lexema, "else") != 0) && (strcmp(tk.lexema, "endi") != 0))
                            {
                                printf("\n 148 %d | %s", tk.codigo, tk.lexema);
                                cmd();
                                printf("\n 149 %d | %s", tk.codigo, tk.lexema);
                                if(strcmp(tk.lexema, "endi") != 0){
                                    if(tk.processado != 1){
                                        tk = analise_lexica(fd);
                                        tk.processado = 0;
                                    }
                                }else{
                                    tk.processado = 0;
                                }

                                printf("\n 150 %d | %s", tk.codigo, tk.lexema);
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
                    printf("\n 151 %d | %s", tk.codigo, tk.lexema);
                    tk = analise_lexica(fd);
                    printf("\n 152 %d | %s", tk.codigo, tk.lexema);
                    while(strcmp(tk.lexema, "endi") != 0)
                    {
                        printf("\n 153 %d | %s", tk.codigo, tk.lexema);
                        cmd();
                        printf("\n 154 %d | %s", tk.codigo, tk.lexema);
                        if(strcmp(tk.lexema, "endi") != 0){
                            if(tk.processado != 1){
                                tk = analise_lexica(fd);
                                tk.processado = 0;
                            }
                        }else{
                            tk.processado = 0;
                        }
                        printf("\n 155 %d | %s", tk.codigo, tk.lexema);
                    }
                }
                if(tk.cat == PVR && strcmp(tk.lexema, "endi") == 0)
                {
                    printf("\n 156 %d | %s", tk.codigo, tk.lexema);
                    while(flag_endi > 1){
                        printf("\n 157 %d | %s", tk.codigo, tk.lexema);
                        if(tk.processado != 1){
                                tk = analise_lexica(fd);
                                printf("\n 158 %d | %s", tk.codigo, tk.lexema);
                                tk.processado = 0;
                                /*if(strcmp(tk.lexema, "endp") == 0)
                                {
                                    tk.processado = 1;
                                }*/
                            }
                        printf("\n FLAG: %d", flag_endi);
                        flag_endi--;
                    }
                    printf("\n 159 %d | %s", tk.codigo, tk.lexema);
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
        atrib();
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

// ======= ATRIB =======
void atrib()
{
    printf("\n 50 %d | %s", tk.codigo, tk.lexema);
    tk = analise_lexica(fd);
    printf("\n 51 %d | %s", tk.codigo, tk.lexema);
    while(tk.codigo != ATRIBUICAO)
    {
        printf("\n 52 %d | %s", tk.codigo, tk.lexema);
        if(tk.cat == SN && tk.codigo == ABRE_COLCHETES)
        {
            printf("\n 53 %d | %s", tk.codigo, tk.lexema);
            tk = analise_lexica(fd);
            printf("\n 54 %d | %s", tk.codigo, tk.lexema);
            expr();
            printf("\n 55 %d | %s", tk.codigo, tk.lexema);
            if(tk.processado != 1)
            {
                tk = analise_lexica(fd);
                tk.processado = 0;
            }
            printf("\n 55 %d | %s", tk.codigo, tk.lexema);
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
    if(tk.cat == SN && tk.codigo == ATRIBUICAO)
    {
        printf("\n 56 %d | %s", tk.codigo, tk.lexema);
        tk = analise_lexica(fd);
        printf("\n 57 %d | %s", tk.codigo, tk.lexema);
        expr();
        printf("\n 58 %d | %s", tk.codigo, tk.lexema);
    }
    else
    {
        error("'=' esperado!");
    }


}

// ======= EXPR =======
void expr()
{
    expr_simp();
    printf("\n 34 %d | %s", tk.codigo, tk.lexema);
    if(tk.processado != 1)
    {
        tk = analise_lexica(fd);
        tk.processado = 0;
    }
    printf("\n 35 %d | %s", tk.codigo, tk.lexema);
    if(tk.cat == SN && (tk.codigo == COMPARACAO || tk.codigo == DIFERENTE || tk.codigo == MENOR_OU_IGUAL
    || tk.codigo == MENOR_QUE || tk.codigo == MAIOR_OU_IGUAL || tk.codigo == MAIOR_QUE))
    {
        tk = analise_lexica(fd);

        expr_simp();
    }
    printf("\n 36 %d | %s", tk.codigo, tk.lexema);
}

// ======= EXPR_SIMP =======
void expr_simp(){
    if(tk.cat == SN && (tk.codigo == ADICAO || tk.codigo == SUBTRACAO))
    {
        tk = analise_lexica(fd);
    }
    printf("\n 29 %d | %s", tk.codigo, tk.lexema);
    termo();
    printf("\n 30 %d | %s", tk.codigo, tk.lexema);
    if(tk.processado != 1)
    {
        tk = analise_lexica(fd);
        tk.processado = 0;
    }
    printf("\n 31 %d | %s", tk.codigo, tk.lexema);
    while((tk.cat == SN && (tk.codigo == ADICAO || tk.codigo == SUBTRACAO || tk.codigo == OR )) /*|| (tk.cat == ID)*/)
    {
        if(tk.cat == SN && (tk.codigo == ADICAO || tk.codigo == SUBTRACAO || tk.codigo == OR ))
        {
            tk = analise_lexica(fd);
        }
        printf("\n 32 %d | %s", tk.codigo, tk.lexema);
        termo();
        printf("\n 33 %d | %s", tk.codigo, tk.lexema);
        if(tk.processado != 1)
        {
            tk = analise_lexica(fd);
            tk.processado = 0;
        }

    }

}

// ======= TERMO =======
void termo(){

    fator();

    printf("\n 27 %d | %s", tk.codigo, tk.lexema);

    printf("\n 28 %d | %s", tk.codigo, tk.lexema);

    while((tk.cat == SN && (tk.codigo == MULTIPLICACAO || tk.codigo == DIVISAO || tk.codigo == AND)) /*|| (tk.cat == ID)*/)
    {
        if(tk.cat == SN && (tk.codigo == MULTIPLICACAO || tk.codigo == DIVISAO || tk.codigo == AND))
        {
            tk = analise_lexica(fd);
        }

        fator();
        //tk = analise_lexica(fd);
    }
    tk.processado = 1;
}

// ======= FATOR ======
void fator(){
    printf("\n 20 %d | %s", tk.codigo, tk.lexema);
    if(tk.cat == ID)
    {
        tk = analise_lexica(fd);

        printf("\n 21 %d | %s", tk.codigo, tk.lexema);

        // SUPER WHILE
        while(tk.cat != CT_I && tk.cat != CT_R && tk.cat != CT_C && tk.codigo != ABRE_PAR && tk.codigo != FECHA_PAR &&
              tk.codigo != NEGACAO && tk.codigo != ADICAO && tk.codigo != SUBTRACAO && tk.codigo != MULTIPLICACAO && tk.codigo != DIVISAO &&
              tk.codigo != COMPARACAO && tk.codigo != DIFERENTE && tk.codigo != MENOR_OU_IGUAL && tk.codigo != MENOR_QUE &&
              tk.codigo != MAIOR_OU_IGUAL && tk.codigo != MAIOR_QUE && tk.codigo != AND && tk.codigo != OR && tk.codigo != VIRGULA &&
              strcmp(tk.lexema, "to") != 0 && strcmp(tk.lexema, "dt") == 0 && strcmp(tk.lexema, "by") == 0 )
        {
            printf("\n 22 %d | %s", tk.codigo, tk.lexema);
            if(tk.cat == SN && tk.codigo == ABRE_COLCHETES)
            {
                printf("\n 23 %d | %s", tk.codigo, tk.lexema);
                tk = analise_lexica(fd);

                printf("\n 24 %d | %s", tk.codigo, tk.lexema);
                expr();

                printf("\n 25 %d | %s", tk.codigo, tk.lexema);

                if(tk.processado != 1)
                {
                    tk = analise_lexica(fd);
                    tk.processado = 0;
                }

                printf("\n 26 %d | %s", tk.codigo, tk.lexema);

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
    }
    else if(tk.cat == CT_I)
    {
        tk = analise_lexica(fd);
    }
    else if(tk.cat == CT_R)
    {
        tk = analise_lexica(fd);
    }
    else if(tk.cat == CT_C)
    {
        tk = analise_lexica(fd);
    }
    else if(tk.cat == SN && tk.codigo == ABRE_PAR)
    {
        printf("\n 40 %d | %s", tk.codigo, tk.lexema);
        tk = analise_lexica(fd);
        printf("\n 41 %d | %s", tk.codigo, tk.lexema);
        expr();
        printf("\n 42 %d | %s", tk.codigo, tk.lexema);
        //tk = analise_lexica(fd);
        printf("\n 43 %d | %s", tk.codigo, tk.lexema);
        if(tk.cat == SN && tk.codigo == FECHA_PAR)
        {
            tk = analise_lexica(fd); // CONFERIR
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
