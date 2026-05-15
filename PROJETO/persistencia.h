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

#endif