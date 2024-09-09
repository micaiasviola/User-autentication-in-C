#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Para usar `exit()`
#include <locale.h>

#define MAX_USUARIOS 100
#define MAX_NOME 50
#define MAX_SENHA 20

typedef struct
{
    char nome[MAX_NOME];
    char senha[MAX_SENHA];
} Usuario;

Usuario user[MAX_USUARIOS]; // Vetor para armazenar os usuarios
int idUsuario = 0;          // Número de usuários cadastrados

// Função para limpar o buffer de entrada
void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // Corpo do loop vazio para apenas limpar o buffer
    }
}

void carregarUsuarios()
{
    FILE *arquivo = fopen("idusersenha.txt", "r");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    idUsuario = 0;
    while (fgets(user[idUsuario].nome, MAX_NOME, arquivo) != NULL)
    {
        user[idUsuario].nome[strcspn(user[idUsuario].nome, "\n")] = '\0';
        char *parse = strtok(user[idUsuario].nome, "|");
        if (parse != NULL)
        {
            strcpy(user[idUsuario].nome, parse);
            parse = strtok(NULL, "|");
            if (parse != NULL)
            {
                strncpy(user[idUsuario].senha, parse, MAX_SENHA - 1);
                user[idUsuario].senha[MAX_SENHA - 1] = '\0';
                idUsuario++;
                if (idUsuario >= MAX_USUARIOS)
                {
                    printf("Número máximo de usuários atingido.\n");
                    break;
                }
            }
        }
    }
    fclose(arquivo);
}

void cadastroUser()
{
    Usuario novoUsuario;
    char buffer[MAX_NOME];

    printf("\tDigite o nome de usuario: ");
    if (fgets(novoUsuario.nome, MAX_NOME, stdin) != NULL)
    {
        novoUsuario.nome[strcspn(novoUsuario.nome, "\n")] = '\0';

        if (strlen(novoUsuario.nome) >= 4)
        {
            int usuarioExistente = 0;
            for (int i = 0; i < idUsuario; i++)
            {
                if (strcmp(user[i].nome, novoUsuario.nome) == 0)
                {
                    usuarioExistente = 1;
                    break;
                }
            }

            if (usuarioExistente)
            {
                printf("Usuário existente!\n");
            }
            else
            {
                printf("Digite a senha: ");
                if (fgets(novoUsuario.senha, MAX_SENHA, stdin) != NULL)
                {
                    novoUsuario.senha[strcspn(novoUsuario.senha, "\n")] = '\0';
                    user[idUsuario] = novoUsuario;
                    idUsuario++;

                    FILE *arquivoescrita = fopen("idusersenha.txt", "a");
                    if (arquivoescrita == NULL)
                    {
                        perror("Não foi possível abrir o arquivo para escrita");
                        return;
                    }
                    fprintf(arquivoescrita, "%d|%s|%s\n", idUsuario, user[idUsuario - 1].nome, user[idUsuario - 1].senha);
                    fclose(arquivoescrita);
                    printf("Usuário cadastrado com sucesso!\n");
                }
                else
                {
                    printf("Erro ao ler a senha.\n");
                }
            }
        }
        else
        {
            printf("Quantidade mínima de 4 caracteres.\n");
        }
    }
    else
    {
        printf("Erro ao ler o nome do usuário.\n");
    }
}

int autenticarUser()
{
    char confirmaNome[MAX_NOME];
    char confirmaSenha[MAX_SENHA];

    printf("Digite o nome de usuario: ");
    fgets(confirmaNome, MAX_NOME, stdin);
    confirmaNome[strcspn(confirmaNome, "\n")] = '\0';

    printf("Digite a senha: ");
    fgets(confirmaSenha, MAX_SENHA, stdin);
    confirmaSenha[strcspn(confirmaSenha, "\n")] = '\0';

    for (int i = 0; i < idUsuario; i++)
    {
        if (strcmp(user[i].nome, confirmaNome) == 0 && strcmp(user[i].senha, confirmaSenha) == 0)
        {
            return 1; // Autenticação bem-sucedida
        }
    }

    return 0; // Falha na autenticação
}

int obterOpcao()
{
    int opcao;
    char buffer[100];

    while (1)
    {
        printf("\nMenu:\n");
        printf("1. Cadastrar usuario\n");
        printf("2. Autenticar usuario\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");

        if (fgets(buffer, sizeof(buffer), stdin))
        {
            if (sscanf(buffer, "%d", &opcao) == 1 && opcao >= 1 && opcao <= 3)
            {
                return opcao;
            }
            else
            {
                printf("Opcao invalida. Tente novamente.\n");
            }
        }
        else
        {
            printf("Erro ao ler a entrada. Tente novamente.\n");
        }
    }
}

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");
    int opcao;

    // Carrega usuários existentes no início do programa
    carregarUsuarios();

    do
    {
        opcao = obterOpcao();

        switch (opcao)
        {
        case 1:
            cadastroUser();
            break;
        case 2:
            if (autenticarUser())
            {
                printf("Usuário autenticado com sucesso!\n");
            }
            else
            {
                printf("Falha na autenticação. Usuário ou senha incorretos.\n");
            }
            break;
        case 3:
            printf("Saindo...\n");
            break;
        default:
            // Não deve chegar aqui devido ao controle na função obterOpcao()
            break;
        }
    } while (opcao != 3);

    return 0;
}
