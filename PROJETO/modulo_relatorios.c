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
#include <string.h>

#include "modulo_artistas.h"
#include "modulo_obras.h"
#include "modulo_colaboracoes.h"
#include "modulo_relatorios.h"
#include "defines.h"
#include "estruturas.h"
#include "utils.h"
#include "listas.h"
#include "persistencia.h"


static bool relatorioObrasPorArtista(const ListaColaboracoes *listaColaboracoes, const ListaObras *listaObras, const ListaArtistas *listaArtistas)
{
    int indiceArtista;
    printf("\n--- Relatorio de Obras por Artista ---\n");
    if(!buscarArtistaPorCPF(listaArtistas, &indiceArtista))
    {
        return false;
    }
    if (indiceArtista == -1)
    {
        printf("Artista nao encontrado.\n");
        return true;
    }
    else
    {
        const Artista *artista = &listaArtistas->itens[indiceArtista];
        const Colaboracao *colab;
        const Obra *obra;
        bool encontrouObras;

        int i, indiceObra;

        printf("Obras do artista %s (CPF: ", artista->nome);
        imprimeCPF(artista->cpf);
        printf("):\n");

        encontrouObras = false;
        for (i = 0; i < listaColaboracoes->total; i++)
        {
            colab = &listaColaboracoes->itens[i];
            if (strcmp(colab->chaveColab.cpf, artista->cpf) == 0)
            {
                encontrouObras = true;
                obra = NULL;
                indiceObra = buscarObraPorID(listaObras, colab->chaveColab.id);
                if (indiceObra != -1)
                {
                    obra = &listaObras->itens[indiceObra];
                    printf("- %s (ID: %d), Papel: %s, Contribuicao: %.2f%%\n", obra->titulo, obra->id, colab->funcaoArtista, colab->percentualContribuicao);
                }
                else
                {
                    printf("- Obra ID %d (nao encontrada), Papel: %s, Contribuicao: %.2f%%\n", colab->chaveColab.id, colab->funcaoArtista, colab->percentualContribuicao);
                }
            }
        }
        if (!encontrouObras)
        {
            printf("Nenhuma obra encontrada para este artista.\n");
        }
    }
    return true; 
}

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
                relatorioObrasPorArtista(listaColaboracoes, listaObras, listaArtistas);
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
