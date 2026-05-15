// Feito integralmente pelo DeepSeek, deverá ser revisado


#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "listas.h"
#include "estruturas.h"

// ========== ARTISTAS ==========

void inicializarListaArtistas(ListaArtistas *lista, int capacidadeInicial)
{
    lista->itens = malloc(sizeof(Artista) * capacidadeInicial);
    lista->total = 0;
    lista->capacidade = (lista->itens != NULL) ? capacidadeInicial : 0;
}

bool adicionarArtista(ListaArtistas *lista, Artista *artista)
{
    if (lista->total >= lista->capacidade)
    {
        int novaCap = lista->capacidade * 2;
        Artista *temp = realloc(lista->itens, sizeof(Artista) * novaCap);
        if (temp == NULL) return false;
        lista->itens = temp;
        lista->capacidade = novaCap;
    }
    lista->itens[lista->total] = *artista; // cópia rasa (telefones já alocados dinamicamente)
    lista->total++;
    return true;
}

int buscarArtistaPorCPF(const ListaArtistas *lista, const char *cpf)
{
    for (int i = 0; i < lista->total; i++)
    {
        if (strcmp(lista->itens[i].cpf, cpf) == 0)
            return i;
    }
    return -1;
}

bool removerArtista(ListaArtistas *lista, int indice)
{
    if (indice < 0 || indice >= lista->total) return false;

    // Libera os telefones do artista removido
    free(lista->itens[indice].telefones);

    // Move os elementos seguintes para trás
    for (int i = indice; i < lista->total - 1; i++)
    {
        lista->itens[i] = lista->itens[i + 1];
    }
    lista->total--;
    return true;
}

void liberarListaArtistas(ListaArtistas *lista)
{
    for (int i = 0; i < lista->total; i++)
    {
        free(lista->itens[i].telefones);
        free(lista->itens[i].redesSociais);
    }
    free(lista->itens);
    lista->itens = NULL;
    lista->total = 0;
    lista->capacidade = 0;
}

// ========== OBRAS ==========

void inicializarListaObras(ListaObras *lista, int capacidadeInicial)
{
    lista->itens = malloc(sizeof(Obra) * capacidadeInicial);
    lista->total = 0;
    lista->capacidade = (lista->itens != NULL) ? capacidadeInicial : 0;
}

bool adicionarObra(ListaObras *lista, Obra *obra)
{
    if (lista->total >= lista->capacidade)
    {
        int novaCap = lista->capacidade * 2;
        Obra *temp = realloc(lista->itens, sizeof(Obra) * novaCap);
        if (temp == NULL) return false;
        lista->itens = temp;
        lista->capacidade = novaCap;
    }
    lista->itens[lista->total] = *obra;
    lista->total++;
    return true;
}

int buscarObraPorID(const ListaObras *lista, int id)
{
    for (int i = 0; i < lista->total; i++)
    {
        if (lista->itens[i].id == id)
            return i;
    }
    return -1;
}

bool removerObra(ListaObras *lista, int indice)
{
    if (indice < 0 || indice >= lista->total) return false;
    for (int i = indice; i < lista->total - 1; i++)
    {
        lista->itens[i] = lista->itens[i + 1];
    }
    lista->total--;
    return true;
}

void liberarListaObras(ListaObras *lista)
{
    free(lista->itens);
    lista->itens = NULL;
    lista->total = 0;
    lista->capacidade = 0;
}

// ========== COLABORACOES ==========

void inicializarListaColaboracoes(ListaColaboracoes *lista, int capacidadeInicial)
{
    lista->itens = malloc(sizeof(Colaboracao) * capacidadeInicial);
    lista->total = 0;
    lista->capacidade = (lista->itens != NULL) ? capacidadeInicial : 0;
}

bool adicionarColaboracao(ListaColaboracoes *lista, Colaboracao *colab)
{
    if (lista->total >= lista->capacidade)
    {
        int novaCap = lista->capacidade * 2;
        Colaboracao *temp = realloc(lista->itens, sizeof(Colaboracao) * novaCap);
        if (temp == NULL) return false;
        lista->itens = temp;
        lista->capacidade = novaCap;
    }
    lista->itens[lista->total] = *colab;
    lista->total++;
    return true;
}

int buscarColaboracao(const ListaColaboracoes *lista, const char *cpf, int idObra)
{
    for (int i = 0; i < lista->total; i++)
    {
        if (strcmp(lista->itens[i].chaveColab.cpf, cpf) == 0 &&
            lista->itens[i].chaveColab.id == idObra)
            return i;
    }
    return -1;
}

bool removerColaboracao(ListaColaboracoes *lista, int indice)
{
    if (indice < 0 || indice >= lista->total) return false;
    for (int i = indice; i < lista->total - 1; i++)
    {
        lista->itens[i] = lista->itens[i + 1];
    }
    lista->total--;
    return true;
}

void liberarListaColaboracoes(ListaColaboracoes *lista)
{
    free(lista->itens);
    lista->itens = NULL;
    lista->total = 0;
    lista->capacidade = 0;
}
