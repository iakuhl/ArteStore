/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: modulo_obras.h                         *
 * Descrição: Protótipos das funções de cadastro,  *
 *            consulta, alteração e remoção de     *
 *            obras de arte.                       *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: ChatGPT (OpenAI), DeepSeek Chat  *
 * Disciplina: APR2                                *
 * Professora: Dra. Eloize Rossi Marques Seno      *
 ***************************************************/

/*******************
 * HEADER DE OBRAS *
 *******************/

#ifndef MODULO_OBRAS_H
#define MODULO_OBRAS_H

#include "estruturas.h"

void imprimirObraPorIndice(const ListaObras *lista, int indice);
void listarTodasObras(const ListaObras *lista);

bool moduloObras(ListaObras *lista);

#endif // MODULO_OBRAS_H