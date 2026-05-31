/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: persistencia.h                         *
 * Descrição: Protótipos das funções responsáveis  *
 *            pela gravação e recuperação dos      *
 *            dados em arquivos binários.          *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: ChatGPT (OpenAI), DeepSeek Chat  *
 * Disciplina: APR2                                *
 * Professora: Dra. Eloize Rossi Marques Seno      *
 ***************************************************/

/**************************
 * HEADER DE PERSISTENCIA *
 **************************/

#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "estruturas.h"
#include "listas.h"

int carregarArtistas(ListaArtistas *lista);
bool salvarArtistas(const ListaArtistas *lista);

int carregarObras(ListaObras *lista);
bool salvarObras(const ListaObras *lista);

int carregarColaboracoes(ListaColaboracoes *lista);
bool salvarColaboracoes(const ListaColaboracoes *lista);

#endif // PERSISTENCIA_H
