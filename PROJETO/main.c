/*
 * Projeto: Sistema de Curadoria de Obras de Artes
 * Arquivo: main.c
 * Autor: Iano de Oliva Kuhlmann
 * Colaboradores: chat.deepseek.com
 * Disciplina: APR2
 * Professora: Dra. Eloize Rossi Marques Seno
 */

#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "utils.h"
#include "estruturas.h"

int menuPrincipal()
{
    int opcao;
    while (true)
    {
        printf("### MENU PRINCIPAL ###\n");
        printf("1 - Menu Artistas\n");
        printf("2 - Menu Obras\n");
        printf("3 - Menu Colaboracoes\n");
        printf("4 - Menu Relatorios\n");
        printf("5 - Encerrar Programa\n");

        printf("Escolha uma opção: ");
        if (!lerInteiro(&opcao))
        {
            // Se falhou (EOF ou erro), sair do programa
            printf("Detectamos um ERRO IRRECUPERÁVEL! Encerrando programa...\n");
            return -1;
        }

        if (opcao >= 1 && opcao <= 5)
            return opcao;
        else
            printf("Opção inválida. Tente novamente.\n");
    }
}

int main()
{
    int opcao;
    do
	{
        opcao = menuPrincipal();
		if (opcao == 1)
		{
			printf("Função em produção...");
		}

		if (opcao == 2)
		{
			printf("Função em produção...");
		}

		if (opcao == 3)
		{
			printf("Função em produção...");
		}

		if (opcao == 4)
		{
			printf("Função em produção...");
		}

        if (opcao == 5)
		{
			printf("Encerrando programa...");
			break;
		}
		if (opcao == -1)
		{
			// Retorno em caso de erro fatal. Deverá liberar memória antes do exit.
			exit(0);
		}
		
    } while (true);

    return 0;
}
