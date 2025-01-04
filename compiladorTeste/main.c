#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.h"
#include "anasint.h"

// |=======| VARIAVEIS GLOBAIS |=======|
FILE *fd;
TOKEN tk;
contLinhas = 1;
lido = 0;
qtdRotulo = 0;
FILE *mp;

// |=======| DECLARA��O DAS FUN��ES |=======|
void testarAnalex();
void testarAnasint();

// |=======| MAIN |=======|
int main()
{
    /*printf("\n |=======| TESTAR ANALISE LEXICA |=======|");
    testarAnalex();*/

    printf("\n |=======| TESTAR ANALISE SINTATICA |=======|");
    testarAnasint();

    return 0;
}

// |=======| FUN��O DO TESTE DA AN�LISE L�XICA |=======|
void testarAnalex(){

    if((fd = fopen("codigo.txt", "r")) == NULL){
        printf("\n[ERRO] - ERRO ao abrir o arquivo\n");
    }

    while(1){

        tk = analise_lexica(fd);

        switch(tk.cat){
            case PVR:
                printf("\n <PVR, %s >", tk.lexema);
                break;
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
                printf("\n <CT_C, %c >", tk.c);
                break;
            case CT_S:
                printf("\n <CT_S, %s >", tk.lexema);
                break;
            case SN: // COLOCAR OS SINAIS
                switch(tk.codigo){
                    case ADICAO:
                        printf("\n <SN , ADICAO - %d>", tk.codigo);
                        break;
                    case SUBTRACAO:
                        printf("\n <SN , SUBTRACAO - %d>", tk.codigo);
                        break;
                    case MULTIPLICACAO:
                        printf("\n <SN , MULTIPLICACAO - %d>", tk.codigo);
                        break;
                    case DIVISAO:
                        printf("\n <SN , DIVISAO - %d>", tk.codigo);
                        break;
                    case ABRE_PAR:
                        printf("\n <SN , ABRE_PAR - %d>", tk.codigo);
                        break;
                    case FECHA_PAR:
                        printf("\n <SN , FECHA_PAR - %d>", tk.codigo);
                        break;
                    case ABRE_COLCHETES:
                        printf("\n <SN , ABRE_COLCHETES - %d>", tk.codigo);
                        break;
                    case FECHA_COLCHETES:
                        printf("\n <SN , FECHA_COLCHETES - %d>", tk.codigo);
                        break;
                    case ABRE_CHAVES:
                        printf("\n <SN , ABRE_CHAVES - %d>", tk.codigo);
                        break;
                    case FECHA_CHAVES:
                        printf("\n <SN , FECHA_CHAVES - %d>", tk.codigo);
                        break;
                    case VIRGULA:
                        printf("\n <SN , VIRGULA - %d>", tk.codigo);
                        break;
                    case MAIOR_OU_IGUAL:
                        printf("\n <SN , MAIOR_OU_IGUAL - %d>", tk.codigo);
                        break;
                    case MAIOR_QUE:
                        printf("\n <SN , MAIOR_QUE - %d>", tk.codigo);
                        break;
                    case MENOR_OU_IGUAL:
                        printf("\n <SN , MENOR_OU_IGUAL - %d>", tk.codigo);
                        break;
                    case MENOR_QUE:
                        printf("\n <SN , MENOR_QUE - %d>", tk.codigo);
                        break;
                    case ATRIBUICAO:
                        printf("\n <SN , ATRIBUICAO - %d>", tk.codigo);
                        break;
                    case COMPARACAO:
                        printf("\n <SN , COMPARACAO  - %d>", tk.codigo);
                        break;
                    case NEGACAO:
                        printf("\n <SN , NEGACAO - %d>", tk.codigo);
                        break;
                    case DIFERENTE:
                        printf("\n <SN , DIFERENTE - %d>", tk.codigo);
                        break;
                    case E_COMERCIAL:
                        printf("\n <SN , E_COMERCIAL - %d>", tk.codigo);
                        break;
                    case AND:
                        printf("\n <SN , AND - %d>", tk.codigo);
                        break;
                    case OR:
                        printf("\n <SN , OR - %d>", tk.codigo);
                        break;
                    default:
                        printf("\n[ERRO] - Erro ao ler o estado");
                        break;
                }
                break;
            case FIM_ARQ:
                printf("\nArquivo lido com sucesso!\n");
                break;
            default:
                printf("\n[ERRO] - Erro ao ler o estado");
                break;
        }
        if(tk.cat == FIM_ARQ){
            break;
        }
    }
    printf("\n -> %d ", contLinhas);
}

// |=======| FUN��O DO TESTE DA AN�LISE SINT�TICA |=======|

void testarAnasint(){

    if((fd = fopen("codigo.txt", "r")) == NULL){
        printf("\n[ERRO] - ERRO ao abrir o arquivo\n");
    }

    if((mp = fopen("maquina_de_pilha.txt", "w")) == NULL){
        printf("\n[ERRO] - ERRO ao abrir o arquivo da maquina de pilha\n");
    }

    while(1){
        //mostrarTabelaDeSimbolos();
        if(tk.processado != 1)
        {
            tk = analise_lexica(fd);
            tk.processado = 0;
        }

        if(tk.cat == FIM_ARQ){
            printf("\nArquivo encerrado\n");
            break;
        }
        prog();

    }
    mostrarTabelaDeSimbolos();
}
