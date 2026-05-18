/****************************************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes                          *
 * Arquivo: persistencia.c                                                  *
 * Autor: Iano de Oliva Kuhlmann                                            *
 * Colaboradores: chat.deepseek.com                                         *
 * Disciplina: APR2                                                         *
 * Professora: Dra. Eloize Rossi Marques Seno                               *
 ****************************************************************************/

/**************************************
 * ARQUIVO DE FUNÇÕES DE PERSISTÊNCIA *
 **************************************/

/*****************************
 * BIBLIOTECAS E IMPORTAÇÕES *
 *****************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "persistencia.h"
#include "defines.h"
#include "estruturas.h"

/************
 * ARTISTAS *
 ************/

bool carregarArtistas(ListaArtistas *lista)
{
    FILE *fp = fopen(NOME_ARQUIVO_ARTISTAS, "rb");
    if (fp == NULL) // Arquivo não existe retorna false, mas a lista será inicializada vazia no módulo principal.
        return false;
    int total;
    size_t lidos = fread(&total, sizeof(int), 1, fp);
    if (lidos != 1) // Arquivo vazio ou corrompido retorna false, mas a lista será inicializada vazia no módulo principal.
    {
        fclose(fp);
        return false; 
    }

    // Inicializa lista com capacidade adequada
    if (total > 0)
        inicializarListaArtistas(lista, total);
    else
    {
        fclose(fp);
        inicializarListaArtistas(lista, 4); // Inicia lista vazia em caso de arquivo vazio, mas não é um erro.
        return true; // Arquivo vazio, mas não é um erro, apenas retorna true com lista vazia.
    }

    if (lista->itens == NULL) // Falha na alocação de memória
    {
        fclose(fp);
        return false;
    }

    int i;
    for (i = 0; i < total; i++)
    {
        Artista a;

        // Inicializa ponteiros dinâmicos como NULL
        a.telefones = NULL;
        a.totalTelefones = 0;
        a.capacidadeTelefones = 0;
        a.redesSociais = NULL;
        a.totalRedesSociais = 0;
        a.capacidadeRedesSociais = 0;

        // Lê CPF
        if (fread(a.cpf, sizeof(char), TAM_CPF, fp) != TAM_CPF)
        {
            fclose(fp);
            return false;
        }
        // Lê nome
        if (fread(a.nome, sizeof(char), TAM_TEXTO_PEQUENO, fp) != TAM_TEXTO_PEQUENO)
        {
            fclose(fp);
            return false;
        }
        // Lê nacionalidade
        if (fread(a.nacionalidade, sizeof(char), TAM_TEXTO_PEQUENO, fp) != TAM_TEXTO_PEQUENO)
        {
            fclose(fp);
            return false;
        }
        // Lê estilo
        if (fread(a.estilo, sizeof(char), TAM_TEXTO_PEQUENO, fp) != TAM_TEXTO_PEQUENO)
        {
            fclose(fp);
            return false;
        }

        // Força terminador nulo no último byte de cada string
        a.cpf[TAM_CPF - 1] = '\0';
        a.nome[TAM_TEXTO_PEQUENO - 1] = '\0';
        a.nacionalidade[TAM_TEXTO_PEQUENO - 1] = '\0';
        a.estilo[TAM_TEXTO_PEQUENO - 1] = '\0';

        // Lê data de nascimento
        if(fread(&a.nascimento.dia, sizeof(int), 1, fp) != 1)
        {
            fclose(fp);
            return false;
        }

        if(fread(&a.nascimento.mes, sizeof(int), 1, fp) != 1)
        {
            fclose(fp);
            return false;
        }

        if(fread(&a.nascimento.ano, sizeof(int), 1, fp) != 1)
        {
            fclose(fp);
            return false;
        }

        // Lê telefones
        int totalTelefones;
        if(fread(&totalTelefones, sizeof(int), 1, fp) != 1)
        {
            fclose(fp);
            return false;
        }
        if (totalTelefones > 0)
        {
            a.telefones = (Telefone *) malloc(sizeof(Telefone) * totalTelefones);
            if (a.telefones == NULL)
            {
                fclose(fp);
                return false;
            }
            a.totalTelefones = totalTelefones;
            a.capacidadeTelefones = totalTelefones;

            int j;
            for (j = 0; j < totalTelefones; j++)
            {
                if(fread(a.telefones[j].numeroTelefone, sizeof(char), TAM_TELEFONE, fp) != TAM_TELEFONE)
                {
                    free(a.telefones);
                    fclose(fp);
                    return false;
                }
                a.telefones[j].numeroTelefone[TAM_TELEFONE - 1] = '\0';
            }
        }

        // Lê redes sociais
        int totalRedes;
        if(fread(&totalRedes, sizeof(int), 1, fp) != 1)
        {
            free(a.redesSociais);
            fclose(fp);
            return false;
        }
        if (totalRedes > 0)
        {
            a.redesSociais = (redeSocial *) malloc(sizeof(redeSocial) * totalRedes);
            if (a.redesSociais == NULL)
            {
                free(a.redesSociais);
                fclose(fp);
                return false;
            }
            a.totalRedesSociais = totalRedes;
            a.capacidadeRedesSociais = totalRedes;
            int j;
            for (j = 0; j < totalRedes; j++)
            {
                if(fread(a.redesSociais[j].redeSocial, sizeof(char), TAM_TEXTO_PEQUENO, fp) != TAM_TEXTO_PEQUENO)
                {
                    free(a.redesSociais);
                    fclose(fp);
                    return false;
                }
                if(fread(a.redesSociais[j].usuario, sizeof(char), TAM_TEXTO_PEQUENO, fp) != TAM_TEXTO_PEQUENO)
                {
                    free(a.redesSociais);
                    fclose(fp);
                    return false;
                }
                a.redesSociais[j].redeSocial[TAM_TEXTO_PEQUENO - 1] = '\0';
                a.redesSociais[j].usuario[TAM_TEXTO_PEQUENO - 1] = '\0';
            }
        }

        // Adiciona artista à lista
        if (adicionarArtista(lista, &a) == false)
        {
            // Libera memória alocada para este artista antes de sair
            free(a.telefones);
            free(a.redesSociais);
            fclose(fp);
            return false;
        }
    }

    fclose(fp);
    return true;
}

bool salvarArtistas(const ListaArtistas *lista)
{
    FILE *fp = fopen(NOME_ARQUIVO_ARTISTAS, "wb");
    if (fp == NULL)
        return false;

    // Escreve o total de artistas
    fwrite(&lista->total, sizeof(int), 1, fp);

    int i;
    for (i = 0; i < lista->total; i++)
    {
        const Artista *a = &lista->itens[i];

        // Campos de texto com tamanho fixo
        fwrite(a->cpf, sizeof(char), TAM_CPF, fp);
        fwrite(a->nome, sizeof(char), TAM_TEXTO_PEQUENO, fp);
        fwrite(a->nacionalidade, sizeof(char), TAM_TEXTO_PEQUENO, fp);
        fwrite(a->estilo, sizeof(char), TAM_TEXTO_PEQUENO, fp);

        // Data
        fwrite(&a->nascimento.dia, sizeof(int), 1, fp);
        fwrite(&a->nascimento.mes, sizeof(int), 1, fp);
        fwrite(&a->nascimento.ano, sizeof(int), 1, fp);

        // Telefones
        fwrite(&a->totalTelefones, sizeof(int), 1, fp);
        int j;
        for (j = 0; j < a->totalTelefones; j++)
        {
            fwrite(a->telefones[j].numeroTelefone, sizeof(char), TAM_TELEFONE, fp);
        }

        // Redes sociais
        fwrite(&a->totalRedesSociais, sizeof(int), 1, fp);
        for (j = 0; j < a->totalRedesSociais; j++)
        {
            fwrite(a->redesSociais[j].redeSocial, sizeof(char), TAM_TEXTO_PEQUENO, fp);
            fwrite(a->redesSociais[j].usuario, sizeof(char), TAM_TEXTO_PEQUENO, fp);
        }
    }

    fclose(fp);
    return true;
}

/*********
 * OBRAS *
 *********/

bool carregarObras(ListaObras *lista)
{
    FILE *fp = fopen(NOME_ARQUIVO_OBRAS, "rb");
    if (fp == NULL) // Arquivo não existe: inicia lista vazia
    {
        inicializarListaObras(lista, 4);
        return true;
    }

    int total;
    size_t lidos = fread(&total, sizeof(int), 1, fp);
    if (lidos != 1) // Arquivo vazio ou corrompido: inicia lista vazia
    {
        fclose(fp);
        inicializarListaObras(lista, 4);
        return true;
    }

    // Inicializa a lista com capacidade igual ao total, se total > 0,
    // senão usa 4 como padrão
    if (total > 0)
    {
        inicializarListaObras(lista, total);
    }
    else
    {
        inicializarListaObras(lista, 4);
    }

    if (lista->itens == NULL)
    {
        fclose(fp);
        return false;
    }

    int i;
    for (i = 0; i < total; i++)
    {
        Obra o;

        // Lê os inteiros
        fread(&o.id, sizeof(int), 1, fp);
        fread(&o.anoCriacao, sizeof(int), 1, fp);
        fread(&o.valorCentavos, sizeof(int), 1, fp);

        // Lê os campos de texto
        fread(o.titulo, sizeof(char), TAM_TEXTO_PEQUENO, fp);
        fread(o.tipo, sizeof(char), TAM_TEXTO_PEQUENO, fp);
        fread(o.descricao, sizeof(char), TAM_TEXTO_GRANDE, fp);

        // Adiciona a obra à lista
        if (adicionarObra(lista, &o) == false)
        {
            fclose(fp);
            return false;
        }
    }

    fclose(fp);
    return true;
}

bool salvarObras(const ListaObras *lista)
{
    FILE *fp = fopen(NOME_ARQUIVO_OBRAS, "wb");
    if (fp == NULL)
    {
        return false;
    }

    // Escreve o total de obras no início do arquivo
    fwrite(&lista->total, sizeof(int), 1, fp);

    // Escreve cada obra
    int i;
    for (i = 0; i < lista->total; i++)
    {
        const Obra *o = &lista->itens[i];

        // Campos inteiros
        fwrite(&o->id, sizeof(int), 1, fp);
        fwrite(&o->anoCriacao, sizeof(int), 1, fp);
        fwrite(&o->valorCentavos, sizeof(int), 1, fp);

        // Campos de texto com tamanho fixo
        fwrite(o->titulo, sizeof(char), TAM_TEXTO_PEQUENO, fp);
        fwrite(o->tipo, sizeof(char), TAM_TEXTO_PEQUENO, fp);
        fwrite(o->descricao, sizeof(char), TAM_TEXTO_GRANDE, fp);
    }

    fclose(fp);
    return true;
}

/****************
 * COLABORAÇÕES *
 ****************/

bool carregarColaboracoes(ListaColaboracoes *lista)
{
    FILE *fp = fopen(NOME_ARQUIVO_COLABORACOES, "rb");
    if (fp == NULL)
    {
        // Arquivo não existe: inicia lista vazia
        inicializarListaColaboracoes(lista, 4);
        return true;
    }

    int total;
    size_t lidos = fread(&total, sizeof(int), 1, fp);
    if (lidos != 1)
    {
        // Arquivo vazio ou corrompido: inicia lista vazia
        fclose(fp);
        inicializarListaColaboracoes(lista, 4);
        return true;
    }

    // Inicializa lista com capacidade adequada
    if (total > 0)
    {
        inicializarListaColaboracoes(lista, total);
    }
    else
    {
        inicializarListaColaboracoes(lista, 4);
    }

    if (lista->itens == NULL)
    {
        fclose(fp);
        return false;
    }

    int i;
    for (i = 0; i < total; i++)
    {
        Colaboracao c;

        // Lê a chave da colaboração (CPF e ID da obra)
        fread(c.chaveColab.cpf, sizeof(char), TAM_CPF, fp);
        fread(&c.chaveColab.id, sizeof(int), 1, fp);

        // Força terminador nulo no CPF
        c.chaveColab.cpf[TAM_CPF - 1] = '\0';

        // Lê função do artista
        fread(c.funcaoArtista, sizeof(char), TAM_TEXTO_PEQUENO, fp);
        c.funcaoArtista[TAM_TEXTO_PEQUENO - 1] = '\0';

        // Lê percentual de contribuição
        fread(&c.percentualContribuicao, sizeof(int), 1, fp);

        // Lê data de entrada
        fread(&c.entrada.dia, sizeof(int), 1, fp);
        fread(&c.entrada.mes, sizeof(int), 1, fp);
        fread(&c.entrada.ano, sizeof(int), 1, fp);

        // Lê data de saída
        fread(&c.saida.dia, sizeof(int), 1, fp);
        fread(&c.saida.mes, sizeof(int), 1, fp);
        fread(&c.saida.ano, sizeof(int), 1, fp);

        // Adiciona colaboração à lista
        if (adicionarColaboracao(lista, &c) == false)
        {
            fclose(fp);
            return false;
        }
    }

    fclose(fp);
    return true;
}

bool salvarColaboracoes(const ListaColaboracoes *lista)
{
    FILE *fp = fopen(NOME_ARQUIVO_COLABORACOES, "wb");
    if (fp == NULL)
    {
        return false;
    }

    // Escreve o total de colaborações
    fwrite(&lista->total, sizeof(int), 1, fp);

    int i;
    for (i = 0; i < lista->total; i++)
    {
        const Colaboracao *c = &lista->itens[i];

        // Chave da colaboração
        fwrite(c->chaveColab.cpf, sizeof(char), TAM_CPF, fp);
        fwrite(&c->chaveColab.id, sizeof(int), 1, fp);

        // Função do artista
        fwrite(c->funcaoArtista, sizeof(char), TAM_TEXTO_PEQUENO, fp);

        // Percentual de contribuição
        fwrite(&c->percentualContribuicao, sizeof(int), 1, fp);

        // Data de entrada
        fwrite(&c->entrada.dia, sizeof(int), 1, fp);
        fwrite(&c->entrada.mes, sizeof(int), 1, fp);
        fwrite(&c->entrada.ano, sizeof(int), 1, fp);

        // Data de saída
        fwrite(&c->saida.dia, sizeof(int), 1, fp);
        fwrite(&c->saida.mes, sizeof(int), 1, fp);
        fwrite(&c->saida.ano, sizeof(int), 1, fp);
    }

    fclose(fp);
    return true;
}
