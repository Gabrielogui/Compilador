#ifndef ANALEX
#define ANALEX

#define TAM_MAX_LEXEMA 31

// |=======| CATEGORIAS |=======|
enum TOKEN_CAT {PVR = 1, ID, CT_I, CT_R, CT_C, CT_S, SN, FIM_ARQ};

/* Onde: ID: Identificador, SN: Sinal; CT_I: Constante num�rica inteira ; CT_R: Constante Real ; CT_C: Constante caactere; CT_S: Constante String; FIM_ARQ: Fim do arquivo*/

// |=======| PALAVRAS RESERVADAS |=======|
enum PALAVRAS_RESERVADAS {CONST = 1, PROT, DEF, INIT, ENDP, CHAR, INT, REAL, BOOL, DO, WHILE, ENDW, VAR, FROM, DT, TO, BY,
                          ENDV, IF, ELIF, ELSE, ENDI, GETINT, GETOUT, GETCHAR, GETREAL, GETSTR, PUTINT, PUTCHAR, PUTREAL, PUTSTR};

// |=======| SINAIS |=======|
enum SINAIS {ADICAO = 1, SUBTRACAO, MULTIPLICACAO, DIVISAO, ABRE_PAR, FECHA_PAR, ABRE_COLCHETES, FECHA_COLCHETES,
            ABRE_CHAVES, FECHA_CHAVES, VIRGULA, MAIOR_OU_IGUAL, MAIOR_QUE, MENOR_OU_IGUAL, MENOR_QUE, ATRIBUICAO, COMPARACAO,
            NEGACAO, DIFERENTE, E_COMERCIAL, AND, OR}; // Sinais v�lidos da linguagem

// |=======| TOKEN |=======|
typedef struct {
    enum TOKEN_CAT cat; // deve receber uma das constantes de enum TOKEN_CAT
    int processado;
    union { // parte vari�vel do registro
        int codigo; // para tokens das categorias SN
        char lexema[TAM_MAX_LEXEMA]; // cadeia de caracteres que corresponde ao nome do token da cat. ID
        int valor_i; // valor da constante inteira em tokens da cat. CT_I
        float valor_r;
        char c;
    };
} TOKEN; // Tipo TOKEN
#endif

// |=======| VARI�VEIS GLOBAIS |=======|
extern int contLinhas; // CONTADOR DE LINHAS
extern FILE *fd;       // ARQUIVO A SER LIDO
extern TOKEN tk;       // TOKEN

// |=======| DECLARA��O DA FUN��O |=======|
TOKEN analise_lexica(FILE*);
int verificarPV(char[]);



