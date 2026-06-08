/***************************************************
 * Projeto: Sistema de Curadoria de Obras de Artes *
 * Arquivo: modulo_colaboracoes.c                  *
 * Descrição: Implementação das funcionalidades    *
 *            de vínculo e gerenciamento das       *
 *            colaborações entre artistas e obras. *
 * Autor: Iano de Oliva Kuhlmann                   *
 * Colaboradores: ChatGPT (OpenAI), DeepSeek Chat  *
 * Disciplina: APR2                                *
 * Professora: Dra. Eloize Rossi Marques Seno      *
 ***************************************************/

/***********************
 * MÓDULO COLABORACOES *
 ***********************/

/*****************************
 * BIBLIOTECAS E IMPORTAÇÕES *
 *****************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "modulo_colaboracoes.h"
#include "defines.h"
#include "estruturas.h"
#include "utils.h"
#include "listas.h"
#include "persistencia.h"

// O módulo de colaborações possui a seguinte estrutura:
// Menu principal:
    // Listar um ou todos.
    // Cadastrar colaboração.
    // Excluir colaboração.
    // Alterar dados (sub-módulo do menu de colaboração):
        // Alterar Percentual Contribuição.
        // Alterar Datas Entrada/Saída.
        // Alterar Função/Papel do Artista.

// Uma colaboração, cujo artista ou obra tenha sido excluído do sistema, não pode ser alterada.

/*********************
 * EXIBIÇÃO DE MENUS *
 *********************/

static int menuColaboracoes()
{
    printf("\n### MENU COLABORACOES ###\n");
    printf("1 - Cadastrar Colaboracao\n");
    printf("2 - Listar Todas as Colaboracoes\n");
    printf("3 - Exibir dados de uma colaboração\n");
    printf("4 - Alterar uma colaboração\n");
    printf("5 - Excluir uma colaboração\n");

    printf("6 - Voltar ao Menu Principal\n");

    return escolherOpcao(1, 6);
}

static int menuAlteracoesColab()
{
    printf("\n### MENU DE ALTERAÇÕES DE COLABORACOES ###\n");
    printf("1 - Alterar Percentual de contribuição\n");
    printf("2 - Alterar Data de Entrada\n");
    printf("3 - Alterar Data de Saída\n");
    printf("4 - Alterar Função do Artista\n");
    printf("5 - Salvar e voltar ao Menu de colaborações\n");

    return escolherOpcao(1, 5);
}

/*******************************
 * FUNÇÕES DO MÓDULO PRINCIPAL *
 *******************************/

static bool cadastrarColaboracao(ListaColaboracoes *listaColaboracoes, const ListaArtistas *listaArtistas, const ListaObras *listaObras)
{
    // Verifica se existem artistas e obras cadastrados
    if (listaArtistas->total == 0)
    {
        printf("\nNao ha artistas cadastrados. Impossivel cadastrar colaboracao.\n");
        return true;
    }
    if (listaObras->total == 0)
    {
        printf("\nNao ha obras cadastradas. Impossivel cadastrar colaboracao.\n");
        return true;
    }

    Colaboracao c;
    Artista *a;
    Obra *o;
    Data dataObraFim; // Colaboração não pode ter iniciado após a publicação da obra.
    char cpfDigitado[TAM_CPF];
    int idObra;
    int indiceArtista;
    int indiceObra;

    printf("\n--- Cadastrar Colaboracao ---\n");

    // CPF do artista
    printf("CPF do artista: ");
    if (!lerString(cpfDigitado, TAM_CPF))
        return false;
    
    if (!removeMascaraCPF(cpfDigitado))
    {
        printf("CPF invalido.\n");
        return true; // Retorna true para não encerrar o módulo, apenas informar que o CPF é inválido.
    }

    indiceArtista = indiceArtistaPorCPF(listaArtistas, cpfDigitado);
    if (indiceArtista == -1)
    {
        printf("Artista nao encontrado.\n");
        return true; // Retorna true para não encerrar o módulo, apenas informar que o artista não foi encontrado.
    }
    a = &listaArtistas->itens[indiceArtista]; // Só para simplificar validações futuras.

    // ID da obra
    printf("ID da obra: ");
    if (!lerInteiro(&idObra))
        return false;

    if (idObra <= 0)
    {
        printf("ID invalido.\n");
        return true; // Retorna true para não encerrar o módulo, apenas informar que o ID é inválido.
    }

    indiceObra = indiceObraPorID(listaObras, idObra);
    if (indiceObra == -1)
    {
        printf("Obra nao encontrada.\n");
        return true; // Retorna true para não encerrar o módulo, apenas informar que a obra não foi encontrada.
    }
    o = &listaObras->itens[indiceObra]; // Só para simplificar validações futuras.
    dataObraFim.dia = 31;  dataObraFim.mes = 12; dataObraFim.ano = o->anoCriacao; // Atribui uma data final para a obra, como dia 31/12 do ano de publicação.

    // Verifica se já existe colaboração com mesma chave (cpf + idObra)
    if (indiceColaboracaoPorChave(listaColaboracoes, cpfDigitado, idObra) != -1)
    {
        printf("Ja existe uma colaboracao entre este artista e esta obra.\n");
        return true; // Retorna true para não encerrar o módulo, apenas informar que a colaboração já existe.
    }

    // Função do artista
    printf("Funcao do artista: ");
    if (!lerString(c.funcaoArtista, TAM_TEXTO_PEQUENO))
        return false;

    // Percentual de contribuição
    printf("Percentual de contribuicao (1-100): ");
    if (!lerInteiro(&c.percentualContribuicao))
    {
        return false;
    }
    if (c.percentualContribuicao < 1 || c.percentualContribuicao > 100)
    {
        printf("Percentual invalido. Deve estar entre 1 e 100.\n");
        return true; // Retorna true para não encerrar o módulo, apenas informar que o percentual é inválido.
    }

    do
    {
        // Data de entrada
        printf("Data de entrada:\n");

        printf("  Dia: ");
        if (!lerInteiro(&c.entrada.dia))
            return false;

        printf("  Mes: ");
        if (!lerInteiro(&c.entrada.mes))
            return false;

        printf("  Ano: ");
        if (!lerInteiro(&c.entrada.ano))
            return false;
        
        if(!validarData(c.entrada))
            printf(MSG_ENTRADA_INVALIDA);

        if(compararDatas(c.entrada, dataObraFim) > 0)
            printf("Data de entrada não pode ser maior que data de publicação da obra!\n");
        
        if(compararDatas(a->nascimento, c.entrada) > 0)
            printf("Data de entrada não pode ser anterior ao nascimento do Artista!\n");

    }while(!validarData(c.entrada) || compararDatas(c.entrada, dataObraFim) > 0 || compararDatas(a->nascimento, c.entrada) > 0);

    do
    {
        // Data de saída
        printf("Data de saida:\n");

        printf("  Dia: ");
        if (!lerInteiro(&c.saida.dia))
            return false;

        printf("  Mes: ");
        if (!lerInteiro(&c.saida.mes))
            return false;

        printf("  Ano: ");
        if (!lerInteiro(&c.saida.ano))
            return false;

        if(!validarData(c.saida))
            printf(MSG_ENTRADA_INVALIDA);

        if(compararDatas(c.entrada, c.saida) > 0)
            printf("Data de saída não pode ser menor que a de entrada!\n");

        if(compararDatas(c.saida, dataObraFim) > 0)
            printf("Data de saída não pode ser maior que data de publicação da obra!\n");

    }while (!validarData(c.saida) || compararDatas(c.entrada, c.saida) > 0 || compararDatas(c.saida, dataObraFim) > 0);

    // Copia os dados da chave para a colaboração
    strncpy(c.chaveColab.cpf, cpfDigitado, TAM_CPF - 1);
    c.chaveColab.cpf[TAM_CPF - 1] = '\0';
    c.chaveColab.id = idObra;

    // Adiciona à lista
    if (!adicionarColaboracao(listaColaboracoes, &c))
    {
        printf("Erro de memoria ao cadastrar colaboracao.\n");
        return true; // Retorna true para não encerrar o módulo, apenas informar que houve um erro de memória.
    }

    printf("Colaboracao cadastrada com sucesso!\n");
    return true;
}

void imprimirColaboracaoPorIndice(const ListaColaboracoes *lista, int indice)
{
    const Colaboracao *c = &lista->itens[indice];
    printf("\n--- Colaboração %d ---\n", indice+1);
    printf("Obra ID: %d\n", c->chaveColab.id);
    printf("CPF Artista: "); imprimeCPF(c->chaveColab.cpf);
    printf("\nFunção: %s\n", c->funcaoArtista);
    printf("Contribuição: %d%%\n", c->percentualContribuicao);
    printf("Entrada: %02d/%02d/%d\n", c->entrada.dia, c->entrada.mes, c->entrada.ano);
    printf("Saída: %02d/%02d/%d\n", c->saida.dia, c->saida.mes, c->saida.ano);
}

void listarTodasColaboracoes(const ListaColaboracoes *listaColaboracoes)
{
    int i;
    if (listaColaboracoes->total == 0)
        printf("\nNenhuma colaboracao cadastrada.\n");
    else
        for (i=0; i < listaColaboracoes->total; i++)
            imprimirColaboracaoPorIndice(listaColaboracoes, i);
}

static bool buscarColab(const ListaColaboracoes *lista, int *indice)
{
    int id;
    char cpf[TAM_CPF];
    printf("Informe o ID da obra: ");
    if (!lerInteiro(&id))
        return false;

    printf("Informe o CPF do artista: ");
    if (!lerString(cpf, TAM_CPF))
        return false;
    
    *indice = indiceColaboracaoPorChave(lista, cpf, id);
    if(*indice == -1)
        printf("Colaboração não encontrada!\n");
    
    return true;
}

static bool excluirColaboracao(ListaColaboracoes *lista, int indice)
{
    char resposta[TAM_SIM_NAO];

    printf("Tem certeza que deseja excluir esta colaboração? (s/n): ");
    
    if (!lerSimNao(resposta))
        return false;
    if (resposta[0] != 's' && resposta[0] != 'S')
    {
        printf(MSG_EXCLUSAO_CANCELADA);
        return true;
    }
    else
    {
        if (removerColaboracao(lista, indice))
        {
            printf("Colaboração excluída com sucesso.\n");
            return true;
        }
        else
        {
            printf("Erro ao excluir colaboração.\n");
            return false;
        }
    }

}

/***********************
 * MODULO E SUB-MODULO *
 ***********************/

static bool moduloAlterarColab(const ListaArtistas *listaArtistas, const ListaObras *listaObras, ListaColaboracoes *listaColaboracoes, int indice)
{
    int op, novoPercentual, indiceArtista, indiceObra;
    char novaFuncao[TAM_TEXTO_PEQUENO], confirma[TAM_SIM_NAO];
    Data novaData, dataFimObra;
    bool executando = true;
    Colaboracao *c = &listaColaboracoes->itens[indice];
    Artista *a;
    Obra *o;
    indiceArtista = indiceArtistaPorCPF(listaArtistas, c->chaveColab.cpf);
    indiceObra = indiceObraPorID(listaObras,c->chaveColab.id);

    if(indiceArtista == -1 || indiceObra == -1)
    {
        printf("Artista ou Obra excluídos do sistema. Não é possível fazer alterações nesta colaboração!");
        return true;
    }


    a = &listaArtistas->itens[indiceArtista];
    o = &listaObras->itens[indiceObra];
    imprimirColaboracaoPorIndice(listaColaboracoes, indice);
    while (executando)
    {
        op = menuAlteracoesColab();
        if (op == -99)
            return false;

        switch (op)
        {
            case 1:
                do
                {
                    printf("Informe o novo percentual de contribuição: ");
                    if(!lerInteiro(&novoPercentual))
                        return false;
                    if(novoPercentual < 0 || novoPercentual > 100)
                        printf("\nValor inválido! Tente novamente (0 a 100): ");
                }while(novoPercentual < 0 || novoPercentual > 100);

                printf(MSG_CONFIRMAR_ALTERACAO_INT, c->percentualContribuicao, novoPercentual);
                if(!lerSimNao(confirma))
                    return false;
                if(confirma[0] == 's' || confirma[0] == 'S')
                {
                    c->percentualContribuicao = novoPercentual;
                    printf(MSG_ALTERACAO_REALIZADA_SUCESSO);
                }
                else
                    printf(MSG_ALTERACAO_CANCELADA);
                break;

            case 2:
                do{ 
                    printf("Informe a nova data de entrada:\n");
                    printf("  Dia: ");
                    if (!lerInteiro(&novaData.dia))
                        return false;
                    printf("  Mês: ");
                    if (!lerInteiro(&novaData.mes))
                        return false;
                    printf("  Ano: ");
                    if (!lerInteiro(&novaData.ano))
                        return false;
                    
                    if (!validarData(novaData))
                        printf(MSG_ENTRADA_INVALIDA);

                    if(compararDatas(novaData, c->saida) > 0)
                        printf("Data de entrada não pode ser posterior a de saída!\n");

                    if(compararDatas(a->nascimento, novaData) > 0)
                        printf("Data de entrada não pode ser anterior ao nascimento do Artista!\n");
                    
                }while (!validarData(novaData) || compararDatas(novaData, c->saida) > 0 || compararDatas(a->nascimento, novaData) > 0);

                printf(MSG_CONFIRMAR_ALTERACAO_DATA, c->entrada.dia, c->entrada.mes, c->entrada.ano, novaData.dia, novaData.mes, novaData.ano);

                if(!lerSimNao(confirma))
                    return false;
                if(confirma[0] == 's' || confirma[0] == 'S')
                {
                    c->entrada = novaData;
                    printf(MSG_ALTERACAO_REALIZADA_SUCESSO);
                }
                else
                    printf(MSG_ALTERACAO_CANCELADA);
                    
                break;

            case 3:
                dataFimObra.dia = 31; dataFimObra.mes = 12; dataFimObra.ano = o->anoCriacao; // 
                do{ 
                    printf("Informe a nova data de saída:\n");
                    printf("  Dia: ");
                    if (!lerInteiro(&novaData.dia))
                        return false;
                    printf("  Mês: ");
                    if (!lerInteiro(&novaData.mes))
                        return false;
                    printf("  Ano: ");
                    if (!lerInteiro(&novaData.ano))
                        return false;
                    
                    if (!validarData(novaData))
                        printf(MSG_ENTRADA_INVALIDA);

                    if(compararDatas(c->entrada, novaData) > 0)
                        printf("Data de saída não pode ser menor que a de entrada! Tente novamente: ");
                    
                    if(compararDatas(novaData, dataFimObra) > 0)
                        printf("Data de saída não pode ser posterior à publicação da obra!\n");

                }while (!validarData(novaData) || compararDatas(c->entrada, novaData) > 0 || compararDatas(novaData, dataFimObra) > 0);

                printf(MSG_CONFIRMAR_ALTERACAO_DATA, c->saida.dia, c->saida.mes, c->saida.ano, novaData.dia, novaData.mes, novaData.ano);

                if(!lerSimNao(confirma))
                    return false;

                if(confirma[0] == 's' || confirma[0] == 'S')
                {
                    c->saida = novaData;
                    printf(MSG_ALTERACAO_REALIZADA_SUCESSO);
                }
                else
                    printf(MSG_ALTERACAO_CANCELADA);

                break;

            case 4:
                printf("Informe a nova Função do Artista: ");
                if(!lerString(novaFuncao, TAM_TEXTO_PEQUENO))
                    return false;
                printf(MSG_CONFIRMAR_ALTERACAO_TEXTO, c->funcaoArtista, novaFuncao);
                if(!lerSimNao(confirma))
                    return false;
                if(confirma[0] == 's' || confirma[0] == 'S')
                {
                    strcpy(c->funcaoArtista,novaFuncao);
                    printf(MSG_ALTERACAO_REALIZADA_SUCESSO);
                }
                else
                    printf(MSG_ALTERACAO_CANCELADA);
                
                break;
                
            case 5:
                if(!salvarColaboracoes(listaColaboracoes))
                {
                    printf(MSG_ERRO_SALVAR_DADOS, "Colaborações");
                    return false;
                }
                printf(MSG_DADOS_SALVOS_SUCESSO, NOME_ARQUIVO_COLABORACOES);
                executando = false;
                break;
        }
    }
    return true;
}

bool moduloColaboracoes(ListaColaboracoes *listaColaboracoes, const ListaArtistas *listaArtistas, const ListaObras *listaObras)
{
    int op, indice;
    bool executando = true;

    while (executando)
    {
        op = menuColaboracoes();
        if (op == -99)
            return false;

        switch (op)
        {
            case 1:
                if(!cadastrarColaboracao(listaColaboracoes, listaArtistas, listaObras))
                {
                    printf("Erro ao cadastrar colaboracao. Encerrando sem salvar.\n");
                    return false;
                }
                else
                    salvarColaboracoes(listaColaboracoes);
                 break;

            case 2:
                listarTodasColaboracoes(listaColaboracoes);
                break;

            case 3:
                if(!buscarColab(listaColaboracoes, &indice))
                    return false;
                break;

            case 4:
                if(!buscarColab(listaColaboracoes, &indice))
                    return false;
                if (indice != -1)
                    if (!moduloAlterarColab(listaArtistas, listaObras, listaColaboracoes, indice))
                        return false;
                break;
                
            case 5:
                if(!buscarColab(listaColaboracoes, &indice))
                    return false;
                if(indice != -1)
                    if(!excluirColaboracao(listaColaboracoes, indice))
                        return false;
                break;
            
            case 6:
                executando = false;
                break;
        }
    }
    return true;
}
