/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: defines.h                              *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: chat.deepseek.com                *
 * Disciplina: APR2                                *
 * Professora: Dra. Eloize Rossi Marques Seno      *
 ***************************************************/

/*************************
 * ARQUIVO DE DEFINIÇÕES *
 *************************/

#ifndef DEFINES_H
#define DEFINES_H

// Todas as definições de constantes, como tamanhos de campos, estão neste arquivo para facilitar a manutenção.
#define TAM_CPF 15
#define TAM_TELEFONE 20
#define TAM_TEXTO_PEQUENO 50
#define TAM_TEXTO_GRANDE 200
#define TAM_BUFFER_LEITURA 64 // Utilizada para leitura de inteiros, garantindo que o buffer seja grande o suficiente para evitar estouro e permitindo a validação adequada da entrada do usuário.

// Mensagens de erro para utils.c
#define MSG_LOOP_INFINITO "O programa será encerrado sem salvar!\n" // Utilizada em utils.c para informar que ocorreu um erro grave durante a leitura de dados. 
#define MSG_ENTRADA_INVALIDA "Entrada inválida! Tente novamente.\n" // Utilizada em utils.c para informar que a entrada do usuário não é válida (ex: letras quando se espera um número, ou caracteres extras após um número).
#define MSG_INTEIRO_INVALIDO "Número inválido! Informe um número inteiro válido: " // Utilizada em utils.c para informar entrada inválida.
#define MSG_EOF_DETECTADO "EOF detectado. Encerrando entrada de dados.\n" // Utilizada em utils.c para informar que o usuário encerrou a entrada de dados (Ctrl+D ou Ctrl+Z).
#define MSG_ERRO_LEITURA "Erro de leitura.\n" // Utilizada em utils.c para informar que ocorreu um erro durante a leitura da entrada de dados (diferente de EOF).
#define MSG_LIMITE_CARACTERES_ATINGIDO "Limite de caracteres atingido. Tente novamente.\n" // Utilizada em utils.c para informar que a entrada de dados ultrapassou o limite permitido para o campo.

// Mensagens de status para persistencia.c e main.c
#define MSG_ARQUIVO_VAZIO "Arquivo vazio. Lista inicializada vazia.\n" // Utilizada em persistencia.c para informar que o arquivo de dados está vazio ou corrompido, e que a lista será inicializada vazia.
#define MSG_ARQUIVO_NAO_ENCONTRADO "Arquivo não encontrado ou corrompido. Lista inicializada vazia.\n" // Utilizada em persistencia.c para informar que o arquivo de dados não foi encontrado, e que a lista será inicializada vazia.
#define MSG_ARQUIVO_CARREGADO "Dados carregados com sucesso.\n" // Utilizada em persistencia.c para informar que os dados foram carregados com sucesso a partir do arquivo.

// Nomes dos arquivos de persistência
#define NOME_ARQUIVO_ARTISTAS "artistas.bin"
#define NOME_ARQUIVO_OBRAS "obras.bin"
#define NOME_ARQUIVO_COLABORACOES "colaboracoes.bin"

#endif // DEFINES_H