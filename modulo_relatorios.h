/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: modulo_relatorios.h                    *
 * Descrição: Protótipos das funções responsáveis  *
 *            pela geração e exibição dos          *
 *            relatórios do sistema.               *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: ChatGPT (OpenAI), DeepSeek Chat  *
 * Disciplina: APR2                                *
 * Professora: Dra. Eloize Rossi Marques Seno      *
 ***************************************************/

/************************
 * HEADER DE RELATORIOS *
 ************************/

#ifndef MODULO_RELATORIOS_H
#define MODULO_RELATORIOS_H

#include "estruturas.h"

bool moduloRelatorios(ListaArtistas *listaArtistas, ListaObras *listaObras, ListaColaboracoes *listaColaboracoes);

#endif // MODULO_RELATORIOS_H