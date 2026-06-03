/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: modulo_obras.c                         *
 * Descrição: Implementação das funcionalidades    *
 *            relacionadas ao gerenciamento de     *
 *            obras de arte.                       *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: ChatGPT (OpenAI), DeepSeek Chat  *
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
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "modulo_obras.h"
#include "defines.h"
#include "estruturas.h"
#include "utils.h"
#include "listas.h"
#include "persistencia.h"

static int menuAlteracaoObras()
{
    printf("### MENU DE ALTERAÇÕES DE OBRAS ###\n");
    printf("Escolha uma opção para alterar:\n");
    printf("1 - Título\n");
    printf("2 - Tipo\n");
    printf("3 - Ano de Criação\n");
    printf("4 - Valor\n");
    printf("5 - Descrição\n");

    printf("6 - Salvar e Retornar ao menu Obras");

    return escolherOpcao(1, 6);
}

static int menuObras()
{
    printf("\n### MENU OBRAS ###\n");
    printf("1 - Cadastrar Obra\n");
    printf("2 - Listar Todas as Obras\n");
    printf("3 - Buscar Obra por ID\n");
    printf("4 - Alterar Obra\n");

    printf("5 - Voltar ao Menu Principal\n");

    return escolherOpcao(1, 5);
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

    if (indiceObraPorID(lista, o.id) != -1)
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

void listarObra(const ListaObras *lista, int indice)
{
    const Obra *o = &lista->itens[indice];
    printf("\n--- Obra %d ---\n", indice + 1);
    printf("ID: %d\n", o->id);
    printf("Título: %s\n", o->titulo);
    printf("Tipo: %s\n", o->tipo);
    printf("Ano: %d\n", o->anoCriacao);
    printf("Valor: "); imprimeValor(o->valorCentavos);
    printf("Descrição: %s\n", o->descricao);
    printf("------------------------\n");
}

void listarTodasObras(const ListaObras *lista)
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

static bool buscarObra(const ListaObras *lista, int *indice)
{
    int id;
    printf("Informe o ID da obra: ");
    if (!lerInteiro(&id))
        return false;

    *indice = indiceObraPorID(lista, id);
    if (*indice == -1)
    {
        printf("Obra não encontrada.\n");
        return true;
    }

    listarObra(lista, *indice);
    return true;
}

static bool moduloAlteracaoObra(ListaObras *lista, int indice)
{
    Obra *o = &lista->itens[indice];
    int op, novoAno, novoValor;
    char confirma[TAM_SIM_NAO], novoTitulo[TAM_TEXTO_PEQUENO], novoTipo[TAM_TEXTO_PEQUENO], novaDescricao[TAM_TEXTO_GRANDE];
    bool executando = true;

    while (executando)
    {
        op = menuAlteracaoObras();
        if (op == -99)
            return false;
        
        switch (op)
        {
            case 1:
                printf("Novo título: ");
                if (!lerString(novoTitulo, TAM_TEXTO_PEQUENO))
                    return false;
                
                printf(MSG_CONFIRMAR_ALTERACAO, o->titulo, novoTitulo);
                if(!lerSimNao(confirma))
                    return false;
                if(confirma[0] == 's' || confirma[0] == 'S')
                {
                    strcpy(o->titulo, novoTitulo);
                    printf(MSG_ALTERACAO_REALIZADA_SUCESSO);
                }
                else
                    printf(MSG_ALTERACAO_CANCELADA);
                break;
            case 2:
                printf("Novo tipo: ");
                if (!lerString(novoTipo, TAM_TEXTO_PEQUENO))
                    return false;
                printf(MSG_CONFIRMAR_ALTERACAO, o->tipo, novoTipo);
                if(!lerSimNao(confirma))
                    return false;
                if(confirma[0] == 's' || confirma[0] == 'S')
                {
                    strcpy(o->tipo, novoTipo);
                    printf(MSG_ALTERACAO_REALIZADA_SUCESSO);
                }
                else
                    printf(MSG_ALTERACAO_CANCELADA);
                break;
            case 3:
                printf("Novo ano de criação: ");
                if (!lerInteiro(&novoAno))
                    return false;
                printf(MSG_CONFIRMAR_ALTERACAO, o->anoCriacao, novoAno);
                if(!lerSimNao(confirma))
                    return false;
                if(confirma[0] == 's' || confirma[0] == 'S')
                {
                    o->anoCriacao = novoAno;
                    printf(MSG_ALTERACAO_REALIZADA_SUCESSO);
                }
                else
                    printf(MSG_ALTERACAO_CANCELADA);
                break;
            case 4:
                printf("Novo valor em centavos: ");

                do
                {
                    if (!lerInteiro(&novoValor))
                        return false;
                    if (novoValor < 0)
                        printf("Valor não pode ser negativo. Tente novamente: ");
                } while (novoValor < 0);

                printf(MSG_CONFIRMAR_ALTERACAO, o->valorCentavos, novoValor);
                if(!lerSimNao(confirma))
                    return false;
                if(confirma[0] == 's' || confirma[0] == 'S')
                {
                    o->valorCentavos = novoValor;
                    printf(MSG_ALTERACAO_REALIZADA_SUCESSO);
                }
                else
                    printf(MSG_ALTERACAO_CANCELADA);
                break;
            case 5:
                printf("Nova descrição: ");
                if (!lerString(novaDescricao, TAM_TEXTO_GRANDE))
                    return false;
                // em implementação
                break;
            case 6:
                executando = false;
                break;
        }
    }
    return true;
}

bool moduloObras(ListaObras *lista)
{
    int indice, op;
    bool executando = true;

    while (executando)
    {
        op = menuObras();
        if (op == -99)
            return false;

        switch (op)
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
                if (!buscarObra(lista, &indice))
                {
                    return false;
                }
                break;
            case 4:
                if(!buscarObra(lista, &indice))
                {
                    return false;
                }
                else
                {
                    indice = indiceObraPorID(lista, indice);
                    if (indice == -1)
                    {
                        printf("Obra não encontrada.\n");
                        break;
                    }

                    if (!moduloAlteracaoObra(lista, indice))
                        return false;
                }
                break;

            case 5:
                executando = false;
                break;
        } // Fim do switch Obras
    }
    return true;
}
