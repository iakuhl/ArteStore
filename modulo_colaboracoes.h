/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: modulo_colaboracoes.h                  *
 * Descrição: Protótipos das funções responsáveis  *
 *            pelo gerenciamento das colaborações  *
 *            entre artistas e obras.              *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: ChatGPT (OpenAI), DeepSeek Chat  *
 * Disciplina: APR2                                *
 * Professora: Dra. Eloize Rossi Marques Seno      *
 ***************************************************/

/**************************
 * HEADER DE COLABORAÇÕES *
 **************************/

#ifndef MODULO_COLABORACOES_H
#define MODULO_COLABORACOES_H

#include "estruturas.h"

void imprimirColaboracaoPorIndice(const ListaColaboracoes *lista, int indice);
void listarTodasColaboracoes(const ListaColaboracoes *listaColaboracoes);

bool moduloColaboracoes(ListaColaboracoes *listaColaboracoes, const ListaArtistas *listaArtistas, const ListaObras *listaObras);

#endif // MODULO_COLABORACOES_H 