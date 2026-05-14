#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "estruturas.h"
#include "listas.h"

bool carregarArtistas(const char *nomeArquivo, ListaArtistas *lista);
bool salvarArtistas(const char *nomeArquivo, const ListaArtistas *lista);

bool carregarObras(const char *nomeArquivo, ListaObras *lista);
bool salvarObras(const char *nomeArquivo, const ListaObras *lista);

bool carregarColaboracoes(const char *nomeArquivo, ListaColaboracoes *lista);
bool salvarColaboracoes(const char *nomeArquivo, const ListaColaboracoes *lista);

#endif