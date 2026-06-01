/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: modulo_artistas.h                      *
 * Descrição: Protótipos das funções de cadastro,  *
 *            consulta, alteração e remoção de     *
 *            artistas.                            *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: ChatGPT (OpenAI), DeepSeek Chat  *
 * Disciplina: APR2                                *
 * Professora: Dra. Eloize Rossi Marques Seno      *
 ***************************************************/

/**********************
 * HEADER DE ARTISTAS *
 **********************/

#ifndef MODULO_ARTISTAS_H
#define MODULO_ARTISTAS_H

#include "estruturas.h"

void listarTodosArtistas(const ListaArtistas *lista);

bool moduloArtistas(ListaArtistas *lista);

#endif // MODULO_ARTISTAS_H