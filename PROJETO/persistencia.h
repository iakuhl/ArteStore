/****************************************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes                          *
 * Arquivo: persistencia.h                                                  *
 * Autor: Iano de Oliva Kuhlmann                                            *
 * Colaboradores: chat.deepseek.com                                         *
 * Link de colaboração: https://chat.deepseek.com/share/jil3nf8yyu9wwz0h8l  *
 * Disciplina: APR2                                                         *
 * Professora: Dra. Eloize Rossi Marques Seno                               *
 ****************************************************************************/


#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "estruturas.h"
#include "listas.h"

bool carregarArtistas(ListaArtistas *lista);
bool salvarArtistas(const ListaArtistas *lista);

bool carregarObras(ListaObras *lista);
bool salvarObras(const ListaObras *lista);

bool carregarColaboracoes(ListaColaboracoes *lista);
bool salvarColaboracoes(const ListaColaboracoes *lista);

#endif // PERSISTENCIA_H