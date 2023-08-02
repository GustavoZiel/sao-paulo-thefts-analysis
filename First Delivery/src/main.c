/*
    Trabalho realizado por:
        Artur De Vlieger Lima   13671574
        Gustavo Gabriel Ribeiro 13672683
*/
#include <stdio.h>
#include "funcionalidades.h"

int main()
{
    //Selecione a funcionalidade
    int funcao;
    scanf("%d", &funcao);

    switch(funcao)
    {
        case 1:
            CRIAR_TABELA();
            break;
        case 2:
            SELECT();
            break;
    }

    return 0;
}
