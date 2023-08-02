#include "csv.h"
#include "string.h"


#define ID_CRIME            1 
#define DATA_CRIME          2
#define NUMERO_ARTIGO       3
#define LUGAR_CRIME         4
#define DESCRICAO_CRIME     5
#define MARCA_CELULAR       6


void ignorar_linha(FILE *arq_in, int tamanho_linha){
    /**
     * Função para ignorar a primeira linha de todo .csv
    */
    char buffer[1024];
    fgets(buffer, sizeof(buffer), arq_in);
}
char* ler_linha(FILE *arq_in, int tamanho_linha){
    /**
     * Função para ler uma linha do .csv, correspondente a 1 registro
    */
    char *buffer;
    buffer = malloc(tamanho_linha);

    // fgets -> NULL, caso a linha estiver completamente vazia (apenas EOF) 
    // fgets -> *ptr, caso contiver qualquer coisa (incluso '\n')
    if(fgets(buffer, tamanho_linha, arq_in) != NULL){
        return buffer;
    }else{
        free(buffer);
        return NULL;
    }
}

Registro *criar_registro(){
    /**
     * Inicialização da estrutura onde cada registro do .csv será armazenado
    */
    Registro *meu_registro;
    meu_registro = malloc(sizeof(Registro));

    // Necessário para evitar aviso do valgrind, inicializando todos os bytes como 0
    memset(meu_registro,0,sizeof(Registro));
    
    // Inicialziando o registro como 'não removido'
    meu_registro->removido = '0';
    
    return meu_registro;
}
void preencher_campos(Registro *meu_registro,char *dados, int campo_atual){
    /**
     * Preenchendo cada campo da estrutura registro com os campos lidos do .csv
     * A escolha do campo a ser preenchido é controlada pela flag 'campo_atual'
    */
    switch(campo_atual){
        case ID_CRIME:
            meu_registro->idCrime = atoi(dados); // string -> int
            break;
        case DATA_CRIME:
            strcpy(meu_registro->dataCrime,dados);
            break;
        case NUMERO_ARTIGO:
            if((*dados) == '\0'){ // Caso numeroArtigo é nulo
                meu_registro->numeroArtigo = -1;
            }else{
                meu_registro->numeroArtigo = atoi(dados);
            }
            break;
        case 4:
            // Campo variável, necessita ser alocado dinamicamente
            meu_registro->lugarCrime = malloc((strlen(dados)+1)*sizeof(char));
            strcpy(meu_registro->lugarCrime,dados);
            break;
        case DESCRICAO_CRIME:
            meu_registro->descricaoCrime = malloc((strlen(dados)+1)*sizeof(char));
            strcpy(meu_registro->descricaoCrime,dados);
            break;
        case MARCA_CELULAR:
            dados[strlen(dados) - 1] = '\0'; // Tirando o '\n' do último campo
            strcpy(meu_registro->marcaCelular,dados);
            break;
    }
}
void preencher_registro(Registro* meu_registro, char* buffer){
    /**
     * Função utilizada para separar a linha em campos (strsep), similar a strtok
    */
    char *token, *ptr_buffer = buffer;
    int campo_atual = 1;
    if(ptr_buffer != NULL){
        token = strsep(&ptr_buffer, ",");
        while(token != NULL){
            preencher_campos(meu_registro,token,campo_atual++);
            token = strsep(&ptr_buffer, ",");
        }
    }
}
void liberar_registro(Registro *meu_registro){
    /**
     * Liberar os campos alocados dinamicamente e a estrutura
    */
    free(meu_registro->lugarCrime);
    free(meu_registro->descricaoCrime);
    free(meu_registro);
}