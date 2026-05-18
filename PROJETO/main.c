/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: main.c                                 *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: chat.deepseek.com                *
 * Disciplina: APR2                                *
 * Professora: Dra. Eloize Rossi Marques Seno      *
 ***************************************************/

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
#include "defines.h"
#include "persistencia.h"
#include "listas.h"
#include "modulo_artistas.h"
#include "modulo_obras.h"
#include "modulo_colaboracoes.h"
#include "modulo_relatorios.h"

// Funções de carregamento e liberação de dados, utilizam as funções de persistência e listas para gerenciar os dados em memória.
void carregarDados(ListaArtistas *listaArtistas, ListaObras *listaObras, ListaColaboracoes *listaColaboracoes)
{
    printf("Carregando %s:\n", NOME_ARQUIVO_ARTISTAS);
    int carregadosArtistas = carregarArtistas(listaArtistas);
    switch (carregadosArtistas)
    {
        case -3:
            printf(MSG_ARQUIVO_CARREGADO); // Lista de artistas carregada com sucesso.
            break;
        case -2:
            printf(MSG_ARQUIVO_NAO_ENCONTRADO); // Arquivo não encontrado, lista iniciada vazia
            break;
        case -1:
            printf(MSG_ARQUIVO_VAZIO); // Arquivo vazio, lista iniciada vazia
            break;
        default:
            printf("Falha ao carregar artistas, foram carregados %d artistas com sucesso.\n", carregadosArtistas);
            printf("Lista iniciada com os artistas carregados.");
            break;
    } // Fim switch Artistas

    printf("\n\nCarregando %s:\n", NOME_ARQUIVO_OBRAS);
    int carregadosObras = carregarObras(listaObras);
    switch (carregadosObras)
    {
        case -3:
            printf(MSG_ARQUIVO_CARREGADO); // Lista de obras carregada com sucesso.
            break;
        case -2:
            printf(MSG_ARQUIVO_NAO_ENCONTRADO); // Arquivo não encontrado, lista iniciada vazia
            break;
        case -1:
            printf(MSG_ARQUIVO_VAZIO); // Arquivo vazio, lista iniciada vazia
            break;
        default:
            printf("Falha ao carregar obras, foram carregadas %d obras com sucesso.\n", carregadosObras);
            printf("Lista iniciada com as obras carregadas.");
            break;
    } // Fim switch Obras

    printf("\n\nCarregando %s:\n", NOME_ARQUIVO_COLABORACOES);
    int carregadosColaboracoes = carregarColaboracoes(listaColaboracoes);
    switch (carregadosColaboracoes)
    {
        case -3:
            printf(MSG_ARQUIVO_CARREGADO); // Lista de colaboracoes carregada com sucesso.
            break;
        case -2:
            printf(MSG_ARQUIVO_NAO_ENCONTRADO); // Arquivo não encontrado, lista iniciada vazia
            break;
        case -1:
            printf(MSG_ARQUIVO_VAZIO); // Arquivo vazio, lista iniciada vazia
            break;
        default:
            printf("Falha ao carregar colaboracoes, foram carregadas %d colaboracoes com sucesso.\n", carregadosColaboracoes);
            printf("Lista iniciada com as colaboracoes carregadas.");
    } // Fim switch Colaboracoes
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
            if(!moduloArtistas(&listaArtistas))
            {
                printf(MSG_LOOP_INFINITO);
                executando = false;
                break;
            }
            break;

        case 2:
			moduloObras(&listaObras);
			salvarObras(&listaObras);
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
			printf(MSG_LOOP_INFINITO);
			executando = false;
            break;
        } // fim switch

    } while (executando);

	// Libera memória das listas antes de encerrar o programa.
	liberarDados(&listaArtistas, &listaObras, &listaColaboracoes);
    return 0;
}
