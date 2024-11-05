#ifndef ANALEX
#define ANALEX

#define TAM_MAX_LEXEMA 31

enum TOKEN_CAT {PR = 1, ID, CT_I, CT_R, CT_C, LT, SN, FIM_ARQ};

/* Onde: ID: Identificador, SN: Sinal; CT_I: Constante numérica inteira */

enum SINAIS {ATRIB = 1, ADICAO, SUBTRACAO, MULTIPLIC, DIVISAO, ABRE_PAR, FECHA_PAR, ABRE_COLCHETES, FECHA_COLCHETES,
            ABRE_CHAVES, FECHA_CHAVES, VIRGULA, MAIOR_OU_IGUAL, MAIOR_QUE, MENOR_OU_IGUAL, MENOR_QUE, ATRIBUICAO, COMPARACAO,
            NEGACAO, DIFERENTE, E_COMERCIAL, AND, OR}; // Sinais válidos da linguagem

typedef struct {
    enum TOKEN_CAT cat; // deve receber uma das constantes de enum TOKEN_CAT
    union { // parte variável do registro
        int codigo; // para tokens das categorias SN
        char lexema[TAM_MAX_LEXEMA]; // cadeia de caracteres que corresponde ao nome do token da cat. ID
        int valor_i; // valor da constante inteira em tokens da cat. CT_I
        float valor_r;
        char c;
    };
} TOKEN; // Tipo TOKEN
#endif

int contLinhas = 1;

