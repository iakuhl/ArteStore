/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: persistencia.c                         *
 * Descrição: Implementação das rotinas de         *
 *            gravação e leitura dos dados em      *
 *            arquivos binários.                   *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: ChatGPT (OpenAI), DeepSeek Chat  *
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

 /***********************************
 * CARREGAMENTO GENÉRICO DE ARQUIVO *
 ***********************************/

FILE *carregarArquivo(char *nome, int *total) // Retorna ponteiro para o arquivo, ou NULL em caso de arquivo inexistente, vazio ou corrompido. O total de itens lidos é alterado por referência.
{
	FILE *arquivo = fopen(nome, "rb"); // Carrega arquivo para leitura.
    if (arquivo == NULL) // Verifica se arquivo existe
    {
        printf(MSG_ARQUIVO_NAO_ENCONTRADO);
        return NULL;
    }
    
    // Verifica se foi possível ler o arquivo e se algum dado foi lido.
	// Todas as listas utilizam o mesmo formato de gravação, com o total de itens no início do arquivo, então é possível usar a mesma função para ler o total e verificar se o arquivo está vazio ou corrompido.
    if (fread(total, sizeof(int), 1, arquivo) != 1 || *total <= 0) 
    {
        fclose(arquivo);
        printf(MSG_ARQUIVO_VAZIO); // Vazio ou corrompido.
        return NULL;
    }

    return arquivo;
}

/********************************************************************************
 * Retornos das funções de carregamento específicas:							*
 * -1: Lista inicializada vazia ou com todos os dados carregados com sucesso.	*
 * -99: Falha ao alocar memória para a lista, mesmo com capacidade adequada.	*
 * i (índice): Último índice carregado com sucesso.								*
 ********************************************************************************/

 /***********
 * ARTISTAS *
 ************/

int carregarArtistas(ListaArtistas *lista) 
{
    FILE *arquivo;
    char nome[] = NOME_ARQUIVO_ARTISTAS;
    int total, i, j, totalTelefones, totalRedes;

	// Tenta ler arquivo e inicializa a lista com capacidade adequada.
	arquivo = carregarArquivo(nome, &total);

	if (arquivo == NULL)
	{
		total = 4;
	}
	
	inicializarListaArtistas(lista, total);

	if (lista->itens == NULL)
	{
		if (arquivo != NULL)
			fclose(arquivo);

		printf(MSG_ERRO_ALOCAR_MEMORIA);
		return -99;
	}

	if (arquivo == NULL)
		return -1;

	// Entra no loop para escrever o arquivo na lista somente após tratar todos os possíveis erros: Arquivo inexistente, vazio, corrompido ou com nenhum item.
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
		if(fread(&totalTelefones, sizeof(int), 1, arquivo) != 1)
		{
			fclose(arquivo);
			return i;
		}

		if(totalTelefones < 0 || totalTelefones > MAX_TELEFONES) // Limite para evitar alocação excessiva em caso de arquivo corrompido.
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
		if(fread(&totalRedes, sizeof(int), 1, arquivo) != 1)
		{
			free(a.telefones);
			fclose(arquivo);
			return i;
		}
		
		if( totalRedes < 0 || totalRedes > MAX_REDES_SOCIAIS) // Limite para evitar alocação excessiva em caso de arquivo corrompido.
		{
			free(a.telefones);
			fclose(arquivo);
			return i;
		}

		if (totalRedes > 0)
		{
			a.redesSociais = (redeSocial *) malloc(sizeof(redeSocial) * totalRedes);
			if (a.redesSociais == NULL)
			{
				free(a.telefones);
				fclose(arquivo);
				return i;
			}
			a.totalRedesSociais = totalRedes;
			a.capacidadeRedesSociais = totalRedes;
			for (j = 0; j < totalRedes; j++)
			{
				if(fread(a.redesSociais[j].redeSocial, sizeof(char), TAM_TEXTO_PEQUENO, arquivo) != TAM_TEXTO_PEQUENO)
				{
					free(a.redesSociais);
					free(a.telefones);
					fclose(arquivo);
					return i;
				}
				if(fread(a.redesSociais[j].usuario, sizeof(char), TAM_TEXTO_PEQUENO, arquivo) != TAM_TEXTO_PEQUENO)
				{
					free(a.redesSociais);
					free(a.telefones);
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
    fclose(arquivo);
	printf(MSG_ARQUIVO_CARREGADO);
    return -1; // Sucesso total, todos os artistas carregados sem erros.
}

bool salvarArtistas(const ListaArtistas *lista)
{
    FILE *arquivo = fopen(NOME_ARQUIVO_ARTISTAS, "wb");
	int i, j;

    if (arquivo == NULL)
        return false;

    // Escreve o total de artistas
    if(fwrite(&lista->total, sizeof(int), 1, arquivo) != 1)
    {
        fclose(arquivo);
        return false;
    }

    for (i = 0; i < lista->total; i++)
    {
        const Artista *a = &lista->itens[i];

        // Campos de texto com tamanho fixo
        if(fwrite(a->cpf, sizeof(char), TAM_CPF, arquivo) != TAM_CPF)
        {
            fclose(arquivo);
            return false;
        }
        if(fwrite(a->nome, sizeof(char), TAM_TEXTO_PEQUENO, arquivo) != TAM_TEXTO_PEQUENO)
        {
            fclose(arquivo);
            return false;
        }
        if(fwrite(a->nacionalidade, sizeof(char), TAM_TEXTO_PEQUENO, arquivo) != TAM_TEXTO_PEQUENO)
        {
            fclose(arquivo);
            return false;
        }
        if(fwrite(a->estilo, sizeof(char), TAM_TEXTO_PEQUENO, arquivo) != TAM_TEXTO_PEQUENO)
        {
            fclose(arquivo);
            return false;
        }

        // Data
        if(fwrite(&a->nascimento.dia, sizeof(int), 1, arquivo) != 1)
        {
            fclose(arquivo);
            return false;
        }
        if(fwrite(&a->nascimento.mes, sizeof(int), 1, arquivo) != 1)
        {
            fclose(arquivo);
            return false;
        }
        if(fwrite(&a->nascimento.ano, sizeof(int), 1, arquivo) != 1)
        {
            fclose(arquivo);
            return false;
        }

        // Telefones
        if(fwrite(&a->totalTelefones, sizeof(int), 1, arquivo) != 1)
        {
            fclose(arquivo);
            return false;
        }
        for (j = 0; j < a->totalTelefones; j++)
        {
            if(fwrite(a->telefones[j].numeroTelefone, sizeof(char), TAM_TELEFONE, arquivo) != TAM_TELEFONE)
            {
                fclose(arquivo);
                return false;
            }
        }

        // Redes sociais
        if(fwrite(&a->totalRedesSociais, sizeof(int), 1, arquivo) != 1)
        {
            fclose(arquivo);
            return false;
        }
        for (j = 0; j < a->totalRedesSociais; j++)
        {
            if(fwrite(a->redesSociais[j].redeSocial, sizeof(char), TAM_TEXTO_PEQUENO, arquivo) != TAM_TEXTO_PEQUENO)
            {
                fclose(arquivo);
                return false;
            }
            if(fwrite(a->redesSociais[j].usuario, sizeof(char), TAM_TEXTO_PEQUENO, arquivo) != TAM_TEXTO_PEQUENO)
            {
                fclose(arquivo);
                return false;
            }
        }
    }
    fclose(arquivo);
    return true;
}

 /********
 * OBRAS *
 *********/

int carregarObras(ListaObras *lista) 
{
    char nome[] = NOME_ARQUIVO_OBRAS;
    FILE *arquivo;
    int total, i;

	// Tenta ler arquivo, após, verifica se o arquivo contém dados e inicializa a lista com capacidade adequada.
	arquivo = carregarArquivo(nome, &total);
	if (arquivo == NULL)
	{
		total = 4;
	}
		
	inicializarListaObras(lista, total);

	if (lista->itens == NULL)
	{
		if (arquivo != NULL)
			fclose(arquivo);

		printf(MSG_ERRO_ALOCAR_MEMORIA);
		return -99;
	}

	if (arquivo == NULL)
		return -1;
    
	// Verifica se a memória foi alocada com sucesso, retorna -99 em caso de falha, para tratar erro na main.
    if (lista->itens == NULL)
    {
        printf(MSG_ERRO_ALOCAR_MEMORIA);
        return -99; // Falha ao alocar memória para a lista, mesmo com capacidade adequada. Retorna -1 para tratar erro na main.
    }
	else
	{
		if (total == 0)
		{
			fclose(arquivo);
			printf(MSG_ARQUIVO_VAZIO); // Arquivo vazio, lista iniciada vazia.
			return -1;
		}
		if(total>0)
		{
			for (i = 0; i < total; i++)
			{
				Obra o;
				
				// Lê os inteiros
				if(fread(&o.id, sizeof(int), 1, arquivo) != 1)
				{
					fclose(arquivo);
					return i;
				}
				if(fread(&o.anoCriacao, sizeof(int), 1, arquivo) != 1)
				{
					fclose(arquivo);
					return i;
				}
				if(fread(&o.valorCentavos, sizeof(int), 1, arquivo) != 1)
				{
					fclose(arquivo);
					return i;
				}
		
				// Lê os campos de texto
				if(fread(o.titulo, sizeof(char), TAM_TEXTO_PEQUENO, arquivo) != TAM_TEXTO_PEQUENO)
				{
					fclose(arquivo);
					return i;
				}
				if(fread(o.tipo, sizeof(char), TAM_TEXTO_PEQUENO, arquivo) != TAM_TEXTO_PEQUENO)
				{
					fclose(arquivo);
					return i;
				}
				if(fread(o.descricao, sizeof(char), TAM_TEXTO_GRANDE, arquivo) != TAM_TEXTO_GRANDE)
				{
					fclose(arquivo);
					return i;
				}
				
				// Adiciona a obra à lista
				if (!adicionarObra(lista, &o))
				{
					fclose(arquivo);
					return i;
				}
			}
		}

	}

    fclose(arquivo);
	printf(MSG_ARQUIVO_CARREGADO);
    return -1;
}

bool salvarObras(const ListaObras *lista)
{
    FILE *arquivo = fopen(NOME_ARQUIVO_OBRAS, "wb");
	int i;
    if (arquivo == NULL)
    {
        return false;
    }

    // Escreve o total de obras no início do arquivo
    if(fwrite(&lista->total, sizeof(int), 1, arquivo) != 1)
    {
        fclose(arquivo);
        return false;
    }

    // Escreve cada obra
    for (i = 0; i < lista->total; i++)
    {
        const Obra *o = &lista->itens[i];

        // Campos inteiros
        if(fwrite(&o->id, sizeof(int), 1, arquivo) != 1)
        {
            fclose(arquivo);
            return false;
        }
        if(fwrite(&o->anoCriacao, sizeof(int), 1, arquivo) != 1)
        {
            fclose(arquivo);
            return false;
        }
        if(fwrite(&o->valorCentavos, sizeof(int), 1, arquivo) != 1)
        {
            fclose(arquivo);
            return false;
        }

        // Campos de texto com tamanho fixo
        if(fwrite(o->titulo, sizeof(char), TAM_TEXTO_PEQUENO, arquivo) != TAM_TEXTO_PEQUENO)
        {
            fclose(arquivo);
            return false;
        }
        if(fwrite(o->tipo, sizeof(char), TAM_TEXTO_PEQUENO, arquivo) != TAM_TEXTO_PEQUENO)
        {
            fclose(arquivo);
            return false;
        }
        if(fwrite(o->descricao, sizeof(char), TAM_TEXTO_GRANDE, arquivo) != TAM_TEXTO_GRANDE)
        {
            fclose(arquivo);
            return false;
        }
    }

    fclose(arquivo);
    return true;
}

/****************
 * COLABORAÇÕES *
 ****************/

int carregarColaboracoes(ListaColaboracoes *lista) 
{
    char nome[] = NOME_ARQUIVO_COLABORACOES;
    FILE *arquivo;
    int total, i;

	arquivo = carregarArquivo(nome, &total);
	if (arquivo == NULL)
	{
		total = 4;
	}
		
	inicializarListaColaboracoes(lista, total);

	if (lista->itens == NULL)
	{
		if (arquivo != NULL)
			fclose(arquivo);

		printf(MSG_ERRO_ALOCAR_MEMORIA);
		return -99;
	}

	if (arquivo == NULL)
		return -1;
    
    for (i = 0; i < total; i++)
    {
        Colaboracao c;

        // Lê a chave da colaboração (CPF e ID da obra)
        if(fread(c.chaveColab.cpf, sizeof(char), TAM_CPF, arquivo) != TAM_CPF)
		{
			fclose(arquivo);
			return i;
		}
        if(fread(&c.chaveColab.id, sizeof(int), 1, arquivo) != 1)
		{
			fclose(arquivo);
			return i;
		}

        // Força terminador nulo no CPF
        c.chaveColab.cpf[TAM_CPF - 1] = '\0';

        // Lê função do artista
        if(fread(c.funcaoArtista, sizeof(char), TAM_TEXTO_PEQUENO, arquivo) != TAM_TEXTO_PEQUENO)
		{
			fclose(arquivo);
			return i;
		}
        c.funcaoArtista[TAM_TEXTO_PEQUENO - 1] = '\0';

        // Lê percentual de contribuição
        if(fread(&c.percentualContribuicao, sizeof(int), 1, arquivo) != 1)
		{
			fclose(arquivo);
			return i;
		}

        // Lê data de entrada
        if(fread(&c.entrada.dia, sizeof(int), 1, arquivo) != 1)
		{
			fclose(arquivo);
			return i;
		}
        if(fread(&c.entrada.mes, sizeof(int), 1, arquivo) != 1)
		{
			fclose(arquivo);
			return i;
		}
        if(fread(&c.entrada.ano, sizeof(int), 1, arquivo) != 1)
		{
			fclose(arquivo);
			return i;
		}

        // Lê data de saída
        if(fread(&c.saida.dia, sizeof(int), 1, arquivo) != 1)
		{
			fclose(arquivo);
			return i;
		}
        if(fread(&c.saida.mes, sizeof(int), 1, arquivo) != 1)
		{
			fclose(arquivo);
			return i;
		}
        if(fread(&c.saida.ano, sizeof(int), 1, arquivo) != 1)
		{
			fclose(arquivo);
			return i;
		}

        // Adiciona colaboração à lista
        if (!adicionarColaboracao(lista, &c))
        {
            fclose(arquivo);
            return i;
        }
    }

    fclose(arquivo);
	printf(MSG_ARQUIVO_CARREGADO);
    return -1;
}

bool salvarColaboracoes(const ListaColaboracoes *lista)
{
    FILE *arquivo = fopen(NOME_ARQUIVO_COLABORACOES, "wb");
	int i;
    if (arquivo == NULL)
    {
        return false;
    }

    // Escreve o total de colaborações
    if(fwrite(&lista->total, sizeof(int), 1, arquivo) != 1)
    {
        fclose(arquivo);
        return false;
    }

    for (i = 0; i < lista->total; i++)
    {
        const Colaboracao *c = &lista->itens[i];

        // Chave da colaboração
        if(fwrite(c->chaveColab.cpf, sizeof(char), TAM_CPF, arquivo) != TAM_CPF)
        {
            fclose(arquivo);
            return false;
        }
        if(fwrite(&c->chaveColab.id, sizeof(int), 1, arquivo) != 1)
        {
            fclose(arquivo);
            return false;
        }

        // Função do artista
        if(fwrite(c->funcaoArtista, sizeof(char), TAM_TEXTO_PEQUENO, arquivo) != TAM_TEXTO_PEQUENO)
        {
            fclose(arquivo);
            return false;
        }

        // Percentual de contribuição
        if(fwrite(&c->percentualContribuicao, sizeof(int), 1, arquivo) != 1)
        {
            fclose(arquivo);
            return false;
        }

        // Data de entrada
        if(fwrite(&c->entrada.dia, sizeof(int), 1, arquivo) != 1)
        {
            fclose(arquivo);
            return false;
        }
        if(fwrite(&c->entrada.mes, sizeof(int), 1, arquivo) != 1)
        {
			fclose(arquivo);
			return false;
		}
        if(fwrite(&c->entrada.ano, sizeof(int), 1, arquivo) != 1)
        {
            fclose(arquivo);
            return false;
        }

        // Data de saída
        if(fwrite(&c->saida.dia, sizeof(int), 1, arquivo) != 1)
        {
            fclose(arquivo);
            return false;
        }
        if(fwrite(&c->saida.mes, sizeof(int), 1, arquivo) != 1)
        {
            fclose(arquivo);
            return false;
        }
        if(fwrite(&c->saida.ano, sizeof(int), 1, arquivo) != 1)
        {
            fclose(arquivo);
            return false;
        }
    }

    fclose(arquivo);
    return true;
}
