/****************************************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes                          *
 * Arquivo: modulo_colaboracoes.c                                           *
 * Autor: Iano de Oliva Kuhlmann                                            *
 * Colaboradores: chat.deepseek.com                                         *
 * Link de colaboração: https://chat.deepseek.com/share/jil3nf8yyu9wwz0h8l  *
 * Disciplina: APR2                                                         *
 * Professora: Dra. Eloize Rossi Marques Seno                               *
 ****************************************************************************/

#include "modulo_colaboracoes.h"

static int menuColaboracoes()
{
    printf("### MENU COLABORACOES ###\n");
    printf("1 - Cadastrar Colaboracao\n");
    printf("2 - Listar Colaboracoes\n");
    printf("3 - Voltar ao Menu Principal\n");

    printf("Escolha uma opção: ");
    return escolherOpcao(1, 3);
}
