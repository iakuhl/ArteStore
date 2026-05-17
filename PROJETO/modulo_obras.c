/****************************************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes                          *
 * Arquivo: modulo_obras.c                                                  *
 * Autor: Iano de Oliva Kuhlmann                                            *
 * Colaboradores: chat.deepseek.com                                         *
 * Link de colaboração: https://chat.deepseek.com/share/jil3nf8yyu9wwz0h8l  *
 * Disciplina: APR2                                                         *
 * Professora: Dra. Eloize Rossi Marques Seno                               *
 ****************************************************************************/

#include "modulo_obras.h"

static int menuObras()
{
    printf("### MENU OBRAS ###\n");
    printf("1 - Cadastrar Obra\n");
    printf("2 - Listar Obras\n");
    printf("3 - Buscar Obra por ID\n");
    printf("4 - Voltar ao Menu Principal\n");

    printf("Escolha uma opção: ");
    return escolherOpcao(1, 4);
}
