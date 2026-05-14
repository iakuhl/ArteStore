/*
 * Projeto: Sistema de Curadoria de Obras de Artes
 * Arquivo: menus.c
 * Autor: Iano de Oliva Kuhlmann
 * Colaboradores: chat.deepseek.com
 * Link de colaboração: https://chat.deepseek.com/share/jil3nf8yyu9wwz0h8l
 * Disciplina: APR2
 * Professora: Dra. Eloize Rossi Marques Seno
 */

/******************************
 * ARQUIVO DE FUNÇÕES DE MENU *
 ******************************/


 /***************
 * BIBLIOTECAS *
 ***************/
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>



/***************
 * IMPORTAÇÕES *
 ***************/
#include "utils.h"
#include "defines.h"
#include "listas.h"
#include "menus.h"



int escolherOpcao(int min, int max)
{
    int opcao;
    while (true)
    {
        printf("Escolha uma opção (%d-%d): ", min, max);
        if (!lerInteiro(&opcao))
        {
            // Se falhou (EOF ou erro), sair do programa
            printf(MSG_LOOP_INFINITO);
            return -1; // Retorna -1 para indicar erro
        }

        if (opcao >= min && opcao <= max)
            return opcao;
        else
            printf(MSG_ENTRADA_INVALIDA);
    }
}

int menuPrincipal()
{
    printf("### MENU PRINCIPAL ###\n");
    printf("1 - Menu Artistas\n");
    printf("2 - Menu Obras\n");
    printf("3 - Menu Colaboracoes\n");
    printf("4 - Menu Relatorios\n");
    printf("5 - Encerrar Programa\n");

    return escolherOpcao(1, 5);
}

int menuArtistas()
{
    printf("### MENU ARTISTAS ###\n");
    printf("1 - Cadastrar Artista\n");
    printf("2 - Listar Artistas\n");
    printf("3 - Buscar Artista por CPF\n");
    printf("4 - Voltar ao Menu Principal\n");

    return escolherOpcao(1, 4);
}

int menuObras()
{
    printf("### MENU OBRAS ###\n");
    printf("1 - Cadastrar Obra\n");
    printf("2 - Listar Obras\n");
    printf("3 - Buscar Obra por ID\n");
    printf("4 - Voltar ao Menu Principal\n");

    return escolherOpcao(1, 4);
}

int menuColaboracoes()
{
    printf("### MENU COLABORACOES ###\n");
    printf("1 - Cadastrar Colaboracao\n");
    printf("2 - Listar Colaboracoes\n");
    printf("3 - Voltar ao Menu Principal\n");

    return escolherOpcao(1, 3);
}

int menuRelatorios()
{
    printf("### MENU RELATORIOS ###\n");
    printf("1 - Relatorio de Artistas\n");
    printf("2 - Relatorio de Obras\n");
    printf("3 - Relatorio de Colaboracoes\n");
    printf("4 - Voltar ao Menu Principal\n");

    return escolherOpcao(1, 4);
}
