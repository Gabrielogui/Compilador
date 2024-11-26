#ifndef ANASINT_H_INCLUDED
#define ANASINT_H_INCLUDED

#define TAM_MAX_LEXEMA 31
#define TAM_MAX_TS 80

// LEXEMA ; ESCOPO ; TIPO ; CATEGORIA ; PASSAGEM ; ZUMBI ; ARRAY ; DIM1 ; DIM2 ; ENDEREÇO ; RÓTULO

// |=======| ENUMs |=======|
// ESCOPO:
enum escopo {GLOBAL, LOCAL};

// TIPO:
enum tipo {INT, REAL, CHAR, BOOL, NA};

// CATEGORIA:
enum categoria {VAR_GLOBAL, VAR_LOCAL, PROTOTIPO, PROCEDIMENTO, PAR_PROCEDIMENTO};

// PASSAGEM:
enum passagem {COPIA, REFERENCIA, NA};

// ZUMBI:
enum zumbi {VIVO, ZUMBI, NA};

// ARRAY:
enum isArray {ESCALAR, VETOR, MATRIZ};

// |=======| STRUCT DO REGISTRO DA TABELA DE SÍMBOLO(TS) |=======|
typedef struct{
    char lexema[TAM_MAX_LEXEMA];
    enum escopo;
}REG_TS;

// |=======| STRUCT DA TABELA DE SÍMBOLO(TS) |=======|
typedef struct{
    REG_TS Linhas[TAM_MAX_TS];
    int topo;
}TABELA_SIMBOLO;


#endif // ANASINT_H_INCLUDED
