
// Feito integralmente pelo DeepSeek deverá ser revisado


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "persistencia.h"

// Função auxiliar para alocar dinamicamente um telefone adicional
static bool adicionarTelefoneParsing(Artista *a, const char *numero)
{
    Telefone *temp = realloc(a->telefones, sizeof(Telefone) * (a->totalTelefones + 1));
    if (!temp) return false;
    a->telefones = temp;
    strncpy(a->telefones[a->totalTelefones].numeroTelefone, numero, TAM_TELEFONE - 1);
    a->telefones[a->totalTelefones].numeroTelefone[TAM_TELEFONE - 1] = '\0';
    a->totalTelefones++;
    a->capacidadeTelefones = a->totalTelefones;
    return true;
}

// Função auxiliar para alocar dinamicamente uma rede social adicional
static bool adicionarRedeParsing(Artista *a, const char *plataforma, const char *usuario)
{
    redeSocial *temp = realloc(a->redesSociais, sizeof(redeSocial) * (a->totalRedesSociais + 1));
    if (!temp) return false;
    a->redesSociais = temp;
    strncpy(a->redesSociais[a->totalRedesSociais].redeSocial, plataforma, TAM_TEXTO_PEQUENO - 1);
    a->redesSociais[a->totalRedesSociais].redeSocial[TAM_TEXTO_PEQUENO - 1] = '\0';
    strncpy(a->redesSociais[a->totalRedesSociais].usuario, usuario, TAM_TEXTO_PEQUENO - 1);
    a->redesSociais[a->totalRedesSociais].usuario[TAM_TEXTO_PEQUENO - 1] = '\0';
    a->totalRedesSociais++;
    a->capacidadeRedesSociais = a->totalRedesSociais;
    return true;
}

static bool parseLinhaArtista(char *linha, Artista *a)
{
    char *token;
    token = strtok(linha, ";");
    if (!token) return false;
    strncpy(a->cpf, token, TAM_CPF - 1);
    a->cpf[TAM_CPF - 1] = '\0';

    token = strtok(NULL, ";");
    if (!token) return false;
    strncpy(a->nome, token, TAM_TEXTO_PEQUENO - 1);
    a->nome[TAM_TEXTO_PEQUENO - 1] = '\0';

    token = strtok(NULL, ";");
    if (!token) return false;
    strncpy(a->nacionalidade, token, TAM_NACIONALIDADE - 1);
    a->nacionalidade[TAM_NACIONALIDADE - 1] = '\0';

    token = strtok(NULL, ";");
    if (!token) return false;
    strncpy(a->estilo, token, TAM_TEXTO_PEQUENO - 1);
    a->estilo[TAM_TEXTO_PEQUENO - 1] = '\0';

    // Data
    token = strtok(NULL, ";");
    if (!token) return false;
    a->nascimento.dia = atoi(token);
    token = strtok(NULL, ";");
    if (!token) return false;
    a->nascimento.mes = atoi(token);
    token = strtok(NULL, ";");
    if (!token) return false;
    a->nascimento.ano = atoi(token);

    // Inicializa arrays dinâmicos
    a->telefones = NULL;
    a->totalTelefones = 0;
    a->capacidadeTelefones = 0;
    a->redesSociais = NULL;
    a->totalRedesSociais = 0;
    a->capacidadeRedesSociais = 0;

    // Telefones
    token = strtok(NULL, ";");
    if (!token) return false;
    char *subtoken = strtok(token, "|");
    while (subtoken)
    {
        if (!adicionarTelefoneParsing(a, subtoken))
        {
            // libera o que foi alocado até agora
            free(a->telefones);
            free(a->redesSociais);
            return false;
        }
        subtoken = strtok(NULL, "|");
    }

    // Redes sociais
    token = strtok(NULL, ";");
    if (token)
    {
        subtoken = strtok(token, "|");
        while (subtoken)
        {
            // Separa plataforma:usuario
            char *delim = strchr(subtoken, ':');
            if (delim)
            {
                *delim = '\0';
                const char *plataforma = subtoken;
                const char *usuario = delim + 1;
                if (!adicionarRedeParsing(a, plataforma, usuario))
                {
                    free(a->telefones);
                    free(a->redesSociais);
                    return false;
                }
            }
            subtoken = strtok(NULL, "|");
        }
    }

    return true;
}

bool carregarArtistas(const char *nomeArquivo, ListaArtistas *lista)
{
    FILE *fp = fopen(nomeArquivo, "r");
    if (!fp)
    {
        inicializarListaArtistas(lista, 4);
        return true;
    }

    inicializarListaArtistas(lista, 4);
    if (!lista->itens) { fclose(fp); return false; }

    char linha[1024];
    while (fgets(linha, sizeof(linha), fp))
    {
        linha[strcspn(linha, "\n")] = '\0';
        if (strlen(linha) == 0) continue;

        Artista a;
        if (!parseLinhaArtista(linha, &a))
        {
            // Em caso de erro no parsing, libera os arrays alocados (se houve alocação parcial)
            free(a.telefones);
            free(a.redesSociais);
            continue; // ignora linha malformada
        }

        if (!adicionarArtista(lista, &a))
        {
            free(a.telefones);
            free(a.redesSociais);
            fclose(fp);
            return false;
        }
    }
    fclose(fp);
    return true;
}

bool salvarArtistas(const char *nomeArquivo, const ListaArtistas *lista)
{
    FILE *fp = fopen(nomeArquivo, "w");
    if (!fp) return false;

    for (int i = 0; i < lista->total; i++)
    {
        const Artista *a = &lista->itens[i];
        fprintf(fp, "%s;%s;%s;%s;%d;%d;%d;",
                a->cpf, a->nome, a->nacionalidade, a->estilo,
                a->nascimento.dia, a->nascimento.mes, a->nascimento.ano);

        // Telefones
        for (int j = 0; j < a->totalTelefones; j++)
        {
            fprintf(fp, "%s", a->telefones[j].numeroTelefone);
            if (j < a->totalTelefones - 1) fprintf(fp, "|");
        }
        fprintf(fp, ";");

        // Redes sociais
        for (int j = 0; j < a->totalRedesSociais; j++)
        {
            fprintf(fp, "%s:%s", a->redesSociais[j].redeSocial, a->redesSociais[j].usuario);
            if (j < a->totalRedesSociais - 1) fprintf(fp, "|");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    return true;
}

// As demais funções (carregarObras, salvarObras, carregarColaboracoes, salvarColaboracoes)
// permanecem as mesmas da resposta anterior.
