#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Para usar `exit()`
#include <locale.h>

#define MAX_USUARIOS 30
#define MAX_NOME 30
#define MAX_SENHA 20

typedef struct
{ // estrutura com os parametros de cada usuario
    char nome[MAX_NOME];
    char senha[MAX_SENHA];
    int id;
} estrutura;

/***VARIAVEIS GLOBAIS****/
int idUsuario = 0; // inicia o id sempre em 0 para começar a varredura
int ident = 0;
estrutura user[MAX_USUARIOS];

void limpabuffer()//função para limpar o buffer apos um endereço de memoria ser escrito, chamo após os fgets
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // corpo vazio para limpar o endereço de memoria
    }
}

void lerUsuarios() // função que executa a leitura do banco de dados, armazena a quantidade de cadastros em idUsuario
{
    FILE *leitura = fopen("idusersenha.txt", "r");
    if (leitura == NULL)
    { // mensagem de erro caso o arquivo esteja vazio
        perror("Erro ao abrir arquivo");
        return;
    }

    idUsuario = 0;
    while (fgets(user[idUsuario].nome, MAX_NOME, leitura) != NULL)
    {
        user[idUsuario].nome[strcspn(user[idUsuario].nome, "\n")] = '\0';
        char *split = strtok(user[idUsuario].nome, "|");

        user[idUsuario].id = atoi(split); // transforma numeros strings em numeros inteiros
                                          //  printf("%iid\n", user[idUsuario].id);
        if (split != NULL)
        {

            split = strtok(NULL, "|");
            strcpy(user[idUsuario].nome, split);
            // printf("%s\n", user[idUsuario].nome);
            if (split != NULL)
            {
                split = strtok(NULL, "|");
                strncpy(user[idUsuario].senha, split, MAX_SENHA - 1);
                // printf("%s\n", user[idUsuario].senha);
                user[idUsuario].senha[MAX_SENHA - 1] = '\0'; // define o ultimo caractere de .senha como nulo
                idUsuario++;

                if (idUsuario >= MAX_USUARIOS)
                {
                    printf("Numero de usuarios excedidos!\n");
                    break;
                }
            }
        }
    }
    fclose(leitura);
}

void cadastrarUsuario() // função responsavel pelo cadastro de usuarios, verifica as condições para cadastro

{
    estrutura novoUsuario; // preciso de uma variavel do tipo struct igual ao user para fazer a comparação dos dados inseridos e ver se o usuario existe ou não

    printf("\tDigite o nome de usuario: ");
    if (fgets(novoUsuario.nome, MAX_NOME, stdin) != NULL)
    {
        novoUsuario.nome[strcspn(novoUsuario.nome, "\n")] = '\0';
        if (strlen(novoUsuario.nome) >= 4)
        {
            int usuarioExiste = 0;
            for (int i = 0; i < idUsuario; i++)
            {
                if (strcmp(user[i].nome, novoUsuario.nome) == 0)
                {
                    usuarioExiste = 1;
                    break;
                }
            }
            if (usuarioExiste)
            {
                printf("Usuario existente");
            }
            else
            {
                printf("\tDigite a senha: ");
                if (fgets(novoUsuario.senha, MAX_SENHA, stdin) != NULL)
                {
                    novoUsuario.senha[strcspn(novoUsuario.senha, "\n")] = '\0';
                    if (strlen(novoUsuario.senha) >= 4)
                    {
                        user[idUsuario] = novoUsuario;
                        idUsuario++;
                        FILE *escrevearquivo = fopen("idusersenha.txt", "a");
                        if (escrevearquivo == NULL)
                        {
                            perror("Erro ao abrir arquivo");
                            return;
                        }
                        fprintf(escrevearquivo, "%d|%s|%s\n", idUsuario, user[idUsuario - 1].nome, user[idUsuario - 1].senha);
                        fclose(escrevearquivo);
                    }
                    else
                    {
                        printf("Quantidade minima de 4 caracteres");
                        return;
                    }
                }
                else
                {
                    printf("Erro ao ler a senha");
                }
            }
        }
        else
        {
            printf("Quantidade minima de 4 caracteres");
        }
    }
    else
    {
        printf("Erro ao ler o nome de usuario");
    }
}

int autentica()
{
    lerUsuarios();
    char confirmaNome[MAX_NOME];
    char confirmaSenha[MAX_SENHA];

    printf("\tDigite o nome de usuario: ");
    fgets(confirmaNome, MAX_NOME, stdin);

    confirmaNome[strcspn(confirmaNome, "\n")] = '\0';

    printf("\tDigite a senha: ");
    fgets(confirmaSenha, MAX_SENHA, stdin);
    confirmaSenha[strcspn(confirmaSenha, "\n")] = '\0';
    int i = 0;
    while(i < idUsuario)
    {
        
        if (strcmp(confirmaNome, user[i].nome) == 0 )
        {
            if(strcmp(confirmaSenha, user[i].senha) == 0){
                ident = i;
                return 1;
            }
            
        }i++;
    }
    return 0;
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
void processarUsuarios()
{
    for (int i = 0; i < idUsuario; i++)
    {
        printf("ID: %d\n", user[i].id);
        printf("Nome: %s\n", user[i].nome);
        printf("Senha: %s\n", user[i].senha);
    }
}
int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");
    int opcao;

    // Carrega Usuarios existentes no início do programa
    lerUsuarios();
    processarUsuarios();
    do
    {
        opcao = obterOpcao();

        switch (opcao)
        {
        case 1:
            cadastrarUsuario();
            break;
        case 2:
            if (autentica())
            {
                printf("Bem vindo %s", user[ident].nome);
                limpabuffer();
            }
            else
            {
                printf("Falha na autenticacao. Usuario ou senha incorretos.\n");
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