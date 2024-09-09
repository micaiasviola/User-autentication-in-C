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
int idUsuario = 0;          // Numero de usuarios cadastrados

// Funcao para limpar o buffer de entrada
void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // corpo do loop vazio para apenas limpar o buffer, consumindo os dados do caracter sem fazer nada com eles apenas consumir
    } // Consome o restante dos caracteres no buffer
      // verifica se o caractere "C" nao é \n e tambem verifica se nao é o fim do arquvio END OF FILE "EOF"
}

void cadastroUser()
{
    char buffer[MAX_NOME];
    Usuario novoUsuario; // Variavel temporaria para novo usuario

    if (idUsuario >= MAX_USUARIOS)
    {
        puts("Numero de usuarios cadastrados excedido!");
        return;
    }
    // LER O NOME DO USUARIO-----------------------------------------------------------------------------------
    FILE *arquivo = fopen("idusersenha.txt", "r"); // Abertura do arquivo no modo de adicao

    if (arquivo == NULL)
    { // se arquivo fol nulo
        perror("Erro ao abrir o arquivo");
        return;
    }
    int linhas = 0;
    char ch;
    while ((ch = fgetc(arquivo)) != EOF)
    {
        if (ch == '\n')
        {
            idUsuario++;
            linhas++;
        }
    }
    printf("\tDigite o nome de usuario: ");
    if (fgets(novoUsuario.nome, MAX_NOME, stdin) != NULL)
    {
        novoUsuario.nome[strcspn(novoUsuario.nome, "\n")] = '\0';
        if (strlen(novoUsuario.nome) >= 4)
        {

            while (fgets(buffer, MAX_NOME, arquivo) != NULL) // importante o !NULL faz o fgets retornar apenas quando chegar ao fim do arquivo
            {                                                // verifica se o usuario ja existe
                buffer[strcspn(buffer, "\n")] = '\0';
                if (strcmp(buffer, novoUsuario.nome) == 0)
                {
                    printf("Nome de usuario existente");
                    fclose(arquivo);
                    return;
                }
            }
            fclose(arquivo);

            user[idUsuario] = novoUsuario;
            FILE *arquivo = fopen("idusersenha.txt", "a");
            fprintf(arquivo, "%i|", idUsuario + 1);
            fprintf(arquivo, "%s", user[idUsuario].nome);
        }
        else
        {
            printf("Quantidade minima de 4 caracteres");
        }
    }
    printf("\n\tDigite a senha: ");
    if (fgets(novoUsuario.senha, MAX_SENHA, stdin) != NULL)
    {
        novoUsuario.senha[strcspn(novoUsuario.senha, "\n")] = '\0'; // Remove o caractere de nova linha
        if (strlen(novoUsuario.senha) >= 4)
        {
            user[idUsuario] = novoUsuario;
            fprintf(arquivo, "|%s", user[idUsuario].senha);
            fprintf(arquivo, "\n");
        }

        fclose(arquivo);

        // idUsuario++; // Incrementa o numero de usuarios
        printf("\t\nUsuario cadastrado com sucesso!!!\n");
    }
    idUsuario = 0;
    linhas = 0;
}

int autenticarUser()
{
    char confirmaNome[MAX_NOME];
    char confirmaSenha[MAX_SENHA];

    printf("Digite o nome de usuario: ");
    fgets(confirmaNome, MAX_NOME, stdin);             // Lê o nome do usuario
    confirmaNome[strcspn(confirmaNome, "\n")] = '\0'; // Remove o caractere de nova linha, a funcao strcspn(confirmaNome, "\n") calcula o comprimento da string ate encontrar o caractere na string
                                                      // de busca, no caso o "\n". a expressao pai confirmaNome[strcspn(confirmaNome, "\n")] = '\0'; usa o valor retornado da funcao strcspn como um "i" indice para acessar o caractere na posicao onde o /n foi encontrado e substitiur por '\0', um nulo

    printf("Digite a senha: ");
    fgets(confirmaSenha, MAX_SENHA, stdin);             // Lê a senha, percorre o TAMANHO MAX no caso, e as entradas sao de teclado STDIN
    confirmaSenha[strcspn(confirmaSenha, "\n")] = '\0'; // Remove o caractere de nova linha

    // Verifica se o nome e senha estao corretos
    for (int i = 0; i < idUsuario; i++)
    {
        if (strcmp(user[i].nome, confirmaNome) == 0 && strcmp(user[i].senha, confirmaSenha) == 0)
        {
            return 1; // Autenticacao bem-sucedida
        }
    }

    return 0; // Falha na autenticacao
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
        { // le uma entrada de teclado até encontrar um /n ou o limite de car e armazena na variavel
            if (sscanf(buffer, "%d", &opcao) == 1 && opcao >= 1 && opcao <= 3)
            {                 // sscanf retorna um numero inteiro de itens lidos, se retornar 1 significa que a conversao funcionou
                return opcao; // retorna para a variavel opcao que é o controle do menu
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

    do
    {
        opcao = obterOpcao(); // Obtém uma opcao valida retornado da funccao

        switch (opcao)
        {
        case 1:
            cadastroUser();
            break;
        case 2:
            if (autenticarUser())
            {
                printf("Usuario autenticado com sucesso!\n");
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
            // Nao deve chegar aqui devido ao controle na funcao obterOpcao()
            break;
        }
    } while (opcao != 3);

    return 0;
}