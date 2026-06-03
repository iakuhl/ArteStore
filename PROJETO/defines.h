/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: defines.h                              *
 * Descrição: Constantes, macros e mensagens       *
 *            utilizadas em todo o sistema.        *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: ChatGPT (OpenAI), DeepSeek Chat  *
 * Disciplina: APR2                                *
 * Professora: Dra. Eloize Rossi Marques Seno      *
 ***************************************************/

/*************************
 * ARQUIVO DE DEFINIÇÕES *
 *************************/

#ifndef DEFINES_H
#define DEFINES_H

/****************************************************************************
 * Definições de constantes (tamanhos de campos, limites de listas e etc.). *
 ****************************************************************************/

// Limites de listas dinâmicas.
#define MAX_TELEFONES 5
#define MAX_REDES_SOCIAIS 5
#define MAX_LISTAS 1000 // Limite para evitar alocação excessiva em caso de arquivos corrompidos.

// Tamanhos de campos de texto, considerando o caractere nulo.
#define TAM_SIM_NAO 3 // Para ler respostas de "sim" ou "não" (s/n) com espaço para o caractere nulo.
#define TAM_CPF 16
#define TAM_TELEFONE 20
#define TAM_TEXTO_PEQUENO 25
#define TAM_TEXTO_MEDIO 50
#define TAM_TEXTO_GRANDE 200
#define TAM_BUFFER_LEITURA 64 // Utilizada para leitura de inteiros, garantindo que o buffer seja grande o suficiente para evitar estouro e permitindo a validação adequada da entrada do usuário.

/***********************************************************************
 * Mensagens de status usadas em: utils, main, persistencia e módulos. *
 ***********************************************************************/

// Erro irrecuperável e saída do programa.
#define MSG_ENCERRANDO_SEM_SALVAR "Devido ao erro, o programa será encerrado sem salvar!\n"
#define MSG_EOF_DETECTADO "EOF detectado. Encerrando entrada de dados.\n"
#define MSG_ERRO_LEITURA "Erro de leitura. Encerrando entrada de dados.\n"
#define MSG_ERRO_ALOCAR_MEMORIA "Ocorreu um erro ao alocar memória!"
#define MSG_ERRO_SALVAR_DADOS "Erro ao salvar dados de %s.\n"

// Mensagens de validação e status para funções de utilidade.
#define MSG_ENTRADA_INVALIDA "Entrada inválida! Tente novamente.\n"
#define MSG_INTEIRO_INVALIDO "Número inválido! Informe um número inteiro válido: "
#define MSG_LIMITE_CARACTERES_ATINGIDO "Limite de caracteres atingido. Tente novamente.\n"
#define MSG_DADOS_SALVOS_SUCESSO "Dados de %s salvos com sucesso!\n"
#define MSG_ARQUIVO_VAZIO "Arquivo vazio. Lista inicializada vazia.\n"
#define MSG_ARQUIVO_NAO_ENCONTRADO "Arquivo não encontrado ou corrompido. Lista inicializada vazia.\n"
#define MSG_ARQUIVO_CARREGADO "Dados carregados com sucesso.\n"

/*********************************************************************
 * Mensagens de status para modulos (artistas, obras, colaborações). *
 *********************************************************************/

#define MSG_ALTERACAO_CANCELADA "Alteração cancelada pelo usuário.\n"
#define MSG_ALTERACAO_REALIZADA_SUCESSO "Alteração realizada com sucesso!\n"
#define MSG_EXCLUSAO_CANCELADA "Exclusão cancelada pelo usuário.\n"
#define MSG_EXCLUSAO_REALIZADA_SUCESSO "Exclusão realizada com sucesso!\n"
#define MSG_CADASTRO_CANCELADO "Cadastro cancelado pelo usuário.\n"
#define MSG_CADASTRO_REALIZADO_SUCESSO "Cadastro realizado com sucesso!\n"
#define MSG_CONFIRMAR_EXCLUSAO "Confirma a exclusão de %s? (s/n): "
#define MSG_CONFIRMAR_ALTERACAO "Confirma a alteração de: %s para: %s? (s/n): "
#define MSG_INFORMAR_CHAVE "Informe o %s do %s que deseja %s: "
#define MSG_NENHUM_CADASTRADO "Nenhum %s cadastrado.\n"

/* ----- Nomes dos arquivos de persistência. ----- */
#define NOME_ARQUIVO_ARTISTAS "artistas.bin"
#define NOME_ARQUIVO_OBRAS "obras.bin"
#define NOME_ARQUIVO_COLABORACOES "colaboracoes.bin"

#endif // DEFINES_H
