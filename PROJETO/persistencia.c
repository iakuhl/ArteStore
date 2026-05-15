

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "defines.h"
#include "persistencia.h"

void carregarDados(ListaArtistas *listaArtistas, ListaObras *listaObras, ListaColaboracoes *listaColaboracoes)
{ 
    if(!carregarArtistas(listaArtistas))
        printf("Erro ao carregar artistas. Iniciando com lista vazia.\n");
    if(!carregarObras(listaObras))
        printf("Erro ao carregar obras. Iniciando com lista vazia.\n");
    if(!carregarColaboracoes(listaColaboracoes))
        printf("Erro ao carregar colaborações. Iniciando com lista vazia.\n");
}

bool liberarDados(ListaArtistas *listaArtistas, ListaObras *listaObras, ListaColaboracoes *listaColaboracoes)
{
	sucesso = true;
	if(!liberarArtistas(listaArtistas))
	{
		printf("Erro de memória em Artistas!\n");
		sucesso = false;
	}
	if(!liberarObras(listaObras))
	{
		printf("Erro de memória em Obras!\n");
		sucesso = false;
	}
	if(!liberarColaboracoes(listaColaboracoes))
	{
		printf("Erro de memória em Colaborações!\n");
		sucesso = false;
	}
	return sucesso;
}





// Função auxiliar para alocar dinamicamente um telefone adicional
static bool adicionarTelefoneParsing(Artista *a, const char *numero)
{
    Telefone *temp = realloc(a->telefones, sizeof(Telefone) * (a->totalTelefones + 1));
    if (temp == NULL)
        return false;

    a->telefones = temp;

    int indice = a->totalTelefones;
    strncpy(a->telefones[indice].numeroTelefone, numero, TAM_TELEFONE - 1);
    a->telefones[indice].numeroTelefone[TAM_TELEFONE - 1] = '\0';

    a->totalTelefones++;
    a->capacidadeTelefones = a->totalTelefones;

    return true;
}

// Função auxiliar para alocar dinamicamente uma rede social adicional
static bool adicionarRedeParsing(Artista *a, const char *plataforma, const char *usuario)
{
    redeSocial *temp = realloc(a->redesSociais, sizeof(redeSocial) * (a->totalRedesSociais + 1));

    if (temp == NULL)
        return false;
    
    a->redesSociais = temp;

    int indice = a->totalRedesSociais;
    strncpy(a->redesSociais[indice].redeSocial, plataforma, TAM_TEXTO_PEQUENO - 1);
    a->redesSociais[indice].redeSocial[TAM_TEXTO_PEQUENO - 1] = '\0';
    strncpy(a->redesSociais[indice].usuario, usuario, TAM_TEXTO_PEQUENO - 1);
    a->redesSociais[indice].usuario[TAM_TEXTO_PEQUENO - 1] = '\0';


    a->totalRedesSociais++;
    a->capacidadeRedesSociais = a->totalRedesSociais;

    return true;
}

static bool analisarLinhaArtista(char *linha, Artista *a)
{
    char *separador; // Cria um separador para dividir a linha em campos usando ";" como delimitador.

    // Copia o CPF para a estrutura do artista.
    separador = strtok(linha, ";");
    if (separador == NULL)
        return false;
    strncpy(a->cpf, separador, TAM_CPF - 1);
    a->cpf[TAM_CPF - 1] = '\0';

    // Copia o nome para a estrutura do artista.
    separador = strtok(NULL, ";");
    if (separador == NULL)
        return false;
    strncpy(a->nome, separador, TAM_TEXTO_PEQUENO - 1);
    a->nome[TAM_TEXTO_PEQUENO - 1] = '\0';

    // Copia a nacionalidade para a estrutura do artista.
    separador = strtok(NULL, ";");
    if (separador == NULL)
        return false;
    strncpy(a->nacionalidade, separador, TAM_NACIONALIDADE - 1);
    a->nacionalidade[TAM_NACIONALIDADE - 1] = '\0';

    // Copia o estilo para a estrutura do artista.
    separador = strtok(NULL, ";");
    if (separador == NULL)
        return false;
    strncpy(a->estilo, separador, TAM_TEXTO_PEQUENO - 1);
    a->estilo[TAM_TEXTO_PEQUENO - 1] = '\0';

    // Copia a data de nascimento (dia,mes,ano) para a estrutura do artista.
    // Dia
    separador = strtok(NULL, ";");
    if (separador == NULL)
        return false;
    a->nascimento.dia = atoi(separador);
    // Mês
    separador = strtok(NULL, ";");
    if (separador == NULL)
        return false;
    a->nascimento.mes = atoi(separador);
    // Ano
    separador = strtok(NULL, ";");
    if (separador == NULL)
        return false;
    a->nascimento.ano = atoi(separador);

    // Copia os telefones para a estrutura do artista (campos brutos).
    char *campoTelefones = strtok(NULL, ";");
    if (campoTelefones == NULL)
        return false;

    // Redes sociais (campos brutos). O campo de redes sociais pode ser opcional, então verificamos se existe antes de tentar processar.
    char *campoRedes = strtok(NULL, ";");

    // Inicializa arrays dinâmicos
    a->telefones = NULL;
    a->totalTelefones = 0;
    a->capacidadeTelefones = 0;
    a->redesSociais = NULL;
    a->totalRedesSociais = 0;
    a->capacidadeRedesSociais = 0;

    // Processa os telefones, separados por "|"
    char *subseparador = strtok(campoTelefones, "|");
    while (subseparador != NULL)
    {
        if (!adicionarTelefoneParsing(a, subseparador))
        {
            // libera o que foi alocado até agora
            free(a->telefones);
            free(a->redesSociais);
            return false;
        }
        subseparador = strtok(NULL, "|");
    }

    // Redes sociais
    if (campoRedes != NULL)
    {
        subseparador = strtok(campoRedes, "|");
        while (subseparador != NULL)
        {
            // Separa plataforma:usuario
            char *doisPontos = strchr(subseparador, ':');
            if (doisPontos != NULL)
            {
                *doisPontos = '\0';
                const char *plataforma = subseparador;
                const char *usuario = doisPontos + 1;
                if (!adicionarRedeParsing(a, plataforma, usuario))
                {
                    free(a->telefones);
                    free(a->redesSociais);
                    return false;
                }
            }
            subseparador = strtok(NULL, "|");
        }
    }

    return true;
}

bool carregarArtistas(ListaArtistas *lista)
{
    FILE *fp = fopen(NOME_ARQUIVO_ARTISTAS, "r");
    if (!fp)
    {
        inicializarListaArtistas(lista, 4);
        return true;
    }

    inicializarListaArtistas(lista, 4);
    if (!lista->itens)
    {
        fclose(fp);
        return false;
    }

    char linha[1024];
    while (fgets(linha, sizeof(linha), fp))
    {
        linha[strcspn(linha, "\n")] = '\0';
        if (strlen(linha) == 0) continue;

        Artista a;
        if (!analisarLinhaArtista(linha, &a))
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

bool salvarArtistas(const ListaArtistas *lista)
{
    FILE *fp = fopen(NOME_ARQUIVO_ARTISTAS, "w");
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
