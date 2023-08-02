#ifndef _CSV_H
#define _CSV_H


    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>


    typedef struct registro_t{
        char removido;
        int idCrime;
        char dataCrime[10];
        int numeroArtigo;
        char marcaCelular[12];
        char *lugarCrime;
        char *descricaoCrime;
    }Registro;


    // Funções para leitura do .csv e armazenamento em registros
    Registro *criar_registro();
    void preencher_registro(Registro* meu_registro, char* buffer);
    void liberar_registro(Registro *meu_registro);
    void ignorar_linha(FILE *arq_in, int tamanho_linha);
    char* ler_linha(FILE *arq_in, int tamanho_linha);

#endif