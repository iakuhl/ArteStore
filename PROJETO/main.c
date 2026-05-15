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
#include "persistencia.h"

void cargarDados(ListaArtistas *listaArtistas, ListaObras *listaObras, ListaColaboracoes *listaColaboracoes)
{ 
    if(!carregarArtistas(listaArtistas))
        printf("Erro ao carregar artistas. Iniciando com lista vazia.\n");
    if(!carregarObras(listaObras))
        printf("Erro ao carregar obras. Iniciando com lista vazia.\n");
    if(!carregarColaboracoes(listaColaboracoes))
        printf("Erro ao carregar colaborações. Iniciando com lista vazia.\n");
}

bool liberarDados(ListaArtistas *listaArtistas, ListaObras *listaObras, ListaColaboracoes *listaColaboracoes)
{
	sucesso = true;
	if(!liberarArtistas(listaArtistas))
	{
		printf("Erro de memória em Artistas!\n");
		sucesso = false;
	}
	if(!liberarObras(listaObras))
	{
		printf("Erro de memória em Obras!\n");
		sucesso = false;
	}
	if(!liberarColaboracoes(listaColaboracoes))
	{
		printf("Erro de memória em Colaborações!\n");
		sucesso = false;
	}
	return sucesso;
}




int main()
{
    ListaArtistas *listaArtistas;
    ListaObras *listaObras;
    ListaColaboracoes *listaColaboracoes;

    listaArtistas = (ListaArtistas *) malloc(sizeof(ListaArtistas));
    listaObras = (ListaObras *) malloc(sizeof(ListaObras));
    listaColaboracoes = (ListaColaboracoes *) malloc(sizeof(ListaColaboracoes));

    cargarDados(listaArtistas, listaObras, listaColaboracoes);
	
    bool executando = true;
    do
	{
        switch (menuPrincipal();)
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
			if(!liberarDados(listaArtistas, listaObras, listaColaboracoes))
			{
				printf("Memória não foi liberada corretamente! Encerrando o programa...");
			}
            executando = false;
            break;

        case -1:
            // Erro detectado, encerrar programa imediatamente após limpar memória, se necessário.
            // Aqui você deve adicionar código para liberar memória alocada dinamicamente, se houver.

			printf("Falha na operação, os dados não foram salvos. Encerrando o programa.");
			
			if(!liberarDados(listaArtistas, listaObras, listaColaboracoes))
			{
				printf("Memória não foi liberada corretamente! Encerrando o programa...");
			}
			executando = false;
            return 1;
        }
		
    } while (executando);

    return 0;
}
