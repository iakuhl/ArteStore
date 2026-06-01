/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: modulo_artistas.c                       *
 * Descrição: Implementação das funcionalidades    *
 *            relacionadas ao gerenciamento de     *
 *            artistas.                            *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: ChatGPT (OpenAI), DeepSeek Chat  *
 * Disciplina: APR2                                *
 * Professora: Dra. Eloize Rossi Marques Seno      *
 ***************************************************/

/*******************
 * MÓDULO ARTISTAS *
 *******************/

/*****************************
 * BIBLIOTECAS E IMPORTAÇÕES *
 *****************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "modulo_artistas.h"
#include "defines.h"
#include "estruturas.h"
#include "utils.h"
#include "listas.h"
#include "persistencia.h"

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

static bool cadastrarArtista(ListaArtistas *lista)
{
    Artista a;
    char resposta[3]; // Para ler a resposta de continuar adicionando telefones ou redes sociais

    printf("--- Cadastrar Artista ---\n");

    printf("CPF: ");
    bool cpfValido = false;
    do{
        if (!lerString(a.cpf, TAM_CPF))
            return false;
        if (!removeMascaraCPF(a.cpf))
        {
            printf("CPF inválido. Tente novamente: ");
            continue;
        }
        if (buscarArtistaPorCPF(lista, a.cpf) != -1)
        {
            printf("CPF já cadastrado. Tente novamente: ");
            continue;
        }
        cpfValido = true;
    }while (!cpfValido);

    // Nome
    printf("Nome: ");
    if (!lerString(a.nome, TAM_TEXTO_PEQUENO))
        return false;

    // Nacionalidade
    printf("Nacionalidade: ");
    if (!lerString(a.nacionalidade, TAM_TEXTO_PEQUENO))
        return false;

    // Estilo
    printf("Estilo: ");
    if (!lerString(a.estilo, TAM_TEXTO_PEQUENO))
        return false;

    // Data de nascimento
    printf("Data de nascimento:\n");
    printf("  Dia: ");
    if (!lerInteiro(&a.nascimento.dia))
        return false;
    printf("  Mês: ");
    if (!lerInteiro(&a.nascimento.mes))
        return false;
    printf("  Ano: ");
    if (!lerInteiro(&a.nascimento.ano))
        return false;

        
    a.telefones = NULL;
    a.totalTelefones = 0;
    a.capacidadeTelefones = 0;

    do
    {
        char telefone[TAM_TELEFONE];
        printf("  Telefone %d: ", a.totalTelefones + 1);
        if (!lerString(telefone, TAM_TELEFONE))
        {
            // EOF ou erro: liberar e sair
            free(a.telefones);
            return false;
        }

        Telefone *temp = (Telefone *) realloc(a.telefones, sizeof(Telefone) * (a.totalTelefones + 1));
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
        
        printf("Deseja adicionar outro telefone? (s/n): ");
        if (!lerSimNao(resposta))
        {
            free(a.telefones);
            return false;
        }
    }while (resposta[0] == 's' || resposta[0] == 'S');
    a.capacidadeTelefones = a.totalTelefones;

    // Redes sociais (opcionais)
    a.redesSociais = NULL;
    a.totalRedesSociais = 0;
    a.capacidadeRedesSociais = 0;

    printf("Deseja adicionar redes sociais? (s/n): ");
    if (!lerSimNao(resposta))
    {
        free(a.telefones);
        return false;
    }

    while (resposta[0] == 's' || resposta[0] == 'S')
    {
        char plataforma[TAM_TEXTO_PEQUENO];
        char usuario[TAM_TEXTO_PEQUENO];

        printf("  Plataforma: ");
        if (!lerString(plataforma, TAM_TEXTO_PEQUENO))
        {
            free(a.telefones);
            free(a.redesSociais);
            return false;
        }

        printf("  Usuário: ");
        if (!lerString(usuario, TAM_TEXTO_PEQUENO))
        {
            free(a.telefones);
            free(a.redesSociais);
            return false;
        }

        redeSocial *temp = (redeSocial *) realloc(a.redesSociais, sizeof(redeSocial) * (a.totalRedesSociais + 1));
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
        
        printf("Deseja adicionar outra rede social? (s/n): ");
        if(!lerSimNao(resposta))
        {
            free(a.telefones);
            free(a.redesSociais);
            return false;
        }
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

static void listarArtista(const ListaArtistas *lista, int indice)
{
    const Artista *a = &lista->itens[indice];
    printf("\n--- Artista %d ---\n", indice + 1);
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
}

void listarTodosArtistas(const ListaArtistas *lista)
{
    int i;
    if (lista->total == 0)
    {
        printf("Nenhum artista cadastrado.\n");
        return;
    }
    for (i = 0; i < lista->total; i++)
    {
        listarArtista(lista, i);
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
    listarArtista(lista, indice);
}

bool moduloArtistas(ListaArtistas *lista)
{
    bool executando = true;
    while (executando)
    {
        switch (menuArtistas())
        {
            case 1:
                if(!cadastrarArtista(lista))
                {
                    printf("Erro ao cadastrar artista!!\n"); 
                    return false;
                }
                else
                {
                    printf("Artista cadastrado com sucesso!\n");
                    printf("Salvando dados...\n");
                    salvarArtistas(lista);
                }
                break;

            case 2:
                listarTodosArtistas(lista);
                break;

            case 3:
                buscarArtista(lista);
                break;

            case 4:  // Voltar ao menu principal
                executando = false;
                break;

            case -1: // Erro irrecuperável
                return false;
        }
    }
    return true;
}
