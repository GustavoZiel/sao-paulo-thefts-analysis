#include "funcionalidades.h"
#include "funcoesFornecidas.h"
#include "csv.h"
#include "binario.h"

// Mensagem de erro pedida pelo trabalho
void erro_1(){
    printf("Falha no processamento do arquivo.\n");
    exit(0);
}
// Funcao geral para abrir arquivos
void abrir_arq(char *nome_arquivo, FILE **arquivo, char* modo){
    (*arquivo) = fopen(nome_arquivo, modo);
    if(*arquivo == NULL) 
        erro_1();
}

void CRIAR_TABELA()
{
    // Abrindo arquivo de entrada .csv
    char nome_arq_ent[100];
    scanf("%s",nome_arq_ent);
    FILE *arq_csv;
    abrir_arq(nome_arq_ent,&arq_csv,"r");


    // Abrindo arquivo de saída .bin
    char nome_arq_saida[100];
    scanf("%s",nome_arq_saida);
    FILE *arq_bin;
    abrir_arq(nome_arq_saida,&arq_bin,"wb");


    // Coloco o cabecalho no inicio do binario ja
    Cabecalho *meu_cabecalho = criar_cabecalho();
    atualiza_cabecalho(arq_bin, meu_cabecalho);
    

    // Ignorar primeira linha do arquivo .csv
    ignorar_linha(arq_csv,1024);

    Registro *meu_registro = NULL;
    char *linha = NULL; // Onde ficará armazenada cada linha durante a leitura
    while(!feof(arq_csv)){ 
        // Lê a linha
        linha = ler_linha(arq_csv,1024);
        
        // Se a linha tiver algo, lê um registro e escreve no binário
        if(linha != NULL && *linha != '\n'){
            meu_registro = criar_registro(linha);
            preencher_registro(meu_registro,linha);
            salvar_binario(arq_bin,meu_registro,meu_cabecalho);
            liberar_registro(meu_registro);
        }
        
        free(linha);
    }

    // Retorno ao inicio para atualizar o cabecalho
    fseek(arq_bin,0,SEEK_SET);

    // Volto o status do cabecalho para zero e somo o tamanho ocupado pelo proprio cabecalho
    termina_cabecalho(meu_cabecalho);
    // Atualizo o cabecalho do inicio do arquivo
    atualiza_cabecalho(arq_bin, meu_cabecalho);
    free(meu_cabecalho);

    
    // Fecho os arquivos e printo o binario
    fclose(arq_csv);
    fclose(arq_bin);

    binarioNaTela(nome_arq_saida);

    return;
}
void SELECT()
{
    // Abrindo arquivo de entrada .bin
    FILE* arq_bin;
    char nome_arq_ent[100];
    scanf("%s",nome_arq_ent);
    abrir_arq(nome_arq_ent,&arq_bin,"rb");


    Cabecalho *meu_cabecalho;
    meu_cabecalho = ler_cabecalho(arq_bin);


    // Caso o arquivo esteja inconsistente (status == '0'), imprimir erro
    if(meu_cabecalho->status == '0'){
        fclose(arq_bin);
        free(meu_cabecalho);
        erro_1();
    }
    // Caso o arquivo não contenha registros
    if(meu_cabecalho->nroRegArq == 0){
        printf("Registro inexistente.\n");
        fclose(arq_bin);
        free(meu_cabecalho);
        exit(0);
    }


    // Lendo cada registro e imprimindo
    Registro *meu_registro;
    int num_registros = meu_cabecalho->nroRegArq;
    while(num_registros > 0){
        meu_registro = ler_registro(arq_bin);
        imprimir_registro(meu_registro);
        liberar_registro(meu_registro);
        num_registros--;
    }

    
    // Desalocando memória
    free(meu_cabecalho);
    
    // Fechando arquivo
    fclose(arq_bin);
}
