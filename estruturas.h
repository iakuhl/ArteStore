/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: estruturas.h                           *
 * Descrição: Definição das estruturas de dados    *
 *            utilizadas pelo sistema.             *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: ChatGPT (OpenAI), DeepSeek Chat  *
 * Disciplina: APR2                                *
 * Professora: Dra. Eloize Rossi Marques Seno      *
 ***************************************************/

/*************************
 * ARQUIVO DE ESTRUTURAS *
 *************************/

#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

// Todas as definições de constantes, como tamanhos de campos, estão no arquivo defines.h para facilitar a manutenção e evitar "números mágicos" espalhados pelo código.
#include "defines.h"

// A validação de dados será realizada nas funções do arquivo utils.c ou nas funções específicas dos módulos.
// Todas as estruturas armazenam dados "crus", a máscara de formatação será aplicada somente na exibição dos relatórios ou na exibição das listagens, mediante formação aplicada na exibição.

/**************
 * ESTRUTURAS *
 **************/

typedef struct
{
	int dia;
	int mes;
	int ano;
} Data;

typedef struct
{
	char numeroTelefone[TAM_TELEFONE];
} Telefone;

typedef struct
{
	char plataforma[TAM_TEXTO_PEQUENO];
	char usuario[TAM_TEXTO_PEQUENO];
} redeSocial;

// Estrutura para Artistas.
typedef struct
{
	char cpf[TAM_CPF]; // Chave única para cada artista.
	char nome[TAM_TEXTO_MEDIO];
	char nacionalidade[TAM_TEXTO_PEQUENO];
	char estilo[TAM_TEXTO_PEQUENO];
	Data nascimento;

	// Array dinâmico de redes sociais, cada artista pode ter um número variável de redes sociais.
	redeSocial *redesSociais;
	int totalRedesSociais;
	int capacidadeRedesSociais;

	// Array dinâmico de telefones, cada artista pode ter um número variável de telefones.
	Telefone *telefones;
	int totalTelefones;
	int capacidadeTelefones;

} Artista;

// Estrutura para Obras de arte.
typedef struct
{
	int id; // ID única para cada obra.
	int anoCriacao;
	
	long long valorCentavos; // Valor em centavos para evitar erro de conversão float, precisei declara como long porque obras de arte tem valores muito altos.

	char titulo[TAM_TEXTO_MEDIO];
	char tipo[TAM_TEXTO_PEQUENO];
	char descricao[TAM_TEXTO_GRANDE];

} Obra;

// Estrutura da chave de colaborações.
typedef struct
{
	char cpf[TAM_CPF]; // Deverá ser validado, somente em Artistas já cadastrados.
	int id; // Deverá ser validado, somente com Obras já cadastradas.
} ChaveColaboracao;

// Estrutura para Colaborações.
typedef struct
{
	ChaveColaboracao chaveColab; // ID única para colaboração (CPF artista + id Obra).

	char funcaoArtista[TAM_TEXTO_PEQUENO];

	int percentualContribuicao; // Percentual de colaboração, valor permitido entre 1 e 100.
	
	Data entrada;
	Data saida;

} Colaboracao;

/********************
 * LISTAS DINÂMICAS *
 ********************/

typedef struct {
	int total;
    Artista *itens;
    int capacidade;
} ListaArtistas;

typedef struct {
	int total;
	Obra *itens;
	int capacidade;
} ListaObras;

typedef struct {
	int total;
	Colaboracao *itens;
	int capacidade;
} ListaColaboracoes;

#endif // ESTRUTURAS_H
