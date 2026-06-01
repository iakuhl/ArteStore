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

// O módulo de artistas é o mais extenso de todo o projeto, ele possui a seguinte estrutura:
// Menu principal:
    // Listar um ou todos.
    // Cadastrar.
    // Excluir.
    // Alterar dados de artista (sub-módulo):
        // Nome.
        // Nacionalidade.
        // Estilo.
        // Gerenciamento de Telefones: ("sub-sub-módulo"):
            // Adicionar.
            // Excluir.
            // Editar/Alterar.
        // Gerenciamento de Redes Sociais: ("sub-sub-módulo"):
            // Adicionar.
            // Excluir.
            // Editar/Alterar.


/*********
 * MENUS *
 *********/

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

static int menuAlteracaoArtista()
{
    printf("### MENU DE ALTERAÇÕES DE ARTISTAS ###\n");
    printf("Escolha uma opção para alterar:\n");
    printf("1 - Nome\n");
    printf("2 - Nacionalidade\n");
    printf("3 - Estilo\n");
    printf("4 - Telefones\n");
    printf("5 - Redes Sociais\n");
    printf("6 - Salvar e Retornar ao menu Artistas");

    printf("Escolha uma opção: ");
    return escolherOpcao(1, 6);
}


/****************************
 * SUB-MODULO DE ALTERAÇÕES *
 ****************************/

/************************
 * MODULO DE ALTERAÇÕES *
 ************************/

/* Protótipos das funções auxiliares (definidas a seguir) */
static void listarTelefones(const Artista *a);
static bool adicionarTelefone(Artista *a);
static bool removerTelefone(Artista *a);
static bool editarTelefone(Artista *a);
static bool alterarTelefones(Artista *a);

static void listarRedesSociais(const Artista *a);
static bool adicionarRedeSocial(Artista *a);
static bool removerRedeSocial(Artista *a);
static bool editarRedeSocial(Artista *a);
static bool alterarRedesSociais(Artista *a);

/*******************************
 * alterarArtista (principal)  *
 *******************************/
static bool alterarArtista(ListaArtistas *lista, int indice)
{
    bool executando = true;
    Artista *a = &lista->itens[indice];

    char novoNome[TAM_TXTO_MEDIO];
    char novaNacionalidade[TAM_TEXTO_PEQUENO];
    char novoEstilo[TAM_TEXTO_PEQUENO];
    char confirma[TAM_SIM_NAO];

    do
    {
        int op = menuAlteracaoArtista();
        if (op == -99)
            return false;

        switch (op)
        {
        case 1: // Nome
            printf("Novo nome: ");
            if (!lerString(novoNome, TAM_TXTO_MEDIO))
                return false;

            printf("Confirma a alteração do nome para \"%s\"? (s/n): ", novoNome);
            if (!lerSimNao(confirma))
                return false;
            if (confirma[0] == 's' || confirma[0] == 'S')
                strcpy(a->nome, novoNome);
            else
                printf("Alteração cancelada.\n");
            break;

        case 2: // Nacionalidade
            printf("Nova nacionalidade: ");
            if (!lerString(novaNacionalidade, TAM_TEXTO_PEQUENO))
                return false;

            printf("Confirma a alteração da nacionalidade para \"%s\"? (s/n): ", novaNacionalidade);
            if (!lerSimNao(confirma))
                return false;
            if (confirma[0] == 's' || confirma[0] == 'S')
                strcpy(a->nacionalidade, novaNacionalidade);
            else
                printf("Alteração cancelada.\n");
            break;

        case 3: // Estilo
            printf("Novo estilo: ");
            if (!lerString(novoEstilo, TAM_TEXTO_PEQUENO))
                return false;

            printf("Confirma a alteração do estilo para \"%s\"? (s/n): ", novoEstilo);
            if (!lerSimNao(confirma))
                return false;
            if (confirma[0] == 's' || confirma[0] == 'S')
                strcpy(a->estilo, novoEstilo);
            else
                printf("Alteração cancelada.\n");
            break;

        case 4: // Telefones
            if (!alterarTelefones(a))
                return false;
            break;

        case 5: // Redes Sociais
            if (!alterarRedesSociais(a))
                return false;
            break;

        case 6: // Salvar e retornar
            executando = false;
            break;
        }
    } while (executando);

    return true;
}

/**************************************
 * GERENCIAMENTO DE TELEFONES E REDES *
 **************************************/

/* ─── Telefones ─── */
static void listarTelefones(const Artista *a)
{
    if (a->totalTelefones == 0)
    {
        printf("Nenhum telefone cadastrado.\n");
        return;
    }
    for (int i = 0; i < a->totalTelefones; i++)
        printf("  %d: %s\n", i + 1, a->telefones[i].numeroTelefone);
}

static bool adicionarTelefone(Artista *a)
{
    char novoNum[TAM_TELEFONE];
    char confirma[TAM_SIM_NAO];

    printf("  Número do telefone: ");
    if (!lerString(novoNum, TAM_TELEFONE))
        return false;

    printf("Confirma a adição do telefone \"%s\"? (s/n): ", novoNum);
    if (!lerSimNao(confirma))
        return false;
    if (!(confirma[0] == 's' || confirma[0] == 'S'))
    {
        printf("Adição cancelada.\n");
        return true;                // não é erro, apenas volta ao menu
    }

    Telefone *temp = (Telefone *)realloc(a->telefones,
                                         sizeof(Telefone) * (a->totalTelefones + 1));
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
    listarTelefones(a);
    if (a->totalTelefones == 0)
        return true;

    printf("Índice do telefone a remover (1 a %d): ", a->totalTelefones);
    int idx = escolherOpcao(1, a->totalTelefones);
    if (idx == -99) return false;
    idx--;

    char confirma[TAM_SIM_NAO];
    printf("Confirma a remoção do telefone \"%s\"? (s/n): ", a->telefones[idx].numeroTelefone);
    if (!lerSimNao(confirma))
        return false;
    if (!(confirma[0] == 's' || confirma[0] == 'S'))
    {
        printf("Remoção cancelada.\n");
        return true;
    }

    for (int i = idx; i < a->totalTelefones - 1; i++)
        a->telefones[i] = a->telefones[i + 1];
    a->totalTelefones--;
    printf("Telefone removido.\n");
    return true;
}

static bool editarTelefone(Artista *a)
{
    listarTelefones(a);
    if (a->totalTelefones == 0)
        return true;

    printf("Índice do telefone a editar (1 a %d): ", a->totalTelefones);
    int idx = escolherOpcao(1, a->totalTelefones);
    if (idx == -99) return false;
    idx--;

    char novoNum[TAM_TELEFONE];
    printf("Novo número: ");
    if (!lerString(novoNum, TAM_TELEFONE))
        return false;

    char confirma[TAM_SIM_NAO];
    printf("Confirma a alteração do telefone de \"%s\" para \"%s\"? (s/n): ",
           a->telefones[idx].numeroTelefone, novoNum);
    if (!lerSimNao(confirma))
        return false;
    if (confirma[0] == 's' || confirma[0] == 'S')
    {
        strcpy(a->telefones[idx].numeroTelefone, novoNum);
        printf("Telefone atualizado.\n");
    }
    else
    {
        printf("Edição cancelada.\n");
    }
    return true;
}

static bool alterarTelefones(Artista *a)
{
    bool gerenciando = true;
    do
    {
        printf("\n### Gerenciar Telefones de %s ###\n", a->nome);
        printf("1 - Listar telefones\n");
        printf("2 - Adicionar telefone\n");
        printf("3 - Remover telefone\n");
        printf("4 - Editar telefone\n");
        printf("5 - Voltar\n");
        printf("Escolha: ");
        int op = escolherOpcao(1, 5);
        if (op == -99) return false;

        switch (op)
        {
        case 1: listarTelefones(a); break;
        case 2: if (!adicionarTelefone(a)) return false; break;
        case 3: if (!removerTelefone(a))   return false; break;
        case 4: if (!editarTelefone(a))    return false; break;
        case 5: gerenciando = false; break;
        }
    } while (gerenciando);
    return true;
}

/* ─── Redes Sociais ─── */
static void listarRedesSociais(const Artista *a)
{
    if (a->totalRedesSociais == 0)
    {
        printf("Nenhuma rede social cadastrada.\n");
        return;
    }
    for (int i = 0; i < a->totalRedesSociais; i++)
        printf("  %d: %s - %s\n", i + 1,
               a->redesSociais[i].redeSocial,
               a->redesSociais[i].usuario);
}

static bool adicionarRedeSocial(Artista *a)
{
    char novaPlat[TAM_TEXTO_PEQUENO];
    char novoUser[TAM_TEXTO_PEQUENO];
    char confirma[TAM_SIM_NAO];

    printf("  Plataforma: ");
    if (!lerString(novaPlat, TAM_TEXTO_PEQUENO))
        return false;
    printf("  Usuário: ");
    if (!lerString(novoUser, TAM_TEXTO_PEQUENO))
        return false;

    printf("Confirma a adição da rede social \"%s\" com usuário \"%s\"? (s/n): ", novaPlat, novoUser);
    if (!lerSimNao(confirma))
        return false;
    if (!(confirma[0] == 's' || confirma[0] == 'S'))
    {
        printf("Adição cancelada.\n");
        return true;
    }

    redeSocial *temp = (redeSocial *)realloc(a->redesSociais,
                                             sizeof(redeSocial) * (a->totalRedesSociais + 1));
    if (temp == NULL)
    {
        printf("Erro de memória ao adicionar rede social.\n");
        return false;
    }
    a->redesSociais = temp;
    strcpy(a->redesSociais[a->totalRedesSociais].redeSocial, novaPlat);
    strcpy(a->redesSociais[a->totalRedesSociais].usuario,     novoUser);
    a->totalRedesSociais++;
    a->capacidadeRedesSociais = a->totalRedesSociais;

    printf("Rede social adicionada.\n");
    return true;
}

static bool removerRedeSocial(Artista *a)
{
    listarRedesSociais(a);
    if (a->totalRedesSociais == 0)
        return true;

    printf("Índice da rede social a remover (1 a %d): ", a->totalRedesSociais);
    int idx = escolherOpcao(1, a->totalRedesSociais);
    if (idx == -99) return false;
    idx--;

    char confirma[TAM_SIM_NAO];
    printf("Confirma a remoção da rede social \"%s\" (usuário \"%s\")? (s/n): ",
           a->redesSociais[idx].redeSocial, a->redesSociais[idx].usuario);
    if (!lerSimNao(confirma))
        return false;
    if (!(confirma[0] == 's' || confirma[0] == 'S'))
    {
        printf("Remoção cancelada.\n");
        return true;
    }

    for (int i = idx; i < a->totalRedesSociais - 1; i++)
        a->redesSociais[i] = a->redesSociais[i + 1];
    a->totalRedesSociais--;
    printf("Rede social removida.\n");
    return true;
}

static bool editarRedeSocial(Artista *a)
{
    listarRedesSociais(a);
    if (a->totalRedesSociais == 0)
        return true;

    printf("Índice da rede social a editar (1 a %d): ", a->totalRedesSociais);
    int idx = escolherOpcao(1, a->totalRedesSociais);
    if (idx == -99) return false;
    idx--;

    char novaPlat[TAM_TEXTO_PEQUENO];
    char novoUser[TAM_TEXTO_PEQUENO];

    printf("Nova plataforma: ");
    if (!lerString(novaPlat, TAM_TEXTO_PEQUENO))
        return false;
    printf("Novo usuário: ");
    if (!lerString(novoUser, TAM_TEXTO_PEQUENO))
        return false;

    char confirma[TAM_SIM_NAO];
    printf("Confirma as alterações (plataforma: \"%s\" -> \"%s\", usuário: \"%s\" -> \"%s\")? (s/n): ",
           a->redesSociais[idx].redeSocial, novaPlat,
           a->redesSociais[idx].usuario, novoUser);
    if (!lerSimNao(confirma))
        return false;
    if (confirma[0] == 's' || confirma[0] == 'S')
    {
        strcpy(a->redesSociais[idx].redeSocial, novaPlat);
        strcpy(a->redesSociais[idx].usuario,     novoUser);
        printf("Rede social atualizada.\n");
    }
    else
    {
        printf("Edição cancelada.\n");
    }
    return true;
}

static bool alterarRedesSociais(Artista *a)
{
    bool gerenciando = true;
    do
    {
        printf("\n### Gerenciar Redes Sociais de %s ###\n", a->nome);
        printf("1 - Listar redes sociais\n");
        printf("2 - Adicionar rede social\n");
        printf("3 - Remover rede social\n");
        printf("4 - Editar rede social\n");
        printf("5 - Voltar\n");
        printf("Escolha: ");
        int op = escolherOpcao(1, 5);
        if (op == -99) return false;

        switch (op)
        {
        case 1: listarRedesSociais(a); break;
        case 2: if (!adicionarRedeSocial(a)) return false; break;
        case 3: if (!removerRedeSocial(a))   return false; break;
        case 4: if (!editarRedeSocial(a))    return false; break;
        case 5: gerenciando = false; break;
        }
    } while (gerenciando);
    return true;
}
/**********************
 * FUNÇÃO DE CADASTRO *
 **********************/

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

/*****************************************
 * FUNÇÕES DE BUSCA E IMPRESSÃO/LISTAGEM *
 *****************************************/

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

/**********************
 * FUNÇÃO DE EXCLUSÃO *
 **********************/

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
// Retorna para a main FALSE em caso de erro irrecuperável (erro de memória, EOF nas funções que recebem entrada do usuário e etc).
// Retorna para a main TRUE quando o usuário desejar encerrar o módulo, os erros simples (entrada inválida) são tratados internamente.
bool moduloArtistas(ListaArtistas *lista)
{
    bool executando = true;
    int indice;
    while (executando)
    {
        switch (menuArtistas())
        {
            case 1: // Cadastrar novo artista
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

            case 2: // Listar todos os artistas (exibição completa de todos os dados de cada artista)
                listarTodosArtistas(lista);
                break;

            case 3: // Imprimir todos os dados de um artista específico, usa a função de busca por CPF para receber o índice, depois imprime por índice
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

            case 4: // Excluir um usuário, após confirmação dos dados, usa as funções de busca por CPF, lerSimNao e Excluir artista.
                printf("Qual artista deseja excluir?");
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

            case 5:  // Alterar um artista específico, usa a função de busca por CPF para receber o índice passar o índice para a função de alterar artista.
                printf("Qual artista deseja alterar?");
                if (!buscarArtistaPorCPF(lista, &indice))
                {
                    printf("Erro ao buscar artista para exclusão!!\n");
                    return false;
                }
                if (indice != -1)
                {
                    if (!alterarArtista(lista, indice))
                    {
                        printf("Erro ao alterar artista.\n");
                        return false;
                    }
                }
                break;
            
            case 6:
                executando = false;
                break;
            
            case -99: // Erro irrecuperável
                return false;
        }
    }
    return true;
}
