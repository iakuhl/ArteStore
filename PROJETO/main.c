/*
 * Projeto: Sistema de Curadoria de Obras de Artes
 * Arquivo: main.c
 * Autor: Iano de Oliva Kuhlmann
 * Colaboradores: chat.deepseek.com
 * Link de colaboração: https://chat.deepseek.com/share/jil3nf8yyu9wwz0h8l
 * Disciplina: APR2
 * Professora: Dra. Eloize Rossi Marques Seno
 */


/********************
 * CÓDIGO PRINCIPAL *
 ********************/


/***************
 * BIBLIOTECAS *
 ***************/
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>


/***************
 * IMPORTAÇÕES *
 ***************/
#include "utils.h"
#include "estruturas.h"
#include "menus.h"



int main()
{
    
    int opcao;
    bool executando = true;
    do
	{
        opcao = menuPrincipal();
        switch (opcao)
        {
        case 1:

            if(!moduleArtistas())
            {
                printf("Erro ao executar o módulo de artistas.\n");
                // Aqui você deve adicionar código para liberar memória alocada dinamicamente, se houver.
            }
            break;

        case 2:
            if(!moduleObras())
            {
                printf("Erro ao executar o módulo de obras.\n");
                // Aqui você deve adicionar código para liberar memória alocada dinamicamente, se houver.
            }
            break;

        case 3:
            if(!moduleColaboracoes())
            {
                printf("Erro ao executar o módulo de colaborações.\n");
                // Aqui você deve adicionar código para liberar memória alocada dinamicamente, se houver.
            }
            break;

        case 4:
            if(!moduleRelatorios())
            {
                printf("Erro ao executar o módulo de relatórios.\n");
                // Aqui você deve adicionar código para liberar memória alocada dinamicamente, se houver.
            }
            break;

        case 5:
            printf("Encerrando programa...");
            // Aqui você deve adicionar código para liberar memória alocada dinamicamente, se houver.
            executando = false;
            break;

        case -1:
            // Erro detectado, encerrar programa imediatamente após limpar memória, se necessário.
            // Aqui você deve adicionar código para liberar memória alocada dinamicamente, se houver.
            executando = false;
            return 1;

        default:
            break;
        }
		
    } while (executando);

    return 0;
}
