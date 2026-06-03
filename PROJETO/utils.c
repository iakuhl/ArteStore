/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: utils.c                                *
 * Descrição: Implementação das funções            *
 *            utilitárias compartilhadas entre os  *
 *            módulos do sistema.                  *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: ChatGPT (OpenAI), DeepSeek Chat  *
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

static bool dadosInformados(char *entrada, int tamanho)
{
	if (fgets(entrada, tamanho, stdin) == NULL)
	{
		if (feof(stdin))
			printf(MSG_EOF_DETECTADO);
		else
			printf(MSG_ERRO_LEITURA);
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
		// Verifica a entrada de dados
		if(!dadosInformados(entrada,sizeof(entrada)))
			return false;

        // Verifica se a entrada ultrapassa o limite do buffer
        if(!verificarLimiteString(entrada))
        {
            printf(MSG_LIMITE_CARACTERES_ATINGIDO);
            continue;
        }

        errno = 0; // Prepara errno para detectar overflow
        valor = strtol(entrada, &fim, 10); // Converte a string para long em base decimal
        // Verifica overflow do long
        if (errno == ERANGE && (valor == LONG_MAX || valor == LONG_MIN))
        {
            printf(MSG_INTEIRO_INVALIDO);
            continue;
        }

		// Confirma se ao menos um caractere numérico foi informado.
        if (fim == entrada)
        {
            printf(MSG_ENTRADA_INVALIDA);
            continue;
        }

		// Verifica se o fim da conversão em strtol é um 'enter' ou terminador de string.
        if (*fim != '\n' && *fim != '\0')
        {
            printf(MSG_ENTRADA_INVALIDA);
            continue;
        }

        // Verifica se cabe em int para retornar
        if (valor < INT_MIN || valor > INT_MAX)
        {
            printf(MSG_INTEIRO_INVALIDO);
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
        {
            printf(MSG_LIMITE_CARACTERES_ATINGIDO);
            continue;
        }

        texto[strcspn(texto, "\n")] = '\0';

        if (strlen(texto) == 0)
        {
            printf(MSG_ENTRADA_INVALIDA);
            continue;
        }

        return true;
    }
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

bool lerSimNao(char *resposta)
{
    do
    {
        if (!lerString(resposta, TAM_SIM_NAO))
            return false;

        if (*resposta == 's' || *resposta == 'S' || *resposta == 'n' || *resposta == 'N')
            return true;

        else
            printf("Resposta inválida. Por favor, responda com 's' ou 'n'.\n");
    } while (resposta[0] != 's' && resposta[0] != 'S' && resposta[0] != 'n' && resposta[0] != 'N');
    return true;
}

int escolherOpcao(int min, int max)
{
    int opcao;
    while (true)
    {
        printf("Escolha uma opção (%d-%d): ", min, max);
        if (!lerInteiro(&opcao))
        {
            return -99; // Retorna -99 para indicar erro crítico
        }

        if (opcao >= min && opcao <= max)
            return opcao;
        else
            printf(MSG_ENTRADA_INVALIDA);
    }
}

void imprimeCPF(const char *cpf) // Função para imprimir um CPF formatado (XXX.XXX.XXX-XX).
{
    printf("%c%c%c.%c%c%c.%c%c%c-%c%c", cpf[0], cpf[1], cpf[2], cpf[3], cpf[4], cpf[5], cpf[6], cpf[7], cpf[8], cpf[9], cpf[10]);
}

void imprimeValor(int valor)
{
    int centavos, reais, divisor;

    centavos = valor % 100;
    reais = valor / 100;

    printf("R$ ");
	// Se não precisar formatar com '.' para milhares, imprime e retorna.
    if (reais < 1000)
    {
        printf("%d,%02d\n", reais, centavos);
        return;
    }
	
    divisor = 1000;

    while (reais / divisor >= 1000)
    {
        divisor = divisor * 1000;
    }

    printf("%d", reais / divisor);
    reais = reais % divisor;

    while (divisor > 1)
    {
        divisor = divisor / 1000;
        printf(".%03d", reais / divisor);
        reais = reais % divisor;
    }

    printf(",%02d\n", centavos);
}


/*
time_t converterData(Data d) {
    struct tm t = {0};
    t.tm_mday = d.dia;
    t.tm_mon = d.mes - 1;     // C usa 0-11
    t.tm_year = d.ano - 1900; // C usa anos desde 1900
    
    return mktime(&t);
}
*/

/*
bool validarData(Data d)
{
	struct tm t = {0};
    // 1. Validação matemática básica (valores impossíveis)
    if (d.ano < 1 || d.mes < 1 || d.mes > 12 || d.dia < 1 || d.dia > 31) {
        return false;
    }
		
	t = converterData(d);
    // mktime tenta normalizar a data. Se falhar, retorna -1
    if (mktime(&t) == -1)
        return false;

    // 3. Se o mktime alterou o dia ou o mês, a data original era inválida
    // Exemplo: se o usuário digitou 31/04, o mktime muda para 01/05.
    if (t.tm_mday != d.dia || (t.tm_mon + 1) != d.mes) {
        return false;
    }
    return true;
}
*/
