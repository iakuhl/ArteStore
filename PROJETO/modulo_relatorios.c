/****************************************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes                          *
 * Arquivo: modulo_relatorios.c                                             *
 * Autor: Iano de Oliva Kuhlmann                                            *
 * Colaboradores: chat.deepseek.com                                         *
 * Link de colaboração: https://chat.deepseek.com/share/jil3nf8yyu9wwz0h8l  *
 * Disciplina: APR2                                                         *
 * Professora: Dra. Eloize Rossi Marques Seno                               *
 ****************************************************************************/

#include "modulo_relatorios.h"

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
