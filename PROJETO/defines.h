/*
 * Projeto: Sistema de Curadoria de Obras de Artes
 * Arquivo: defines.h
 * Autor: Iano de Oliva Kuhlmann
 * Colaboradores: chat.deepseek.com
 * Link de colaboração: https://chat.deepseek.com/share/jil3nf8yyu9wwz0h8l
 * Disciplina: APR2
 * Professora: Dra. Eloize Rossi Marques Seno
 */

/*************************
 * ARQUIVO DE DEFINIÇÕES *
 *************************/

 // Todas as definições de constantes, como tamanhos de campos, estão neste arquivo para facilitar a manutenção e evitar "números mágicos" espalhados pelo código.
#define TAM_CPF 12
#define TAM_TELEFONE 20
#define TAM_NACIONALIDADE 30
#define TAM_TEXTO_PEQUENO 50
#define TAM_TEXTO_GRANDE 200
#define TAM_BUFFER_LEITURA 64 // Utilizada para leitura de inteiros, garantindo que o buffer seja grande o suficiente para evitar estouro e permitindo a validação adequada da entrada do usuário.

// Mensagens de erro
#define MSG_LOOP_INFINITO "Detectamos um ERRO IRRECUPERÁVEL!\n" // Utilizada nos menus para informar o usuário sobre a ocorrência de um erro de leitura (EOF ou erro de entrada).
#define MSG_ENTRADA_INVALIDA "Entrada inválida! Tente novamente.\n" // Utilizada nos menus para informar o usuário sobre a entrada de dados inválida (fora do intervalo permitido ou tipo de dado incorreto).

// Nomes dos arquivos de persistência
#define NOME_ARQUIVO_ARTISTAS "artistas.txt"
#define NOME_ARQUIVO_OBRAS "obras.txt"
#define NOME_ARQUIVO_COLABORACOES "colaboracoes.txt"