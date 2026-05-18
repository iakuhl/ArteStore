/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: modulo_relatorios.c                    *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: chat.deepseek.com                *
 * Disciplina: APR2                                *
 * Professora: Dra. Eloize Rossi Marques Seno      *
 ***************************************************/

/*********************
 * MÓDULO RELATÓRIOS *
 *********************/

/*****************************
 * BIBLIOTECAS E IMPORTAÇÕES *
 *****************************/

#include <stdio.h>
#include <stdbool.h>

#include "modulo_relatorios.h"
#include "defines.h"
#include "estruturas.h"
#include "utils.h"
#include "listas.h"
#include "persistencia.h"

static int menuRelatorios()
{
    printf("### MENU RELATORIOS ###\n");
    printf("1 - Relatorio de Artistas\n");
    printf("2 - Relatorio de Obras\n");
    printf("3 - Relatorio de Colaboracoes\n");
    printf("4 - Voltar ao Menu Principal\n");

    printf("Escolha uma opção: ");
    return escolherOpcao(1, 4);
}

bool moduloRelatorios(ListaArtistas *listaArtistas, ListaObras *listaObras, ListaColaboracoes *listaColaboracoes)
{
    do
    {
        switch (menuRelatorios())
        {
            case 1:
                // Relatório de Artistas
                break;
            case 2:
                // Relatório de Obras
                break;
            case 3:
                // Relatório de Colaborações
                break;
            case 4:
                printf("Voltando ao Menu Principal...\n");
                return true;
        }
    } while (true);
}
