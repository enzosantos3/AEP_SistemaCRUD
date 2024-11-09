#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "usuarios"
#define CHAVE_CRIPTOGRAFIA 3

typedef struct {
    char nome[50];
    int idade;
} Usuario;

void criptografar(char *texto) {
    for (int i = 0; i < strlen(texto); i++) {
        texto[i] += CHAVE_CRIPTOGRAFIA;
    }
}

void descriptografar(char *texto) {
    for (int i = 0; i < strlen(texto); i++) {
        texto[i] -= CHAVE_CRIPTOGRAFIA;
    }
}

void adicionar_usuario() {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("\033[47;32mErro ao abrir o arquivo.\033[0m\n");
        return;
    }
    Usuario usuario;
    printf("\033[47;32mDigite o nome do usuario: \033[0m");
    scanf(" %[^\n]s", usuario.nome);
    printf("\033[47;32mDigite a idade do usuario: \033[0m");
    scanf("%d", &usuario.idade);

    criptografar(usuario.nome);
    fprintf(file, "\033[47;32m%s %d\n\033[0m", usuario.nome, usuario.idade + CHAVE_CRIPTOGRAFIA);
    fclose(file);
    printf("\033[47;32mUsuario adicionado com sucesso!\033[0m\n");
}

void listar_usuarios() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("\033[47;32mNenhum usuario cadastrado.\033[0m\n");
        return;
    }
    Usuario usuario;
    printf("\033[47;32m\nLista de Usuarios:\n\033[0m");
    while (fscanf(file, "%s %d\n", usuario.nome, &usuario.idade) != EOF) {
        descriptografar(usuario.nome);
        usuario.idade -= CHAVE_CRIPTOGRAFIA;
        printf("\033[47;32mNome: %s | Idade: %d\n\033[0m", usuario.nome, usuario.idade);
    }
    fclose(file);
}

void atualizar_usuario() {
    FILE *file = fopen(FILENAME, "r+");
    if (file == NULL) {
        printf("\033[47;32mErro ao abrir o arquivo.\033[0m\n");
        return;
    }

    char nome[50];
    printf("\033[47;32mDigite o nome do usuario para atualizar: \033[0m");
    scanf(" %[^\n]s", nome);

    int encontrado = 0;
    long pos;
    Usuario usuario;
    while (fscanf(file, "%s %d\n", usuario.nome, &usuario.idade) != EOF) {
        pos = ftell(file);
        descriptografar(usuario.nome);
        if (strcmp(usuario.nome, nome) == 0) {
            encontrado = 1;
            printf("\033[47;32mDigite o novo nome: \033[0m");
            scanf(" %[^\n]s", usuario.nome);
            printf("\033[47;32mDigite a nova idade: \033[0m");
            scanf("%d", &usuario.idade);

            criptografar(usuario.nome);
            fseek(file, pos - strlen(usuario.nome) - 4, SEEK_SET);
            fprintf(file, "\033[47;32m%s %d\n\033[0m", usuario.nome, usuario.idade + CHAVE_CRIPTOGRAFIA);
            printf("\033[47;32mUsuario atualizado com sucesso!\033[0m\n");
            break;
        }
    }
    if (!encontrado) {
        printf("\033[47;32mUsuario nao encontrado.\033[0m\n");
    }
    fclose(file);
}

void excluir_usuario() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("\033[47;32mErro ao abrir o arquivo.\033[0m\n");
        return;
    }
    
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("\033[47;32mErro ao criar arquivo temporario.\033[0m\n");
        fclose(file);
        return;
    }

    char nome[50];
    printf("\033[47;32mDigite o nome do usuario para excluir: \033[0m");
    scanf(" %[^\n]s", nome);

    int encontrado = 0;
    Usuario usuario;
    while (fscanf(file, "%s %d\n", usuario.nome, &usuario.idade) != EOF) {
        descriptografar(usuario.nome);
        if (strcmp(usuario.nome, nome) != 0) {
            criptografar(usuario.nome);
            fprintf(tempFile, "\033[47;32m%s %d\n\033[0m", usuario.nome, usuario.idade + CHAVE_CRIPTOGRAFIA);
        } else {
            encontrado = 1;
        }
    }
    
    fclose(file);
    fclose(tempFile);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (encontrado) {
        printf("\033[47;32mUsuario excluido com sucesso!\033[0m\n");
    } else {
        printf("\033[47;32mUsuario nao encontrado.\033[0m\n");
    }
}

int main() {
    int opcao;

    do {
        
        printf("\033[47;32m\033[1m\n-----Menu de Opcoes----\n");
        printf("\033[47;32m\033[1m1. Adicionar Usuario   \n");
        printf("\033[47;32m\033[1m2. Listar Usuarios     \n");
        printf("\033[47;32m\033[1m3. Atualizar Usuario   \n");
        printf("\033[47;32m\033[1m4. Excluir Usuario     \n");
        printf("\033[47;32m\033[1m0. Sair                \n");
        printf("\033[47;32m\033[1mEscolha uma opcao:    ");
        scanf("%d", &opcao);

        printf("\033[0m");

        switch (opcao) {
            case 1:
                adicionar_usuario();
                break;
            case 2:
                listar_usuarios();
                break;
            case 3:
                atualizar_usuario();
                break;
            case 4:
                excluir_usuario();
                break;
            case 0:
                printf("\033[1mSaindo...\n");
                break;
            default:
                printf("\033[1mOpcao invalida.\n\033[0m\n");
        }
    } while (opcao != 0);

    return 0;
}
