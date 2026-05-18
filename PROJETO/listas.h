/****************************************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes                          *
 * Arquivo: listas.h                                                        *
 * Autor: Iano de Oliva Kuhlmann                                            *
 * Colaboradores: chat.deepseek.com                                         *
 * Disciplina: APR2                                                         *
 * Professora: Dra. Eloize Rossi Marques Seno                               *
 ****************************************************************************/

/********************
 * HEADER DE LISTAS *
 ********************/

#ifndef LISTAS_H
#define LISTAS_H

#include "estruturas.h"

// Inicializa uma lista de artistas com capacidade inicial
void inicializarListaArtistas(ListaArtistas *lista, int capacidadeInicial);
// Adiciona um artista ao final da lista, realocando se necessário
bool adicionarArtista(ListaArtistas *lista, Artista *artista);
// Busca um artista pelo CPF (retorna índice ou -1)
int buscarArtistaPorCPF(const ListaArtistas *lista, const char *cpf);
// Remove um artista pelo índice
bool removerArtista(ListaArtistas *lista, int indice);
// Libera toda a memória da lista de artistas (inclui telefones)
void liberarListaArtistas(ListaArtistas *lista);

// Análogas para obras
void inicializarListaObras(ListaObras *lista, int capacidadeInicial);
bool adicionarObra(ListaObras *lista, Obra *obra);
int buscarObraPorID(const ListaObras *lista, int id);
bool removerObra(ListaObras *lista, int indice);
void liberarListaObras(ListaObras *lista);

// Análogas para colaborações
void inicializarListaColaboracoes(ListaColaboracoes *lista, int capacidadeInicial);
bool adicionarColaboracao(ListaColaboracoes *lista, Colaboracao *colab);
int buscarColaboracao(const ListaColaboracoes *lista, const char *cpf, int idObra);
bool removerColaboracao(ListaColaboracoes *lista, int indice);
void liberarListaColaboracoes(ListaColaboracoes *lista);

#endif // LISTAS_H