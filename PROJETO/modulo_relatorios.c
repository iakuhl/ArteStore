/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: modulo_relatorios.c                    *
 * Descrição: Implementação das funcionalidades    *
 *            de geração, consulta e exibição dos  *
 *            relatórios do sistema.               *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: ChatGPT (OpenAI), DeepSeek Chat  *
 * Disciplina: APR2                                *
 * Professora: Dra. Eloize Rossi Marques Seno      *
 ***************************************************/

/*********************
 * MÓDULO RELATÓRIOS *
 *********************/

/*****************************
 * BIBLIOTECAS E IMPORTAÇÕES *
 *****************************/

#include <stdio.h>
#include <stdbool.h>

#include "modulo_relatorios.h"
#include "defines.h"
#include "estruturas.h"
#include "utils.h"
#include "listas.h"
#include "persistencia.h"

static int menuRelatorios()
{
    printf("### MENU RELATORIOS ###\n");
    printf("1 - Relatorio de Artistas\n");
    printf("2 - Relatorio de Obras\n");
    printf("3 - Relatorio de Colaboracoes\n");
    printf("4 - Voltar ao Menu Principal\n");

    printf("Escolha uma opção: ");
    return escolherOpcao(1, 4);
}

bool moduloRelatorios(ListaArtistas *listaArtistas, ListaObras *listaObras, ListaColaboracoes *listaColaboracoes)
{
    do
    {
        switch (menuRelatorios())
        {
            case 1:
                // Todas as obras de um artista específico, listando o título da obra, a função do artista na obra e o percentual de contribuição.
                break;
            case 2:
                // Todas as obras de um tipo específico (ex: pintura, escultura, etc), listando o título da obra, o nome do artista principal e a data de criação.
                break;
            case 3:
                // Todas as colaborações realizadas entre as datas X e Y (informadas pelo usuário), listando o nome do artista, o título da obra, o papel desempenhado por cada artista e o percentual de contribuição.
                break;
            case 4:
                printf("Voltando ao Menu Principal...\n");
                return true;
        }
    } while (true);
}
