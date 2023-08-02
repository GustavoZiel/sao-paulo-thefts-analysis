#include "binario.h"


// Funções para ler do binário
Cabecalho* ler_cabecalho(FILE *arq_bin){
    /**
     * Função reponsável por ler os primeiros 17 bytes de um arquivo binário e
     * preencher uma estrutura de cabeçalho com os campos lidos
    */
    Cabecalho *header;
    header = malloc(sizeof(Cabecalho));

    fread(&(header->status),1,sizeof(header->status),arq_bin); // 1 byte
    fread(&(header->proxByteOffset),1,sizeof(header->proxByteOffset),arq_bin); // 8 bytes
    fread(&(header->nroRegArq),1,sizeof(header->nroRegArq),arq_bin); // 4 bytes
    fread(&(header->nroRegRem),1,sizeof(header->nroRegRem),arq_bin); // 4 bytes

    return header;
}
void imprimir_cabecalho(Cabecalho *header){
    /**
     * Função não utilizada no código, feita para debugação
    */
    printf("Status: %c\n",header->status);
    printf("ProxByteOffset: %lld\n",header->proxByteOffset);
    printf("Registros: %d\n",header->nroRegArq);
    printf("Removidos: %d\n",header->nroRegRem);
    printf("\n");
}
Registro* ler_registro(FILE *arq_bin){
    /**
     * Função que lê uma sequência de 'x' bytes do arquivo binário e preenche
     * uma estrutura de registro com os campos lidos
    */
    
    
    // Inicializando e alocando estrutura onde os dados lidos serão armazenados
    Registro *data;
    data = malloc(sizeof(Registro));
    data->lugarCrime = NULL;
    data->descricaoCrime = NULL;
    
    
    /*Leitura*/

    // Não podem ser nulos, Lê normalmente
    fread(&(data->removido),1,sizeof(data->removido),arq_bin);
    fread(&(data->idCrime),1,sizeof(data->idCrime),arq_bin);

    // Caso dataCrime nulo, campo será preenchido por 10 chars de '$'
    fread(&(data->dataCrime),1,sizeof(data->dataCrime),arq_bin);
    // Caso numeroArtigo nulo, campo será preenchido por -1
    fread(&(data->numeroArtigo),1,sizeof(data->numeroArtigo),arq_bin);

    /* MarcaCelular pode não preencher todos os 12 chars ou ser nulo, em ambos
    os casos, o espaço livre será preenchido por '$' */
    fread(&(data->marcaCelular),1,sizeof(data->marcaCelular),arq_bin);
    
    
    /* Leitura dos campos de tamanho variável, necessário alocação dinâmica */

    /* Lê byte a byte até encontrar um delimitador de campo '|', preenche o que
    foi lido no campo da estrutura */

    // LugarCrime
    int qtd_chars = 0;
    char letra_atual;
    do{
        fread(&letra_atual,1,sizeof(char),arq_bin);
        if(letra_atual != '|'){
            data->lugarCrime = realloc(data->lugarCrime,(++qtd_chars)*sizeof(char));
            data->lugarCrime[qtd_chars-1] = letra_atual;
        }
    }while(letra_atual != '|');
    data->lugarCrime = realloc(data->lugarCrime,(++qtd_chars)*sizeof(char));
    data->lugarCrime[qtd_chars-1] = '\0'; // Transformando em string imprimível
    
    // DescriçãoCrime
    qtd_chars = 0;
    do{
        fread(&letra_atual,1,sizeof(char),arq_bin);
        if(letra_atual != '|'){
            data->descricaoCrime = realloc(data->descricaoCrime,(++qtd_chars)*sizeof(char));
            data->descricaoCrime[qtd_chars-1] = letra_atual;
        }
    }while(letra_atual != '|');
    data->descricaoCrime = realloc(data->descricaoCrime,(++qtd_chars)*sizeof(char));
    data->descricaoCrime[qtd_chars-1] = '\0'; // Transformando em string imprimível
    
    
    // Consumindo o byte que delimita o final do registro (char '#')
    fread(&letra_atual,1,sizeof(char),arq_bin);
    

    // Retornando registro totalmente preenchido
    return data;
}
void imprimir_registro(Registro *data){
    /**
     * Função utilizada para imprimir os registros como especificado pelo trabalho
    */
    

    // Se o registro está marcado como removido, não é imprimido
    if(data->removido == '1') return;
    

    /*Impressão*/

    // idCrime não pode ser nulo
    printf("%d, ",data->idCrime);
    
    // Tratando nulo da dataCrime
    if(data->dataCrime[0] == '$'){
        printf("NULO, ");
    }
    else{
        for(int i = 0; i < 10; i++){
            printf("%c",data->dataCrime[i]);
        }
        printf(", ");
    }

    // Tratando nulo do nroArtigo
    if(data->numeroArtigo == -1){
        printf("NULO, ");
    }
    else{
        printf("%d, ",data->numeroArtigo);
    }

    // Tratando nulo do lugarCrime
    if(*data->lugarCrime == '\0'){
        printf("NULO, ");
    }
    else{
        printf("%s, ",data->lugarCrime);
    }

    // Tratando nulo da descriçãoCrime
    if(*data->descricaoCrime == '\0'){
        printf("NULO, ");
    }
    else{
        printf("%s, ",data->descricaoCrime);
    }

    
    // Tratando nulo da marcaCelular
    if(data->marcaCelular[0] == '$'){
        printf("NULO\n");
    }
    else{
        for(int i = 0; i < 12; i++){
            if(data->marcaCelular[i] != '$'){
                printf("%c",data->marcaCelular[i]);
            }
        }
        printf("\n");
    }
}


// Funções para escrever no binário
Cabecalho *criar_cabecalho()
{
    // Cria o cabecalho assim como as especificacoes do tranalho
    Cabecalho *meu_cabecalho = malloc(sizeof(Cabecalho));
    
    meu_cabecalho->status = '0';// Ja inicia com 0 para indicar instabilidade
    meu_cabecalho->proxByteOffset = 0;
    meu_cabecalho->nroRegArq = 0;
    meu_cabecalho->nroRegRem = 0;

    return meu_cabecalho;
}
// Utilizado para atualizar o cabecalho, pode precisar de um FSEEK antes
void atualiza_cabecalho(FILE *nome_arquivo, Cabecalho *meu_cabecalho)
{
    fwrite(&(meu_cabecalho->status), sizeof(char), 1, nome_arquivo);
    fwrite(&(meu_cabecalho->proxByteOffset), sizeof(long long int), 1, nome_arquivo);
    fwrite(&(meu_cabecalho->nroRegArq), sizeof(int), 1, nome_arquivo);
    fwrite(&(meu_cabecalho->nroRegRem), sizeof(int), 1, nome_arquivo);
}
// Utilizado para fazer as ultimas modificacoes no cabecalho antes de salva-lo
void termina_cabecalho(Cabecalho *meu_cabecalho)
{
    if(meu_cabecalho == NULL)return;

    // Terminei de usar, atualizo para indicar estabilidade
    meu_cabecalho->status = '1';

    // Somo o tamanho do proprio cabecalho, ja que ele foi iniciado como 0
    meu_cabecalho->proxByteOffset += 17;
    
    // Usado para debugar
    // printf("cabecalho \n%c \n %lld \n %d \n %d\n", meu_cabecalho->status, meu_cabecalho->proxByteOffset, meu_cabecalho->nroRegArq, meu_cabecalho->nroRegRem);

    return;
}
// Funcao que auxilia para lidar com strings na hora de escrever no binario
void escreve_string(int tam, char *string, FILE *nome_arquivo, Cabecalho *meu_cabecalho, int flag)
{
    // Se for uma string variavel, tamanho 0 para nao entrar no segundo while, que eh para campos fixos
    int  i = 0;
    // Salva a string ate antes do \0
    while(string[i] != '\0')
    {
        fwrite(&(string[i]), sizeof(char), 1, nome_arquivo);
        i++;
    }
    /*Se nao tiver dado o tamanho, que pode ser tanto na data se for nulo quanto 
    na marca do celular se nao estiver lotada a string ele completa com $ */
    while(i < tam)
    {
        fwrite(enchimento, sizeof(char), 1, nome_arquivo);
        i++;
    }
    if(flag)//flag indica para campos de tamanho variavel, ou seja, que precisam de delimitador
    {    
        fwrite(delimitador_campo, sizeof(char), 1, nome_arquivo);
    }
    // i dos bytes escritos, 1 do delimitador se for variável
    meu_cabecalho->proxByteOffset += (i+flag);

    return;
}
// Funcao para salvar um registro lido no arquivo binario
void salvar_binario(FILE *nome_arquivo, Registro *info, Cabecalho *meu_cabecalho)
{
    if(info == NULL) return;
    
    // Escreve no binário o removido e o delimitador
    fwrite(&(info->removido), sizeof(char), 1, nome_arquivo);
    
    // Escrever no binário o idCrime e o delimitador
    fwrite(&(info->idCrime), sizeof(int), 1, nome_arquivo);

    // 4 do int, 1 do removido
    meu_cabecalho->proxByteOffset += 5;

    // Agora para escrever o datacrime
    escreve_string(10, info->dataCrime, nome_arquivo, meu_cabecalho,0);// flag 0, sem delimitador
    
    // Agora o numeroArtigo 
    fwrite(&(info->numeroArtigo), sizeof(int), 1, nome_arquivo);
    
    // 4 do int
    meu_cabecalho->proxByteOffset += 4;

    // Agora a marcaCelular
    escreve_string(12, info->marcaCelular, nome_arquivo, meu_cabecalho,0);// flag 0, sem delimitador

    // Agora a lugarCrime, tamanho 0 pq o tamanho eh variavel
    escreve_string(0, info->lugarCrime, nome_arquivo, meu_cabecalho,1);// flag 1, com delimitador

    // Agora a descricaoCrime, tamanho 0 pq o tamanho eh variavel
    escreve_string(0, info->descricaoCrime, nome_arquivo, meu_cabecalho,1);
    
    // Acabou o registro, coloco o delimitador
    fwrite(delimitador_reg, sizeof(char), 1, nome_arquivo);

    // Coloco tambem o tamanho do delimitador de registro
    meu_cabecalho->proxByteOffset += 1;

    // Coloquei um registro inteiro aumento o cabecalho
    meu_cabecalho->nroRegArq++;

    return;
}