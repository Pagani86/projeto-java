
/*
 * Sistema de Gerenciamento de Intervenções Artísticas Urbanas
 * Tema 20 - Estruturas dinâmicas + Arquivos binários e texto
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
 
/* ===================== DEFINIÇÕES DE TAMANHO ===================== */
#define MAX_CPF         15
#define MAX_NOME        100
#define MAX_NOME_ART    80
#define MAX_ESTILO      50
#define MAX_PAIS        50
#define MAX_REDE        200
#define MAX_CIDADE      80
#define MAX_TIPO_ESP    50
#define MAX_DESC        300
#define MAX_TIPO_INT    50
#define MAX_MATERIAL    200
#define MAX_COD         20
 
/* ===================== ESTRUTURAS ===================== */
 
typedef struct Artista {
    char cpf[MAX_CPF];
    char nome[MAX_NOME];
    char nome_artistico[MAX_NOME_ART];
    char estilo[MAX_ESTILO];
    char pais_origem[MAX_PAIS];
    char redes_sociais[MAX_REDE];   /* separadas por ';' */
    struct Artista *prox;
} Artista;
 
typedef struct EspacoPublico {
    char codigo[MAX_COD];
    char nome_local[MAX_NOME];
    char cidade[MAX_CIDADE];
    char tipo_espaco[MAX_TIPO_ESP];
    char descricao[MAX_DESC];
    struct EspacoPublico *prox;
} EspacoPublico;
 
typedef struct Intervencao {
    char cpf_artista[MAX_CPF];
    char cod_espaco[MAX_COD];
    char data[15];            /* DD/MM/AAAA */
    char tipo_intervencao[MAX_TIPO_INT];
    char material_utilizado[MAX_MATERIAL];
    float duracao;            /* horas */
    char autorizada;          /* 'S' ou 'N' */
    struct Intervencao *prox;
} Intervencao;
 
/* ===================== LISTAS LIGADAS GLOBAIS ===================== */
Artista      *lista_artistas    = NULL;
EspacoPublico *lista_espacos    = NULL;
Intervencao  *lista_intervencoes = NULL;
 
/* ===================== ARQUIVOS ===================== */
#define ARQ_ARTISTAS    "artistas.bin"
#define ARQ_ESPACOS     "espacos.bin"
#define ARQ_INTERVENCOES "intervencoes.bin"
 
/* ===================== UTILITÁRIOS ===================== */
 
void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
 
void ler_string(const char *prompt, char *dest, int tam) {   //essa função é usada para ler strings do usuário, garantindo que não haja estouro de buffer e removendo a nova linha final.
    printf("%s", prompt);
    fgets(dest, tam, stdin);
    dest[strcspn(dest, "\n")] = '\0';
}
 
void pausar(void) {
    printf("\nPressione ENTER para continuar...");
    limpar_buffer();
}
 
void cabecalho(const char *titulo) {
    printf("\n============================================================\n");
    printf("  %s\n", titulo);
    printf("============================================================\n");
}
 
/* Converte string para maiúsculo */
void maiusculo(char *s) {
    for (; *s; s++) *s = toupper((unsigned char)*s); //transforma letras minusculas em maiusculas, usei no terminal para confirmar as decisoes do usuario
}


/* Verifica se o CPF e valido pelo calculo dos digitos verificadores.
   Aceita CPF com ou sem pontos e hifen. */
int cpf_valido(const char *cpf) {
    int num[11];
    int i, j = 0;
    int soma, resto, digito1, digito2;
    int todos_iguais = 1;

    for (i = 0; cpf[i] != '\0'; i++) {
        if (isdigit((unsigned char)cpf[i])) {
            if (j >= 11) return 0;
            num[j] = cpf[i] - '0';
            j++;
        }
    }

    if (j != 11) return 0;

    for (i = 1; i < 11; i++) {
        if (num[i] != num[0]) {
            todos_iguais = 0;
            break;
        }
    }
    if (todos_iguais) return 0;

    soma = 0;
    for (i = 0; i < 9; i++) {
        soma += num[i] * (10 - i);
    }
    resto = soma % 11;
    if (resto < 2) digito1 = 0;
    else digito1 = 11 - resto;

    soma = 0;
    for (i = 0; i < 10; i++) {
        soma += num[i] * (11 - i);
    }
    resto = soma % 11;
    if (resto < 2) digito2 = 0;
    else digito2 = 11 - resto;

    return (digito1 == num[9] && digito2 == num[10]);
}

int comparar_sem_case(const char *a, const char *b) {  // compara a string se sem diferenciar os caracteres maiusculos ou minusculas
    while (*a && *b) {
        unsigned char ca = (unsigned char)tolower((unsigned char)*a);
        unsigned char cb = (unsigned char)tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        a++;
        b++;
    }
    return (unsigned char)tolower((unsigned char)*a) - (unsigned char)tolower((unsigned char)*b);
}

int existe_intervencao_do_artista(const char *cpf) {
    Intervencao *p = lista_intervencoes;
    while (p) {
        if (strcmp(p->cpf_artista, cpf) == 0) return 1;
        p = p->prox;
    }
    return 0;
}

int existe_intervencao_no_espaco(const char *codigo) {
    Intervencao *p = lista_intervencoes;
    while (p) {
        if (strcmp(p->cod_espaco, codigo) == 0) return 1;
        p = p->prox;
    }
    return 0;
}

 
/* ===================== PERSISTÊNCIA - ARTISTAS ===================== */
 
void salvar_artistas(void) {
    FILE *f = fopen(ARQ_ARTISTAS, "wb");
    if (!f) { perror("Erro ao abrir artistas.bin"); return; }
    Artista *p = lista_artistas;
    while (p) {
        /* Grava struct sem o ponteiro */
        fwrite(p->cpf,           sizeof(p->cpf),           1, f);
        fwrite(p->nome,          sizeof(p->nome),           1, f);
        fwrite(p->nome_artistico,sizeof(p->nome_artistico), 1, f);
        fwrite(p->estilo,        sizeof(p->estilo),         1, f);
        fwrite(p->pais_origem,   sizeof(p->pais_origem),    1, f);
        fwrite(p->redes_sociais, sizeof(p->redes_sociais),  1, f);
        p = p->prox;
    }
    fclose(f);
}
 
void carregar_artistas(void) {
    FILE *f = fopen(ARQ_ARTISTAS, "rb");
    if (!f) return;
    Artista tmp;
    while (
        fread(tmp.cpf,           sizeof(tmp.cpf),           1, f) == 1 &&
        fread(tmp.nome,          sizeof(tmp.nome),           1, f) == 1 &&
        fread(tmp.nome_artistico,sizeof(tmp.nome_artistico), 1, f) == 1 &&
        fread(tmp.estilo,        sizeof(tmp.estilo),         1, f) == 1 &&
        fread(tmp.pais_origem,   sizeof(tmp.pais_origem),    1, f) == 1 &&
        fread(tmp.redes_sociais, sizeof(tmp.redes_sociais),  1, f) == 1
    ) {
        Artista *novo = (Artista *)malloc(sizeof(Artista));
        *novo = tmp;
        novo->prox = NULL;
        if (!lista_artistas) {
            lista_artistas = novo;
        } else {
            Artista *p = lista_artistas; // *p = ponteiro auxiliar criado para percorrer a lista ligada de artistas. Ele começa apontando para o início da lista (lista_artistas).
            while (p->prox) p = p->prox;
            p->prox = novo;
        }
    }
    fclose(f);
}
 
/* ===================== PERSISTÊNCIA - ESPAÇOS ===================== */
 
void salvar_espacos(void) {
    FILE *f = fopen(ARQ_ESPACOS, "wb");
    if (!f) { perror("Erro ao abrir espacos.bin"); return; }
    EspacoPublico *p = lista_espacos;
    while (p) {
        fwrite(p->codigo,     sizeof(p->codigo),     1, f);
        fwrite(p->nome_local, sizeof(p->nome_local),  1, f);
        fwrite(p->cidade,     sizeof(p->cidade),      1, f);
        fwrite(p->tipo_espaco,sizeof(p->tipo_espaco), 1, f);
        fwrite(p->descricao,  sizeof(p->descricao),   1, f);
        p = p->prox;
    }
    fclose(f);
}
 
void carregar_espacos(void) {
    FILE *f = fopen(ARQ_ESPACOS, "rb");
    if (!f) return;
    EspacoPublico tmp;
    while (
        fread(tmp.codigo,     sizeof(tmp.codigo),     1, f) == 1 &&
        fread(tmp.nome_local, sizeof(tmp.nome_local),  1, f) == 1 &&
        fread(tmp.cidade,     sizeof(tmp.cidade),      1, f) == 1 &&
        fread(tmp.tipo_espaco,sizeof(tmp.tipo_espaco), 1, f) == 1 &&
        fread(tmp.descricao,  sizeof(tmp.descricao),   1, f) == 1
    ) {
        EspacoPublico *novo = (EspacoPublico *)malloc(sizeof(EspacoPublico));
        *novo = tmp;
        novo->prox = NULL;
        if (!lista_espacos) {
            lista_espacos = novo;
        } else {
            EspacoPublico *p = lista_espacos;
            while (p->prox) p = p->prox;
            p->prox = novo;
        }
    }
    fclose(f);
}
 
/* ===================== PERSISTÊNCIA - INTERVENÇÕES ===================== */
 
void salvar_intervencoes(void) {
    FILE *f = fopen(ARQ_INTERVENCOES, "wb");
    if (!f) { perror("Erro ao abrir intervencoes.bin"); return; }
    Intervencao *p = lista_intervencoes;
    while (p) {
        fwrite(p->cpf_artista,      sizeof(p->cpf_artista),      1, f);
        fwrite(p->cod_espaco,       sizeof(p->cod_espaco),        1, f);
        fwrite(p->data,             sizeof(p->data),              1, f);
        fwrite(p->tipo_intervencao, sizeof(p->tipo_intervencao),  1, f);
        fwrite(p->material_utilizado,sizeof(p->material_utilizado),1, f);
        fwrite(&p->duracao,         sizeof(p->duracao),           1, f);
        fwrite(&p->autorizada,      sizeof(p->autorizada),        1, f);
        p = p->prox;
    }
    fclose(f);
}
 
void carregar_intervencoes(void) {
    FILE *f = fopen(ARQ_INTERVENCOES, "rb");
    if (!f) return;
    Intervencao tmp;
    while (
        fread(tmp.cpf_artista,      sizeof(tmp.cpf_artista),      1, f) == 1 &&
        fread(tmp.cod_espaco,       sizeof(tmp.cod_espaco),        1, f) == 1 &&
        fread(tmp.data,             sizeof(tmp.data),              1, f) == 1 &&
        fread(tmp.tipo_intervencao, sizeof(tmp.tipo_intervencao),  1, f) == 1 &&
        fread(tmp.material_utilizado,sizeof(tmp.material_utilizado),1, f) == 1 &&
        fread(&tmp.duracao,         sizeof(tmp.duracao),           1, f) == 1 &&
        fread(&tmp.autorizada,      sizeof(tmp.autorizada),        1, f) == 1
    ) {
        Intervencao *novo = (Intervencao *)malloc(sizeof(Intervencao));
        *novo = tmp;
        novo->prox = NULL;
        if (!lista_intervencoes) {
            lista_intervencoes = novo;
        } else {
            Intervencao *p = lista_intervencoes;
            while (p->prox) p = p->prox;
            p->prox = novo;
        }
    }
    fclose(f);
}
 
/* ===================== BUSCA ===================== */
 
Artista *buscar_artista(const char *cpf) {
    Artista *p = lista_artistas;
    while (p) {
        if (strcmp(p->cpf, cpf) == 0) return p;
        p = p->prox;
    }
    return NULL;
}
 
EspacoPublico *buscar_espaco(const char *codigo) {
    EspacoPublico *p = lista_espacos;
    while (p) {
        if (strcmp(p->codigo, codigo) == 0) return p;
        p = p->prox;
    }
    return NULL;
}
 
/* Chave da intervenção: CPF + Código + Data */
Intervencao *buscar_intervencao(const char *cpf, const char *cod, const char *data) {
    Intervencao *p = lista_intervencoes;
    while (p) {
        if (strcmp(p->cpf_artista, cpf) == 0 &&
            strcmp(p->cod_espaco, cod) == 0 &&
            strcmp(p->data, data) == 0) return p;
        p = p->prox;
    }
    return NULL;
}
 
/* ===================== IMPRESSÃO ===================== */
 
void imprimir_artista(const Artista *a) {
    printf("  CPF           : %s\n", a->cpf);
    printf("  Nome          : %s\n", a->nome);
    printf("  Nome Artistico: %s\n", a->nome_artistico);
    printf("  Estilo        : %s\n", a->estilo);
    printf("  Pais de Origem: %s\n", a->pais_origem);
    printf("  Redes Sociais : %s\n", a->redes_sociais);
}
 
void imprimir_espaco(const EspacoPublico *e) {
    printf("  Codigo        : %s\n", e->codigo);
    printf("  Nome do Local : %s\n", e->nome_local);
    printf("  Cidade        : %s\n", e->cidade);
    printf("  Tipo de Espaco: %s\n", e->tipo_espaco);
    printf("  Descricao     : %s\n", e->descricao);
}
 
void imprimir_intervencao(const Intervencao *i) {
    printf("  CPF Artista   : %s\n", i->cpf_artista);
    printf("  Cod. Espaco   : %s\n", i->cod_espaco);
    printf("  Data          : %s\n", i->data);
    printf("  Tipo          : %s\n", i->tipo_intervencao);
    printf("  Material      : %s\n", i->material_utilizado);
    printf("  Duracao (h)   : %.1f\n", i->duracao);
    printf("  Autorizada    : %c\n", i->autorizada);
}
 
/* ===================== SUBMENU ARTISTAS ===================== */
 
void listar_todos_artistas(void) {
    cabecalho("LISTA DE ARTISTAS");
    if (!lista_artistas) { printf("Nenhum artista cadastrado.\n"); pausar(); return; }
    Artista *p = lista_artistas;
    int n = 1;
    while (p) {
        printf("\n--- Artista %d ---\n", n++);
        imprimir_artista(p);
        p = p->prox;
    }
    pausar();
}
 
void listar_um_artista(void) {
    cabecalho("CONSULTAR ARTISTA");
    char cpf[MAX_CPF];
    ler_string("CPF do artista: ", cpf, sizeof(cpf));
    Artista *a = buscar_artista(cpf);
    if (!a) { printf("Artista nao encontrado.\n"); }
    else { printf("\n"); imprimir_artista(a); }
    pausar();
}
 
void incluir_artista(void) {
    cabecalho("INCLUIR ARTISTA");
    Artista *novo = (Artista *)malloc(sizeof(Artista));
    ler_string("CPF           : ", novo->cpf, MAX_CPF);
    if (!cpf_valido(novo->cpf)) {
        printf("ERRO: CPF invalido! Digite um CPF matematicamente valido.\n");
        free(novo); pausar(); return;
    }
    if (buscar_artista(novo->cpf)) {
        printf("ERRO: CPF ja cadastrado!\n");
        free(novo); pausar(); return;
    }
    ler_string("Nome          : ", novo->nome, MAX_NOME);
    ler_string("Nome Artistico: ", novo->nome_artistico, MAX_NOME_ART);
    ler_string("Estilo        : ", novo->estilo, MAX_ESTILO);
    ler_string("Pais de Origem: ", novo->pais_origem, MAX_PAIS);
    ler_string("Redes Sociais (separe por ';'): ", novo->redes_sociais, MAX_REDE);
    novo->prox = NULL;
 
    if (!lista_artistas) {
        lista_artistas = novo;
    } else {
        Artista *p = lista_artistas;
        while (p->prox) p = p->prox;
        p->prox = novo;
    }
    salvar_artistas();
    printf("Artista incluido com sucesso!\n");
    pausar();
}
 
void alterar_artista(void) {
    cabecalho("ALTERAR ARTISTA");
    char cpf[MAX_CPF];
    ler_string("CPF do artista: ", cpf, sizeof(cpf));
    Artista *a = buscar_artista(cpf);
    if (!a) { printf("Artista nao encontrado.\n"); pausar(); return; }
    printf("Deixe em branco para manter o valor atual.\n\n");
 
    char tmp[MAX_NOME];
    ler_string("Nome          : ", tmp, MAX_NOME);   if (tmp[0]) strcpy(a->nome, tmp);
    ler_string("Nome Artistico: ", tmp, MAX_NOME_ART); if (tmp[0]) strcpy(a->nome_artistico, tmp);
    ler_string("Estilo        : ", tmp, MAX_ESTILO); if (tmp[0]) strcpy(a->estilo, tmp);
    ler_string("Pais de Origem: ", tmp, MAX_PAIS);   if (tmp[0]) strcpy(a->pais_origem, tmp);
    ler_string("Redes Sociais : ", tmp, MAX_REDE);   if (tmp[0]) strcpy(a->redes_sociais, tmp);
 
    salvar_artistas();
    printf("Artista alterado com sucesso!\n");
    pausar();
}
 
void excluir_artista(void) {
    cabecalho("EXCLUIR ARTISTA");
    char cpf[MAX_CPF];
    ler_string("CPF do artista: ", cpf, sizeof(cpf));
    Artista *a = buscar_artista(cpf);
    if (!a) { printf("Artista nao encontrado.\n"); pausar(); return; }
 
    printf("\n--- Dados do artista ---\n");
    imprimir_artista(a);
    char conf[5];
    ler_string("\nConfirma exclusao? (S/N): ", conf, sizeof(conf));
    maiusculo(conf);
    if (conf[0] != 'S') { printf("Exclusao cancelada.\n"); pausar(); return; }
 
    /* Remover da lista ligada */
    if (lista_artistas == a) {
        lista_artistas = a->prox;
    } else {
        Artista *prev = lista_artistas;
        while (prev->prox && prev->prox != a)
        prev = prev->prox;
        if (prev->prox){
        prev->prox = a->prox;
        }
    }
    free(a);
    salvar_artistas();
    printf("Artista excluido com sucesso!\n");
    pausar();
}
 
void submenu_artistas(void) {
    int op;
    do {
        cabecalho("SUBMENU - ARTISTAS");
        printf("1. Listar todos\n");
        printf("2. Listar um artista\n");
        printf("3. Incluir artista\n");
        printf("4. Alterar artista\n");
        printf("5. Excluir artista\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op); limpar_buffer();
        switch (op) {
            case 1: listar_todos_artistas(); break;
            case 2: listar_um_artista();     break;
            case 3: incluir_artista();       break;
            case 4: alterar_artista();       break;
            case 5: excluir_artista();       break;
            case 0: break;
            default: printf("Opcao invalida.\n"); pausar();
        }
    } while (op != 0);
}
 
/* ===================== SUBMENU ESPAÇOS PÚBLICOS ===================== */
 
void listar_todos_espacos(void) {
    cabecalho("LISTA DE ESPACOS PUBLICOS");
    if (!lista_espacos) { printf("Nenhum espaco cadastrado.\n"); pausar(); return; }
    EspacoPublico *p = lista_espacos;
    int n = 1;
    while (p) {
        printf("\n--- Espaco %d ---\n", n++);
        imprimir_espaco(p);
        p = p->prox;
    }
    pausar();
}
 
void listar_um_espaco(void) {
    cabecalho("CONSULTAR ESPACO PUBLICO");
    char cod[MAX_COD];
    ler_string("Codigo do espaco: ", cod, sizeof(cod));
    EspacoPublico *e = buscar_espaco(cod);
    if (!e) { printf("Espaco nao encontrado.\n"); }
    else { printf("\n"); imprimir_espaco(e); }
    pausar();
}
 
void incluir_espaco(void) {
    cabecalho("INCLUIR ESPACO PUBLICO");
    EspacoPublico *novo = (EspacoPublico *)malloc(sizeof(EspacoPublico));
    ler_string("Codigo        : ", novo->codigo, MAX_COD);
    if (buscar_espaco(novo->codigo)) {
        printf("ERRO: Codigo ja cadastrado!\n");
        free(novo); pausar(); return;
    }
    ler_string("Nome do Local : ", novo->nome_local,  MAX_NOME);
    ler_string("Cidade        : ", novo->cidade,       MAX_CIDADE);
    ler_string("Tipo de Espaco: ", novo->tipo_espaco,  MAX_TIPO_ESP);
    ler_string("Descricao     : ", novo->descricao,    MAX_DESC);
    novo->prox = NULL;
 
    if (!lista_espacos) {
        lista_espacos = novo;
    } else {
        EspacoPublico *p = lista_espacos;
        while (p->prox) p = p->prox;
        p->prox = novo;
    }
    salvar_espacos();
    printf("Espaco incluido com sucesso!\n");
    pausar();
}
 
void alterar_espaco(void) {
    cabecalho("ALTERAR ESPACO PUBLICO");
    char cod[MAX_COD];
    ler_string("Codigo do espaco: ", cod, sizeof(cod));
    EspacoPublico *e = buscar_espaco(cod);
    if (!e) { printf("Espaco nao encontrado.\n"); pausar(); return; }
    printf("Deixe em branco para manter o valor atual.\n\n");
 
    char tmp[MAX_DESC];
    ler_string("Nome do Local : ", tmp, MAX_NOME);      if (tmp[0]) strcpy(e->nome_local, tmp);
    ler_string("Cidade        : ", tmp, MAX_CIDADE);    if (tmp[0]) strcpy(e->cidade, tmp);
    ler_string("Tipo de Espaco: ", tmp, MAX_TIPO_ESP);  if (tmp[0]) strcpy(e->tipo_espaco, tmp);
    ler_string("Descricao     : ", tmp, MAX_DESC);      if (tmp[0]) strcpy(e->descricao, tmp);
 
    salvar_espacos();
    printf("Espaco alterado com sucesso!\n");
    pausar();
}
 
void excluir_espaco(void) {
    cabecalho("EXCLUIR ESPACO PUBLICO");
    char cod[MAX_COD];
    ler_string("Codigo do espaco: ", cod, sizeof(cod));
    EspacoPublico *e = buscar_espaco(cod);
    if (!e) { printf("Espaco nao encontrado.\n"); pausar(); return; }
 
    printf("\n--- Dados do espaco ---\n");
    imprimir_espaco(e);
    char conf[5];
    ler_string("\nConfirma exclusao? (S/N): ", conf, sizeof(conf));
    maiusculo(conf);
    if (conf[0] != 'S') { printf("Exclusao cancelada.\n"); pausar(); return; }
 
    if (lista_espacos == e) {
        lista_espacos = e->prox;
    } else {
        EspacoPublico *prev = lista_espacos;
        while (prev->prox && prev->prox != e) prev = prev->prox;
        if (prev->prox) prev->prox = e->prox;
    }
    free(e);
    salvar_espacos();
    printf("Espaco excluido com sucesso!\n");
    pausar();
}
 
void submenu_espacos(void) {
    int op;
    do {
        cabecalho("SUBMENU - ESPACOS PUBLICOS");
        printf("1. Listar todos\n");
        printf("2. Listar um espaco\n");
        printf("3. Incluir espaco\n");
        printf("4. Alterar espaco\n");
        printf("5. Excluir espaco\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op); limpar_buffer();
        switch (op) {
            case 1: listar_todos_espacos(); break;
            case 2: listar_um_espaco();     break;
            case 3: incluir_espaco();       break;
            case 4: alterar_espaco();       break;
            case 5: excluir_espaco();       break;
            case 0: break;
            default: printf("Opcao invalida.\n"); pausar();
        }
    } while (op != 0);
}
 
/* ===================== SUBMENU INTERVENÇÕES ===================== */
 
void listar_todas_intervencoes(void) {
    cabecalho("LISTA DE INTERVENCOES ARTISTICAS");
    if (!lista_intervencoes) { printf("Nenhuma intervencao cadastrada.\n"); pausar(); return; }
    Intervencao *p = lista_intervencoes;
    int n = 1;
    while (p) {
        printf("\n--- Intervencao %d ---\n", n++);
        imprimir_intervencao(p);
        p = p->prox;
    }
    pausar();
}
 
void listar_uma_intervencao(void) {
    cabecalho("CONSULTAR INTERVENCAO");
    char cpf[MAX_CPF], cod[MAX_COD], data[15];
    ler_string("CPF do artista  : ", cpf,  sizeof(cpf));
    ler_string("Codigo do espaco: ", cod,  sizeof(cod));
    ler_string("Data (DD/MM/AAAA): ", data, sizeof(data));
    Intervencao *i = buscar_intervencao(cpf, cod, data);
    if (!i) { printf("Intervencao nao encontrada.\n"); }
    else { printf("\n"); imprimir_intervencao(i); }
    pausar();
}
 
void incluir_intervencao(void) {
    cabecalho("INCLUIR INTERVENCAO ARTISTICA");
    Intervencao *novo = (Intervencao *)malloc(sizeof(Intervencao));
 
    ler_string("CPF do artista   : ", novo->cpf_artista, MAX_CPF);
    if (!buscar_artista(novo->cpf_artista)) {
        printf("ERRO: Artista nao encontrado!\n");
        free(novo); pausar(); return;
    }
    ler_string("Codigo do espaco : ", novo->cod_espaco, MAX_COD);
    if (!buscar_espaco(novo->cod_espaco)) {
        printf("ERRO: Espaco nao encontrado!\n");
        free(novo); pausar(); return;
    }
    ler_string("Data (DD/MM/AAAA): ", novo->data, 15);
    if (buscar_intervencao(novo->cpf_artista, novo->cod_espaco, novo->data)) {
        printf("ERRO: Intervencao ja cadastrada para esta chave!\n");
        free(novo); pausar(); return;
    }
    ler_string("Tipo de Intervencao: ", novo->tipo_intervencao, MAX_TIPO_INT);
    ler_string("Material Utilizado : ", novo->material_utilizado, MAX_MATERIAL);
 
    printf("Duracao (horas)    : ");
    scanf("%f", &novo->duracao); limpar_buffer();
 
    char aut[5];
    ler_string("Autorizada (S/N)   : ", aut, sizeof(aut));
    maiusculo(aut);
    novo->autorizada = (aut[0] == 'S') ? 'S' : 'N';
    novo->prox = NULL;
 
    if (!lista_intervencoes) {
        lista_intervencoes = novo;
    } else {
        Intervencao *p = lista_intervencoes;
        while (p->prox) p = p->prox;
        p->prox = novo;
    }
    salvar_intervencoes();
    printf("Intervencao incluida com sucesso!\n");
    pausar();
}
 
void alterar_intervencao(void) {
    cabecalho("ALTERAR INTERVENCAO");
    char cpf[MAX_CPF], cod[MAX_COD], data[15];
    ler_string("CPF do artista   : ", cpf,  sizeof(cpf));
    ler_string("Codigo do espaco : ", cod,  sizeof(cod));
    ler_string("Data (DD/MM/AAAA): ", data, sizeof(data));
    Intervencao *i = buscar_intervencao(cpf, cod, data);
    if (!i) { printf("Intervencao nao encontrada.\n"); pausar(); return; }
    printf("Deixe em branco para manter o valor atual (exceto duracao).\n\n");
 
    char tmp[MAX_MATERIAL];
    ler_string("Tipo de Intervencao: ", tmp, MAX_TIPO_INT);     if (tmp[0]) strcpy(i->tipo_intervencao, tmp);
    ler_string("Material Utilizado : ", tmp, MAX_MATERIAL);     if (tmp[0]) strcpy(i->material_utilizado, tmp);
 
    printf("Duracao (horas) [atual %.1f, 0=manter]: ", i->duracao);
    float dur; scanf("%f", &dur); limpar_buffer();
    if (dur > 0) i->duracao = dur;
 
    char aut[5];
    ler_string("Autorizada (S/N) [atual %c]: ", aut, sizeof(aut));
    maiusculo(aut);
    if (aut[0] == 'S' || aut[0] == 'N') i->autorizada = aut[0];
 
    salvar_intervencoes();
    printf("Intervencao alterada com sucesso!\n");
    pausar();
}
 
void excluir_intervencao(void) {
    cabecalho("EXCLUIR INTERVENCAO");
    char cpf[MAX_CPF], cod[MAX_COD], data[15];
    ler_string("CPF do artista   : ", cpf,  sizeof(cpf));
    ler_string("Codigo do espaco : ", cod,  sizeof(cod));
    ler_string("Data (DD/MM/AAAA): ", data, sizeof(data));
    Intervencao *i = buscar_intervencao(cpf, cod, data);
    if (!i) { printf("Intervencao nao encontrada.\n"); pausar(); return; }
 
    printf("\n--- Dados da intervencao ---\n");
    imprimir_intervencao(i);
    char conf[5];
    ler_string("\nConfirma exclusao? (S/N): ", conf, sizeof(conf));
    maiusculo(conf);
    if (conf[0] != 'S') { printf("Exclusao cancelada.\n"); pausar(); return; }
 
    if (lista_intervencoes == i) {
        lista_intervencoes = i->prox;
    } else {
        Intervencao *prev = lista_intervencoes;
        while (prev->prox && prev->prox != i) prev = prev->prox;
        if (prev->prox) prev->prox = i->prox;
    }
    free(i);
    salvar_intervencoes();
    printf("Intervencao excluida com sucesso!\n");
    pausar();
}
 
void submenu_intervencoes(void) {
    int op;
    do {
        cabecalho("SUBMENU - INTERVENCOES ARTISTICAS");
        printf("1. Listar todas\n");
        printf("2. Listar uma intervencao\n");
        printf("3. Incluir intervencao\n");
        printf("4. Alterar intervencao\n");
        printf("5. Excluir intervencao\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op); limpar_buffer();
        switch (op) {
            case 1: listar_todas_intervencoes(); break;
            case 2: listar_uma_intervencao();    break;
            case 3: incluir_intervencao();       break;
            case 4: alterar_intervencao();       break;
            case 5: excluir_intervencao();       break;
            case 0: break;
            default: printf("Opcao invalida.\n"); pausar();
        }
    } while (op != 0);
}
 
/* ===================== SUBMENU RELATÓRIOS ===================== */
 
/*
 * Relatório 1: Intervenções por artista (CPF fornecido)
 * Campos: nome artístico, tipo de intervenção, descrição do espaço, data, local, cidade
 */
void relatorio_intervencoes_por_artista(void) {
    cabecalho("RELATORIO: INTERVENCOES POR ARTISTA");
    char cpf[MAX_CPF];
    ler_string("CPF do artista: ", cpf, sizeof(cpf));
    Artista *art = buscar_artista(cpf);
    if (!art) { printf("Artista nao encontrado.\n"); pausar(); return; }
 
    FILE *f = fopen("relatorio_artista.txt", "w");
    if (!f) { perror("Erro ao criar relatorio"); pausar(); return; }
 
    fprintf(f, "RELATORIO: INTERVENCOES DO ARTISTA\n");
    fprintf(f, "CPF: %s | Nome: %s | Nome Artistico: %s\n", art->cpf, art->nome, art->nome_artistico);
    fprintf(f, "========================================================\n\n");
 
    int cont = 0;
    Intervencao *p = lista_intervencoes;
    while (p) {
        if (strcmp(p->cpf_artista, cpf) == 0) {
            EspacoPublico *e = buscar_espaco(p->cod_espaco);
            cont++;
            fprintf(f, "[%d]\n", cont);
            fprintf(f, "  Nome Artistico     : %s\n", art->nome_artistico);
            fprintf(f, "  Tipo de Intervencao: %s\n", p->tipo_intervencao);
            fprintf(f, "  Descricao do Local : %s\n", e ? e->descricao : "(espaco nao encontrado)");
            fprintf(f, "  Data               : %s\n", p->data);
            fprintf(f, "  Local              : %s\n", e ? e->nome_local : "(espaco nao encontrado)");
            fprintf(f, "  Cidade             : %s\n", e ? e->cidade : "(espaco nao encontrado)");
            fprintf(f, "\n");
 
            /* Exibe no terminal também */
            printf("[%d]\n", cont);
            printf("  Nome Artistico     : %s\n", art->nome_artistico);
            printf("  Tipo de Intervencao: %s\n", p->tipo_intervencao);
            printf("  Descricao do Local : %s\n", e ? e->descricao : "(espaco nao encontrado)");
            printf("  Data               : %s\n", p->data);
            printf("  Local              : %s\n", e ? e->nome_local : "(espaco nao encontrado)");
            printf("  Cidade             : %s\n", e ? e->cidade : "(espaco nao encontrado)");
            printf("\n");
        }
        p = p->prox;
    }
    if (cont == 0) {
        printf("Nenhuma intervencao encontrada para este artista.\n");
        fprintf(f, "Nenhuma intervencao encontrada.\n");
    } else {
        printf("Total: %d intervencao(oes)\n", cont);
        fprintf(f, "Total: %d intervencao(oes)\n", cont);
    }
    fclose(f);
    printf("\nRelatorio salvo em: relatorio_artista.txt\n");
    pausar();
}
 
/*
 * Relatório 2: Intervenções por cidade
 * Campos: nome do artista, nome do espaço, tipo de intervenção
 */
void relatorio_intervencoes_por_cidade(void) {
    cabecalho("RELATORIO: INTERVENCOES POR CIDADE");
    char cidade[MAX_CIDADE];
    ler_string("Nome da cidade: ", cidade, sizeof(cidade));
 
    FILE *f = fopen("relatorio_cidade.txt", "w");
    if (!f) { perror("Erro ao criar relatorio"); pausar(); return; }
 
    fprintf(f, "RELATORIO: INTERVENCOES NA CIDADE DE %s\n", cidade);
    fprintf(f, "========================================================\n\n");
 
    int cont = 0;
    Intervencao *p = lista_intervencoes;
    while (p) {
        EspacoPublico *e = buscar_espaco(p->cod_espaco);
        if (e && comparar_sem_case(e->cidade, cidade) == 0) {
            Artista *a = buscar_artista(p->cpf_artista);
            cont++;
            fprintf(f, "[%d]\n", cont);
            fprintf(f, "  Artista            : %s\n", a ? a->nome : "(artista nao encontrado)");
            fprintf(f, "  Nome do Espaco     : %s\n", e->nome_local);
            fprintf(f, "  Tipo de Intervencao: %s\n", p->tipo_intervencao);
            fprintf(f, "\n");
 
            printf("[%d]\n", cont);
            printf("  Artista            : %s\n", a ? a->nome : "(artista nao encontrado)");
            printf("  Nome do Espaco     : %s\n", e->nome_local);
            printf("  Tipo de Intervencao: %s\n", p->tipo_intervencao);
            printf("\n");
        }
        p = p->prox;
    }
    if (cont == 0) {
        printf("Nenhuma intervencao encontrada para esta cidade.\n");
        fprintf(f, "Nenhuma intervencao encontrada.\n");
    } else {
        printf("Total: %d intervencao(oes)\n", cont);
        fprintf(f, "Total: %d intervencao(oes)\n", cont);
    }
    fclose(f);
    printf("\nRelatorio salvo em: relatorio_cidade.txt\n");
    pausar();
}
 
/*
 * Relatório 3: Intervenções não autorizadas
 * Campos: descrição, nome do local, cidade
 */
void relatorio_nao_autorizadas(void) {
    cabecalho("RELATORIO: INTERVENCOES NAO AUTORIZADAS");
 
    FILE *f = fopen("relatorio_nao_autorizadas.txt", "w");
    if (!f) { perror("Erro ao criar relatorio"); pausar(); return; }
 
    fprintf(f, "RELATORIO: INTERVENCOES NAO AUTORIZADAS\n");
    fprintf(f, "========================================================\n\n");
 
    int cont = 0;
    Intervencao *p = lista_intervencoes;
    while (p) {
        if (p->autorizada == 'N') {
            EspacoPublico *e = buscar_espaco(p->cod_espaco);
            cont++;
            fprintf(f, "[%d]\n", cont);
            fprintf(f, "  Descricao do Local : %s\n", e ? e->descricao : "(espaco nao encontrado)");
            fprintf(f, "  Nome do Local      : %s\n", e ? e->nome_local : "(espaco nao encontrado)");
            fprintf(f, "  Cidade             : %s\n", e ? e->cidade    : "(espaco nao encontrado)");
            fprintf(f, "\n");
 
            printf("[%d]\n", cont);
            printf("  Descricao do Local : %s\n", e ? e->descricao : "(espaco nao encontrado)");
            printf("  Nome do Local      : %s\n", e ? e->nome_local : "(espaco nao encontrado)");
            printf("  Cidade             : %s\n", e ? e->cidade    : "(espaco nao encontrado)");
            printf("\n");
        }
        p = p->prox;
    }
    if (cont == 0) {
        printf("Nenhuma intervencao nao autorizada encontrada.\n");
        fprintf(f, "Nenhuma intervencao nao autorizada encontrada.\n");
    } else {
        printf("Total: %d intervencao(oes) nao autorizada(s)\n", cont);
        fprintf(f, "Total: %d intervencao(oes) nao autorizada(s)\n", cont);
    }
    fclose(f);
    printf("\nRelatorio salvo em: relatorio_nao_autorizadas.txt\n");
    pausar();
}
 
void submenu_relatorios(void) {
    int op;
    do {
        cabecalho("SUBMENU - RELATORIOS");
        printf("1. Intervencoes de um artista especifico\n");
        printf("2. Intervencoes em uma cidade especifica\n");
        printf("3. Intervencoes nao autorizadas\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op); limpar_buffer();
        switch (op) {
            case 1: relatorio_intervencoes_por_artista(); break;
            case 2: relatorio_intervencoes_por_cidade();  break;
            case 3: relatorio_nao_autorizadas();          break;
            case 0: break;
            default: printf("Opcao invalida.\n"); pausar();
        }
    } while (op != 0);
}
 
/* ===================== LIBERAR MEMÓRIA ===================== */
 
void liberar_memoria(void) {
    Artista *a = lista_artistas;
    while (a) { Artista *t = a->prox; free(a); a = t; }
 
    EspacoPublico *e = lista_espacos;
    while (e) { EspacoPublico *t = e->prox; free(e); e = t; }
 
    Intervencao *i = lista_intervencoes;
    while (i) { Intervencao *t = i->prox; free(i); i = t; }
}
 
/* ===================== MAIN ===================== */
 
int main(void) {
    /* Carrega dados persistidos */
    carregar_artistas();
    carregar_espacos();
    carregar_intervencoes();
 
    int op;
    do {
        cabecalho("SISTEMA DE GESTAO DE ARTE URBANA");
        printf("1. Submenu de Artistas\n");
        printf("2. Submenu de Espacos Publicos\n");
        printf("3. Submenu de Intervencoes Artisticas\n");
        printf("4. Submenu Relatorios\n");
        printf("5. Sair\n");
        printf("Opcao: ");
        scanf("%d", &op); limpar_buffer();
        switch (op) {
            case 1: submenu_artistas();    break;
            case 2: submenu_espacos();     break;
            case 3: submenu_intervencoes();break;
            case 4: submenu_relatorios();  break;
            case 5: printf("Encerrando o sistema. Ate logo!\n\n"); break;
            default: printf("Opcao invalida.\n"); pausar();
        }
    } while (op != 5);
 
    liberar_memoria();
    return 0;
}