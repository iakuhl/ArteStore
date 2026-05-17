/****************************************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes                          *
 * Arquivo: utils.h                                                         *
 * Autor: Iano de Oliva Kuhlmann                                            *
 * Colaboradores: chat.deepseek.com                                         *
 * Link de colaboração: https://chat.deepseek.com/share/jil3nf8yyu9wwz0h8l  *
 * Disciplina: APR2                                                         *
 * Professora: Dra. Eloize Rossi Marques Seno                               *
 ****************************************************************************/
 
#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

static void limparBuffer(); // Limpa o buffer de entrada para evitar problemas com entradas inválidas.
static bool verificarLimiteString(const char *texto); // Verifica se a string ultrapassa o limite permitido, considerando o caractere nulo.
static bool dadosInformados(const char *entrada, int tamanho); // Verifica se a entrada de dados foi realizada corretamente, tratando casos de EOF e erros de leitura.

bool lerInteiro(int *numero); // Validação robusta para entrada de inteiros, trata casos de overflow, entradas não numéricas e números fora da faixa de int.
bool lerString(char texto[], int tamanho); // Validação robusta para entrada de strings, trata casos de entradas vazias, excesso de caracteres e erros de leitura.

bool validarCPF(const char cpf[]); // Função para validar CPF.
bool removeMascaraCPF(char *cpf); // Remove pontos e hífen de um CPF, mantendo apenas os dígitos, e valida o resultado. Retorna true se o CPF é válido (com ou sem máscara) e false caso contrário.

int escolherOpcao(int min, int max); // Função para ler a escolha do usuário em um menu, garantindo que seja um número inteiro dentro do intervalo permitido.

#endif // UTILS_H