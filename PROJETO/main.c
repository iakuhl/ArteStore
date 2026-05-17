/****************************************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes                          *
 * Arquivo: main.c                                                          *
 * Autor: Iano de Oliva Kuhlmann                                            *
 * Colaboradores: chat.deepseek.com                                         *
 * Link de colaboração: https://chat.deepseek.com/share/jil3nf8yyu9wwz0h8l  *
 * Disciplina: APR2                                                         *
 * Professora: Dra. Eloize Rossi Marques Seno                               *
 ****************************************************************************/

/*********************
 * ARQUIVO PRINCIPAL *
 *********************/

/*****************************
 * BIBLIOTECAS E IMPORTAÇÕES *
 *****************************/

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "utils.h"
#include "estruturas.h"
#include "persistencia.h"
#include "listas.h"

// Funções de carregamento e liberação de dados, utilizam as funções de persistência e listas para gerenciar os dados em memória.
void carregarDados(ListaArtistas *listaArtistas, ListaObras *listaObras, ListaColaboracoes *listaColaboracoes)
{ 
    carregarArtistas(listaArtistas);
    //carregarObras(listaObras);
    //carregarColaboracoes(listaColaboracoes);
}

void liberarDados(ListaArtistas *listaArtistas, ListaObras *listaObras, ListaColaboracoes *listaColaboracoes)
{
	liberarListaArtistas(listaArtistas);
	//liberarListaObras(listaObras);
	//liberarListaColaboracoes(listaColaboracoes);
}

int menuPrincipal()
{
    printf("### MENU PRINCIPAL ###\n");
    printf("1 - Menu Artistas\n");
    printf("2 - Menu Obras\n");
    printf("3 - Menu Colaboracoes\n");
    printf("4 - Menu Relatorios\n");
    printf("5 - Encerrar Programa\n");

    printf("Escolha uma opção: ");
    return escolherOpcao(1, 5);
}

int main()
{
    // Declaração das listas principais
    ListaArtistas listaArtistas;
    ListaObras listaObras;
    ListaColaboracoes listaColaboracoes;

    // Inicialização das listas com capacidade inicial (pode ser ajustada conforme necessidade)
    carregarDados(&listaArtistas, &listaObras, &listaColaboracoes);
	
    bool executando = true;
    do
	{
        switch (menuPrincipal())
        {
        case 1:
            moduloArtistas(&listaArtistas);
			salvarArtistas(&listaArtistas);
            break;

        case 2:
			//moduloObras(&listaObras);
			//salvarObras(&listaObras);
			break;

        case 3:
			//moduloColaboracoes(&listaColaboracoes, &listaArtistas, &listaObras);
			//salvarColaboracoes(&listaColaboracoes);
			break;

        case 4:
			//moduloRelatorios(&listaArtistas, &listaObras, &listaColaboracoes);
			break;

        case 5:
			printf("Encerrando programa...");
			salvarArtistas(&listaArtistas);
			//salvarObras(&listaObras);
			//salvarColaboracoes(&listaColaboracoes);
            executando = false;
            break;

        case -1:
			printf("Erro Irrecuperável, encerrando sem salvar.\n");
			executando = false;
            break;
        } // fim switch

    } while (executando);

	// Libera memória das listas antes de encerrar o programa.
	liberarDados(&listaArtistas, &listaObras, &listaColaboracoes);
    return 0;
}
