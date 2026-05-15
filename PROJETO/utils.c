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


/***************
 * IMPORTAÇÕES *
 ***************/
#include "defines.h"
#include "utils.h"

/**********************************
 * FUNÇÕES DE TRATAMENTO DE DADOS *
 **********************************/

void limparBuffer() // Limpa o buffer de entrada para evitar problemas com entradas inválidas.
{
    int c;
    do {
        c = getchar();
    } while(c != '\n' && c != EOF);
}

bool verificarLimiteString(const char *texto) // Verifica se a string ultrapassa o limite permitido, considerando o caractere nulo.
{
    /* 
    * Tratamento de buffer, verifica se a entrada coube completamente na string.
	* Se '\n' não estiver presente, a entrada excedeu o limite.
    * Se houve excesso, limpa o buffer, informa erro ao usuário e retorna falso.
    * Se couber na string retorna true.
    */
    if (strchr(texto, '\n') == NULL)
    {
        printf("Erro: Limite de caracteres atingido. Tente novamente.\n");
        limparBuffer();
        return false;
    }
    return true;
}

bool dadosInformados(char *entrada, int tamanho)
{
	if (fgets(entrada, tamanho, stdin) == NULL)
	{
		if (feof(stdin))
			printf("\nEntrada encerrada pelo usuário.\n");
		else
			printf("Erro de leitura.\n");
		return false;
	}
	return true;
}

bool lerInteiro(int *numero) // Validação robusta para entrada de inteiros.
{
    char entrada[TAM_BUFFER_LEITURA];
    char *fim;
    long valor;

    while (true)
    {
        printf("Digite um número inteiro: ");

		// Verifica a entrada de dados
		if(!dadosInformados(entrada,sizeof(entrada)))
			return false;

        // Verifica se a entrada ultrapassa o limite do buffer
        if(!verificarLimiteString(entrada))
            continue;

        errno = 0; // Prepara errno para detectar overflow
        valor = strtol(entrada, &fim, 10); // Converte a string para long em base decimal
        // Verifica overflow do long
        if (errno == ERANGE && (valor == LONG_MAX || valor == LONG_MIN))
        {
            printf("Número muito grande! Informe um número inteiro: ");
            continue;
        }

		// Confirma se ao menos um caractere numérico foi informado.
        if (fim == entrada)
        {
            printf("Entrada inválida! Informe um número inteiro: ");
            continue;
        }

		// Verifica se o fim da conversão em strtol é um 'enter' ou terminador de string.
        if (*fim != '\n' && *fim != '\0')
        {
            printf("Entrada inválida! Informe um número inteiro: ");
            continue;
        }

        // Verifica se cabe em int para retornar
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
		// Verifica a entrada de dados.
		if(!dadosInformados(texto,tamanho))
			return false;
        // Verifica se a entrada ultrapassa o limite do buffer
        if(!verificarLimiteString(texto))
            continue;

        texto[strcspn(texto, "\n")] = '\0';
		
        if (strlen(texto) == 0)
        {
            printf("Entrada inválida! Informe um texto: ");
            continue;
        }

        return true;
    }
}

bool removeMascaraCPF(char *cpf) // 
{
	// Se CPF já vier sem máscara, sai da função.
	if (validarCPF(cpf))
		return true;
	
	// Remove os "." e o "-", caso o usuário entre com a formatação 000.000.000-00
	cpf[srtcspn(cpf,".")] = "";
	cpf[srtcspn(cpf,".")] = "";
	cpf[srtcspn(cpf,"-")] = "";
	
	// Verifica se existe algum valor informado, após remover a máscara de formatação.
	if (strlen(cpf) == 0)
		return false;
	
	// Verifica se após a remoção da máscara o CPF é válido
	if(validarCPF(cpf))
		return true;
	else
		return false;
}

bool validarCPF(const char cpf[]) // Função para validar CPF, deverá ser aprimorada para tratar casos de CPFs com formatação (com pontos e hífen).
{
	int i;
	
    if (strlen(cpf) != 11)
        return false;

    for (i = 0; i < 11; i++)
        if (cpf[i] < '0' || cpf[i] > '9')
            return false;

    return true;
}
