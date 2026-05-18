/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: modulo_obras.c                         *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: chat.deepseek.com                *
 * Disciplina: APR2                                *
 * Professora: Dra. Eloize Rossi Marques Seno      *
 ***************************************************/

/****************
 * MÓDULO OBRAS *
 ****************/

/*****************************
 * BIBLIOTECAS E IMPORTAÇÕES *
 *****************************/

#include <stdio.h>
#include <stdbool.h>

#include "modulo_obras.h"
#include "defines.h"
#include "estruturas.h"
#include "utils.h"
#include "listas.h"
#include "persistencia.h"

static int menuObras()
{
    printf("\n### MENU OBRAS ###\n");
    printf("1 - Cadastrar Obra\n");
    printf("2 - Listar Todas as Obras\n");
    printf("3 - Buscar Obra por ID\n");
    printf("4 - Voltar ao Menu Principal\n");

    printf("Escolha uma opção: ");
    return escolherOpcao(1, 4);
}

static bool cadastrarObra(ListaObras *lista)
{
    Obra o;

    printf("\n--- Cadastrar Obra ---\n");

    // ID
    printf("ID da obra (número inteiro positivo): ");
    if (lerInteiro(&o.id) == false)
        return false;

    if (o.id <= 0)
    {
        printf("ID inválido. Deve ser um número positivo.\n");
        return true; // Retorna true para não encerrar o módulo, apenas informar que houve um erro de validação.
    }

    if (buscarObraPorID(lista, o.id) != -1)
    {
        printf("ID já cadastrado.\n");
        return true; // Retorna true para não encerrar o módulo, apenas informar que houve um erro de validação.
    }

    // Ano de criação
    printf("Ano de criação: ");
    if (lerInteiro(&o.anoCriacao) == false)
    {
        return false;
    }

    // Valor em centavos
    printf("Valor em centavos: ");
    if (lerInteiro(&o.valorCentavos) == false)
    {
        return false;
    }
    if (o.valorCentavos < 0)
    {
        printf("Valor não pode ser negativo.\n");
        return true; // Retorna true para não encerrar o módulo, apenas informar que houve um erro de validação.
    }

    // Título
    printf("Título: ");
    if (lerString(o.titulo, TAM_TEXTO_PEQUENO) == false)
    {
        return false;
    }

    // Tipo
    printf("Tipo: ");
    if (lerString(o.tipo, TAM_TEXTO_PEQUENO) == false)
    {
        return false;
    }

    // Descrição
    printf("Descrição: ");
    if (lerString(o.descricao, TAM_TEXTO_GRANDE) == false)
    {
        return false;
    }

    // Adiciona à lista
    if (adicionarObra(lista, &o) == false)
    {
        printf("Erro de memória ao cadastrar obra.\n");
        return true; // Retorna true para não encerrar o módulo, apenas informar que houve um erro de memória.
    }

    printf("Obra cadastrada com sucesso!\n");
    return true;
}

static void listarObra(const ListaObras *lista, int indice)
{
    const Obra *o = &lista->itens[indice];
    printf("\n--- Obra %d ---\n", indice + 1);
    printf("ID: %d\n", o->id);
    printf("Título: %s\n", o->titulo);
    printf("Tipo: %s\n", o->tipo);
    printf("Ano: %d\n", o->anoCriacao);
    printf("Valor: R$ %d,%02d\n", o->valorCentavos / 100, o->valorCentavos % 100);
    printf("Descrição: %s\n", o->descricao);
    printf("------------------------\n");
}

static void listarTodasObras(const ListaObras *lista)
{
    if (lista->total == 0)
    {
        printf("\nNenhuma obra cadastrada.\n");
        return;
    }

    int i;
    for (i = 0; i < lista->total; i++)
        listarObra(lista, i);
}

static bool buscarObra(const ListaObras *lista)
{
    int id;
    printf("ID da obra a buscar: ");
    if (!lerInteiro(&id))
        return false;

    int indice = buscarObraPorID(lista, id);
    if (indice == -1)
    {
        printf("Obra não encontrada.\n");
        return true;
    }

    listarObra(lista, indice);
    return true;
}

bool moduloObras(ListaObras *lista)
{
    bool executando = true;

    while (executando)
    {
        switch (menuObras())
        {
            case 1:
                if (!cadastrarObra(lista))
                {
                    return false;
                }
                break;
            case 2:
                listarTodasObras(lista);
                break;
            case 3:
                if (!buscarObra(lista))
                {
                    return false;
                }
                break;
            case 4:
                executando = false;
                break;
            case -1:
                return false;
        }
    }
    return true;
}
