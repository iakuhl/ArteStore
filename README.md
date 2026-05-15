# Sistema de Curadoria de Obras de Artes

**Projeto da disciplina APR2**  
**Autoria:** Iano de Oliva Kuhlmann  
**Colaboradores:** chat.deepseek.com
**Professora:** Dra. Eloize Rossi Marques Seno

---

## 1. Descrição

Sistema em linguagem C para gerenciamento de artistas, obras de arte e colaborações. Os dados são armazenados em arquivos texto (`.txt`) e mantidos em memória por meio de listas dinâmicas. O programa oferece menus interativos para cadastrar, listar, buscar e excluir registros, além de gerar relatórios.

---

## 2. Estrutura do Projeto

| Arquivo | Descrição |
|--------|------------|
| `estruturas.h` | Definição de constantes, tipos (Data, Telefone, redeSocial, Artista, Obra, Colaboracao) e listas dinâmicas (ListaArtistas, ListaObras, ListaColaboracoes). |
| `utils.h` / `utils.c` | Funções utilitárias de entrada: `lerInteiro`, `lerString`, `limparBuffer`, `validarCPF`. |
| `menus.h` / `menus.c` | Exibição dos menus e captura de opção (`menuPrincipal`, `menuArtistas`, `escolherOpcao`). |
| `listas.h` / `listas.c` | Manipulação das listas dinâmicas: inicializar, adicionar, buscar, remover e liberar. |
| `persistencia.h` / `persistencia.c` | Leitura e gravação dos arquivos de dados. |
| `main.c` | Função principal: carrega dados, controla fluxo dos menus, salva e libera memória. |

---

## 3. Estruturas de Dados

- **Data** (dia, mês, ano)
- **Telefone** (numeroTelefone)
- **redeSocial** (plataforma, usuario)
- **Artista** (cpf, nome, nacionalidade, estilo, nascimento, telefones\*, redesSociais\*, total/capacidade)
- **Obra** (id, anoCriacao, valorCentavos, titulo, tipo, descricao)
- **ChaveColaboracao** (cpf, idObra)
- **Colaboracao** (chave, funcaoArtista, percentualContribuicao, entrada, saida)

Cada lista dinâmica (`ListaArtistas`, `ListaObras`, `ListaColaboracoes`) contém um ponteiro para array, total de elementos e capacidade atual.

---

## 4. Funcionalidades Implementadas

### 4.1 Leitura Robustas (Utils)
- `lerInteiro` e `lerString` com `fgets`, tratamento de buffer overflow, validação de conteúdo e detecção de EOF.
- `validarCPF` (11 dígitos numéricos).

### 4.2 Sistema de Menus
- Menus hierárquicos (Principal, Artistas, Obras, Colaborações, Relatórios).
- Função centralizada `escolherOpcao` que valida intervalo e trata erros de entrada.
- Mensagens padronizadas via `#define`.

### 4.3 Listas Dinâmicas
- Alocação dinâmica com capacidade inicial 4, expansão automática (dobro) quando necessário.
- Operações de adição, busca (por CPF ou ID), remoção (com deslocamento e liberação de subitens) e liberação total da memória.

### 4.4 Persistência em Arquivos
- **Formato dos arquivos:**
  - `artistas.txt`:  
    `cpf;nome;nacionalidade;estilo;dia;mes;ano;tel1|tel2;plataforma:usuario|plataforma:usuario`
  - `obras.txt`:  
    `id;anoCriacao;valorCentavos;titulo;tipo;descricao`
  - `colaboracoes.txt`:  
    `cpf;idObra;funcao;percentual;diaEnt;mesEnt;anoEnt;diaSai;mesSai;anoSai`
- Carregamento com parsing robusto e tratamento de arquivo inexistente (lista vazia).
- Salvamento sobrescreve arquivo a cada retorno de módulo ou ao final do programa.

---

## 5. Compilação e Execução

```bash
gcc -Wall -Wextra -std=c99 -o curadoria main.c utils.c menus.c listas.c persistencia.c
./curadoria
