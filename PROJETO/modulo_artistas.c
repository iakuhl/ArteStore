/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: modulo_artistas.c                      *
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
    printf("2 - Listar todos os artistas\n");
    printf("3 - Imprimir informações de um artista\n");
    printf("4 - Excluir artista\n");
    printf("5 - Voltar ao Menu Principal\n");

    printf("Escolha uma opção: ");
    return escolherOpcao(1, 5);
}

static bool cadastrarArtista(ListaArtistas *lista)
{
    Artista a;
    char resposta[TAM_SIM_NAO], telefone[TAM_TELEFONE], plataforma[TAM_TEXTO_PEQUENO], usuario[TAM_TEXTO_PEQUENO];
    bool cpfValido = false;


    printf("--- Cadastrar novo artista ---\n");

    printf("CPF: ");
    do{
        if (!lerString(a.cpf, TAM_CPF))
            return false;
        
        if (!removeMascaraCPF(a.cpf))
        {
            printf("CPF inválido. Tente novamente: ");
            continue;
        }

        if (indiceArtistaPorCPF(lista, a.cpf) != -1)
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

    printf("Telefones (mínimo 1):\n");
    do
    {
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

void imprimirArtistaPorIndice(const ListaArtistas *lista, int indice)
{
    const Artista *a = &lista->itens[indice]; // Cria um ponteiro "clone" para facilitar a leitura dos campos do artista sem precisar usar lista->itens[indice] repetidamente.
    printf("\n--- Artista %d ---\n", indice + 1);
    printf("Nome: %s\n", a->nome);

    printf("CPF: ");
    imprimeCPF(a->cpf); // Imprime o CPF formatado
    printf("\n");

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
        imprimirArtistaPorIndice(lista, i);
    }
}

bool buscarArtistaPorCPF(const ListaArtistas *lista, int *indice)
{
    char cpf[TAM_CPF];
    printf("Informe o CPF: ");
    if (!lerString(cpf, TAM_CPF))
        return false;
    if (!removeMascaraCPF(cpf))
    {
        printf("CPF inválido.\n");
        return true; 
    }

    *indice = indiceArtistaPorCPF(lista, cpf);
    if (*indice == -1)
    {
        printf("Artista não encontrado.\n");
        return true;
    }
    return true;
}

static bool excluirArtista(ListaArtistas *lista, int indice)
{
    imprimirArtistaPorIndice(lista, indice);
    printf("Tem certeza que deseja excluir este artista? (s/n): ");
    char resposta[TAM_SIM_NAO];
    if (!lerSimNao(resposta))
        return false;
    
    if (removerArtista(lista, indice))
    {
        printf("Artista excluído com sucesso.\n");
        return true;
    }
    else
    {
        printf("Erro ao excluir artista.\n");
        return false;
    }
}

bool moduloArtistas(ListaArtistas *lista)
{
    bool executando = true;
    int indice;
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
                    if(!salvarArtistas(lista))
                    {
                        printf("Erro ao salvar dados dos artistas!!\n");
                        return false;
                    }
                    else
                    {
                        printf("Dados dos artistas salvos com sucesso!\n");
                    }
                }
                break;

            case 2:
                listarTodosArtistas(lista);
                break;

            case 3:
                if (!buscarArtistaPorCPF(lista, &indice))
                {
                    printf("Erro ao buscar artista!!\n");
                    return false;
                }
                if (indice != -1)
                {
                    imprimirArtistaPorIndice(lista, indice);
                }
                break;

            case 4:
                if (!buscarArtistaPorCPF(lista, &indice))
                {
                    printf("Erro ao buscar artista para exclusão!!\n");
                    return false;
                }
                if (indice != -1)
                {
                    if (!excluirArtista(lista, indice))
                    {
                        printf("Erro ao excluir artista.\n");
                        return false;
                    }
                }
                break;

            case 5:  // Voltar ao menu principal
                executando = false;
                break;

            case -1: // Erro irrecuperável
                return false;
        }
    }
    return true;
}
