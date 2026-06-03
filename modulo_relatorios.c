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

static int menuRelatorios()
{
    printf("### MENU RELATORIOS ###\n");
    printf("1 - Relatorio de Obras por Artista\n");
    printf("2 - Relatorio de Obras por Tipo\n");
    printf("3 - Relatorio de Colaboracoes por data\n");
    printf("4 - Voltar ao Menu Principal\n");

    return escolherOpcao(1, 4);
}

static bool relatorioObrasPorArtista(const ListaColaboracoes *listaColaboracoes, const ListaObras *listaObras, const ListaArtistas *listaArtistas)
{
    int indiceArtista, i, indiceObra;
    const Artista *artista;
    const Colaboracao *colab;
    const Obra *obra;
    bool encontrouObras;

    printf("\n--- Relatorio de Obras por Artista ---\n");
    if(!buscarArtistaPorCPF(listaArtistas, &indiceArtista))
        return false;

    if (indiceArtista == -1)
        return true;
    else
    {
        artista = &listaArtistas->itens[indiceArtista];
        
        printf("Obras de %s", artista->nome);

        encontrouObras = false;
        for (i = 0; i < listaColaboracoes->total; i++)
        {
            colab = &listaColaboracoes->itens[i];
            if (strcmp(colab->chaveColab.cpf, artista->cpf) == 0)
            {
                encontrouObras = true;
                indiceObra = indiceObraPorID(listaObras, colab->chaveColab.id);
                if (indiceObra != -1)
                {
                    obra = &listaObras->itens[indiceObra];
                    printf("- %s (ID: %d), Papel: %s, Contribuicao: %d%%\n", obra->titulo, obra->id, colab->funcaoArtista, colab->percentualContribuicao);
                }
                else
                {
                    printf("- Obra ID %d (nao encontrada), Papel: %s, Contribuicao: %d%%\n", colab->chaveColab.id, colab->funcaoArtista, colab->percentualContribuicao);
                }
            }
        }
        if (!encontrouObras)
            printf("Nenhuma obra encontrada para este artista.\n");
    }
    return true; 
}



static bool relatorioObrasPorTipo(const ListaColaboracoes *listaColaboracoes, const ListaObras *listaObras, const ListaArtistas *listaArtistas)
{
    int indiceArtista, i, j, indiceObra;
    char tipoInformado[TAM_TEXTO_PEQUENO];
    const Colaboracao *colab;
    const Obra *obra;
    bool encontrouColab;

    printf("\n--- Relatorio de Obras por Tipo ---\n");
    printf("Informe o tipo desejado: ");
    if (!lerString(tipoInformado,TAM_TEXTO_PEQUENO))
        return false;

    // Percorre Obras para encontrar obras que tenham o tipo informado pelo usuário
    for (i = 0; i < listaObras->total; i++)
    {
        encontrouColab = false;
        obra = &listaObras->itens[i];
        if (strcmp(obra->tipo, tipoInformado) == 0)
        {
            // Lista a Obra
            listarObra(listaObras,i);
            printf("  Por:\n");
            // Percorre colaborações buscando os artistas da Obra
            for (j = 0; j < listaColaboracoes->total; j++)
            {
                colab = &listaColaboracoes->itens[j];
                if (colab->chaveColab.id == obra->id && colab->chaveColab.cpf != NULL)
                {
                    indiceArtista = indiceArtistaPorCPF(listaArtistas, colab->chaveColab.cpf);
                    if (indiceArtista != -1)
                    {
                        printf("      %s\n", listaArtistas->itens[indiceArtista].nome);
                        encontrouColab = true;
                    }
                }
            }
            if(!encontrouColab)
                printf("      Artistas não identificado");
        }
    }
    return true; 
}

bool moduloRelatorios(ListaArtistas *listaArtistas, ListaObras *listaObras, ListaColaboracoes *listaColaboracoes)
{
    int op;
    do
    {
        op = menuRelatorios();
        switch (op)
        {
            case 1:
                // Todas as obras de um artista específico, listando o título da obra, a função do artista na obra e o percentual de contribuição.
                relatorioObrasPorArtista(listaColaboracoes, listaObras, listaArtistas);
                break;
            case 2:
                // Todas as obras de um tipo específico (ex: pintura, escultura, etc), listando o título da obra, o nome do artista principal e a data de criação.
                relatorioObrasPorTipo(listaColaboracoes, listaObras, listaArtistas);
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
