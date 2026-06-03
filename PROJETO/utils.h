/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: utils.h                                *
 * Descrição: Protótipos das funções utilitárias   *
 *            utilizadas por diferentes módulos    *
 *            do sistema.                          *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: ChatGPT (OpenAI), DeepSeek Chat  *
 * Disciplina: APR2                                *
 * Professora: Dra. Eloize Rossi Marques Seno      *
 ***************************************************/
 
/*******************
 * HEADER DE UTILS *
 *******************/

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

bool lerInteiro(int *numero); // Validação robusta para entrada de inteiros, trata casos de overflow, entradas não numéricas e números fora da faixa de int.
bool lerString(char texto[], int tamanho); // Validação robusta para entrada de strings, trata casos de entradas vazias, excesso de caracteres e erros de leitura.

bool validarCPF(const char cpf[]); // Função para validar CPF.
bool removeMascaraCPF(char *cpf); // Remove pontos e hífen de um CPF, mantendo apenas os dígitos, e valida o resultado. Retorna true se o CPF é válido (com ou sem máscara) e false caso contrário.

bool lerSimNao(char *resposta); // Função para ler uma resposta de sim ou não do usuário, retornando true para sim e false para não.
int escolherOpcao(int min, int max); // Função para ler a escolha do usuário em um menu, garantindo que seja um número inteiro dentro do intervalo permitido.
void imprimeCPF(const char *cpf); // Função para imprimir um CPF formatado (XXX.XXX.XXX-XX).
void imprimeValor(int valor); // Função para imprimir valor em reais formatado (R$ 00.000,00).

#endif // UTILS_H
