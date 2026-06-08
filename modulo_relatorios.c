/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: modulo_relatorios.c                    *
 * Descrição: Implementação das funcionalidades    *
 *            de geração, consulta e exibição dos  *
 *            relatórios do sistema.               *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: ChatGPT (OpenAI), DeepSeek Chat  *
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
#include <string.h>
#include <time.h>

#include "modulo_artistas.h"
#include "modulo_obras.h"
#include "modulo_colaboracoes.h"
#include "modulo_relatorios.h"
#include "defines.h"
#include "estruturas.h"
#include "utils.h"
#include "listas.h"
#include "persistencia.h"

static int menuRelatorios()
{
    printf("\n### MENU RELATORIOS ###\n");
    printf("1 - Relatorio de Obras por Artista\n");
    printf("2 - Relatorio de Obras por Tipo\n");
    printf("3 - Relatorio de Colaboracoes por data\n");
    printf("4 - Voltar ao Menu Principal\n");

    return escolherOpcao(1, 4);
}

static void preencherValorEmRelatorio(FILE *arquivo, long long valor)
{
    long long centavos;
    long long reais;
    long long divisor;
    centavos = valor % 100;
    reais = valor / 100;
    fprintf(arquivo, "R$ ");
    if (reais < 1000)
    {
        fprintf(arquivo, "%lld,%02lld\n", reais, centavos);
        return;
    }
    divisor = 1000;
    while (reais / divisor >= 1000)
    {
        divisor *= 1000;
    }
    fprintf(arquivo, "%lld", reais / divisor);
    reais %= divisor;
    while (divisor > 1)
    {
        divisor /= 1000;
        fprintf(arquivo, ".%03lld", reais / divisor);
        reais %= divisor;
    }
    fprintf(arquivo, ",%02lld\n", centavos);
}

static void preencherRelatorioObrasPorTipo(FILE *arquivo, const Obra *o)
{
    fprintf(arquivo, "\n--- Obra ---\n");
    fprintf(arquivo, "ID: %d\n", o->id);
    fprintf(arquivo, "Título: %s\n", o->titulo);
    fprintf(arquivo, "Tipo: %s\n", o->tipo);
    fprintf(arquivo, "Ano: %d\n", o->anoCriacao);

    fprintf(arquivo, "Valor: "); preencherValorEmRelatorio(arquivo, o->valorCentavos);

    fprintf(arquivo, "Descrição: %s\n", o->descricao);
}

static bool relatorioObrasPorArtista(const ListaColaboracoes *listaColaboracoes, const ListaObras *listaObras, const ListaArtistas *listaArtistas)
{
    int indiceArtista, i, indiceObra;
    const Artista *artista;
    const Colaboracao *colab;
    const Obra *obra;
    FILE *arquivo;
    bool encontrouObras;
    
    if(!buscarArtistaPorCPF(listaArtistas, &indiceArtista))
        return false;

    if (indiceArtista == -1)
        return true;

    arquivo = abrirArquivoRelatorio(NOME_ARQUIVO_RELATORIO_OBRA_POR_ARTISTA);
    if (arquivo == NULL)
        return false;
    printf("\n--- Relatorio de Obras por Artista ---\n");
    fprintf(arquivo,"\n--- Relatorio de Obras por Artista ---\n");
    artista = &listaArtistas->itens[indiceArtista];
    
    printf("Obras de %s\n", artista->nome);
    fprintf(arquivo, "Obras de %s\n", artista->nome);

    encontrouObras = false;
    for (i = 0; i < listaColaboracoes->total; i++)
    {
        colab = &listaColaboracoes->itens[i];
        if (strcmp(colab->chaveColab.cpf, artista->cpf) == 0)
        {
            encontrouObras = true;
            indiceObra = indiceObraPorID(listaObras, colab->chaveColab.id);
            if (indiceObra != -1)
            {
                obra = &listaObras->itens[indiceObra];
                printf("- %s (ID: %d), Papel: %s, Contribuicao: %d%%\n",
                    obra->titulo,
                    obra->id,
                    colab->funcaoArtista,
                    colab->percentualContribuicao);

                fprintf(arquivo, "- %s (ID: %d), Papel: %s, Contribuicao: %d%%\n",
                    obra->titulo,
                    obra->id,
                    colab->funcaoArtista,
                    colab->percentualContribuicao);
            }
            else
            {
                printf("- Obra ID %d (nao encontrada), Papel: %s, Contribuicao: %d%%\n",
                    colab->chaveColab.id,
                    colab->funcaoArtista,
                    colab->percentualContribuicao);

                fprintf(arquivo, "- Obra ID %d (nao encontrada), Papel: %s, Contribuicao: %d%%\n",
                    colab->chaveColab.id,
                    colab->funcaoArtista,
                    colab->percentualContribuicao);
            }
        }
    }
    if (!encontrouObras)
    {
        printf("Nenhuma obra encontrada para este artista.\n");
        fprintf(arquivo, "Nenhuma obra encontrada para este artista.\n");
    }
    fprintf(arquivo,"\n"); // Fim do relatório.
    fecharArquivoRelatorio(arquivo);
    printf("Relatório salvo em %s\n", NOME_ARQUIVO_RELATORIO_OBRA_POR_ARTISTA);
    return true; 
}

static bool relatorioObrasPorTipo(const ListaColaboracoes *listaColaboracoes, const ListaObras *listaObras, const ListaArtistas *listaArtistas)
{
    int indiceArtista, i, j, indiceObra;
    char tipoInformado[TAM_TEXTO_PEQUENO];
    FILE *arquivo;
    const Colaboracao *colab;
    const Obra *obra;
    bool encontrouColab;

    printf("\n--- Relatorio de Obras por Tipo ---\n");
    printf("Informe o tipo desejado: ");
    if (!lerString(tipoInformado,TAM_TEXTO_PEQUENO))
        return false;

    arquivo = abrirArquivoRelatorio(NOME_ARQUIVO_RELATORIO_OBRA_POR_TIPO);
    if (arquivo == NULL)
        return false;
    // Percorre Obras para encontrar obras que tenham o tipo informado pelo usuário
    for (i = 0; i < listaObras->total; i++)
    {
        encontrouColab = false;
        obra = &listaObras->itens[i];
        if (strcmp(obra->tipo, tipoInformado) == 0)
        {
            // Lista a Obra
            imprimirObraPorIndice(listaObras,i);
            preencherRelatorioObrasPorTipo(arquivo,obra);

            printf("  Por:\n");
            fprintf(arquivo,"  Por:\n");
            // Percorre colaborações buscando os artistas da Obra
            for (j = 0; j < listaColaboracoes->total; j++)
            {
                colab = &listaColaboracoes->itens[j];
                if (colab->chaveColab.id == obra->id && colab->chaveColab.cpf != NULL)
                {
                    indiceArtista = indiceArtistaPorCPF(listaArtistas, colab->chaveColab.cpf);
                    if (indiceArtista != -1)
                    {
                        printf("      %s\n", listaArtistas->itens[indiceArtista].nome);
                        fprintf(arquivo,"      %s\n", listaArtistas->itens[indiceArtista].nome);
                        encontrouColab = true;
                    }
                }
            }
            if(!encontrouColab)
            {
                printf("      Artistas não identificado");
                fprintf(arquivo,"      Artistas não identificado");
            }
        }
    }
    fprintf(arquivo,"\n"); // Fim do relatório.
    fecharArquivoRelatorio(arquivo);
    printf("Relatório salvo em %s\n", NOME_ARQUIVO_RELATORIO_OBRA_POR_TIPO);
    return true; 
}

static bool relatorioColabPorPeriodo(const ListaColaboracoes *listaColaboracoes, const ListaObras *listaObras, const ListaArtistas *listaArtistas)
{
    Data dataInicial, dataFinal;
    int i, indiceArtista, indiceObra;
    Colaboracao *c;
    Artista *a;
    Obra *o;
    FILE *arquivo;

    // Coleta data inicial válida.
    do{
        printf("Informe a data inicial:\n");
        printf("  Dia: ");
        if (!lerInteiro(&dataInicial.dia))
            return false;
        printf("  Mês: ");
        if (!lerInteiro(&dataInicial.mes))
            return false;
        printf("  Ano: ");
        if (!lerInteiro(&dataInicial.ano))
            return false;
        
        if (!validarData(dataInicial))
            printf(MSG_ENTRADA_INVALIDA);
    }while (!validarData(dataInicial));

    // Coleta data final válida.
    do{
        printf("Informe a data final:\n");
        printf("  Dia: ");
        if (!lerInteiro(&dataFinal.dia))
            return false;
        printf("  Mês: ");
        if (!lerInteiro(&dataFinal.mes))
            return false;
        printf("  Ano: ");
        if (!lerInteiro(&dataFinal.ano))
            return false;
        
        if (!validarData(dataFinal))
            printf(MSG_ENTRADA_INVALIDA);
    }while (!validarData(dataFinal));

    arquivo = abrirArquivoRelatorio(NOME_ARQUIVO_RELATORIO_COLAB);
    if (arquivo == NULL)
        return false;
    printf("Colaborações realizadas entre %02d/%02d/%d e %02d/%02d/%d:\n\n", dataInicial.dia, dataInicial.mes, dataInicial.ano, dataFinal.dia, dataFinal.mes, dataFinal.ano);
    fprintf(arquivo, "Colaborações realizadas entre %02d/%02d/%d e %02d/%02d/%d:\n\n", dataInicial.dia, dataInicial.mes, dataInicial.ano, dataFinal.dia, dataFinal.mes, dataFinal.ano);
    for (i = 0; i < listaColaboracoes->total; i++)
    {
        c = &listaColaboracoes->itens[i];

        // Compara se a entrada é igual ou posterior à data início E a saída é igual ou anterior à data fim.
        // Ou seja, para exibir no relatório, a colaboração tem que ter iniciado e encerrado entre o período informado pelo usuário.
        if(compararDatas(c->entrada, dataInicial) >= 0 && compararDatas(c->saida, dataFinal) <= 0)
        {
            indiceArtista = indiceArtistaPorCPF(listaArtistas, c->chaveColab.cpf);
            indiceObra = indiceObraPorID(listaObras,c->chaveColab.id);

            printf("%dª Colaboração: ", i+1);
            fprintf(arquivo, "%dª Colaboração:", i+1);
            if (indiceArtista != -1 && indiceObra != -1)
            {
                a = &listaArtistas->itens[indiceArtista];
                o = &listaObras->itens[indiceObra];

                printf("\n%s em %s (ID: %d) - ", a->nome, o->titulo, o->id);
                printf("participa como: %s.\n", c->funcaoArtista);
                printf("Sendo sua contribuição correspondente à %d%% da obra.\n\n", c->percentualContribuicao);

                fprintf(arquivo, "\n%s em %s (ID: %d) - ", a->nome, o->titulo, o->id);
                fprintf(arquivo, "participa como: %s.\n", c->funcaoArtista);
                fprintf(arquivo, "Sendo sua contribuição correspondente à %d%% da obra.\n\n", c->percentualContribuicao);
            }
            else
            {
                if (indiceArtista == -1 || indiceObra == -1)
                {
                    printf("O Artista %s ou a Obra %d da colaboração (Artista: %s)/(Obra: %d) foram excluídos do sistema!\n\n",
                        c->chaveColab.cpf,
                        c->chaveColab.id,
                        c->chaveColab.cpf,
                        c->chaveColab.id);
                    fprintf(arquivo, "O Artista %s ou a Obra %d da colaboração (Artista: %s)/(Obra: %d) foram excluídos do sistema!\n\n",
                        c->chaveColab.cpf,
                        c->chaveColab.id,
                        c->chaveColab.cpf,
                        c->chaveColab.id);
                }
            }
        }
    }
    fprintf(arquivo,"\n"); // Fim do relatório.
    fecharArquivoRelatorio(arquivo);
    printf("Relatório salvo em %s\n", NOME_ARQUIVO_RELATORIO_COLAB);
    return true;
}

bool moduloRelatorios(ListaArtistas *listaArtistas, ListaObras *listaObras, ListaColaboracoes *listaColaboracoes)
{
    int op;
    do
    {
        op = menuRelatorios();
        switch (op)
        {
            case 1:
                // Todas as obras de um artista específico, listando o título da obra, a função do artista na obra e o percentual de contribuição.
                if(!relatorioObrasPorArtista(listaColaboracoes, listaObras, listaArtistas))
                    return false;
                
                break;
            case 2:
                // Todas as obras de um tipo específico (ex: pintura, escultura, etc), listando o título da obra, o nome do artista principal e a data de criação.
                if(!relatorioObrasPorTipo(listaColaboracoes, listaObras, listaArtistas))
                    return false;
                break;
            case 3:
                // Todas as colaborações realizadas entre as datas X e Y (informadas pelo usuário), listando o nome do artista, o título da obra, o papel desempenhado por cada artista e o percentual de contribuição.
                if(!relatorioColabPorPeriodo(listaColaboracoes, listaObras, listaArtistas))
                    return false;
                break;
            case 4:
                printf("Voltando ao Menu Principal...\n");
                return true;
        }
    } while (true);
}
