#ifndef ANASINT_H_INCLUDED
#define ANASINT_H_INCLUDED

#define TAM_MAX_LEXEMA 31
#define TAM_MAX_TS 80
#define TAM_MAX_EXP 999

// LEXEMA ; ESCOPO ; TIPO ; CATEGORIA ; PASSAGEM ; ZUMBI ; ARRAY ; DIM1 ; DIM2 ; ENDERE�O ; R�TULO

// |=======| ENUMs |=======|
// ESCOPO:
enum escopo {GLOBAL = 1, LOCAL};

// TIPO:
enum tipo {INT_TIPO = 1, REAL_TIPO, CHAR_TIPO, BOOL_TIPO, NA_TIPO};

// CATEGORIA:
enum categoria {VAR_GLOBAL = 1, VAR_LOCAL, PROTOTIPO, PROCEDIMENTO, PAR_PROCEDIMENTO};

// PASSAGEM:
enum passagem {COPIA = 1, REFERENCIA, NA_PASSAGEM};

// ZUMBI:
enum zumbi {VIVO = 1, ZUMBI, NA_ZUMBI};

// ARRAY:
enum isArray {ESCALAR = 1, VETOR, MATRIZ, NA_ISARRAY};

// � CONSTANTE:
enum eh_const {SIM = 1, NAO};

// |=======| STRUCT DO REGISTRO DA TABELA DE S�MBOLO(TS) |=======|
typedef struct{
    char lexema[TAM_MAX_LEXEMA];
    enum escopo escopo;
    enum tipo tipo;
    enum categoria categoria;
    enum passagem passagem;
    enum zumbi zumbi;
    enum isArray isArray;
    int dim01;
    int dim02;
    enum eh_const eh_const;
    union {
        int constInt;
        float constReal;
        char constChar;
        char constString[TAM_MAX_LEXEMA];
        int constBool; // 0 = TRUE ; 1 = FALSE
    };
    int endereco;
    char rotulo[8]; // PROCEDIMENTOS
}REG_TS;

// |=======| STRUCT DA TABELA DE S�MBOLO(TS) |=======|
typedef struct{
    REG_TS Linhas[TAM_MAX_TS];
    int topo;
}TABELA_SIMBOLO;

// |=======| STRUCT DA EXPRESSÃO |=======|
// ENUM DO TIPO DA EXPRESSÃO
enum tipoExpr {INT_EXPR = 1, REAL_EXPR, CHAR_EXPR, BOOL_EXPR};

typedef struct{
    enum tipoExpr tipoExpr;
    int result;
}EXPRESSAO;


typedef struct{
    EXPRESSAO expressao[TAM_MAX_EXP];
    int topo;
}EXPRESSOES;


// |=======| DECLARA��ES DAS FUN��ES |=======|

void prog();
void decl_list_var();
void decl_def_proc();
void decl_var();
void cmd();
void atrib();
void expr();
void expr_simp();
void termo();
void fator();
void op_rel();

int consultaTabelaDeSimbolos(char[]);

// |=======| VARI�VEIS GLOBAIS |=======|
extern TABELA_SIMBOLO ts; // TABELA DE SÍMBOLO
extern int qtdRotulo;     // QUANTIDADE ROTULO
extern FILE *mp;          // ARQUIVO DA MAQUINA DE PILHA
extern int qtdVarGlobais;
//extern int lido;

#endif // ANASINT_H_INCLUDED
