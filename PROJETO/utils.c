/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: utils.c                                *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: chat.deepseek.com                *
 * Disciplina: APR2                                *
 * Professora: Dra. Eloize Rossi Marques Seno      *
 ***************************************************/

/***********************************************************
 * ARQUIVO DE FUNÇÕES UTILITÁRIAS E DE TRATAMENTO DE DADOS *
 ***********************************************************/

/*****************************
 * BIBLIOTECAS E IMPORTAÇÕES *
 *****************************/

#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "utils.h"
#include "defines.h"

/**********************************************
 * FUNÇÕES DE TRATAMENTO E VALIDAÇÃO DE DADOS *
 **********************************************/

static void limparBuffer() // Limpa o buffer de entrada para evitar problemas com entradas inválidas.
{
    int c;
    c = getchar();
    while(c != '\n' && c != EOF)
	{
		c = getchar();
	}
}

static bool verificarLimiteString(const char *texto) // Verifica se a string ultrapassa o limite permitido, considerando o caractere nulo.
{
    /* Tratamento de buffer, verifica se a entrada coube completamente na string. Se '\n' não estiver presente, a entrada excedeu o limite. *
     * Se houve excesso, limpa o buffer e retorna falso. Se couber na string retorna true.                                                  */

    if (strchr(texto, '\n') == NULL)
    {
        limparBuffer();
        return false;
    }
    return true;
}

static bool dadosInformados(const char *entrada, int tamanho)
{
	if (fgets(entrada, tamanho, stdin) == NULL)
	{
		if (feof(stdin))
			printf("MSG_EOF_DETECTADO");
		else
			printf("MSG_ERRO_LEITURA");
		return false;
	}
	return true;
}

int lerInteiro(int *numero) // Validação robusta para entrada de inteiros.
{
    char entrada[TAM_BUFFER_LEITURA];
    char *fim;
    long valor;

	// Verifica a entrada de dados
	if(!dadosInformados(entrada,sizeof(entrada)))
		return -1;

	// Verifica se a entrada ultrapassa o limite do buffer
	if(!verificarLimiteString(entrada))
		printf("MSG_LIMITE_CARACTERES_ATINGIDO");
		return 0;

	errno = 0; // Prepara errno para detectar overflow
	valor = strtol(entrada, &fim, 10); // Converte a string para long em base decimal
	// Verifica overflow do long
	if (errno == ERANGE && (valor == LONG_MAX || valor == LONG_MIN))
	{
		printf(MSG_INTEIRO_INVALIDO);
		return 0;
	}

	// Confirma se ao menos um caractere numérico foi informado.
	if (fim == entrada)
	{
		printf(MSG_ENTRADA_INVALIDA);
		return 0;
	}

	// Verifica se o fim da conversão em strtol é um 'enter' ou terminador de string.
	if (*fim != '\n' && *fim != '\0')
	{
		printf(MSG_ENTRADA_INVALIDA);
		return 0;
	}

	// Verifica se cabe em int para retornar
	if (valor < INT_MIN || valor > INT_MAX)
	{
		printf(MSG_INTEIRO_INVALIDO);
		return 0;
	}

	*numero = (int)valor;
	return 1;
}

int lerString(char texto[], int tamanho) // Validação robusta para entrada de strings.
{
		// Verifica a entrada de dados.
		if(!dadosInformados(texto,tamanho))
			return -1; // Erro irrecuperável, sair sem salvar

        // Verifica se a entrada ultrapassa o limite do buffer
        if(!verificarLimiteString(texto))
        {
            printf("MSG_LIMITE_CARACTERES_ATINGIDO");
            return 0; // Erro, tente novamente
        }

        texto[strcspn(texto, "\n")] = '\0';
		
        if (strlen(texto) == 0)
        {
            printf(MSG_ENTRADA_INVALIDA);
            return 0; // Erro, tente novamente
        }

        return 1; // Sucesso
}

bool removeMascaraCPF(char *cpf)
{
    // Se já for válido (11 dígitos), não faz nada.
    if (validarCPF(cpf))
        return true;

    // Remove pontos e hífen, mantendo apenas os dígitos
    int i, j;
	i=0;
	j=0;
	while(cpf[i] != '\0')
	{
		if (cpf[i] >= '0' && cpf[i] <= '9')
        {
            cpf[j] = cpf[i];
            j++;
        }
		i++;
	}
	cpf[j] = '\0';

    // Após remoção, verifica validade (já inclui teste de comprimento e dígitos)
    return validarCPF(cpf);
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

/************************
 * FUNÇÕES DE UTILIDADE *
 ************************/

int escolherOpcao(int min, int max)
{
    int opcao;
	int validacao = 0;
    while (validacao == 0)
    {
        printf("Escolha uma opção (%d-%d): ", min, max);
		validacao = lerInteiro(&opcao);
        if (validacao == -1)
        {
            // Se falhou (EOF ou erro), retornar ao menu principal e limpar memória.
            printf(MSG_LOOP_INFINITO);
            return -1; // Retorna -1 para indicar erro
        }
		
		if (validacao == 1)
	        if (opcao >= min && opcao <= max)
	            return opcao;
        else
            printf(MSG_ENTRADA_INVALIDA);
    }
}
