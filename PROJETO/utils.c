/*
 * Projeto: Sistema de Curadoria de Obras de Artes
 * Arquivo: utils.c
 * Autor: Iano de Oliva Kuhlmann
 * Colaboradores: chat.deepseek.com
 * Link de colaboração: https://chat.deepseek.com/share/jil3nf8yyu9wwz0h8l
 * Disciplina: APR2
 * Professora: Dra. Eloize Rossi Marques Seno
 */


/***********************************************************
 * ARQUIVO DE FUNÇÕES UTILITÁRIAS E DE TRATAMENTO DE DADOS *
 ***********************************************************/


 /**************
 * DECLARAÇÕES *
 ***************/
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <errno.h>

/***************
 * IMPORTAÇÕES *
 ***************/

#include "estruturas.h"
#include "utils.h"

/**********************************
 * FUNÇÕES DE TRATAMENTO DE DADOS *
 **********************************/

void limparBuffer()
{
    int c;
    c = getchar();
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

bool lerInteiro(int *numero) // Validação robusta para entrada de inteiros.
{
    char entrada[TAM_BUFFER_LEITURA];
    char *fim;
    long valor;

    while (true)
    {
        printf("Digite um número inteiro: ");
        if (fgets(entrada, sizeof(entrada), stdin) == NULL)
        {
            if (feof(stdin))
                printf("\nEntrada encerrada pelo usuário.\n");
            else
                printf("Erro de leitura.\n");
            return false;
        }

		/* 
		 * Tratamento de buffer, verifica se o tamanho da string informada é maior que o limite (tamanho -1).
		 * Se for maior, limpa o buffer e informa erro ao usuário e solicita nova entrada de dados.
		 * Garante entrada válida.
		 */
        if (strchr(entrada, '\n') == NULL)
        {
            printf("Entrada muito longa. Informe um número inteiro: ");
            limparBuffer();
            continue;
        }

        // Prepara errno para detectar overflow
        errno = 0;
        valor = strtol(entrada, &fim, 10);

        // Verifica overflow do long
        if (errno == ERANGE && (valor == LONG_MAX || valor == LONG_MIN))
        {
            printf("Número muito grande! Informe um número inteiro: ");
            continue;
        }

        if (fim == entrada)
        {
            printf("Entrada inválida! Informe um número inteiro: ");
            continue;
        }

        if (*fim != '\n' && *fim != '\0')
        {
            printf("Entrada inválida! Informe um número inteiro: ");
            continue;
        }

        // Verifica se cabe em int
        if (valor < INT_MIN || valor > INT_MAX)
        {
            printf("Número fora da faixa de inteiro! Informe um número inteiro: ");
            continue;
        }

        *numero = (int)valor;
        return true;
    }
}


bool lerString(char texto[], int tamanho) // Validação robusta para entrada de strings.
{
    while (true)
    {
        if (fgets(texto, tamanho, stdin) == NULL)
        {
            if (feof(stdin))
                printf("\nEntrada encerrada pelo usuário.\n");
            else
                printf("Erro de leitura.\n");
            return false;
        }
		
		/* 
		 * Tratamento de buffer, verifica se o tamanho da string informada é maior que o limite (tamanho -1).
		 * Se for maior, limpa o buffer e informa erro ao usuário e solicita nova entrada de dados.
		 * Garante entrada válida.
		 */
        if (strchr(texto, '\n') == NULL)
        {
            printf("Texto muito longo! Limite de %d caracteres. Tente novamente: ", tamanho - 1);
            limparBuffer();
            continue;
        }

        texto[strcspn(texto, "\n")] = '\0';

        if (strlen(texto) == 0)
        {
            printf("Entrada inválida! Informe um texto: ");
            continue;
        }

        return true;
    }
}

// Função para validar CPF, deverá ser aprimorada para tratar casos de CPFs com formatação (com pontos e hífen).
bool validarCPF(const char cpf[])
{
    if (strlen(cpf) != 11)
        return false;

    for (int i = 0; i < 11; i++)
    {
        if (cpf[i] < '0' || cpf[i] > '9')
            return false;
    }

    return true;
}
