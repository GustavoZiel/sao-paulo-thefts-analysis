#ifndef _BIN_H
#define _BIN_H


    #include "csv.h"


    #define delimitador_campo "|"
    #define delimitador_reg "#"
    #define enchimento "$"
    
    
    typedef struct cabecalho_t {
        char status;
        long long int proxByteOffset;
        int nroRegArq;
        int nroRegRem;
    } Cabecalho;


    // Funções para ler do binário
    Cabecalho* ler_cabecalho(FILE *arq_bin);
    void imprimir_cabecalho(Cabecalho *header);
    Registro* ler_registro(FILE *arq_bin);
    void imprimir_registro(Registro *data);

    // Funções para escrever no binário
    Cabecalho *criar_cabecalho();
    void atualiza_cabecalho(FILE *nome_arquivo, Cabecalho *meu_cabecalho);
    void termina_cabecalho(Cabecalho *meu_cabecalho);
    void salvar_binario(FILE *nome_arquivo, Registro *info, Cabecalho *meu_cabecalho);
    
#endif