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

// O módulo de artistas possui a seguinte estrutura:
// Menu principal:
    // Listar um ou todos.
    // Cadastrar artista.
    // Excluir artista.
    // Alterar dados (sub-módulo do menu de artistas):
        // Alterar Nome.
        // Alterar Nacionalidade.
        // Alterar Estilo.
        // Alterar Telefones: (sub-módulo do menu de alteração de artistas):
            // Adicionar Telefone.
            // Excluir Telefone.
            // Editar/Alterar Telefone.
        // Alterar Redes Sociais: (sub-módulo do menu de alteração de artistas):
            // Adicionar Rede Social.
            // Excluir Rede Social.
            // Editar/Alterar Rede Social.

/*********************
 * EXIBIÇÃO DE MENUS *
 *********************/

static int menuArtistas()
{
    printf("\n### MENU ARTISTAS ###\n");
    printf("1 - Cadastrar Artista\n");
    printf("2 - Listar todos os artistas\n");
    printf("3 - Exibir dados de um artista\n");
    printf("4 - Alterar dados de um artista\n");
    printf("5 - Excluir artista\n");

    printf("6 - Voltar ao Menu Principal\n");

    return escolherOpcao(1, 6);
}

static int menuAlteracaoArtista()
{
    printf("\n### MENU DE ALTERAÇÕES DE ARTISTAS ###\n");
    printf("Escolha uma opção para alterar:\n");
    printf("1 - Nome\n");
    printf("2 - Nacionalidade\n");
    printf("3 - Estilo\n");
    printf("4 - Telefones\n");
    printf("5 - Redes Sociais\n");

    printf("6 - Salvar e Retornar ao menu Artistas");

    return escolherOpcao(1, 6);
}

static int menuTelefones()
{
    printf("\n### Gerenciar Telefones ###\n");
    printf("1 - Adicionar telefone\n");
    printf("2 - Remover telefone\n");
    printf("3 - Editar telefone\n");

    printf("4 - Voltar\n");

    return escolherOpcao(1, 4);
}

static int menuRedesSociais()
{
    printf("\n### Gerenciar Redes Sociais ###\n");
    printf("1 - Adicionar rede social\n");
    printf("2 - Remover rede social\n");
    printf("3 - Editar rede social\n");

    printf("4 - Voltar\n");

    return escolherOpcao(1, 4);
}

/*************************************************
 * FUNÇÕES DE GERENCIAMENTO DE TELEFONES E REDES *
 *************************************************/

/* ─── Telefones ─── */
static void listarTelefones(const Artista *a)
{
    int i;
    if (a->totalTelefones > 0)
        for (i = 0; i < a->totalTelefones; i++)
            printf("  %d: %s\n", i + 1, a->telefones[i].numeroTelefone);
    else
        printf("Nenhum telefone cadastrado.\n");
}

static bool adicionarTelefone(Artista *a)
{
    Telefone *temp;
    char novoNum[TAM_TELEFONE];

    printf("  Número do telefone: ");
    if (!lerString(novoNum, TAM_TELEFONE))
        return false;

    temp = (Telefone *)realloc(a->telefones, sizeof(Telefone) * (a->totalTelefones + 1));
    if (temp == NULL)
    {
        printf("Erro de memória ao adicionar telefone.\n");
        return false;
    }
    a->telefones = temp;
    strcpy(a->telefones[a->totalTelefones].numeroTelefone, novoNum);
    a->totalTelefones++;
    a->capacidadeTelefones = a->totalTelefones;

    printf("Telefone adicionado.\n");
    return true;
}

static bool removerTelefone(Artista *a)
{
    int idx, i;
    char confirma[TAM_SIM_NAO];

    idx = escolherOpcao(1, a->totalTelefones);
    if (idx == -99)
        return false;
    idx--;

    printf(MSG_CONFIRMAR_EXCLUSAO, a->telefones[idx].numeroTelefone);
    if (!lerSimNao(confirma))
        return false;
    
    if (confirma[0] == 's' || confirma[0] == 'S')
    {
        for (i = idx; i < a->totalTelefones - 1; i++)
        {
            a->telefones[i] = a->telefones[i + 1];
        }
        a->totalTelefones--;
        printf("Telefone removido.\n");
    }
    else
        printf(MSG_EXCLUSAO_CANCELADA);
    
    return true;
}

static bool editarTelefone(Artista *a)
{
    int idx;
    char confirma[TAM_SIM_NAO], novoNum[TAM_TELEFONE];

    idx = escolherOpcao(1, a->totalTelefones);
    if (idx == -99)
        return false;

    idx--;

    printf("Novo número: ");
    if (!lerString(novoNum, TAM_TELEFONE))
        return false;

    printf(MSG_CONFIRMAR_ALTERACAO_TEXTO, a->telefones[idx].numeroTelefone, novoNum);
    if (!lerSimNao(confirma))
        return false;
    
    if (confirma[0] == 's' || confirma[0] == 'S')
    {
        strcpy(a->telefones[idx].numeroTelefone, novoNum);
        printf(MSG_ALTERACAO_REALIZADA_SUCESSO);
    }
    else
        printf(MSG_ALTERACAO_CANCELADA);
    
    return true;
}

/* ─── Redes Sociais ─── */
static void listarRedesSociais(const Artista *a)
{
    int i;
    if (a->totalRedesSociais > 0)
        for (i = 0; i < a->totalRedesSociais; i++)
            printf("  %d: %s - %s\n", i + 1, a->redesSociais[i].plataforma, a->redesSociais[i].usuario);
    else
        printf("Nenhuma rede social cadastrada.\n");
}

static bool adicionarRedeSocial(Artista *a)
{
    char novaPlat[TAM_TEXTO_PEQUENO], novoUser[TAM_TEXTO_PEQUENO];
    redeSocial *temp = (redeSocial *)realloc(a->redesSociais, sizeof(redeSocial) * (a->totalRedesSociais + 1));

    printf("  Plataforma: ");
    if (!lerString(novaPlat, TAM_TEXTO_PEQUENO))
        return false;
    
    printf("  Usuário: ");
    if (!lerString(novoUser, TAM_TEXTO_PEQUENO))
        return false;
    
    if (temp == NULL)
    {
        printf("Erro de memória ao adicionar rede social.\n");
        return false;
    }
    a->redesSociais = temp;
    strcpy(a->redesSociais[a->totalRedesSociais].plataforma, novaPlat);
    strcpy(a->redesSociais[a->totalRedesSociais].usuario, novoUser);
    a->totalRedesSociais++;
    a->capacidadeRedesSociais = a->totalRedesSociais;
    
    printf("Rede social adicionada.\n");
    return true;
}

static bool removerRedeSocial(Artista *a)
{
    int idx, i;
    char confirma[TAM_SIM_NAO];
    
    listarRedesSociais(a);
    if (a->totalRedesSociais == 0)
        return true;

    printf("Qual rede social deseja remover (1 a %d): ", a->totalRedesSociais);
    idx = escolherOpcao(1, a->totalRedesSociais);
    if (idx == -99) return false;
    idx--;

    printf("Confirma a remoção da rede social \"%s\" (usuário \"%s\")? (s/n): ", a->redesSociais[idx].plataforma, a->redesSociais[idx].usuario);
    if (!lerSimNao(confirma))
        return false;
    
    // Percorre a lista de Redes Sociais a partir do índice, sobreescrevendo o índice atual com o próximo, até o final da lista.
    if (confirma[0] == 's' || confirma[0] == 'S')
    {
        for (i = idx; i < a->totalRedesSociais - 1; i++)
        {
            a->redesSociais[i] = a->redesSociais[i + 1];
        }
        a->totalRedesSociais--;
        printf("Rede social removida.\n");
        return true;
    }
    else
    {
        printf(MSG_EXCLUSAO_CANCELADA);
        return true;
    }
}

static bool editarRedeSocial(Artista *a)
{
    int idx, i;
    char confirma[TAM_SIM_NAO], novaPlat[TAM_TEXTO_PEQUENO], novoUser[TAM_TEXTO_PEQUENO];
    
    listarRedesSociais(a);
    if (a->totalRedesSociais == 0)
        return true;

    printf("Qual rede social deseja editar (1 a %d): ", a->totalRedesSociais);
    idx = escolherOpcao(1, a->totalRedesSociais);
    if (idx == -99)
        return false;
    idx--;

    printf("Nova plataforma: ");
    if (!lerString(novaPlat, TAM_TEXTO_PEQUENO))
        return false;
    
    printf("Novo usuário: ");
    if (!lerString(novoUser, TAM_TEXTO_PEQUENO))
        return false;

    printf("Confirma as alterações (plataforma: \"%s\" -> \"%s\", usuário: \"%s\" -> \"%s\")? (s/n): ", a->redesSociais[idx].plataforma, novaPlat, a->redesSociais[idx].usuario, novoUser);
    if (!lerSimNao(confirma))
        return false;
    
    if (confirma[0] == 's' || confirma[0] == 'S')
    {
        strcpy(a->redesSociais[idx].plataforma, novaPlat);
        strcpy(a->redesSociais[idx].usuario, novoUser);
        printf("Rede social atualizada.\n");
    }
    else
    {
        printf(MSG_ALTERACAO_CANCELADA);
    }
    return true;
}

/*******************************
 * FUNÇÕES DO MÓDULO PRINCIPAL *
 *******************************/

static bool cadastrarArtista(ListaArtistas *lista)
{
    Artista a;
    Telefone *tempTelefones;
    redeSocial *tempRedes;
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
    if (!lerString(a.nome, TAM_TEXTO_MEDIO))
        return false;

    // Nacionalidade
    printf("Nacionalidade: ");
    if (!lerString(a.nacionalidade, TAM_TEXTO_PEQUENO))
        return false;

    // Estilo
    printf("Estilo: ");
    if (!lerString(a.estilo, TAM_TEXTO_PEQUENO))
        return false;
    
    do{ 
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
        
        if (!validarData(a.nascimento))
            printf(MSG_ENTRADA_INVALIDA);
    }while (!validarData(a.nascimento));
        
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

        tempTelefones = (Telefone *) realloc(a.telefones, sizeof(Telefone) * (a.totalTelefones + 1));
        if (tempTelefones == NULL)
        {
            printf(MSG_ERRO_ALOCAR_MEMORIA);
            free(a.telefones);
            return false;
        }
        a.telefones = tempTelefones;
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

        tempRedes = (redeSocial *) realloc(a.redesSociais, sizeof(redeSocial) * (a.totalRedesSociais + 1));
        if (tempRedes == NULL)
        {
            printf(MSG_ERRO_ALOCAR_MEMORIA);
            free(a.telefones);
            free(a.redesSociais);
            return false;
        }
        a.redesSociais = tempRedes;
        strncpy(a.redesSociais[a.totalRedesSociais].plataforma, plataforma, TAM_TEXTO_PEQUENO - 1);
        a.redesSociais[a.totalRedesSociais].plataforma[TAM_TEXTO_PEQUENO - 1] = '\0';
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
    if (!adicionarArtista(lista, &a))
    {
        printf(MSG_ERRO_ALOCAR_MEMORIA);
        free(a.telefones);
        free(a.redesSociais);
        return false;
    }

    printf(MSG_CADASTRO_REALIZADO_SUCESSO);
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
            printf("  %s: %s\n", a->redesSociais[j].plataforma, a->redesSociais[j].usuario);
        }
    }
}

void listarTodosArtistas(const ListaArtistas *lista)
{
    int i;
    if (lista->total == 0)
        printf("Nenhum artista cadastrado.\n");
    else
        for (i = 0; i < lista->total; i++)
            imprimirArtistaPorIndice(lista, i);
    return;
}

bool buscarArtistaPorCPF(const ListaArtistas *lista, int *indice)
{
    char cpf[TAM_CPF];
    printf("Informe o CPF do artista: ");
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
    char resposta[TAM_SIM_NAO];
    imprimirArtistaPorIndice(lista, indice);
    printf("Tem certeza que deseja excluir este artista? (s/n): ");
    
    if (!lerSimNao(resposta))
        return false;
    if (resposta[0] != 's' && resposta[0] != 'S')
    {
        printf(MSG_EXCLUSAO_CANCELADA);
        return true;
    }
    else
    {
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
}

/*************************
 * MODULOS E SUB-MODULOS *
 *************************/

// Retornam FALSE em caso de erro irrecuperável (erro de memória, EOF nas funções que recebem entrada do usuário e etc).
// Retornam TRUE quando o usuário desejar encerrar o módulo, os erros simples (entrada inválida) são tratados internamente.

static bool moduloTelefones(Artista *a)
{
    bool executando = true;
    do
    {
        int op;
        op = menuTelefones();
        if (op == -99)
            return false;
        
        switch (op)
        {   
        case 1:
            if (!adicionarTelefone(a))
                return false;
            break;
            
        case 2:
            if (a->totalTelefones > 0)
            {
                printf("Qual telefone deseja remover?\n");
                listarTelefones(a);
                if (!removerTelefone(a))
                    return false;
            }
            else
                printf(MSG_NENHUM_CADASTRADO, "telefone");

            break;
            
        case 3:
            if (a->totalTelefones > 0)
            {
                printf("Qual telefone deseja editar?\n");
                listarTelefones(a);
                if (!editarTelefone(a))
                    return false;
            }
            else
                printf(MSG_NENHUM_CADASTRADO, "telefone");

            break;
            
        case 4:
            executando = false;
            break;
        }
    } while (executando);
    return true;
}

static bool moduloRedesSociais(Artista *a)
{
    bool executando = true;
    do
    {
        int op;
        op = menuRedesSociais();
        if (op == -99)
            return false;

        switch (op)
        {
        case 1:
            listarRedesSociais(a);
            break;

        case 2:
            if (!adicionarRedeSocial(a))
                return false;
            break;

        case 3:
            if (!removerRedeSocial(a))  
                return false;
            break;

        case 4:
            if (!editarRedeSocial(a))
                return false;
            break;

        case 5:
            executando = false;
            break;
        }
    } while (executando);
    return true;
}

static bool moduloAlteracaoArtista(ListaArtistas *lista, int indice)
{
    bool executando = true;
    Artista *a = &lista->itens[indice];
    int op;
    char novoNome[TAM_TEXTO_MEDIO], novaNacionalidade[TAM_TEXTO_PEQUENO], novoEstilo[TAM_TEXTO_PEQUENO], confirma[TAM_SIM_NAO];

    imprimirArtistaPorIndice(lista, indice);

    do
    {
        op = menuAlteracaoArtista();
        if (op == -99)
            return false;

        switch (op)
        {
        case 1:
            printf("Novo nome: ");
            if (!lerString(novoNome, TAM_TEXTO_MEDIO))
                return false;

            printf(MSG_CONFIRMAR_ALTERACAO_TEXTO, a->nome, novoNome);
            if (!lerSimNao(confirma))
                return false;
            
            if (confirma[0] == 's' || confirma[0] == 'S')
            {
                strcpy(a->nome, novoNome);
                printf(MSG_ALTERACAO_REALIZADA_SUCESSO);
            }
            else
                printf(MSG_ALTERACAO_CANCELADA);

            break;

        case 2:
            printf("Nova nacionalidade: ");
            if (!lerString(novaNacionalidade, TAM_TEXTO_PEQUENO))
                return false;

            printf(MSG_CONFIRMAR_ALTERACAO_TEXTO, a->nacionalidade, novaNacionalidade);
            if (!lerSimNao(confirma))
                return false;
            if (confirma[0] == 's' || confirma[0] == 'S')
            {
                strcpy(a->nacionalidade, novaNacionalidade);
                printf(MSG_ALTERACAO_REALIZADA_SUCESSO);
            }
            else
                printf(MSG_ALTERACAO_CANCELADA);

            break;

        case 3:
            printf("Novo estilo: ");
            if (!lerString(novoEstilo, TAM_TEXTO_PEQUENO))
                return false;

            printf(MSG_CONFIRMAR_ALTERACAO_TEXTO, a->estilo, novoEstilo);
            if (!lerSimNao(confirma))
                return false;
            
            if (confirma[0] == 's' || confirma[0] == 'S')
            {
                strcpy(a->estilo, novoEstilo);
                printf(MSG_ALTERACAO_REALIZADA_SUCESSO);
            }
            else
                printf(MSG_ALTERACAO_CANCELADA);
            break;

        case 4:
            if (!moduloTelefones(a))
                return false;
            break;

        case 5:
            if (!moduloRedesSociais(a))
                return false;
            break;

        case 6: // Salvar e retornar
            executando = false;
            break;
        }
    } while (executando);

    return true;
}

bool moduloArtistas(ListaArtistas *lista)
{
    bool executando = true;
    int indice, op;

    do
    {
        op = menuArtistas();
        if (op == -99)
            return false;
        switch (op)
        {
            case 1: // Cadastrar novo artista
                if(!cadastrarArtista(lista))
                    return false;
                else
                {
                    printf("Artista cadastrado com sucesso!\n");
                    printf("Salvando dados...\n");
                    if(!salvarArtistas(lista))
                        return false;
                    else
                        printf(MSG_DADOS_SALVOS_SUCESSO, "artistas");
                }
                break;

            case 2:
                listarTodosArtistas(lista);
                break;

            case 3:
                if (!buscarArtistaPorCPF(lista, &indice))
                    return false;

                if (indice != -1)
                    imprimirArtistaPorIndice(lista, indice);
            
                break;

            case 4:
                if (!buscarArtistaPorCPF(lista, &indice))
                    return false;

                if (indice != -1)
                    if (!moduloAlteracaoArtista(lista, indice))
                        return false;
                break;

            case 5:
                if (!buscarArtistaPorCPF(lista, &indice))
                    return false;

                if (indice != -1)
                    if (!excluirArtista(lista, indice))
                        return false;
                break;

            case 6:
                executando = false;
                break;
            
            case -99: // Erro irrecuperável
                return false;
        } // fim do switch do menu de artistas
    }while (executando);
    return true;
}
