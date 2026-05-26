/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: persistencia.c                         *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: chat.deepseek.com                *
 * Disciplina: APR2                                *
 * Professora: Dra. Eloize Rossi Marques Seno      *
 ***************************************************/

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

 /***************************
 * CARREGAMENTO DE ARTISTAS *
 ****************************/

bool carregarArquivo(char *nome, FILE *arquivo, int *total)
{
    arquivo = fopen(nome, "rb"); // Carrega arquivo para leitura.
    if (arquivo == NULL) // Verifica se arquivo existe
    {
        printf(MSG_NAO_ENCONTRADO);
        return false; // Retorna falso para criar lista vazia, após informar ao usuário.
    }
    
    // Verifica se foi possível ler o arquivo e se algum dado foi lido.
    size_t lidos = fread(&total, sizeof(int), 1, arquivo);
    if (lidos != 1 || total < 0) 
    {
        fclose(arquivo);
        printf(MSG_ARQUIVO_VAZIO); // Vazio ou corrompido.
        return false; // Retorna falso para criar lista vazia, após informar ao usuário.
    }
    
    fclose(arquivo);
    printf(MSG_ARQUIVO_CARREGADO);
    return true; // Retorna verdadeiro, arquivo carregado com sucesso.
}

int carregarArtistas(ListaArtistas *lista)
{
    // Tenta ler arquivo 
    FILE *arquivo;
    char nome[] = NOME_ARQUIVO_ARTISTAS;
    int *total;
    if(!carregaArquivo(&nome, &arquivo, &total)) // Se retorno falso, arquivo não carregado, inicia lista vazia.
	{
        inicializarListaArtistas(lista, 4);
		return -2;
	}
    else
        inicializarListaArtistas(lista, *total);
    
    if (lista->itens == NULL)
    {
        return -1; // Falha ao alocar memória para a lista, mesmo com capacidade adequada. Retorna -1 para tratar erro na main.
    }
    
    if(*total>0)
    {
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
            if (fread(a.cpf, sizeof(char), TAM_CPF, arquivo) != TAM_CPF)
            {
                fclose(arquivo);
                return i;
            }
            // Lê nome
            if (fread(a.nome, sizeof(char), TAM_TEXTO_PEQUENO, arquivo) != TAM_TEXTO_PEQUENO)
            {
                fclose(arquivo);
                return i;
            }
            // Lê nacionalidade
            if (fread(a.nacionalidade, sizeof(char), TAM_TEXTO_PEQUENO, arquivo) != TAM_TEXTO_PEQUENO)
            {
                fclose(arquivo);
                return i;
            }
            // Lê estilo
            if (fread(a.estilo, sizeof(char), TAM_TEXTO_PEQUENO, arquivo) != TAM_TEXTO_PEQUENO)
            {
                fclose(arquivo);
                return i;
            }
    
            // Força terminador nulo no último byte de cada string
            a.cpf[TAM_CPF - 1] = '\0';
            a.nome[TAM_TEXTO_PEQUENO - 1] = '\0';
            a.nacionalidade[TAM_TEXTO_PEQUENO - 1] = '\0';
            a.estilo[TAM_TEXTO_PEQUENO - 1] = '\0';
    
            // Lê data de nascimento
            if(fread(&a.nascimento.dia, sizeof(int), 1, arquivo) != 1)
            {
                fclose(arquivo);
                return i;
            }
    
            if(fread(&a.nascimento.mes, sizeof(int), 1, arquivo) != 1)
            {
                fclose(arquivo);
                return i;
            }
    
            if(fread(&a.nascimento.ano, sizeof(int), 1, arquivo) != 1)
            {
                fclose(arquivo);
                return i;
            }
    
            // Lê telefones
            int totalTelefones;
            if(fread(&totalTelefones, sizeof(int), 1, arquivo) != 1)
            {
                fclose(arquivo);
                return i;
            }
            if (totalTelefones > 0)
            {
                a.telefones = (Telefone *) malloc(sizeof(Telefone) * totalTelefones);
                if (a.telefones == NULL)
                {
                    fclose(arquivo);
                    return i;
                }
                a.totalTelefones = totalTelefones;
                a.capacidadeTelefones = totalTelefones;
    
                int j;
                for (j = 0; j < totalTelefones; j++)
                {
                    if(fread(a.telefones[j].numeroTelefone, sizeof(char), TAM_TELEFONE, arquivo) != TAM_TELEFONE)
                    {
                        free(a.telefones);
                        fclose(arquivo);
                        return i;
                    }
                    a.telefones[j].numeroTelefone[TAM_TELEFONE - 1] = '\0';
                }
            }
    
            // Lê redes sociais
            int totalRedes;
            if(fread(&totalRedes, sizeof(int), 1, arquivo) != 1)
            {
                free(a.redesSociais);
                fclose(arquivo);
                return i;
            }
            if (totalRedes > 0)
            {
                a.redesSociais = (redeSocial *) malloc(sizeof(redeSocial) * totalRedes);
                if (a.redesSociais == NULL)
                {
                    free(a.redesSociais);
                    fclose(arquivo);
                    return i;
                }
                a.totalRedesSociais = totalRedes;
                a.capacidadeRedesSociais = totalRedes;
                int j;
                for (j = 0; j < totalRedes; j++)
                {
                    if(fread(a.redesSociais[j].redeSocial, sizeof(char), TAM_TEXTO_PEQUENO, arquivo) != TAM_TEXTO_PEQUENO)
                    {
                        free(a.redesSociais);
                        fclose(arquivo);
                        return i;
                    }
                    if(fread(a.redesSociais[j].usuario, sizeof(char), TAM_TEXTO_PEQUENO, arquivo) != TAM_TEXTO_PEQUENO)
                    {
                        free(a.redesSociais);
                        fclose(arquivo);
                        return i;
                    }
                    a.redesSociais[j].redeSocial[TAM_TEXTO_PEQUENO - 1] = '\0';
                    a.redesSociais[j].usuario[TAM_TEXTO_PEQUENO - 1] = '\0';
                }
            }
    
            // Adiciona artista à lista
            if (!adicionarArtista(lista, &a))
            {
                // Libera memória alocada para este artista antes de sair
                free(a.telefones);
                free(a.redesSociais);
                fclose(arquivo);
                return i; // Retorna o número de artistas carregados com sucesso antes do erro
            }
        }
    }
    fclose(arquivo);
    return -3; // Sucesso total, todos os artistas carregados sem erros.
}

bool salvarArtistas(const ListaArtistas *lista)
{
    FILE *fp = fopen(NOME_ARQUIVO_ARTISTAS, "wb");
    if (fp == NULL)
        return i;

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
    return 1;
}

 /************************
 * CARREGAMENTO DE OBRAS *
 *************************/

bool carregarObras(ListaObras *lista)
{
    char nome[] = NOME_ARQUIVO_OBRAS;
    FILE *arquivo;
    int *total;
    
    if(!carregaArquivo(&nome, &arquivo, &total)) // Se retorno falso, arquivo não carregado, inicia lista vazia.
	{
        inicializarListaObras(lista, 4);
		return -2;
	}
    else
        inicializarListaObras(lista, *total);
    
    if (lista->itens == NULL)
    {
        printf(MSG_ERRO_ALOCAR_MEMORIA);
        return -1; // Falha ao alocar memória para a lista, mesmo com capacidade adequada. Retorna -1 para tratar erro na main.
    }

    if(*total>0)
    {
        int i;
        for (i = 0; i < *total; i++)
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
            if (!adicionarObra(lista, &o))
            {
                fclose(fp);
                return i;
            }
        }
    }

    fclose(fp);
    return 1;
}

bool salvarObras(const ListaObras *lista)
{
    FILE *fp = fopen(NOME_ARQUIVO_OBRAS, "wb");
    if (fp == NULL)
    {
        return i;
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
    return 1;
}

/****************
 * COLABORAÇÕES *
 ****************/

bool carregarColaboracoes(ListaColaboracoes *lista)
{
    char[] = NOME_ARQUIVO_COLABORACOES;
    FILE *arquivo;
    int *total;
    
    if(!carregaArquivo(nome, &arquivo, &total))
        inicializarListaColaboracoes(lista, 4);
    else
        inicializarListaColaboracoes(lista, *total);
    
    int i;
    for (i = 0; i < *total; i++)
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
        if (!adicionarColaboracao(lista, &c))
        {
            fclose(fp);
            return i;
        }
    }

    fclose(fp);
    return 1;
}

bool salvarColaboracoes(const ListaColaboracoes *lista)
{
    FILE *fp = fopen(NOME_ARQUIVO_COLABORACOES, "wb");
    if (fp == NULL)
    {
        return i;
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
    return 1;
}
