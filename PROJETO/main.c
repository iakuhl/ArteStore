/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: main.c                                 *
 * Descrição: Ponto de entrada do sistema,         *
 *            controle do menu principal e fluxo   *
 *            geral da aplicação.                  *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: ChatGPT (OpenAI), DeepSeek Chat  *
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
// Mensagens personalizadas para cada retorno possível (exemplo: Lista vazia, lista semi-preenchida, arquivo corrompido...)

/************************************************************************************************************
 * FUNÇÕES DE CARREGAMENTO/ SALVAMENTO E LIBERAÇÃO DE DADOS, ITERAGEM DIRETAMENTE COM PERSISTÊNCIA E LISTAS *
 ************************************************************************************************************/

bool carregarDados(ListaArtistas *listaArtistas, ListaObras *listaObras, ListaColaboracoes *listaColaboracoes)
{
    // CARREGAMENTO DE ARTISTAS
    printf("Carregando %s:\n", NOME_ARQUIVO_ARTISTAS);

    int artistasCarregados = carregarArtistas(listaArtistas);
    // Se -99, houve um erro crítico (arquivo corrompido, leitura falhou, etc) e o programa não pode continuar.
    if (artistasCarregados == -99)
        return false;

    if (artistasCarregados >= 0)
    {
        printf("Falha ao carregar lista de artistas. "
               "Foram carregados %d artistas com sucesso.\n",
               artistasCarregados);

        printf("Lista iniciada com os artistas carregados:\n");
        listarTodosArtistas(listaArtistas);
    }


    // CARREGAMENTO DE OBRAS
    printf("\n\nCarregando %s:\n", NOME_ARQUIVO_OBRAS);

    int obrasCarregadas = carregarObras(listaObras);
    // Se -99, houve um erro crítico (arquivo corrompido, leitura falhou, etc) e o programa não pode continuar.
    if (obrasCarregadas == -99)
        return false;

    if (obrasCarregadas >= 0)
    {
        printf("Falha ao carregar lista de obras. "
               "Foram carregadas %d obras com sucesso.\n",
               obrasCarregadas);

        printf("Lista iniciada com as obras carregadas.\n");
    }


    // CARREGAMENTO DE COLABORAÇÕES
    printf("\n\nCarregando %s:\n", NOME_ARQUIVO_COLABORACOES);

    int colaboracoesCarregadas = carregarColaboracoes(listaColaboracoes);
    // Se -99, houve um erro crítico (arquivo corrompido, leitura falhou, etc) e o programa não pode continuar.
    if (colaboracoesCarregadas == -99)
        return false;

    if (colaboracoesCarregadas >= 0)
    {
        printf("Falha ao carregar lista de colaborações. "
               "Foram carregadas %d colaborações com sucesso.\n",
               colaboracoesCarregadas);

        printf("Lista iniciada com as colaborações carregadas.\n");
    }
    return true;
}

void liberarDados(ListaArtistas *listaArtistas, ListaObras *listaObras, ListaColaboracoes *listaColaboracoes)
{
	liberarListaArtistas(listaArtistas);
	liberarListaObras(listaObras);
	liberarListaColaboracoes(listaColaboracoes);
}

void salvarDados(ListaArtistas *listaArtistas, ListaObras *listaObras, ListaColaboracoes *listaColaboracoes)
{
	salvarArtistas(&listaArtistas);
	salvarObras(&listaObras);
	salvarColaboracoes(&listaColaboracoes);
}


/*****************************************
 * MENU PRINCIPAL E EXECUÇÃO DO PROGRAMA *
 *****************************************/

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
	
    bool executando = true;
	if(carregarDados(&listaArtistas, &listaObras, &listaColaboracoes))
	{
	    do
		{
	        switch (menuPrincipal())
	        {
	        case 1:
	            if(!moduloArtistas(&listaArtistas))
	            {
	                printf(MSG_LOOP_INFINITO);
	                executando = false;
	            }
                else
                {
                    printf("Salvando dados...\n");
                    salvarArtistas(&listaArtistas);
                }
	            break;
	
	        case 2:
				if(!moduloObras(&listaObras))
				{
					printf(MSG_LOOP_INFINITO);
					executando = false;
				}
                else
                {
                    printf("Salvando dados...\n");
                    salvarObras(&listaObras);
                }
				break;
	
	        case 3:
				if(!moduloColaboracoes(&listaColaboracoes, &listaArtistas, &listaObras))
				{
					printf(MSG_LOOP_INFINITO);
					executando = false;
				}
                else
                {
                    printf("Salvando dados...\n");
                    salvarColaboracoes(&listaColaboracoes);
                }
				break;
	
	        case 4:
				if(!moduloRelatorios(&listaArtistas, &listaObras, &listaColaboracoes))
				{
					printf(MSG_LOOP_INFINITO);
					executando = false;
				}
                else
                {
                    printf("Salvando dados...\n");
                    salvarDados(&listaArtistas, &listaObras, &listaColaboracoes);
                }
				break;
	
	        case 5:
				printf("Encerrando programa...");
				salvarDados(&listaArtistas, &listaObras, &listaColaboracoes);
	            executando = false;
	            break;
	
	        case -99:
				printf(MSG_LOOP_INFINITO);
				executando = false;
	            break;
	        } // fim switch
	
	    } while (executando);
	}
	
	// Libera memória das listas antes de encerrar o programa.
	liberarDados(&listaArtistas, &listaObras, &listaColaboracoes);
    return 0;
}
