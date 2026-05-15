/*
 * Projeto: Sistema de Curadoria de Obras de Artes
 * Arquivo: utils.h
 * Autor: Iano de Oliva Kuhlmann
 * Colaboradores: chat.deepseek.com
 * Link de colaboração: https://chat.deepseek.com/share/jil3nf8yyu9wwz0h8l
 * Disciplina: APR2
 * Professora: Dra. Eloize Rossi Marques Seno
 */
 
#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

void limparBuffer(); // Limpa o buffer de entrada para evitar problemas com entradas inválidas.
bool verificarLimiteString(const char *texto); // Verifica se a string ultrapassa o limite permitido, considerando o caractere nulo.

bool lerInteiro(int *numero); // Validação robusta para entrada de inteiros, trata casos de overflow, entradas não numéricas e números fora da faixa de int.
bool lerString(char texto[], int tamanho); // Validação robusta para entrada de strings, trata casos de entradas vazias, excesso de caracteres e erros de leitura.


bool validarCPF(const char cpf[]); // Função para validar CPF.


#endif