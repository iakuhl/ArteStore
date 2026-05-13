/*
 * Projeto: Sistema de Curadoria de Obras de Artes
 * Arquivo: utils.c
 * Autor: Iano de Oliva Kuhlmann
 * Colaboradores: chat.deepseek.com
 * Disciplina: APR2
 * Professora: Dra. Eloize Rossi Marques Seno
 */


#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define TAM_BUFFER_LEITURA 32

// FUNÇÕES UTILITARIAS


/*
 // Validação robusta para entrada de inteiros
*/
bool lerInteiro(int *numero)
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
            int c;
			c = getchar();
            while (c != '\n' && c != EOF)
			{
				c = getchar();
			}
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


// Validação robusta para entrada de strings

bool lerString(char texto[], int tamanho)
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
            int c;
			c = getchar();
            while (c != '\n' && c != EOF)
			{
				c = getchar();
			}
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
