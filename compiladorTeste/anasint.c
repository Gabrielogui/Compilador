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

// |=======| EXPRESSÃO |=======|
EXPRESSOES expressoes;

// |=======| ENDERECO |=======|
int qtdEndereco = 0;

int var_virg_aux = 0;
int flag_endi = 0;
int boolFlag = 0;

// |=======| FUNÇÃO MÁQUINA DE PILHA |=======|

char *gerarRotulo(){
    static char label[8];
    snprintf(label, sizeof(label), "L%d", qtdRotulo);
    qtdRotulo++;
    printf("\n%s", label);
    system("pause");
    return label;
}

// |=======| FUNÇÃO TABELA DE SÍMBOLOS |=======|

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

void excluirTabelaDeSimbolos(){
    // TIRANDO O TOPO VAZIO
    memset(&ts.Linhas[ts.topo], 0, sizeof(REG_TS));
    ts.topo--;
    // limpando a tabela de simbolos
    while(ts.Linhas[ts.topo].categoria == VAR_LOCAL && ts.Linhas[ts.topo].escopo == LOCAL)
    {
        memset(&ts.Linhas[ts.topo], 0, sizeof(REG_TS));
        ts.topo--;
    }

    int topo_auxx = ts.topo;
    while(ts.Linhas[topo_auxx].zumbi == VIVO)
    {
        ts.Linhas[topo_auxx].zumbi = ZUMBI;
        topo_auxx--;
    }

    ts.topo++;
    tk.processado = 0; // PEGANDO O PRÓXIMO TOKEN, QUE PODE SER O ENDFILE
}

void mostrarTabelaDeSimbolos(){
    int topoAux = ts.topo;
    //system("cls||clear");

    printf("\n |================================================ TABELA DE SIMBOLOS ================================================|");
    for( topoAux ; topoAux >= 0 ; topoAux--){

        printf("\n | POSICAO  : %d", topoAux                     );
        printf("\n | LEXEMA   : %s", ts.Linhas[topoAux].lexema   );
        printf("\n | ESCOPO   : %d", ts.Linhas[topoAux].escopo   );
        printf("\n | TIPO     : %d", ts.Linhas[topoAux].tipo     );
        printf("\n | CATEGORIA: %d", ts.Linhas[topoAux].categoria);
        printf("\n | PASSAGEM : %d", ts.Linhas[topoAux].passagem );
        printf("\n | ZUMBI    : %d", ts.Linhas[topoAux].zumbi    );
        printf("\n | ARRAY    : %d", ts.Linhas[topoAux].isArray  );
        printf("\n | DIM01    : %d", ts.Linhas[topoAux].dim01    );
        printf("\n | DIM02    : %d", ts.Linhas[topoAux].dim02    );
        printf("\n | EH_CONST : %d", ts.Linhas[topoAux].eh_const );
        printf("\n | ROTULO   : %s", ts.Linhas[topoAux].rotulo   );
        printf("\n | ENDERECO : %d", ts.Linhas[topoAux].endereco );

        printf("\n |--------------------------------------------------------------------------------------------------------------------|");
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
   // printf("\n%d", contLinhas);
   // printf("\n%s", tk.lexema);

    //mostrarTabelaDeSimbolos();

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
            int consulta = consultaTabelaDeSimbolos(tk.lexema);
            if((consulta != -1) && (ts.Linhas[consulta].categoria == PROTOTIPO) ){
                error("Esse identificador de prototipo ja foi utilizado! ");
            }
            if((consulta != -1) && (ts.Linhas[consulta].categoria == PROCEDIMENTO)){
                error("Esse prototipo tem que ser declarado antes do procedimento! ");
            }


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

                ts.Linhas[ts.topo].zumbi = NA_ZUMBI;
                ts.Linhas[ts.topo].eh_const = NAO;
                ts.Linhas[ts.topo].escopo = LOCAL;
                ts.Linhas[ts.topo].categoria = PAR_PROCEDIMENTO;
                tk = analise_lexica(fd);
            }
            else if(strcmp(tk.lexema, "real") == 0)
            {
                ts.Linhas[ts.topo].tipo = REAL_TIPO;

                ts.Linhas[ts.topo].zumbi = NA_ZUMBI;
                ts.Linhas[ts.topo].eh_const = NAO;
                ts.Linhas[ts.topo].escopo = LOCAL;
                ts.Linhas[ts.topo].categoria = PAR_PROCEDIMENTO;
                tk = analise_lexica(fd);
            }
            else if(strcmp(tk.lexema, "char") == 0)
            {
                ts.Linhas[ts.topo].tipo = CHAR_TIPO;

                ts.Linhas[ts.topo].zumbi = NA_ZUMBI;
                ts.Linhas[ts.topo].eh_const = NAO;
                ts.Linhas[ts.topo].escopo = LOCAL;
                ts.Linhas[ts.topo].categoria = PAR_PROCEDIMENTO;
                tk = analise_lexica(fd);
            }
            else if(strcmp(tk.lexema, "bool") == 0)
            {
                ts.Linhas[ts.topo].tipo = BOOL_TIPO;

                ts.Linhas[ts.topo].zumbi = NA_ZUMBI;
                ts.Linhas[ts.topo].eh_const = NAO;
                ts.Linhas[ts.topo].escopo = LOCAL;
                ts.Linhas[ts.topo].categoria = PAR_PROCEDIMENTO;
                tk = analise_lexica(fd);
            }
            else if(tk.cat == SN && tk.codigo == FECHA_PAR) saida_aux = 1;
            else
            {
                error("tipo esperado!");
            }

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
                if(saida_aux == 1) pass;
                else{
                    ts.Linhas[ts.topo].isArray = ESCALAR;
                    ts.topo++;
                }

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
        strcpy(ts.Linhas[ts.topo].rotulo, gerarRotulo());

        tk = analise_lexica(fd);
        if(tk.cat == PVR && strcmp(tk.lexema, "init") == 0)
        {
            int consulta = consultaTabelaDeSimbolos(tk.lexema);
            if(consulta != -1){
                error("O init ja foi declarado! ");
            }
            strcpy(ts.Linhas[ts.topo].lexema, tk.lexema);

            ts.topo++;
            tk = analise_lexica(fd);
        }
        else if(tk.cat == ID)
        {

            int defFlag = 0;
            int consulta = consultaTabelaDeSimbolos(tk.lexema);
            if((consulta != -1) && (ts.Linhas[consulta].categoria == PROCEDIMENTO)){
                error("Esse identificador de procedimento ja foi utilizado! ");
            }

            if((consulta != -1) && (ts.Linhas[consulta].categoria == PROTOTIPO)){
                defFlag = 1;
            }

            strcpy(ts.Linhas[ts.topo].lexema, tk.lexema);
            ts.topo++;
            tk = analise_lexica(fd);
            if(tk.cat == SN && tk.codigo == ABRE_PAR)
            {
                int i = 1;
                do{

                    tk = analise_lexica(fd);

                    if(tk.cat == SN && tk.codigo == E_COMERCIAL)
                    {
                        if(defFlag == 1 && (ts.Linhas[consulta + i].passagem != REFERENCIA)){
                            error("Precisa ser passado por referencia! ");
                        }
                        ts.Linhas[ts.topo].passagem = REFERENCIA;
                        tk = analise_lexica(fd);
                    }
                    else if((verificarTipo(tk.lexema)) == 0)
                    {
                        ts.Linhas[ts.topo].passagem = COPIA;
                    }
                    else if(tk.cat == SN && tk.codigo == FECHA_PAR)
                    {
                        if(defFlag == 1 && ts.Linhas[consulta + 1].categoria == PAR_PROCEDIMENTO){
                            error("Procedimento precisa ter parametros! ");
                        }
                        break;
                    }

                    if(strcmp(tk.lexema, "int") == 0)
                    {

                        if(defFlag == 1){
                            if(ts.Linhas[consulta + i].categoria == PAR_PROCEDIMENTO){
                                if(ts.Linhas[consulta + i].tipo != INT_TIPO){
                                    error("Tipo incompativel 1! ");
                                }
                            }else{
                                error("Nao tem parametros suficiente! ");
                            }
                        }
                        ts.Linhas[ts.topo].tipo = INT_TIPO;
                    }
                    else if(strcmp(tk.lexema, "real") == 0)
                    {
                        if(defFlag == 1){
                            if(ts.Linhas[consulta + i].categoria == PAR_PROCEDIMENTO){
                                if(ts.Linhas[consulta + i].tipo != REAL_TIPO){
                                    error("Tipo incompativel 1! ");
                                }
                            }else{
                                error("Nao tem parametros suficiente! ");
                            }
                        }
                        ts.Linhas[ts.topo].tipo = REAL_TIPO;
                    }
                    else if(strcmp(tk.lexema, "char") == 0)
                    {
                        if(defFlag == 1){
                            if(ts.Linhas[consulta + i].categoria == PAR_PROCEDIMENTO){
                                if(ts.Linhas[consulta + i].tipo != CHAR_TIPO){
                                    error("Tipo incompativel 1! ");
                                }
                            }else{
                                error("Nao tem parametros suficiente! ");
                            }
                        }
                        ts.Linhas[ts.topo].tipo = CHAR_TIPO;
                    }
                    else if(strcmp(tk.lexema, "bool") == 0)
                    {
                        if(defFlag == 1){
                            if(ts.Linhas[consulta + i].categoria == PAR_PROCEDIMENTO){
                                if(ts.Linhas[consulta + i].tipo != BOOL_TIPO){
                                    error("Tipo incompativel 1! ");
                                }
                            }else{
                                error("Nao tem parametros suficiente! ");
                            }
                        }
                        ts.Linhas[ts.topo].tipo = BOOL_TIPO;
                    }
                    else
                    {
                        error("tipo esperado!");
                    }
                    ts.Linhas[ts.topo].zumbi     = VIVO;
                    ts.Linhas[ts.topo].eh_const  = NAO;
                    ts.Linhas[ts.topo].escopo    = LOCAL;
                    ts.Linhas[ts.topo].categoria = PAR_PROCEDIMENTO;
                    tk = analise_lexica(fd);
                    if(tk.cat == ID)
                    {

                        if(consultaTabelaDeSimbolos(tk.lexema) != -1 &&
                           (ts.Linhas[consultaTabelaDeSimbolos(tk.lexema)].categoria == PAR_PROCEDIMENTO &&
                            ts.Linhas[consultaTabelaDeSimbolos(tk.lexema)].zumbi == VIVO)){
                            error("Identificador de parametro ja usado! ");
                        }

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
                    i++;
                }while(tk.cat == SN && tk.codigo == VIRGULA);
                if(defFlag == 1){
                    if(ts.Linhas[consulta + i].categoria == PAR_PROCEDIMENTO){
                        error("O prototipo tem mais parametros! ");
                    }
                }
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
           // printf("\n 1 %d | %s", tk.codigo, tk.lexema);
            ts.Linhas[ts.topo].escopo    = LOCAL;
            ts.Linhas[ts.topo].categoria = VAR_LOCAL;
            ts.Linhas[ts.topo].passagem  = NA_PASSAGEM;
            ts.Linhas[ts.topo].zumbi     = NA_ZUMBI;
            decl_list_var();
            //printf("\n 2 %d | %s", tk.codigo, tk.lexema);
            if(tk.processado != 1)
            {
                tk = analise_lexica(fd);
                tk.processado = 0;
            }
            //printf("\n 3 %d | %s", tk.codigo, tk.lexema);
        }

       // printf("\n 4 %d | %s", tk.codigo, tk.lexema);

        while(strcmp(tk.lexema, "endp") != 0) // verificar depois se esta certo
        {
            //printf("\n 5 %d | %s", tk.codigo, tk.lexema);
            cmd();
           // printf("\n 6 %d | %s", tk.codigo, tk.lexema);
            if(tk.processado != 1)
            {
                tk = analise_lexica(fd);
                tk.processado = 0;
            }
            //printf("\n 7 %d | %s", tk.codigo, tk.lexema);
        }



      //  printf("\n 8 %d | %s", tk.codigo, tk.lexema);
        // fazer função cmd



        if(tk.cat == PVR && strcmp(tk.lexema, "endp") == 0)
        {

            excluirTabelaDeSimbolos();

        }
        else
        {
            error("endp esperado!");
        }
    }
}

// ======= DECL_LIST_VAR =======
void decl_list_var(){
    int constVirgula;

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

        if(ts.Linhas[ts.topo].escopo == GLOBAL){
            constVirgula = ts.Linhas[ts.topo].eh_const;
            printf("\n lexema: %s , tipo: %d %d", ts.Linhas[ts.topo].lexema, ts.Linhas[ts.topo].tipo, constVirgula);
            ts.topo++;

            ts.Linhas[ts.topo].escopo    = GLOBAL;
            ts.Linhas[ts.topo].categoria = VAR_GLOBAL;
            ts.Linhas[ts.topo].passagem  = NA_PASSAGEM;
            ts.Linhas[ts.topo].zumbi     = NA_ZUMBI;
            ts.Linhas[ts.topo].eh_const  = constVirgula;

            //printf("\n tipo: %d ", ts.Linhas[ts.topo].tipo);
        }else{
            constVirgula = ts.Linhas[ts.topo].eh_const;
            ts.topo++;

            ts.Linhas[ts.topo].escopo    = LOCAL;
            ts.Linhas[ts.topo].categoria = VAR_LOCAL;
            ts.Linhas[ts.topo].passagem  = NA_PASSAGEM;
            ts.Linhas[ts.topo].zumbi     = NA_ZUMBI;
            ts.Linhas[ts.topo].eh_const  = constVirgula;
        }

        decl_var();
    }
    if (!(tk.cat == SN && tk.codigo == VIRGULA))
    {
        ts.topo++;
        tk.processado = 1;
    }
}

// ======= DECL_VAR =======
void decl_var(){


    // DECLARA��O DE VARI�VEIS:
    if(var_virg_aux == 1){
        int topoAux_virg = ts.topo - 1; // CUIDADO COM ISSO
        ts.Linhas[ts.topo].tipo = ts.Linhas[topoAux_virg].tipo;
        var_virg_aux = 0;
    }else if(strcmp(tk.lexema, "int") == 0){
        ts.Linhas[ts.topo].tipo = INT_TIPO;
        tk = analise_lexica(fd);
    }else if(strcmp(tk.lexema, "real") == 0){
        //printf("\n tk.lexema de real: ", tk.lexema);
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

    int consulta = consultaTabelaDeSimbolos(tk.lexema);
    if(consulta == -1) pass;
    else{
        if(ts.Linhas[consulta].escopo == ts.Linhas[ts.topo].escopo && ts.Linhas[consulta].zumbi == VIVO){
            error("Variavel com esse nome ja foi declarada! ");
        }else if(ts.Linhas[consulta].zumbi == VIVO) error("Parametro ja declarado! ");

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

        ts.Linhas[ts.topo].isArray = ESCALAR;
    }

    //ts.topo++; // INCREMENTA��O DO TOPO


}

// ======= CMD =======
void cmd()
{
  //  printf("\n 9 %d | %s", tk.codigo, tk.lexema);
    if(tk.cat == PVR && strcmp(tk.lexema, "do") == 0) // VOLTAR PARA REVER A PORRA DA VÍRGULA
    {
        int flag_virgula = 0;

        tk = analise_lexica(fd);

        if(tk.cat == ID)
        {
            int consulta = consultaTabelaDeSimbolos(tk.lexema);

            if(consulta == -1){
                error("Identificador nao foi declarado antes");
            }
            if(ts.Linhas[consulta].categoria != PROCEDIMENTO && ts.Linhas[consulta].categoria != PROTOTIPO){
                error("Identificador nao eh um procedimento ou prototipo");
            }

            tk = analise_lexica(fd);

            if(tk.cat == SN && tk.codigo == ABRE_PAR)
            {
                tk = analise_lexica(fd);
                int i = 1;
                do{

                    if(tk.cat == SN && tk.codigo == VIRGULA){
                        tk = analise_lexica(fd);
                        flag_virgula = 1;
                    }

                    if(tk.cat == SN && tk.codigo == FECHA_PAR){
                        break;
                    }

                    flag_virgula = 0;
                    expr();

                    if(ts.Linhas[consulta + i].tipo == INT_TIPO){
                        if(expressoes.expressao[expressoes.topo].tipoExpr == INT_EXPR || expressoes.expressao[expressoes.topo].tipoExpr == CHAR_EXPR) pass;
                        else error("Tipos invalidos! Parametro inteiro. Esperando inteiro ou char! ");
                    }else if(ts.Linhas[consulta + i].tipo == CHAR_TIPO){
                        if(expressoes.expressao[expressoes.topo].tipoExpr || expressoes.expressao[expressoes.topo].tipoExpr == CHAR_EXPR) pass;
                        else error("Tipos invalidos! Parametro char. Esperando inteiro ou char! ");
                    }else if(ts.Linhas[consulta + i].tipo == REAL_TIPO){
                        if(expressoes.expressao[expressoes.topo].tipoExpr == REAL_EXPR) pass;
                        else error("Tipos Invalidos! Paremetro Real. Esperando real");
                    }else if(ts.Linhas[consulta + i].tipo == BOOL_TIPO){
                        if(expressoes.expressao[expressoes.topo].tipoExpr || expressoes.expressao[expressoes.topo].tipoExpr == BOOL_EXPR) pass;
                        else error("Tipos Invalidos! BOOL so eh compativel com inteiro ou bool");
                    }else{
                        error("Precisa de menos argumentos! ");
                    }

                    if(tk.processado != 1)
                    {
                        tk = analise_lexica(fd);
                        tk.processado = 0;
                    }

                    i++;
                }while(tk.cat == SN && tk.codigo == VIRGULA);

                if(ts.Linhas[consulta + i].categoria == PAR_PROCEDIMENTO){
                    error("O procedimento tem mais parametros! ");
                }


                if(tk.cat == SN && tk.codigo == FECHA_PAR)
                {
                    if(flag_virgula == 1){
                        error("Virgula mal colocada! ");
                    }
                    tk.processado = 0;
                }
                else
                {
               //     printf("\n 18 %d | %s", tk.codigo, tk.lexema);
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

        tk = analise_lexica(fd);

        if(tk.cat == SN && tk.codigo == ABRE_PAR)
        {

            tk = analise_lexica(fd);

            expr();

            // EXPRESSAO DO WHILE PRECISA SER BOOL
            if(expressoes.expressao[expressoes.topo].tipoExpr == BOOL_TIPO) pass;
            else error("Expressao do WHILE precisa ser do tipo booleano! ");

            if(tk.processado != 1)
            {
                tk = analise_lexica(fd);
                tk.processado = 0;
            }

           // printf("\n 65 %d | %s", tk.codigo, tk.lexema);
            if(tk.cat == SN && tk.codigo == FECHA_PAR)
            {
                tk = analise_lexica(fd);
                if(tk.cat == PVR && strcmp(tk.lexema, "endw") == 0)
                {
            //        printf("\n 66 %d | %s", tk.codigo, tk.lexema);
                    pass; // TALVEZ ALGO AQUI
                }
                else
                {
             //       printf("\n 67 %d | %s", tk.codigo, tk.lexema);
                    while(strcmp(tk.lexema, "endw") != 0)
                    {
              //          printf("\n 68 %d | %s", tk.codigo, tk.lexema);
                        cmd();
              //          printf("\n 69 %d | %s", tk.codigo, tk.lexema);
                        if(tk.processado != 1)
                        {
                            tk = analise_lexica(fd);
                            tk.processado = 0;
                        }
                //        printf("\n 691 %d | %s", tk.codigo, tk.lexema);
                    }
                    if(strcmp(tk.lexema, "endw") != 0)
                    {
                        error("'endw' esperado!");
                    }
               //     printf("\n 692 %d | %s", tk.codigo, tk.lexema);
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
      //  printf("\n 90 %d | %s", tk.codigo, tk.lexema);
        tk = analise_lexica(fd);
      //  printf("\n 91 %d | %s", tk.codigo, tk.lexema);
        if(tk.cat == ID)
        {
            int consulta = consultaTabelaDeSimbolos(tk.lexema);
            if(consulta == -1){
                error("Identificador nao foi declarado antes");
            }
            if(ts.Linhas[consulta].categoria != VAR_LOCAL && ts.Linhas[consulta].categoria != VAR_GLOBAL &&
                ts.Linhas[consulta].categoria != PAR_PROCEDIMENTO){
                error("Identificador do var precisa ser uma variavel ou parametro! ");
            }

            tk = analise_lexica(fd);
        //    printf("\n 92 %d | %s", tk.codigo, tk.lexema);
            if(tk.cat == PVR && strcmp(tk.lexema, "from") == 0)
            {
         //       printf("\n 921 %d | %s", tk.codigo, tk.lexema);
                tk = analise_lexica(fd); // TALVEZ NAO TENHA
          //      printf("\n 93 %d | %s", tk.codigo, tk.lexema);
                expr();
          //      printf("\n 94 %d | %s", tk.codigo, tk.lexema);
                if(tk.processado != 1)
                {
                    tk = analise_lexica(fd);
                    tk.processado = 0;
                }
         //       printf("\n 95 %d | %s", tk.codigo, tk.lexema);
                if(tk.cat == PVR && ((strcmp(tk.lexema, "to") == 0) || (strcmp(tk.lexema, "dt") == 0)))
                {
           //         printf("\n 951 %d | %s", tk.codigo, tk.lexema);
                    tk = analise_lexica(fd); // TALVEZ NAO TENHA
           //         printf("\n 96 %d | %s", tk.codigo, tk.lexema);
                    expr();
            //        printf("\n 97 %d | %s", tk.codigo, tk.lexema);
                    if(tk.processado != 1)
                    {
                        tk = analise_lexica(fd);
                        tk.processado = 0;
                    }
            //        printf("\n 971 %d | %s", tk.codigo, tk.lexema);
                    if(tk.cat == PVR && strcmp(tk.lexema, "by") == 0)
                    {
             //           printf("\n 972 %d | %s", tk.codigo, tk.lexema);
                        tk = analise_lexica(fd);
             //           printf("\n 98 %d | %s", tk.codigo, tk.lexema);
                        if(tk.cat == CT_I || tk.cat == ID)
                        {
              //              printf("\n 981 %d | %s", tk.codigo, tk.lexema);
                            if(tk.cat == ID){
                          //      printf("\n 99 %d | %s", tk.codigo, tk.lexema);
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
                       //     printf("\n 100 %d | %s", tk.codigo, tk.lexema);
                            tk = analise_lexica(fd);
                        //    printf("\n 101 %d | %s", tk.codigo, tk.lexema);
                        }
                        else
                        {
                            error("inteiro ou identificador esperado!");
                        }
                    }
                    while(strcmp(tk.lexema, "endv") != 0)
                    {
                  //      printf("\n 102 %d | %s", tk.codigo, tk.lexema);
                        cmd();
                   //     printf("\n 103 %d | %s", tk.codigo, tk.lexema);
                        if(tk.processado != 1){
                            tk = analise_lexica(fd);
                            tk.processado = 0;
                        }
                 //       printf("\n 104 %d | %s", tk.codigo, tk.lexema);
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
        // TOKEN ESPERANDO O ABRE PARÊNTESE
        tk = analise_lexica(fd);
        if(tk.cat == SN && tk.codigo == ABRE_PAR) pass;
        else error("Abre Parentese - '(' Esperado no if! ");

        tk = analise_lexica(fd);
        expr();

        // EXPRESSAO DO IF PRECISA SER BOOL
        if(expressoes.expressao[expressoes.topo].tipoExpr == BOOL_TIPO) pass;
        else error("Expressao do IF precisa ser do tipo booleano! ");

        // TOKEN ESPERANDO O FECHA PARÊNTESE - APÓS 'expr()'
        if(tk.cat == SN && tk.codigo == FECHA_PAR) tk.processado = 0;
        else error("Fecha Parentese - ')' Esperado no if! ");

        // LAÇO DO CMD '{cmd}'
        while(1){
            // CONFERINDO SE ELE SAI DE CMD PROCESSADO (SE PEGOU O PRÓXIMO APÓS CMD)
            if(tk.processado != 1){
                tk = analise_lexica(fd);
                tk.processado = 0;
            }
            // CONDIÇÕES PARA SAIR DO LAÇO
            if(tk.cat == PVR && ((strcmp(tk.lexema, "elif") == 0) || (strcmp(tk.lexema, "else") == 0) || (strcmp(tk.lexema, "endi") == 0))) break;

            cmd();
        }

        // LAÇO DO ELIF
        while(tk.cat == PVR && strcmp(tk.lexema, "elif") == 0){

            // TOKEN ESPERANDO O ABRE PARÊNTESE
            tk = analise_lexica(fd);
            if(tk.cat == SN && tk.codigo == ABRE_PAR) pass;
            else error("Abre Parentese - '(' Esperado no elif! ");

            tk = analise_lexica(fd);
            expr();

            // EXPRESSAO DO ELIF PRECISA SER BOOL
            if(expressoes.expressao[expressoes.topo].tipoExpr == BOOL_TIPO) pass;
            else error("Expressao do ELIF precisa ser do tipo booleano! ");

            // TOKEN ESPERANDO O FECHA PARÊNTESE - APÓS 'expr()'
            if(tk.cat == SN && tk.codigo == FECHA_PAR) tk.processado = 0;
            else error("Fecha Parentese - ')' Esperado no elif! ");

            while(1){
            // CONFERINDO SE ELE SAI DE CMD PROCESSADO (SE PEGOU O PRÓXIMO APÓS CMD)
            if(tk.processado != 1){
                tk = analise_lexica(fd);
                tk.processado = 0;
            }

            // CONDIÇÕES PARA SAIR DO LAÇO
            if(tk.cat == PVR && ((strcmp(tk.lexema, "elif") == 0) || (strcmp(tk.lexema, "else") == 0) || (strcmp(tk.lexema, "endi") == 0))) break;

            cmd();

            }

        }

        // ELSE
        if(tk.cat == PVR && strcmp(tk.lexema, "else") == 0){ // if(expr) {cmd} {elif(expr) {cmd} } [else {cmd}] endi

            tk.processado = 0;
            // LAÇO DO CMD '{cmd}'
            while(1){
                // CONFERINDO SE ELE SAI DE CMD PROCESSADO (SE PEGOU O PRÓXIMO APÓS CMD)
                if(tk.processado != 1){
                    tk = analise_lexica(fd);
                    tk.processado = 0;
                }

                // CONDIÇÕES PARA SAIR DO LAÇO
                if(tk.cat == PVR && (strcmp(tk.lexema, "endi") == 0)) break;

                cmd();

            }
        }

        if(tk.cat == PVR && strcmp(tk.lexema, "endi") == 0) tk.processado = 0;
        else error("'endi' Esperado! ");

        // NÃO PRECISA PEGAR PRÓXIMO -> COLOCAR TK.PROCESSADO = 0
    }
    else if(tk.cat == ID)
    {
        int consulta = consultaTabelaDeSimbolos(tk.lexema);
        if(consulta == -1){
            error("Identificador nao foi declarado antes");
        }
        if(ts.Linhas[consulta].categoria != VAR_LOCAL && ts.Linhas[consulta].categoria != VAR_GLOBAL &&
            ts.Linhas[consulta].categoria != PAR_PROCEDIMENTO){
            error("Identificador do var precisa ser uma variavel ou parametro! ");
        }
        atrib(consulta);
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
void atrib(int consulta)
{

    tk = analise_lexica(fd);

    while(tk.codigo != ATRIBUICAO)
    {

        if(tk.cat == SN && tk.codigo == ABRE_COLCHETES)
        {

            tk = analise_lexica(fd);

            expr();
            if(expressoes.expressao[expressoes.topo].tipoExpr != INT_EXPR) error("Precisa ser inteiro entre []! ");

            if(tk.processado != 1)
            {
                tk = analise_lexica(fd);
                tk.processado = 0;
            }

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

        tk = analise_lexica(fd);

        expr();

        if(ts.Linhas[consulta].tipo == INT_TIPO){
            if(expressoes.expressao[expressoes.topo].tipoExpr == INT_EXPR ){
                 expressoes.expressao[expressoes.topo].tipoExpr = INT_EXPR;
            }else if(expressoes.expressao[expressoes.topo].tipoExpr == CHAR_EXPR){
                 expressoes.expressao[expressoes.topo].tipoExpr = INT_EXPR;
            }
            else error("Tipos invalidos! Int so eh valido com int ou char! ");
        }else if(ts.Linhas[consulta].tipo == CHAR_TIPO){
            if(expressoes.expressao[expressoes.topo].tipoExpr == INT_EXPR){
                expressoes.expressao[expressoes.topo].tipoExpr = INT_EXPR;
            }else if(expressoes.expressao[expressoes.topo].tipoExpr == CHAR_EXPR){
                expressoes.expressao[expressoes.topo].tipoExpr = CHAR_EXPR;
            }
            else error("Tipos invalidos! Char so eh valido com int ou char! ");
        }else if(ts.Linhas[consulta].tipo == REAL_TIPO){
            if(expressoes.expressao[expressoes.topo].tipoExpr == REAL_EXPR) expressoes.expressao[expressoes.topo].tipoExpr = REAL_EXPR;
            else error("Tipos Invalidos! Real so eh compativel com real");
        }else if(ts.Linhas[consulta].tipo == BOOL_TIPO){
            if(expressoes.expressao[expressoes.topo].tipoExpr == INT_EXPR || expressoes.expressao[expressoes.topo].tipoExpr == BOOL_EXPR){
                expressoes.expressao[expressoes.topo].tipoExpr = BOOL_EXPR;
            }
            else error("Tipos Invalidos! BOOL so eh compativel com inteiro ou bool");

            // CONFERIR SE EH 0 = FALSO ; DIFERENTE DE 0 = VERDADEIRO;
        }

        // APAGAR


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

    if(tk.processado != 1)
    {
        tk = analise_lexica(fd);
        tk.processado = 0;
    }

    if(tk.cat == SN && (tk.codigo == COMPARACAO || tk.codigo == DIFERENTE || tk.codigo == MENOR_OU_IGUAL
    || tk.codigo == MENOR_QUE || tk.codigo == MAIOR_OU_IGUAL || tk.codigo == MAIOR_QUE))
    {
        boolFlag = 1;
        tk = analise_lexica(fd);

        expr_simp();

        if(expressoes.expressao[expressoes.topo].tipoExpr == INT_EXPR){
            if(expressoes.expressao[expressoes.topo - 1].tipoExpr == INT_EXPR || expressoes.expressao[expressoes.topo - 1].tipoExpr == CHAR_EXPR) pass;
            else error("Tipo Invalido na expr! Int so aceita int ou char! "); // 2 * 3 * 3.2 ->
        }else if(expressoes.expressao[expressoes.topo].tipoExpr == CHAR_EXPR){
            if(expressoes.expressao[expressoes.topo - 1].tipoExpr == INT_EXPR || expressoes.expressao[expressoes.topo - 1].tipoExpr == CHAR_EXPR) pass;
            else error("Tipo Invalido na expr! Char so aceita int ou char! ");
        }else if(expressoes.expressao[expressoes.topo].tipoExpr == REAL_EXPR){
            if(expressoes.expressao[expressoes.topo - 1].tipoExpr == REAL_EXPR) pass;
            else error("Tipo Invalido na expr! Real so aceita real! ");
        }else if(expressoes.expressao[expressoes.topo].tipoExpr == BOOL_EXPR){
            if(expressoes.expressao[expressoes.topo - 1].tipoExpr == INT_EXPR || expressoes.expressao[expressoes.topo - 1].tipoExpr == BOOL_EXPR) pass;
            else error("Tipo Invalido na expr! bool so aceita int! ");
        }
    }

    if(boolFlag == 1){
        expressoes.topo++;
        expressoes.expressao[expressoes.topo].tipoExpr = BOOL_EXPR;

        boolFlag = 0;
    }

}

// ======= EXPR_SIMP =======
void expr_simp(){
    if(tk.cat == SN && (tk.codigo == ADICAO || tk.codigo == SUBTRACAO))
    {
        tk = analise_lexica(fd);
    }

    termo();

    if(tk.processado != 1)
    {
        tk = analise_lexica(fd);
        tk.processado = 0;
    }

    while((tk.cat == SN && (tk.codigo == ADICAO || tk.codigo == SUBTRACAO || tk.codigo == OR )) /*|| (tk.cat == ID)*/)
    {
        if(tk.cat == SN && (tk.codigo == ADICAO || tk.codigo == SUBTRACAO || tk.codigo == OR ))
        {
            if(tk.cat == SN && tk.codigo == OR) boolFlag = 1; // a + b && c - d || e * f
            tk = analise_lexica(fd);
        }



        termo();

        // a > b + 2 - 'x'

        if(expressoes.expressao[expressoes.topo].tipoExpr == INT_EXPR){
            if(expressoes.expressao[expressoes.topo - 1].tipoExpr == INT_EXPR || expressoes.expressao[expressoes.topo - 1].tipoExpr == CHAR_EXPR) pass;
            else error("Tipo Invalido na expr! Int so aceita int ou char! "); // 2 * 3 * 3.2 ->
        }else if(expressoes.expressao[expressoes.topo].tipoExpr == CHAR_EXPR){
            if(expressoes.expressao[expressoes.topo - 1].tipoExpr == INT_EXPR || expressoes.expressao[expressoes.topo - 1].tipoExpr == CHAR_EXPR) pass;
            else error("Tipo Invalido na expr! Char so aceita int ou char! ");
        }else if(expressoes.expressao[expressoes.topo].tipoExpr == REAL_EXPR){
            if(expressoes.expressao[expressoes.topo - 1].tipoExpr == REAL_EXPR) pass;
            else error("Tipo Invalido na expr! Real so aceita real! ");
        }else if(expressoes.expressao[expressoes.topo].tipoExpr == BOOL_EXPR){
            if(expressoes.expressao[expressoes.topo - 1].tipoExpr == INT_EXPR || expressoes.expressao[expressoes.topo - 1].tipoExpr == BOOL_EXPR){
                expressoes.topo++;
                expressoes.expressao[expressoes.topo].tipoExpr = BOOL_EXPR;
            }else error("Tipo Invalido na expr! bool so aceita int! ");
        }

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

    while((tk.cat == SN && (tk.codigo == MULTIPLICACAO || tk.codigo == DIVISAO || tk.codigo == AND)) /*|| (tk.cat == ID)*/)
    {
        if(tk.cat == SN && (tk.codigo == MULTIPLICACAO || tk.codigo == DIVISAO || tk.codigo == AND))
        {
            if(tk.cat == SN && tk.codigo == AND) boolFlag = 1;
            tk = analise_lexica(fd);
        }

        fator();

        if(expressoes.expressao[expressoes.topo].tipoExpr == INT_EXPR){
            if(expressoes.expressao[expressoes.topo - 1].tipoExpr == INT_EXPR || expressoes.expressao[expressoes.topo - 1].tipoExpr == CHAR_EXPR) pass;
            else error("Tipo Invalido na expr! Int so aceita int ou char! "); // 2 * 3 * 3.2 ->
        }else if(expressoes.expressao[expressoes.topo].tipoExpr == CHAR_EXPR){
            if(expressoes.expressao[expressoes.topo - 1].tipoExpr == INT_EXPR || expressoes.expressao[expressoes.topo - 1].tipoExpr == CHAR_EXPR) pass;
            else error("Tipo Invalido na expr! Char so aceita int ou char! ");
        }else if(expressoes.expressao[expressoes.topo].tipoExpr == REAL_EXPR){
            if(expressoes.expressao[expressoes.topo - 1].tipoExpr == REAL_EXPR) pass;
            else error("Tipo Invalido na expr! Real so aceita real! ");
        }else if(expressoes.expressao[expressoes.topo].tipoExpr == BOOL_EXPR){
            if(expressoes.expressao[expressoes.topo - 1].tipoExpr == INT_EXPR || expressoes.expressao[expressoes.topo - 1].tipoExpr == BOOL_EXPR) pass;
            else error("Tipo Invalido na expr! bool so aceita int! ");
        }

        //expressoes.topo--; // DESTRUIR



    }



    tk.processado = 1;
}

// ======= FATOR ======
void fator(){
    if(tk.cat == ID)
    {
        int consulta = consultaTabelaDeSimbolos(tk.lexema);
        // PROCEDIMENTO NÃO PODE FAZER PARTE DE UMA EXPRESSÃO
        if(consulta == -1) error("Identificador nao declarado! ");

        if((consulta != -1) && (ts.Linhas[consulta].categoria == PROCEDIMENTO || ts.Linhas[consulta].categoria == PROTOTIPO)){
            error("Procedimento ou prototipo nao podem ser passados como expressao! ");
        }

        expressoes.topo++;
        expressoes.expressao[expressoes.topo].tipoExpr = ts.Linhas[consulta].tipo;

        tk = analise_lexica(fd);


        // SUPER WHILE
        while(tk.cat != CT_I && tk.cat != CT_R && tk.cat != CT_C && tk.codigo != ABRE_PAR && tk.codigo != FECHA_PAR &&
              tk.codigo != NEGACAO && tk.codigo != ADICAO && tk.codigo != SUBTRACAO && tk.codigo != MULTIPLICACAO && tk.codigo != DIVISAO &&
              tk.codigo != COMPARACAO && tk.codigo != DIFERENTE && tk.codigo != MENOR_OU_IGUAL && tk.codigo != MENOR_QUE &&
              tk.codigo != MAIOR_OU_IGUAL && tk.codigo != MAIOR_QUE && tk.codigo != AND && tk.codigo != OR && tk.codigo != VIRGULA &&
              strcmp(tk.lexema, "to") != 0 && strcmp(tk.lexema, "dt") == 0 && strcmp(tk.lexema, "by") == 0 )
        {

            if(tk.cat == SN && tk.codigo == ABRE_COLCHETES)
            {

                tk = analise_lexica(fd);

                expr();

                if(tk.processado != 1)
                {
                    tk = analise_lexica(fd);
                    tk.processado = 0;
                }

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
        expressoes.topo++;
        expressoes.expressao[expressoes.topo].tipoExpr = INT_EXPR;
        tk = analise_lexica(fd);
    }
    else if(tk.cat == CT_R)
    {
        expressoes.topo++;
        expressoes.expressao[expressoes.topo].tipoExpr = REAL_EXPR;
        tk = analise_lexica(fd);
    }
    else if(tk.cat == CT_C)
    {
        expressoes.topo++;
        expressoes.expressao[expressoes.topo].tipoExpr = CHAR_EXPR;
        tk = analise_lexica(fd);
    }
    else if(tk.cat == SN && tk.codigo == ABRE_PAR)
    {

        tk = analise_lexica(fd);

        expr();

        //tk = analise_lexica(fd);
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
        expressoes.topo++;
        expressoes.expressao[expressoes.topo].tipoExpr = BOOL_EXPR;
        boolFlag = 1;

        tk = analise_lexica(fd);

        fator();
    }
    else
    {
        error("fator invalido!");
    }


}
