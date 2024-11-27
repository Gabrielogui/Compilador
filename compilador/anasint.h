#ifndef ANASINT_H_INCLUDED
#define ANASINT_H_INCLUDED

#define TAM_MAX_LEXEMA 31
#define TAM_MAX_TS 80

// LEXEMA ; ESCOPO ; TIPO ; CATEGORIA ; PASSAGEM ; ZUMBI ; ARRAY ; DIM1 ; DIM2 ; ENDEREÇO ; RÓTULO

// |=======| ENUMs |=======|
// ESCOPO:
enum escopo {GLOBAL, LOCAL};

// TIPO:
enum tipo {INT_TIPO, REAL_TIPO, CHAR_TIPO, BOOL_TIPO, NA_TIPO};

// CATEGORIA:
enum categoria {VAR_GLOBAL, VAR_LOCAL, PROTOTIPO, PROCEDIMENTO, PAR_PROCEDIMENTO};

// PASSAGEM:
enum passagem {COPIA, REFERENCIA, NA_PASSAGEM};

// ZUMBI:
enum zumbi {VIVO, ZUMBI, NA_ZUMBI};

// ARRAY:
enum isArray {ESCALAR, VETOR, MATRIZ};

// |=======| STRUCT DO REGISTRO DA TABELA DE SÍMBOLO(TS) |=======|
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
    //int endereco;
    //int rotulo;
}REG_TS;

// |=======| STRUCT DA TABELA DE SÍMBOLO(TS) |=======|
typedef struct{
    REG_TS Linhas[TAM_MAX_TS];
    int topo;
}TABELA_SIMBOLO;

// |=======| DECLARAÇÕES DAS FUNÇÕES |=======|

void prog();

void decl_list_var();
void decl_def_proc();
void decl_var();

#endif // ANASINT_H_INCLUDED
