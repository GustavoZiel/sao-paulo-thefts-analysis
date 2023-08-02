#include "funcionalidades.h"
#include "funcoesFornecidas.h"
#include "registro.h"
#include "filtro.h"
#include "indice.h"
#include "util.h"

void CRIAR_TABELA()
{
    // Abrindo arquivo de entrada .csv
    char nome_arq_ent[100];
    scanf("%s",nome_arq_ent);
    FILE *arq_csv;
    abrir_arq(&arq_csv, nome_arq_ent, "r");

    // Abrindo arquivo de saída .bin
    char nome_arq_saida[100];
    scanf("%s",nome_arq_saida);
    FILE *arq_bin;
    abrir_arq(&arq_bin, nome_arq_saida, "wb");

    // Escrevendo cabeçalho no início do arquivo
    Cabecalho *meu_cabecalho = criar_cabecalho();
    atualiza_cabecalho(arq_bin, meu_cabecalho);
    
    // Ignorar primeira linha do arquivo .csv
    ignorar_linha(arq_csv,1024);

    
    // Loop de leitura do .csv
    Registro *meu_registro = NULL;
    char *linha = NULL; // Onde ficará armazenada cada linha durante a leitura
    while(!feof(arq_csv)){ 
        // Lê a linha
        linha = ler_linha(arq_csv,1024);
        
        // Se a linha tiver algo, lê um registro e escreve no binário
        if(linha != NULL && *linha != '\n'){
            meu_registro = criar_registro(linha);
            preencher_registro(meu_registro,linha);
            salvar_registro(arq_bin,meu_registro,meu_cabecalho);
            delimita_registro(arq_bin, meu_cabecalho);
            liberar_registro(meu_registro);
        }
        
        free(linha);
    }

    // Atualizar cabecalho no inicio do arquivo
    atualiza_cabecalho(arq_bin, meu_cabecalho);
    free(meu_cabecalho);
    estabiliza_arq(arq_bin);
    
    // Fechando arquivos e imprimindo binário
    fclose(arq_csv);
    fclose(arq_bin);
    binarioNaTela(nome_arq_saida);
}

void SELECT()
{
    // Abrindo arquivo de entrada .bin
    FILE* arq_bin;
    char nome_arq_ent[100];
    scanf("%s",nome_arq_ent);
    abrir_arq(&arq_bin, nome_arq_ent, "rb");

    // Lendo cabeçalho
    Cabecalho *meu_cabecalho;
    meu_cabecalho = ler_cabecalho(arq_bin);

    // Lendo cada registro e imprimindo
    Registro *meu_registro;
    int num_registros = meu_cabecalho->nroRegArq;
    while(num_registros > 0){
        meu_registro = ler_registro_binario(arq_bin, NULL);
        imprimir_registro(meu_registro);
        liberar_registro(meu_registro);
        num_registros--;
    }

    // Desalocando memória e fechando arquivo
    free(meu_cabecalho);
    fclose(arq_bin);
}

void CREATE_INDEX(){
    /* Lendo os dados providos pelo usuário */
    
    char nome_arq_bin[tam_buffer], campoIndexado[tam_buffer], tipoDado[tam_buffer], arq_bin_indice[tam_buffer];
    scanf("%s %s %s %s",nome_arq_bin,campoIndexado,tipoDado,arq_bin_indice);


    /* Abrindo arquivos e checando consistência */

    // Abrindo arquivo de registros
    FILE *arq_bin;
    abrir_arq(&arq_bin, nome_arq_bin, "rb"); // Apenas leitura binária
    // Lendo cabeçalho
    Cabecalho *meu_cabecalho;
    meu_cabecalho = ler_cabecalho(arq_bin); // Checa consistência nessa função


    /* Criando índice */
    
    Indice *meu_indice;
    meu_indice = criar_indice(campoIndexado,tipoDado);

    preencher_indice(arq_bin,meu_indice,meu_cabecalho);
    ordenar_indice(meu_indice);


    /* Escrevendo no arquivo*/

    FILE *arq_indice;
    abrir_arq(&arq_indice, arq_bin_indice, "wb"); // Apenas escrita binária
    salvar_indice(arq_indice,meu_indice);
    // Mudando status para consistente
    estabiliza_arq(arq_indice);


    /* Liberando memória alocada e fechando arquivos*/
    free(meu_cabecalho);
    liberar_indice(meu_indice);
    fclose(arq_bin);
    fclose(arq_indice);

    //Binário na tela
    binarioNaTela(arq_bin_indice);
}

void SELECT_WHERE(){
    /* Lendo os dados providos pelo usuário */
    
    char nome_arq_bin[tam_buffer], campoIndexado[tam_buffer], tipoDado[tam_buffer], arq_bin_indice[tam_buffer];
    scanf("%s %s %s %s",nome_arq_bin,campoIndexado,tipoDado,arq_bin_indice);

    /* Abrindo arquivos e checando consistência */

    // Abrindo arquivo de registros
    FILE *arq_bin;
    
    // Como não há escrita nessa funcionalidade, ambos os arquivos de índice e 
    // de registros serão abertos apenas para leitura binária
    abrir_arq(&arq_bin, nome_arq_bin, "rb"); 
    
    // Lendo cabeçalho
    Cabecalho *meu_cabecalho;
    // Consistência do arquívo é checada nessa função
    meu_cabecalho = ler_cabecalho(arq_bin); 

    // Abrindo arquivo de indíce
    FILE *arq_indice;
    abrir_arq(&arq_indice, arq_bin_indice, "rb");
    
    // Criando estrutura onde os índices serão armazenados em memória primária
    Indice *meu_indice;
    meu_indice = criar_indice(campoIndexado,tipoDado);

    // Checando consistência do índice nessa função
    if(!ler_cabecalho_indice(meu_indice,arq_indice)){
        /* Caso não consiga ler o cabeçalho (arquivo inconsistênte), desaloca memória 
        já alocada alocada, fecha arquivos abertos e encerra programa */
        free(meu_cabecalho);
        fclose(arq_bin);
        erro_1();
    }


    /* Armazenando todos os índices em memória primária */
    
    ler_indice_binario(meu_indice,arq_indice);


    /* Realizando a busca com algum filtro determinado pelo usuário */
    
    Filtro *meu_filtro;
    char *valor_buscado;

    // Lendo número de buscas
    int num_buscas;
    scanf("%d",&num_buscas);

    for(int i = 0; i < num_buscas; i++){
        printf("Resposta para a busca %d\n",i+1);
        
        // 'Filtro' é a estrutura que armazena os campos que serão filtrados
        meu_filtro = criar_filtro();
        
        // Se um usuário informar um campo que está indexado, buscar no arquivo de índice primeiro
        if(preencher_filtro(meu_filtro,meu_indice,&valor_buscado)){
            busca_indice(arq_bin,meu_filtro,meu_indice,valor_buscado);
        }
        // Caso não tenha buscado por um valor de um campo indexado, fará busca sequencial
        else{
            busca_sequencial(arq_bin,meu_filtro,meu_cabecalho);
        }
        
        // Retonando o cursor para o ínicio do arquivo (depois do cabeçalho, 17 bytes) para a próxima busca
        fseek(arq_bin,17,SEEK_SET);

        // Reseta o filtro para a próxima busca e liberando valor buscado
        liberar_filtro(meu_filtro);
    }


    /* Após todas as buscas, memória utilizada é desalocada e arquivos são fechados */

    liberar_indice(meu_indice);
    free(meu_cabecalho);
    fclose(arq_bin);  
    fclose(arq_indice);
}

void DELETE(){
    /* Lendo os dados de entrada */
    
    char nome_arq_bin[tam_buffer], campoIndexado[tam_buffer], tipoDado[tam_buffer], arq_bin_indice[tam_buffer];
    scanf("%s %s %s %s",nome_arq_bin,campoIndexado,tipoDado,arq_bin_indice);


    /* Abrindo arquivos e checando consistência */

    // Arquivo de registros
    FILE *arq_bin;
    // Há escrita no arquivo, então abre para leitura e escrita binária
    abrir_arq(&arq_bin, nome_arq_bin,"r+b");
    Cabecalho *meu_cabecalho;
    meu_cabecalho = ler_cabecalho(arq_bin);
    // Como abre para leitura e escrita, muda o status para inconsistente
    desestabiliza_arq(arq_bin,17);

    // Arquivo de indíce
    FILE *arq_indice;
    // A priori, o arquivo é apenas aberto para leitura e a escrita serve para marcar instabilidade
    abrir_arq(&arq_indice, arq_bin_indice, "r+b");

    Indice *meu_indice;
    meu_indice = criar_indice(campoIndexado,tipoDado);
    if(!ler_cabecalho_indice(meu_indice,arq_indice)){
        /* Caso não consiga ler o cabeçalho (arquivo inconsistênte), desaloca memória 
        já alocada alocada, fecha arquivos abertos e encerra programa */
        estabiliza_arq(arq_bin);
        free(meu_cabecalho);
        fclose(arq_bin);
        erro_1();
    }
    
    // Desestabilizo o arquivo, pois irei alterar o binário, ja tornando o indice inválido
    desestabiliza_arq(arq_indice,5);

    /* Armazenando todos os índices em memória primária */

    ler_indice_binario(meu_indice,arq_indice);
    fclose(arq_indice);


    /* Realizando a deleção com algum filtro determinado pelo usuário */
    
    Filtro *meu_filtro;
    char *valor_buscado;
    
    // Lendo número de buscas
    int num_buscas;
    scanf("%d",&num_buscas);

    for(int i = 0; i < num_buscas; i++){
        meu_filtro = criar_filtro();

        if(preencher_filtro(meu_filtro,meu_indice,&valor_buscado)){
            remocao_indice(arq_bin,meu_filtro,meu_cabecalho,meu_indice,valor_buscado);
        }
        else{
            remocao_sequencial(arq_bin,meu_filtro,meu_cabecalho,meu_indice,valor_buscado);
        }
        
        // Retonando o cursor do arquivo para o início (depois do cabeçalho) para a próxima busca
        fseek(arq_bin,17,SEEK_SET);
        
        // Reseta o filtro para a próxima busca
        liberar_filtro(meu_filtro);
    }

    ordenar_indice(meu_indice);


    /* Arquivo de índice */

    // Abrindo arquivo do índice novamente, dessa vez para escrita
    abrir_arq(&arq_indice, arq_bin_indice, "wb");
    desestabiliza_arq(arq_indice, -1);

    // Salvando índices no arquivo
    salvar_indice(arq_indice,meu_indice);
    estabiliza_arq(arq_indice);

    // Liberando memória e fechando arquivo
    liberar_indice(meu_indice);
    fclose(arq_indice);
    

    /* Arquivo de registros */

    meu_cabecalho->status = consistente;
    atualiza_cabecalho(arq_bin,meu_cabecalho);
    free(meu_cabecalho);
    fclose(arq_bin);


    // Binário na tela
    binarioNaTela(nome_arq_bin);
    binarioNaTela(arq_bin_indice);
}

void INSERT_INTO(){
    /* Lendo os dados de entrada */
    
    char nome_arq_bin[tam_buffer], campoIndexado[tam_buffer], tipoDado[tam_buffer], arq_bin_indice[tam_buffer];
    scanf("%s %s %s %s",nome_arq_bin,campoIndexado,tipoDado,arq_bin_indice);


    /* Abrindo arquivos e checando consistência */

    // Arquivo de registros
    FILE *arq_bin;
    abrir_arq(&arq_bin, nome_arq_bin,"r+b"); // O arquivo pode ser modificado, abro para leitura e escrita
    Cabecalho *meu_cabecalho;
    meu_cabecalho = ler_cabecalho(arq_bin); // Checa consistência nessa função
    // Como abre para leitura e escrita, muda o status para inconsistente
    desestabiliza_arq(arq_bin,17);

    // Arquivo de indíce
    FILE *arq_indice;
    abrir_arq(&arq_indice, arq_bin_indice, "r+b"); // Apenas leitura binária por hora
    // Criando estrutura onde os índices serão armazenados e lendo o cabecalho do arquivo de índice
    Indice *meu_indice;
    meu_indice = criar_indice(campoIndexado,tipoDado);
    if(!ler_cabecalho_indice(meu_indice,arq_indice)){
        /* Caso não consiga ler o cabeçalho (arquivo inconsistênte), desaloca memória 
        alocada, fecha arquivos e encerra programa */
        estabiliza_arq(arq_bin);
        free(meu_cabecalho);
        fclose(arq_bin);
        fclose(arq_indice);
        erro_1();
    }
    
    // Desestabilizo o arquivo, pois irei alterar o binário, ja tornando o indice inválido
    desestabiliza_arq(arq_indice,5);

    /* Armazenando todos os índices em memória primária */

    ler_indice_binario(meu_indice,arq_indice);
    fclose(arq_indice);

    /* Adicionando os índices */

    // Indo para o final do arquivo inserir novos registros
    fseek(arq_bin,0,SEEK_END);

    // Loop de adição dos registros
    int num_adicoes;
    scanf("%d",&num_adicoes);
    for(int i = 0; i < num_adicoes; i++){
        insere_final(arq_bin,arq_indice, &meu_indice, meu_cabecalho);
    }

    // Ordenando estrutura onde está armazenado os índices em memórima primária
    ordenar_indice(meu_indice);

    /* Arquivo de índice */

    // Abrindo arquivo do índice novamente, dessa vez para escrita
    abrir_arq(&arq_indice, arq_bin_indice, "wb");
    desestabiliza_arq(arq_indice, -1);

    // Salvando índices no arquivo
    salvar_indice(arq_indice,meu_indice);
    estabiliza_arq(arq_indice);

    // Liberando memória e fechando arquivo
    liberar_indice(meu_indice);
    fclose(arq_indice);
    

    /* Arquivo de registros */

    meu_cabecalho->status = consistente;
    atualiza_cabecalho(arq_bin,meu_cabecalho);
    free(meu_cabecalho);
    fclose(arq_bin);


    // Binário na tela
    binarioNaTela(nome_arq_bin);
    binarioNaTela(arq_bin_indice);
}

void UPDATE(){
    /* Lendo os dados de entrada */
    bool flag_busca_indice, flag_atu_ind;
    char *valor_buscado, *valor_atualizado;  
    Filtro *meu_filtro, *filtro_atualiza; // Utilizado para busca
    char nome_arq_bin[tam_buffer], campoIndexado[tam_buffer], tipoDado[tam_buffer], nome_arq_ind[tam_buffer];
    scanf("%s %s %s %s",nome_arq_bin,campoIndexado,tipoDado,nome_arq_ind);


    /* Abrindo arquivos e checando consistência */

    // Arquivo de registros
    FILE *arq_bin;
    // Há escrita no arquivo, então abre para leitura e escrita binária
    abrir_arq(&arq_bin, nome_arq_bin,"r+b");
    Cabecalho *meu_cabecalho;
    meu_cabecalho = ler_cabecalho(arq_bin);
    // Como abre para leitura e escrita, muda o status para inconsistente
    desestabiliza_arq(arq_bin,17);

    // Arquivo de indíce
    FILE *arq_indice;
    // A priori, o arquivo é apenas aberto para leitura
    abrir_arq(&arq_indice, nome_arq_ind, "r+b");

    Indice *meu_indice;
    meu_indice = criar_indice(campoIndexado,tipoDado);
    if(!ler_cabecalho_indice(meu_indice,arq_indice)){
        /* Caso não consiga ler o cabeçalho (arquivo inconsistênte), desaloca memória 
        já alocada alocada, fecha arquivos abertos e encerra programa */
        estabiliza_arq(arq_bin);
        free(meu_cabecalho);
        fclose(arq_bin);
        erro_1();
    }
    
    // Desestabilizo o arquivo, pois irei alterar o binário, ja tornando o indice inválido
    desestabiliza_arq(arq_indice,5);

    /* Armazenando todos os índices em memória primária */

    ler_indice_binario(meu_indice,arq_indice);
    fclose(arq_indice);
    
    // Lendo número de buscas
    int num_de_atualizacoes;
    scanf("%d",&num_de_atualizacoes);
    for(int i = 0; i < num_de_atualizacoes; i++)
    {
        flag_busca_indice = false; // Flag para indicar se a busca fara uso do indice
        flag_atu_ind = false; // Flag para indicar se o campo do indice sera atualizado
        
        meu_filtro = criar_filtro(); // Filtro para a busca
        filtro_atualiza = criar_filtro(); // Filtro para o que sera atualizado
        flag_busca_indice = preencher_filtro(meu_filtro,meu_indice,&valor_buscado); // Preencho o filtro de busca
        flag_atu_ind = preencher_filtro(filtro_atualiza, meu_indice, &valor_atualizado); // Preencho o filtro de atualizacao

        if(flag_busca_indice == true){ // Se for indice
            atualizacao_ind(arq_bin,meu_filtro, filtro_atualiza, meu_cabecalho,meu_indice,valor_buscado, flag_atu_ind);
        }
        else{ // Se for pelo binario
            atualizacao_seq(arq_bin,meu_filtro,filtro_atualiza,meu_cabecalho,meu_indice, flag_atu_ind);
        }
            
        // Retonando o cursor do arquivo para o início (depois do cabeçalho) para a próxima busca
        fseek(arq_bin,17,SEEK_SET);
        
        // Reseta o filtro para a próxima busca
        liberar_filtro(meu_filtro);
        liberar_filtro(filtro_atualiza);
    }
    free(valor_atualizado);

    // Abro novamente o indice para atualiza lo e o desestabilizo
    abrir_arq(&arq_indice, nome_arq_ind, "wb");
    desestabiliza_arq(arq_indice, -1);

    // Salvando índices no arquivo
    salvar_indice(arq_indice,meu_indice);
    estabiliza_arq(arq_indice);

    // Liberando memória e fechando arquivo
    liberar_indice(meu_indice);
    fclose(arq_indice);
    

    /* Arquivo de registros */

    meu_cabecalho->status = consistente;
    atualiza_cabecalho(arq_bin,meu_cabecalho);
    free(meu_cabecalho);
    fclose(arq_bin);


    // Binário na tela
    binarioNaTela(nome_arq_bin);
    binarioNaTela(nome_arq_ind);
}
