/*
 * Projeto: Sistema de Curadoria de Obras de Artes
 * Arquivo: estruturas.h
 * Autor: Iano de Oliva Kuhlmann
 * Colaboradores: chat.deepseek.com
 * Link de colaboração: https://chat.deepseek.com/share/jil3nf8yyu9wwz0h8l
 * Disciplina: APR2
 * Professora: Dra. Eloize Rossi Marques Seno
 */


/*************************
 * ARQUIVO DE ESTRUTURAS *
 *************************/

#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

// Todas as definições de constantes, como tamanhos de campos, estão no arquivo defines.h para facilitar a manutenção e evitar "números mágicos" espalhados pelo código.
#include "defines.h"

// Todos os dados deverão passar por validação no arquivo utils.c e/ou na execução do código principal / modulo específico.

// Todas as estruturas armazenam dados "crus", a máscara de formatação será aplicada somente na exibição dos relatórios.


/**************
 * ESTRUTURAS *
 **************/

// Estrutura para datas.
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
	char redeSocial[TAM_TEXTO_PEQUENO];
	char usuario[TAM_TEXTO_PEQUENO];
}redeSocial;

// Estrutura para Artistas.
typedef struct
{
	char cpf[TAM_CPF]; // Chave única para cada artista.
	char nome[TAM_TEXTO_PEQUENO];
	char nacionalidade[TAM_TEXTO_PEQUENO];
	char estilo[TAM_TEXTO_PEQUENO];
	Data nascimento;

	redeSocial *redesSociais; // Array dinâmico de redes sociais, cada artista pode ter um número variável de redes sociais.
	int totalRedesSociais;
	int capacidadeRedesSociais;
	Telefone *telefones;
	int totalTelefones; // Quantidade de telefones cadastrados para o artista.
	int capacidadeTelefones; // Capacidade atual do array de telefones.

} Artista;

// Estrutura para Obras de arte.
typedef struct
{
	int id; // ID única para cada obra.
	int anoCriacao;
	
	int valorCentavos; // Valor em centavos para evitar erro de conversão float.

	char titulo[TAM_TEXTO_PEQUENO];
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
    Artista *itens;
    int total;
    int capacidade;
} ListaArtistas;

typedef struct {
	Obra *itens;
	int total;
	int capacidade;
} ListaObras;

typedef struct {
	Colaboracao *itens;
	int total;
	int capacidade;
} ListaColaboracoes;

#endif
