/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: persistencia.h                         *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: chat.deepseek.com                *
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

bool carregarObras(ListaObras *lista);
bool salvarObras(const ListaObras *lista);

bool carregarColaboracoes(ListaColaboracoes *lista);
bool salvarColaboracoes(const ListaColaboracoes *lista);

#endif // PERSISTENCIA_H