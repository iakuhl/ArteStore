/****************************************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes                          *
 * Arquivo: modulo_artistas.c                                               *
 * Autor: Iano de Oliva Kuhlmann                                            *
 * Colaboradores: chat.deepseek.com                                         *
 * Link de colaboração: https://chat.deepseek.com/share/jil3nf8yyu9wwz0h8l  *
 * Disciplina: APR2                                                         *
 * Professora: Dra. Eloize Rossi Marques Seno                               *
 ****************************************************************************/

/*****************************************
 * ARQUIVO DE FUNÇÕES DO MÓDULO ARTISTAS *
 *****************************************/

/*****************************
 * BIBLIOTECAS E IMPORTAÇÕES *
 *****************************/

#include <stdio.h>
#include <stdbool.h>
#include "defines.h"
#include "estruturas.h"

#include "modulo_artistas.h"
#include "utils.h"
#include "listas.h"
#include "persistencia.h"

// Protótipos das funções internas
static bool cadastrarArtista(ListaArtistas *lista);
static void listarArtistas(const ListaArtistas *lista);
static void buscarArtista(const ListaArtistas *lista);
// static void editarArtista(ListaArtistas *lista);  // se desejar implementar depois
// static void excluirArtista(ListaArtistas *lista);

static int menuArtistas()
{
    printf("### MENU ARTISTAS ###\n");
    printf("1 - Cadastrar Artista\n");
    printf("2 - Listar Artistas\n");
    printf("3 - Buscar Artista por CPF\n");
    printf("4 - Voltar ao Menu Principal\n");

    printf("Escolha uma opção: ");
    return escolherOpcao(1, 4);
}

void moduloArtistas(ListaArtistas *lista)
{
    bool executando = true;
    while (executando)
    {
        switch (menuArtistas())
        {
            case 1:
                if(!cadastrarArtista(lista))
                    printf("Erro ao cadastrar artista. Tente novamente.\n"); 
                else
                    salvarArtistas(lista);
                break;

            case 2:
                listarArtistas(lista);
                break;

            case 3:
                buscarArtista(lista);
                break;

            case 4:  // Voltar ao menu principal
                executando = false;
                break;

            case -1: // Erro irrecuperável
                executando = false;
                break;
        }
    }
}

static bool cadastrarArtista(ListaArtistas *lista) // Dados obrigatórios, apenas.
{
    Artista a;

    printf("--- Cadastrar Artista ---\n");

    printf("CPF: ");
    if (!lerString(a.cpf, TAM_CPF))
        return false;
    if (!removeMascaraCPF(a.cpf))
    {
        printf("CPF inválido.\n");
        return false;
    }

    // Verifica duplicidade
    if (buscarArtistaPorCPF(lista, a.cpf) != -1)
    {
        printf("CPF já cadastrado.\n");
        return false;
    }

    // Nome
    printf("Nome: ");
    if (lerString(a.nome, TAM_TEXTO_PEQUENO) == false)
        return false;

    // Nacionalidade
    printf("Nacionalidade: ");
    if (lerString(a.nacionalidade, TAM_TEXTO_PEQUENO) == false)
        return false;

    // Estilo
    printf("Estilo: ");
    if (lerString(a.estilo, TAM_TEXTO_PEQUENO) == false)
        return false;

    // Data de nascimento
    printf("Data de nascimento:\n");
    printf("  Dia: ");
    if (lerInteiro(&a.nascimento.dia) == false)
        return false;
    printf("  Mês: ");
    if (lerInteiro(&a.nascimento.mes) == false)
        return false;
    printf("  Ano: ");
    if (lerInteiro(&a.nascimento.ano) == false)
        return false;

    // Telefones (ao menos um obrigatório; você pode definir como regra)
    a.telefones = NULL;
    a.totalTelefones = 0;
    a.capacidadeTelefones = 0;

    printf("Telefones (deixe vazio para encerrar):\n");
    while (true)
    {
        char telefone[TAM_TELEFONE];
        printf("  Telefone %d: ", a.totalTelefones + 1);
        if (lerString(telefone, TAM_TELEFONE) == false)
        {
            // EOF ou erro: liberar e sair
            free(a.telefones);
            return false;
        }
        if (strlen(telefone) == 0)
            break;

        Telefone *temp = realloc(a.telefones, sizeof(Telefone) * (a.totalTelefones + 1));
        if (temp == NULL)
        {
            free(a.telefones);
            printf("Erro de memória.\n");
            return false;
        }
        a.telefones = temp;
        strncpy(a.telefones[a.totalTelefones].numeroTelefone, telefone, TAM_TELEFONE - 1);
        a.telefones[a.totalTelefones].numeroTelefone[TAM_TELEFONE - 1] = '\0';
        a.totalTelefones++;
    }
    a.capacidadeTelefones = a.totalTelefones;

    // Redes sociais (opcionais)
    a.redesSociais = NULL;
    a.totalRedesSociais = 0;
    a.capacidadeRedesSociais = 0;

    printf("Redes sociais (deixe plataforma vazia para encerrar):\n");
    while (true)
    {
        char plataforma[TAM_TEXTO_PEQUENO];
        char usuario[TAM_TEXTO_PEQUENO];

        printf("  Plataforma: ");
        if (lerString(plataforma, TAM_TEXTO_PEQUENO) == false)
        {
            free(a.telefones);
            free(a.redesSociais);
            return false;
        }
        if (strlen(plataforma) == 0)
            break;

        printf("  Usuário: ");
        if (lerString(usuario, TAM_TEXTO_PEQUENO) == false)
        {
            free(a.telefones);
            free(a.redesSociais);
            return false;
        }

        redeSocial *temp = realloc(a.redesSociais, sizeof(redeSocial) * (a.totalRedesSociais + 1));
        if (temp == NULL)
        {
            free(a.telefones);
            free(a.redesSociais);
            printf("Erro de memória.\n");
            return false;
        }
        a.redesSociais = temp;
        strncpy(a.redesSociais[a.totalRedesSociais].redeSocial, plataforma, TAM_TEXTO_PEQUENO - 1);
        a.redesSociais[a.totalRedesSociais].redeSocial[TAM_TEXTO_PEQUENO - 1] = '\0';
        strncpy(a.redesSociais[a.totalRedesSociais].usuario, usuario, TAM_TEXTO_PEQUENO - 1);
        a.redesSociais[a.totalRedesSociais].usuario[TAM_TEXTO_PEQUENO - 1] = '\0';
        a.totalRedesSociais++;
    }
    a.capacidadeRedesSociais = a.totalRedesSociais;

    // Adiciona à lista
    if (adicionarArtista(lista, &a) == false)
    {
        printf("Erro ao adicionar artista à lista.\n");
        free(a.telefones);
        free(a.redesSociais);
        return false;
    }

    printf("Artista cadastrado com sucesso!\n");
    return true;
}

static void listarArtistas(const ListaArtistas *lista)
{
    if (lista->total == 0)
    {
        printf("Nenhum artista cadastrado.\n");
        return;
    }
    int i;
    for (i = 0; i < lista->total; i++)
    {
        const Artista *a = &lista->itens[i];
        printf("\n--- Artista %d ---\n", i + 1);
        printf("CPF: %s\n", a->cpf);
        printf("Nome: %s\n", a->nome);
        printf("Nascimento: %02d/%02d/%d\n", a->nascimento.dia, a->nascimento.mes, a->nascimento.ano);
        printf("Estilo: %s\n", a->estilo);
        printf("Nacionalidade: %s\n", a->nacionalidade);

        if (a->totalTelefones > 0)
        {
            printf("Telefones:\n");
            int j;
            for (j = 0; j < a->totalTelefones; j++)
            {
                printf("  %s\n", a->telefones[j].numeroTelefone);
            }
        }

        if (a->totalRedesSociais > 0)
        {
            printf("Redes Sociais:\n");
            int j;
            for (j = 0; j < a->totalRedesSociais; j++)
            {
                printf("  %s: %s\n", a->redesSociais[j].redeSocial, a->redesSociais[j].usuario);
            }
        }
        printf("------------------------\n");
    }
}

static void buscarArtista(const ListaArtistas *lista)
{
    char cpf[TAM_CPF];
    printf("Buscar por CPF: ");
    if (lerString(cpf, TAM_CPF) == false)
        return;
    if (removeMascaraCPF(cpf) == false)
    {
        printf("CPF inválido.\n");
        return;
    }

    int indice = buscarArtistaPorCPF(lista, cpf);
    if (indice == -1)
    {
        printf("Artista não encontrado.\n");
        return;
    }

    const Artista *a = &lista->itens[indice];
    printf("\n--- Artista encontrado ---\n");
    printf("CPF: %s\n", a->cpf);
    printf("Nome: %s\n", a->nome);
    printf("Nascimento: %02d/%02d/%d\n", a->nascimento.dia, a->nascimento.mes, a->nascimento.ano);
    printf("Estilo: %s\n", a->estilo);
    printf("Nacionalidade: %s\n", a->nacionalidade);
    if (a->totalTelefones > 0)
    {
        printf("Telefones:\n");
        int j;
        for (j = 0; j < a->totalTelefones; j++)
        {
            printf("  %s\n", a->telefones[j].numeroTelefone);
        }
    }
    if (a->totalRedesSociais > 0)
    {
        printf("Redes Sociais:\n");
        int j;
        for (j = 0; j < a->totalRedesSociais; j++)
        {
            printf("  %s: %s\n", a->redesSociais[j].redeSocial, a->redesSociais[j].usuario);
        }
    }
    printf("---------------------------\n");
}
