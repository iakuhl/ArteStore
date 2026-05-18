/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: modulo_colaboracoes.c                  *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: chat.deepseek.com                *
 * Disciplina: APR2                                *
 * Professora: Dra. Eloize Rossi Marques Seno      *
 ***************************************************/

/***********************
 * MÓDULO COLABORACOES *
 ***********************/

/*****************************
 * BIBLIOTECAS E IMPORTAÇÕES *
 *****************************/

#include <stdio.h>
#include <stdbool.h>

#include "modulo_colaboracoes.h"
#include "defines.h"
#include "estruturas.h"
#include "utils.h"
#include "listas.h"
#include "persistencia.h"

static int menuColaboracoes()
{
    printf("\n### MENU COLABORACOES ###\n");
    printf("1 - Cadastrar Colaboracao\n");
    printf("2 - Listar Colaboracoes\n");
    printf("3 - Voltar ao Menu Principal\n");

    printf("Escolha uma opção: ");
    return escolherOpcao(1, 3);
}

static bool cadastrarColaboracao(ListaColaboracoes *listaColaboracoes, const ListaArtistas *listaArtistas, const ListaObras *listaObras)
{
    // Verifica se existem artistas e obras cadastrados
    if (listaArtistas->total == 0)
    {
        printf("\nNao ha artistas cadastrados. Impossivel cadastrar colaboracao.\n");
        return false;
    }
    if (listaObras->total == 0)
    {
        printf("\nNao ha obras cadastradas. Impossivel cadastrar colaboracao.\n");
        return false;
    }

    Colaboracao c;
    char cpfDigitado[TAM_CPF];
    int idObra;
    int indiceArtista;
    int indiceObra;

    printf("\n--- Cadastrar Colaboracao ---\n");

    // CPF do artista
    printf("CPF do artista: ");
    if (lerString(cpfDigitado, TAM_CPF) == false)
        return false;
    
    if (removeMascaraCPF(cpfDigitado) == false)
    {
        printf("CPF invalido.\n");
        return true; // Retorna true para não encerrar o módulo, apenas informar que o CPF é inválido.
    }

    indiceArtista = buscarArtistaPorCPF(listaArtistas, cpfDigitado);
    if (indiceArtista == -1)
    {
        printf("Artista nao encontrado.\n");
        return true; // Retorna true para não encerrar o módulo, apenas informar que o artista não foi encontrado.
    }

    // ID da obra
    printf("ID da obra: ");
    if (lerInteiro(&idObra) == false)
        return false;

    if (idObra <= 0)
    {
        printf("ID invalido.\n");
        return true; // Retorna true para não encerrar o módulo, apenas informar que o ID é inválido.
    }

    indiceObra = buscarObraPorID(listaObras, idObra);
    if (indiceObra == -1)
    {
        printf("Obra nao encontrada.\n");
        return true; // Retorna true para não encerrar o módulo, apenas informar que a obra não foi encontrada.
    }

    // Verifica se já existe colaboração com mesma chave (cpf + idObra)
    if (buscarColaboracao(listaColaboracoes, cpfDigitado, idObra) != -1)
    {
        printf("Ja existe uma colaboracao entre este artista e esta obra.\n");
        return true; // Retorna true para não encerrar o módulo, apenas informar que a colaboração já existe.
    }

    // Função do artista
    printf("Funcao do artista: ");
    if (lerString(c.funcaoArtista, TAM_TEXTO_PEQUENO) == false)
        return false;

    // Percentual de contribuição
    printf("Percentual de contribuicao (1-100): ");
    if (lerInteiro(&c.percentualContribuicao) == false)
    {
        return false;
    }
    if (c.percentualContribuicao < 1 || c.percentualContribuicao > 100)
    {
        printf("Percentual invalido. Deve estar entre 1 e 100.\n");
        return true; // Retorna true para não encerrar o módulo, apenas informar que o percentual é inválido.
    }

    // Data de entrada
    printf("Data de entrada:\n");

    printf("  Dia: ");
    if (lerInteiro(&c.entrada.dia) == false)
        return false;

    printf("  Mes: ");
    if (lerInteiro(&c.entrada.mes) == false)
        return false;

    printf("  Ano: ");
    if (lerInteiro(&c.entrada.ano) == false)
        return false;

    // Data de saída
    printf("Data de saida:\n");

    printf("  Dia: ");
    if (lerInteiro(&c.saida.dia) == false)
        return false;

    printf("  Mes: ");
    if (lerInteiro(&c.saida.mes) == false)
        return false;

    printf("  Ano: ");
    if (lerInteiro(&c.saida.ano) == false)
        return false;

    // Copia os dados da chave para a colaboração
    strncpy(c.chaveColab.cpf, cpfDigitado, TAM_CPF - 1);
    c.chaveColab.cpf[TAM_CPF - 1] = '\0';
    c.chaveColab.id = idObra;

    // Adiciona à lista
    if (adicionarColaboracao(listaColaboracoes, &c) == false)
    {
        printf("Erro de memoria ao cadastrar colaboracao.\n");
        return true; // Retorna true para não encerrar o módulo, apenas informar que houve um erro de memória.
    }

    printf("Colaboracao cadastrada com sucesso!\n");
}

static void listarColaboracoes(const ListaColaboracoes *listaColaboracoes, const ListaArtistas *listaArtistas, const ListaObras *listaObras)
{
    if (listaColaboracoes->total == 0)
    {
        printf("\nNenhuma colaboracao cadastrada.\n");
        return;
    }

    for (int i = 0; i < listaColaboracoes->total; i++)
    {
        const Colaboracao *colab = &listaColaboracoes->itens[i];
        const Artista *artista = NULL;
        const Obra *obra = NULL;

        // Busca o artista correspondente
        int indiceArtista = buscarArtistaPorCPF(listaArtistas, colab->chaveColab.cpf);
        if (indiceArtista != -1)
        {
            artista = &listaArtistas->itens[indiceArtista];
        }

        // Busca a obra correspondente
        int indiceObra = buscarObraPorID(listaObras, colab->chaveColab.id);
        if (indiceObra != -1)
        {
            obra = &listaObras->itens[indiceObra];
        }

        printf("\n--- Colaboracao %d ---\n", i + 1);
        if (artista != NULL)
        {
            printf("Artista: %s (CPF: %s)\n", artista->nome, colab->chaveColab.cpf);
        }
        else
        {
            printf("Artista: CPF %s (nao encontrado)\n", colab->chaveColab.cpf);
        }
        if (obra != NULL)
        {
            printf("Obra: %s (ID: %d)\n", obra->titulo, colab->chaveColab.id);
        }
        else
        {
            printf("Obra: ID %d (nao encontrada)\n", colab->chaveColab.id);
        }
        printf("Funcao: %s\n", colab->funcaoArtista);
        printf("Contribuicao: %d%%\n", colab->percentualContribuicao);
        printf("Entrada: %02d/%02d/%d\n", colab->entrada.dia, colab->entrada.mes, colab->entrada.ano);
        printf("Saida: %02d/%02d/%d\n", colab->saida.dia, colab->saida.mes, colab->saida.ano);
        printf("------------------------\n");
    }
}

bool moduloColaboracoes(ListaColaboracoes *listaColaboracoes, const ListaArtistas *listaArtistas, const ListaObras *listaObras)
{
    int opcao;
    bool executando = true;

    while (executando)
    {
        opcao = menuColaboracoes();

        switch (opcao)
        {
            case 1:
                if(!cadastrarColaboracao(listaColaboracoes, listaArtistas, listaObras))
                {
                    printf("Erro ao cadastrar colaboracao. Encerrando sem salvar.\n");
                    return false;
                }
                else
                    salvarColaboracoes(listaColaboracoes);
                 break;

            case 2:
                listarColaboracoes(listaColaboracoes, listaArtistas, listaObras);
                break;

            case 3:
                executando = false;
                break;

            case -1:
                executando = false;
                break;
        }
    }
    return true;
}
