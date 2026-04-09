#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    /* Implementacao de getch() para Linux/Mac */
    #include <termios.h>
    #include <unistd.h>
    int getch(void) {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
    #define system_cls() system("clear")
#endif

#ifdef _WIN32
    #define system_cls() system("cls")
#endif

#define MAX_IMOVEIS 100
#define MAX_CLIENTES 100
#define MAX_CORRETORES 100
#define MAX_PROPOSTAS 100

#define ARQ_IMOVEIS "imoveis.txt"
#define ARQ_CLIENTES "clientes.txt"
#define ARQ_CORRETORES "corretores.txt"
#define ARQ_PROPOSTAS "propostas.txt"

typedef struct 
{
    char endereco[100];
    int id_imovel;
    float preco;
    int qtd_quartos;
    int qtd_banheiros;
    int qtd_vagas_carro;
} imovel;

typedef struct
{
    int id_cliente;
    char nome[100];
    char rg[20];
    char cpf[20];
    char data[20];
    char tel[20];
    char endereco[100];
} cliente;

typedef struct
{
    int id_corretor;
    char nome[100];
    char rg[20];
    char cpf[20];
    char data[20];
    char tel[20];
    char endereco[100];
    char senha[12];
} corretor;

typedef struct
{
	int id_proposta;
	int id_imovel;
    int id_cliente;
    int id_corretor;
    float valor_proposta;
    float entrada;
    float prestacao;
} proposta;

/* Declaracoes antecipadas */
int substituir_arquivo(const char *destino);
FILE *abrir_leitura(const char *caminho);
FILE *abrir_escrita(const char *caminho);
void cadastrar_imovel(void);
void cadastrar_cliente(void);
void cadastrar_corretor(void);
void cadastrar_proposta(void);
void consultar_imovel(void);
void consultar_cliente(void);
void consultar_corretor(void);
void consultar_propostas(void);
void atualizar_imovel(void);
void atualizar_cliente(void);
void atualizar_corretor(void);
void atualizar_propostas(void);
void remover_imovel(void);
void remover_cliente(void);
void remover_corretor(void);
void remover_propostas(void);
void listarimoveis(void);
void listarclientes(void);
void listarcorretores(void);
void listarpropostas(void);
void calcular_financiamento(void);
int fazer_login(void);


/* Limpa todos os caracteres restantes no buffer de entrada */
void limpar_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}



/* Substitui 'destino' por 'temp.txt' de forma atomica.
   Retorna 1 em sucesso, 0 se temp.txt nao puder ser aberto (original intacto). */
int substituir_arquivo(const char *destino)
{
#ifdef _WIN32
    /* Windows nao tem access(); usa fopen como verificacao */
    FILE *teste = fopen("temp.txt", "r");
    if (teste == NULL)
    {
        printf("\nErro: arquivo temporario nao encontrado. Operacao cancelada.\n\n");
        return 0;
    }
    fclose(teste);
#else
    if (access("temp.txt", F_OK) != 0)
    {
        printf("\nErro: arquivo temporario nao encontrado. Operacao cancelada.\n\n");
        return 0;
    }
#endif

    if (remove(destino) != 0)
    {
        printf("\nErro ao remover arquivo original. Operacao cancelada.\n\n");
        return 0;
    }

    if (rename("temp.txt", destino) != 0)
    {
        printf("\nErro ao substituir arquivo. Operacao cancelada.\n\n");
        return 0;
    }

    return 1;
}

/* Helpers de arquivo */
FILE *abrir_leitura(const char *caminho)
{
    return fopen(caminho, "r");
}

FILE *abrir_escrita(const char *caminho)
{
    return fopen(caminho, "a+");
}

void tirarespaco (char texto[])
{
	size_t i;
	for (i=0; i<strlen(texto); i++)
	{
		if (texto[i] == ' ')
		{
			texto[i] = '+';
		}
	}
}

void colocarespaco (char texto[])
{
	size_t i;
	for (i=0; i<strlen(texto); i++)
	{
		if (texto[i] == '+')
		{
			texto[i] = ' ';
		}
	}
}

void imprimir_dados_imovel(imovel i)
{
	printf("Imovel\n\n");
	printf("Endereco: %s\n", i.endereco);
	printf("ID do imovel: %i\n", i.id_imovel);
	printf("Preco: %.2f\n", i.preco);
	printf("Quantidade de quartos: %i\n", i.qtd_quartos);
	printf("Quantidade de banheiros: %i\n", i.qtd_banheiros);
	printf("Quantidade de vagas de carro: %i\n", i.qtd_vagas_carro);
}

void imprimir_dados_cliente(cliente c)
{
	printf("Dados do Cliente\n\n");
	printf("Nome do Cliente: %s\n", c.nome);
	printf("ID: %i\n", c.id_cliente);
	printf("CPF: %s\n", c.cpf);
	printf("RG: %s\n", c.rg);
	printf("Data de nascimento: %s\n", c.data);
	printf("Telefone: %s\n", c.tel);
	printf("Endereco: %s\n", c.endereco);
}

void imprimir_dados_corretor(corretor c)
{
	printf("Dados do Corretor\n\n");
	printf("Nome do Corretor: %s\n", c.nome);
	printf("ID do corretor: %i\n", c.id_corretor);
	printf("CPF: %s\n", c.cpf);
	printf("RG: %s\n", c.rg);
	printf("Data de nascimento: %s\n", c.data);
	printf("Telefone: %s\n", c.tel);
	printf("Endereco: %s\n", c.endereco);
}

void imprimir_dados_propostas(proposta p)
{
	printf("Dados da proposta\n\n");
    printf("ID da Proposta: %i\n", p.id_proposta);
    printf("ID do Imovel: %i\n", p.id_imovel);
    printf("ID do Cliente: %i\n", p.id_cliente);
    printf("Valor da proposta: %.2f\n", p.valor_proposta);
    printf("Valor de entrada: %.2f\n", p.entrada);
    printf("Valor da Prestacao: %.2f\n", p.prestacao);
}

imovel pesquisar_imovel(int id_imovel)
{
    FILE *arq = abrir_leitura(ARQ_IMOVEIS);
    imovel tmp = {0};
    tmp.id_imovel = -1;

    if (arq == NULL) return tmp;

    while (fscanf(arq, "%s %i %f %i %i %i", tmp.endereco, &tmp.id_imovel, &tmp.preco, &tmp.qtd_quartos, &tmp.qtd_banheiros, &tmp.qtd_vagas_carro) == 6)
	{
		colocarespaco(tmp.endereco);
        if (tmp.id_imovel == id_imovel)
		{
            break;
        }
        else
		{
			tmp.id_imovel = -1;
		}
    }

    fclose(arq);

    return tmp;
}

cliente pesquisar_cliente(int id_cliente)
{
    FILE *arq = abrir_leitura(ARQ_CLIENTES);
    cliente tmp = {0};
    tmp.id_cliente = -1;

    if (arq == NULL) return tmp;

    while (fscanf(arq, "%s %i %s %s %s %s %s", tmp.nome, &tmp.id_cliente, tmp.cpf, tmp.rg, tmp.data, tmp.tel, tmp.endereco) == 7)
	{
		colocarespaco(tmp.nome);
		colocarespaco(tmp.cpf);
		colocarespaco(tmp.rg);
		colocarespaco(tmp.data);
		colocarespaco(tmp.tel);
		colocarespaco(tmp.endereco);
        if (tmp.id_cliente == id_cliente)
		{
            break;
        }
        else
		{
			tmp.id_cliente = -1;
		}
    }

    fclose(arq);

    return tmp;
}

corretor pesquisar_corretor(int id_corretor)
{
    FILE *arq = abrir_leitura(ARQ_CORRETORES);
    corretor tmp = {0};
    tmp.id_corretor = -1;

    if (arq == NULL) return tmp;

    while (fscanf(arq, "%s %i %s %s %s %s %s %s", tmp.nome, &tmp.id_corretor, tmp.cpf, tmp.rg, tmp.data, tmp.tel, tmp.endereco, tmp.senha) == 8)
	{
		colocarespaco(tmp.nome);
		colocarespaco(tmp.cpf);
		colocarespaco(tmp.rg);
		colocarespaco(tmp.data);
		colocarespaco(tmp.tel);
		colocarespaco(tmp.endereco);
        if (tmp.id_corretor == id_corretor)
		{
            break;
        }
        else
		{
			tmp.id_corretor = -1;
		}
    }

    fclose(arq);

    return tmp;
}

proposta pesquisar_propostas(int id_proposta)
{
    FILE *arq = abrir_leitura(ARQ_PROPOSTAS);
    proposta tmp = {0};
    tmp.id_proposta = -1;

    if (arq == NULL) return tmp;

    while (fscanf(arq, "%i %i %i %i %f %f %f", &tmp.id_proposta, &tmp.id_imovel, &tmp.id_cliente, &tmp.id_corretor, &tmp.valor_proposta, &tmp.entrada, &tmp.prestacao) == 7)
    {
        if (tmp.id_proposta == id_proposta)
        {
            break;
        }
        else
        {
            tmp.id_proposta = -1;
        }
    }

    fclose(arq);

    return tmp;
}

int contar_qtd_imoveis(void)
{
	FILE *arq = abrir_leitura(ARQ_IMOVEIS);
	imovel tmp;
	int c = 0;
	if (arq == NULL) return 0;
	while (fscanf(arq, "%s %i %f %i %i %i", tmp.endereco, &tmp.id_imovel, &tmp.preco, &tmp.qtd_quartos, &tmp.qtd_banheiros, &tmp.qtd_vagas_carro) == 6)
	{
		c++;
	}
	fclose(arq);
	return c;
}

int contar_qtd_cliente(void)
{
	FILE *arq = abrir_leitura(ARQ_CLIENTES);
	cliente tmp;
	int c = 0;
	if (arq == NULL) return 0;
	while (fscanf(arq, "%s %i %s %s %s %s %s", tmp.nome, &tmp.id_cliente, tmp.cpf, tmp.rg, tmp.data, tmp.tel, tmp.endereco) == 7)
	{
		c++;
	}
	fclose(arq);
	return c;
}

int contar_qtd_corretor(void)
{
	FILE *arq = abrir_leitura(ARQ_CORRETORES);
	corretor tmp;
	int c = 0;
	if (arq == NULL) return 0;
	while (fscanf(arq, "%s %i %s %s %s %s %s %s", tmp.nome, &tmp.id_corretor, tmp.cpf, tmp.rg, tmp.data, tmp.tel, tmp.endereco, tmp.senha) == 8)
	{
		c++;
	}
	fclose(arq);
	return c;
}

int contar_qtd_propostas(void)
{
    FILE *arq = abrir_leitura(ARQ_PROPOSTAS);
    proposta tmp;
    int c = 0;
    if (arq == NULL) return 0;
    while (fscanf(arq, "%i %i %i %i %f %f %f", &tmp.id_proposta, &tmp.id_imovel, &tmp.id_cliente, &tmp.id_corretor, &tmp.valor_proposta, &tmp.entrada, &tmp.prestacao) == 7)
    {
        c++;
    }
    fclose(arq);
    return c;
}

void removerimoveldoarquivo(int id_imovelpesq)
{
	imovel tmp;
	FILE* arq = abrir_leitura(ARQ_IMOVEIS);
	FILE* arqNovo = fopen("temp.txt", "w");
	if (arq == NULL || arqNovo == NULL) { if(arq) fclose(arq); if(arqNovo) fclose(arqNovo); return; }

	while(fscanf(arq, "%s %i %f %i %i %i", tmp.endereco, &tmp.id_imovel, &tmp.preco, &tmp.qtd_quartos, &tmp.qtd_banheiros, &tmp.qtd_vagas_carro) == 6)
	{
		if (tmp.id_imovel != id_imovelpesq)
		{
			fprintf(arqNovo,"%s %i %f %i %i %i\n", tmp.endereco, tmp.id_imovel, tmp.preco, tmp.qtd_quartos, tmp.qtd_banheiros, tmp.qtd_vagas_carro);
		}
	}
	fclose(arq);
	fclose(arqNovo);

	remove(ARQ_IMOVEIS);
	rename("temp.txt", ARQ_IMOVEIS);
}

void removerclientedoarquivo(int id_clientepesq)
{
	cliente tmp;
	FILE* arq = abrir_leitura(ARQ_CLIENTES);
	FILE* arqNovo = fopen("temp.txt", "w");
	if (arq == NULL || arqNovo == NULL) { if(arq) fclose(arq); if(arqNovo) fclose(arqNovo); return; }

	while(fscanf(arq, "%s %i %s %s %s %s %s", tmp.nome, &tmp.id_cliente, tmp.cpf, tmp.rg, tmp.data, tmp.tel, tmp.endereco) == 7)
	{
		if (tmp.id_cliente != id_clientepesq)
		{
			fprintf(arqNovo,"%s %i %s %s %s %s %s\n", tmp.nome, tmp.id_cliente, tmp.cpf, tmp.rg, tmp.data, tmp.tel, tmp.endereco);
		}
	}
	fclose(arq);
	fclose(arqNovo);

	remove(ARQ_CLIENTES);
	rename("temp.txt", ARQ_CLIENTES);
}

void removercorretordoarquivo(int id_corretorpesq)
{
	corretor tmp;
	FILE* arq = abrir_leitura(ARQ_CORRETORES);
	FILE* arqNovo = fopen("temp.txt", "w");
	if (arq == NULL || arqNovo == NULL) { if(arq) fclose(arq); if(arqNovo) fclose(arqNovo); return; }

	while(fscanf(arq, "%s %i %s %s %s %s %s %s", tmp.nome, &tmp.id_corretor, tmp.cpf, tmp.rg, tmp.data, tmp.tel, tmp.endereco, tmp.senha) == 8)
	{
		if (tmp.id_corretor != id_corretorpesq)
		{
			fprintf(arqNovo,"%s %i %s %s %s %s %s %s\n", tmp.nome, tmp.id_corretor, tmp.cpf, tmp.rg, tmp.data, tmp.tel, tmp.endereco, tmp.senha);
		}
	}
	fclose(arq);
	fclose(arqNovo);

	remove(ARQ_CORRETORES);
	rename("temp.txt", ARQ_CORRETORES);
}

void removerpropostadoarquivo(int id_propostapesq)
{
	proposta tmp;
	FILE* arq = abrir_leitura(ARQ_PROPOSTAS);
	FILE* arqNovo = fopen("temp.txt", "w");
	if (arq == NULL || arqNovo == NULL) { if(arq) fclose(arq); if(arqNovo) fclose(arqNovo); return; }

	while(fscanf(arq, "%i %i %i %i %f %f %f", &tmp.id_proposta, &tmp.id_imovel, &tmp.id_cliente, &tmp.id_corretor, &tmp.valor_proposta, &tmp.entrada, &tmp.prestacao) == 7)
	{
		if (tmp.id_proposta != id_propostapesq)
		{
			fprintf(arqNovo, "%i %i %i %i %f %f %f\n", tmp.id_proposta, tmp.id_imovel, tmp.id_cliente, tmp.id_corretor, tmp.valor_proposta, tmp.entrada, tmp.prestacao);
		}
	}
	fclose(arq);
	fclose(arqNovo);

	remove(ARQ_PROPOSTAS);
	rename("temp.txt", ARQ_PROPOSTAS);
}

void cadastrar_imovel(void)
{
	imovel novo, pesq;
    FILE *arq = abrir_escrita(ARQ_IMOVEIS);
	
    if (contar_qtd_imoveis() >= MAX_IMOVEIS) 
	{
		printf("Numero maximo de imoveis atingido.\n");
    } 
	else 
	{   
        printf("Informe a ID do imovel: "); //A imobiliaria possui um sistema interno de identificação, para controle e organização.
        scanf(" %i", &novo.id_imovel);
        limpar_buffer();
        
        pesq = pesquisar_imovel(novo.id_imovel);
        
        if (pesq.id_imovel != -1) 
        {
        	imprimir_dados_imovel(pesq);
	    	printf("\n\nEste imovel ja esta cadastrado\n\n");
        }
        else
        {
        	printf("Endereco: ");
        	fgets(novo.endereco, sizeof(novo.endereco), stdin);
            novo.endereco[strcspn(novo.endereco, "\n")] = '\0';
            
            tirarespaco(novo.endereco);
        	
            printf("Preco do imovel: ");
            scanf("%f", &novo.preco);
            limpar_buffer();
            
            printf("Informe a quantidade de quartos: ");
            scanf("%i", &novo.qtd_quartos);
            limpar_buffer();
            
            printf("Informe a quantidade de banheiros: ");
            scanf("%i", &novo.qtd_banheiros);
            limpar_buffer();
            
            printf("Informe o numero de vagas para carros: ");
            scanf("%i", &novo.qtd_vagas_carro);
            limpar_buffer();

            if (arq != NULL)
            {
                fprintf(arq,"%s %i %f %i %i %i", novo.endereco, novo.id_imovel, novo.preco, novo.qtd_quartos, novo.qtd_banheiros, novo.qtd_vagas_carro);
                fprintf(arq, "\n");
                printf("\nImovel cadastrado com sucesso.\n\n");
            }
            else
            {
                printf("\nErro ao cadastrar!\n\n");
            }
        }
    }
    
    fclose(arq);
}

void cadastrar_cliente(void)
{
	cliente novo, pesq;
    FILE *arq = abrir_escrita(ARQ_CLIENTES);
	
    if (contar_qtd_cliente() >= MAX_CLIENTES) 
	{
		printf("Numero maximo de clientes atingido.\n");
    } 
	else 
	{   
        printf("Informe a ID do cliente: ");
        scanf(" %i", &novo.id_cliente);
        limpar_buffer();
        
        pesq = pesquisar_cliente(novo.id_cliente);
        
        if (pesq.id_cliente != -1) 
        {
        	imprimir_dados_cliente(pesq);
	    	printf("\n\nEste cliente ja esta cadastrado\n\n");
        }
        else
        {
        	printf("Nome: ");
        	fgets(novo.nome, sizeof(novo.nome), stdin);
            novo.nome[strcspn(novo.nome, "\n")] = '\0';
            tirarespaco(novo.nome);
			
			printf("CPF: ");
        	fgets(novo.cpf, sizeof(novo.cpf), stdin);
            novo.cpf[strcspn(novo.cpf, "\n")] = '\0';
            tirarespaco(novo.cpf);
        	
        	printf("RG: ");
        	fgets(novo.rg, sizeof(novo.rg), stdin);
            novo.rg[strcspn(novo.rg, "\n")] = '\0';
            tirarespaco(novo.rg);
        	
        	printf("Data de nascimento (DD/MM/AAAA): ");
        	fgets(novo.data, sizeof(novo.data), stdin);
            novo.data[strcspn(novo.data, "\n")] = '\0';
            tirarespaco(novo.data);
			
			printf("Numero do telefone: ");
        	fgets(novo.tel, sizeof(novo.tel), stdin);
            novo.tel[strcspn(novo.tel, "\n")] = '\0';
            tirarespaco(novo.tel);
			
			printf("Endereco: ");
        	fgets(novo.endereco, sizeof(novo.endereco), stdin);
            novo.endereco[strcspn(novo.endereco, "\n")] = '\0';
            tirarespaco(novo.endereco);

            if (arq != NULL)
            {
                fprintf(arq,"%s %i %s %s %s %s %s", novo.nome, novo.id_cliente, novo.cpf, novo.rg, novo.data, novo.tel, novo.endereco);
                fprintf(arq, "\n");
                printf("\nCliente cadastrado com sucesso.\n\n");
            }
            else
            {
                printf("\nErro ao cadastrar!\n\n");
            }
        }
    }
    
    fclose(arq);
}

void cadastrar_corretor(void)
{
	corretor novo, pesq;
    FILE *arq = abrir_escrita(ARQ_CORRETORES);
	
    if (contar_qtd_corretor() >= MAX_CORRETORES) 
	{
		printf("Numero maximo de corretores atingido.\n");
    } 
	else 
	{   
        printf("Informe a ID do corretor: ");
        scanf(" %i", &novo.id_corretor);
        limpar_buffer();
        
        pesq = pesquisar_corretor(novo.id_corretor);
        
        if (pesq.id_corretor != -1) 
        {
        	imprimir_dados_corretor(pesq);
	    	printf("\n\nEste corretor ja esta cadastrado\n\n");
        }
        else
        {
        	printf("Nome: ");
        	fgets(novo.nome, sizeof(novo.nome), stdin);
            novo.nome[strcspn(novo.nome, "\n")] = '\0';
            tirarespaco(novo.nome);
			
			printf("CPF: ");
        	fgets(novo.cpf, sizeof(novo.cpf), stdin);
            novo.cpf[strcspn(novo.cpf, "\n")] = '\0';
            tirarespaco(novo.cpf);
        	
        	printf("RG: ");
        	fgets(novo.rg, sizeof(novo.rg), stdin);
            novo.rg[strcspn(novo.rg, "\n")] = '\0';
            tirarespaco(novo.rg);
        	
        	printf("Data de nascimento (DD/MM/AAAA): ");
        	fgets(novo.data, sizeof(novo.data), stdin);
            novo.data[strcspn(novo.data, "\n")] = '\0';
            tirarespaco(novo.data);
			
			printf("Numero do telefone: ");
        	fgets(novo.tel, sizeof(novo.tel), stdin);
            novo.tel[strcspn(novo.tel, "\n")] = '\0';
            tirarespaco(novo.tel);
			
			printf("Endereco: ");
        	fgets(novo.endereco, sizeof(novo.endereco), stdin);
            novo.endereco[strcspn(novo.endereco, "\n")] = '\0';
            tirarespaco(novo.endereco);
            
            printf("Senha: ");
        	fgets(novo.senha, sizeof(novo.senha), stdin);
            novo.senha[strcspn(novo.senha, "\n")] = '\0';
        	limpar_buffer();

            if (arq != NULL)
            {
                fprintf(arq,"%s %i %s %s %s %s %s %s", novo.nome, novo.id_corretor, novo.cpf, novo.rg, novo.data, novo.tel, novo.endereco, novo.senha);
                fprintf(arq, "\n");
                printf("\nCorretor cadastrado com sucesso.\n\n");
            }
            else
            {
                printf("\nErro ao cadastrar!\n\n");
            }
        }
    }
    
    fclose(arq);
}

void cadastrar_proposta(void)
{
	proposta novo, pesq;
    FILE *arq = abrir_escrita(ARQ_PROPOSTAS);
    int id_imovel, id_cliente;
    
    if (contar_qtd_propostas() >= MAX_PROPOSTAS) 
	{
		printf("Numero maximo de propostas atingido.\n");
    }
    else 
	{   
        printf("Informe a ID da proposta: ");
        scanf(" %i", &novo.id_proposta);
        limpar_buffer();
        
        pesq = pesquisar_propostas(novo.id_proposta);
        
        if (pesq.id_proposta != -1) 
        {
        	imprimir_dados_propostas(pesq);
	    	printf("\n\nEsta proposta ja esta cadastrada\n\n");
        }
        else
        {
        	printf("Digite a ID do imovel para o qual deseja registrar a proposta: ");
    		scanf("%i", &id_imovel);
    		limpar_buffer();
			
			printf("Digite o ID do cliente que esta fazendo a proposta: ");
    		scanf("%i", &id_cliente);
    		limpar_buffer();

    		printf("Digite o ID do corretor responsavel: ");
    		scanf("%i", &novo.id_corretor);
    		limpar_buffer();
			
			printf("Digite o valor da proposta: ");
    		scanf("%f", &novo.valor_proposta);
    		limpar_buffer();
    		
    		printf("Digite o valor da entrada: ");
    		scanf("%f", &novo.entrada);
    		limpar_buffer();
    		
    		printf("Digite o valor da parcela: ");
    		scanf("%f", &novo.prestacao);
    		limpar_buffer();
    		
    		imovel imv = pesquisar_imovel(id_imovel);
    		cliente cli = pesquisar_cliente(id_cliente);
    		
    		if (imv.id_imovel != -1 && cli.id_cliente != -1) 
			{
                if (arq != NULL) 
				{
                    fprintf(arq, "%i %i %i %i %.2f %.2f %.2f\n", novo.id_proposta, id_imovel, id_cliente, novo.id_corretor, novo.valor_proposta, novo.entrada, novo.prestacao);
                    printf("\nProposta cadastrada com sucesso.\n\n");
                } else 
				{
                    printf("\nErro ao cadastrar!\n\n");
                }
            } 
			else 
			{
                printf("Imovel ou cliente nao encontrado. Certifique-se de que os IDs informados existem.\n");
            }
        }
    }
    
    fclose(arq);
}

void consultar_imovel(void)
{
	imovel pesq;
	int id_imovelpesq;

    printf("Consulta de imovel\n\n");

    printf("Informe a ID do imovel: ");
    scanf("%i", &id_imovelpesq);
    limpar_buffer();

    pesq = pesquisar_imovel(id_imovelpesq);
    
    if (pesq.id_imovel != -1)
	{
		printf("\n\n");
		imprimir_dados_imovel(pesq);
	}
	else
	{
		printf("Imovel nao cadastrado!\n\n");
	}
}

void consultar_cliente(void)
{
	cliente pesq;
	int id_clientepesq;

    printf("Consulta de cliente\n\n");

    printf("Informe a ID do cliente: ");
    scanf("%i", &id_clientepesq);
    limpar_buffer();

    pesq = pesquisar_cliente(id_clientepesq);
    
    if (pesq.id_cliente != -1)
	{
		printf("\n\n");
		imprimir_dados_cliente(pesq);
	}
	else
	{
		printf("Cliente nao cadastrado!\n\n");
	}
}

void consultar_corretor(void)
{
	corretor pesq;
	int id_corretorpesq;

    printf("Consulta de corretor\n\n");

    printf("Informe a ID do corretor: ");
    scanf("%i", &id_corretorpesq);
    limpar_buffer();

    pesq = pesquisar_corretor(id_corretorpesq);
    
    if (pesq.id_corretor != -1)
	{
		printf("\n\n");
		imprimir_dados_corretor(pesq);
	}
	else
	{
		printf("Corretor nao cadastrado!\n\n");
	}
}

void consultar_propostas(void)
{
	proposta pesq;
	int id_propostapesq;

    printf("Consulta de propostas\n\n");

    printf("Informe a ID da proposta: ");
    scanf("%i", &id_propostapesq);
    limpar_buffer();

    pesq = pesquisar_propostas(id_propostapesq);
    
    if (pesq.id_proposta != -1)
	{
		printf("\n\n");
		imprimir_dados_propostas(pesq);
	}
	else
	{
		printf("Proposta nao cadastrada!\n\n");
	}
}

void atualizar_imovel(void)
{
	imovel novo, pesq;
	int id_imovelpesq;
	FILE* arq;

    printf("Alteracao de dados\n\n");

    printf("Informe a ID do imovel: ");
    scanf("%i", &id_imovelpesq);
    limpar_buffer();

	pesq = pesquisar_imovel(id_imovelpesq);
	
	if (pesq.id_imovel != -1)
	{
		printf("\n\n"); 
		imprimir_dados_imovel(pesq);

		printf("\n\nForneca novos dados para serem atualizados\n\n");
		
		novo.id_imovel = pesq.id_imovel;
		
		printf("Endereco: ");
		fgets(novo.endereco, sizeof(novo.endereco), stdin);
        novo.endereco[strcspn(novo.endereco, "\n")] = '\0';
        tirarespaco(novo.endereco);
	    printf("Preco: ");
	    scanf("%f", &novo.preco);
	    limpar_buffer();
	    printf("Quantidade de quartos: ");
		scanf("%i", &novo.qtd_quartos);
		limpar_buffer();
		printf("Quantidade de banheiros: ");
		scanf("%i", &novo.qtd_banheiros);
		limpar_buffer();
		printf("Quantidade de vagas para carros: ");
		scanf("%i", &novo.qtd_vagas_carro);
	    limpar_buffer();

		/* 1. Copia todos os registros EXCETO o antigo para temp.txt */
		removerimoveldoarquivo(pesq.id_imovel);

        /* 2. Abre temp.txt para ADICIONAR o registro atualizado */
        arq = fopen("temp.txt", "a+");
        if (arq == NULL)
        {
            printf("\nErro ao gravar alteracao. Operacao cancelada.\n\n");
        }
        else
        {
    		fprintf(arq,"%s %i %f %i %i %i\n", novo.endereco, novo.id_imovel, novo.preco, novo.qtd_quartos, novo.qtd_banheiros, novo.qtd_vagas_carro);
    		fclose(arq);
    		/* 3. So substitui o original depois de gravar com sucesso */
    		if (substituir_arquivo(ARQ_IMOVEIS))
    			printf("\n\nAlteracao concluida\n\n");
        }
	}
	else
	{
		printf("Imovel nao cadastrado!\n\n");
	}
}

void atualizar_cliente(void)
{
	cliente novo, pesq;
	int id_clientepesq;
	FILE* arq;

    printf("Alteracao de dados\n\n");

    printf("Informe a ID do cliente: ");
    scanf("%i", &id_clientepesq);
    limpar_buffer();

	pesq = pesquisar_cliente(id_clientepesq);
	
	if (pesq.id_cliente != -1)
	{
		printf("\n\n"); 
		imprimir_dados_cliente(pesq);

		printf("\n\nForneca novos dados para serem atualizados\n\n");
		
		novo.id_cliente = pesq.id_cliente;
		
		printf("Nome: ");
		fgets(novo.nome, sizeof(novo.nome), stdin);
        novo.nome[strcspn(novo.nome, "\n")] = '\0';
        tirarespaco(novo.nome);
        
        printf("CPF: ");
        fgets(novo.cpf, sizeof(novo.cpf), stdin);
        novo.cpf[strcspn(novo.cpf, "\n")] = '\0';
        tirarespaco(novo.cpf);
        
        printf("RG: ");
        fgets(novo.rg, sizeof(novo.rg), stdin);
        novo.rg[strcspn(novo.rg, "\n")] = '\0';
        tirarespaco(novo.rg);
		     	
        printf("Data de nascimento (DD/MM/AAAA): ");
        fgets(novo.data, sizeof(novo.data), stdin);
        novo.data[strcspn(novo.data, "\n")] = '\0';
        tirarespaco(novo.data);
			
		printf("Numero do telefone: ");
        fgets(novo.tel, sizeof(novo.tel), stdin);
        novo.tel[strcspn(novo.tel, "\n")] = '\0';
        tirarespaco(novo.tel);
        
		printf("Endereco: ");
		fgets(novo.endereco, sizeof(novo.endereco), stdin);
        novo.endereco[strcspn(novo.endereco, "\n")] = '\0';
        tirarespaco(novo.endereco);

		removerclientedoarquivo(pesq.id_cliente);

        arq = fopen("temp.txt", "a+");
        if (arq == NULL)
        {
            printf("\nErro ao gravar alteracao. Operacao cancelada.\n\n");
        }
        else
        {
    		fprintf(arq,"%s %i %s %s %s %s %s\n", novo.nome, novo.id_cliente, novo.cpf, novo.rg, novo.data, novo.tel, novo.endereco);
    		fclose(arq);
    		if (substituir_arquivo(ARQ_CLIENTES))
    			printf("\n\nAlteracao concluida\n\n");
        }
	}
	else
	{
		printf("Cliente nao cadastrado!\n\n");
	}
}

void atualizar_corretor(void)
{
	corretor novo, pesq;
	int id_corretorpesq;
	FILE* arq;

    printf("Alteracao de dados\n\n");

    printf("Informe a ID do corretor: ");
    scanf("%i", &id_corretorpesq);
    limpar_buffer();

	pesq = pesquisar_corretor(id_corretorpesq);
	
	if (pesq.id_corretor != -1)
	{
		printf("\n\n"); 
		imprimir_dados_corretor(pesq);

		printf("\n\nForneca novos dados para serem atualizados\n\n");
		
		novo.id_corretor = pesq.id_corretor;
		
		printf("Nome: ");
		fgets(novo.nome, sizeof(novo.nome), stdin);
        novo.nome[strcspn(novo.nome, "\n")] = '\0';
        tirarespaco(novo.nome);
        
		printf("CPF: ");
        fgets(novo.cpf, sizeof(novo.cpf), stdin);
        novo.cpf[strcspn(novo.cpf, "\n")] = '\0';
        tirarespaco(novo.cpf);
		     	
        printf("RG: ");
        fgets(novo.rg, sizeof(novo.rg), stdin);
        novo.rg[strcspn(novo.rg, "\n")] = '\0';
        tirarespaco(novo.rg);
		   	
        printf("Data de nascimento (DD/MM/AAAA): ");
        fgets(novo.data, sizeof(novo.data), stdin);
        novo.data[strcspn(novo.data, "\n")] = '\0';
        tirarespaco(novo.data);
			
		printf("Numero do telefone: ");
        fgets(novo.tel, sizeof(novo.tel), stdin);
        novo.tel[strcspn(novo.tel, "\n")] = '\0';
        tirarespaco(novo.tel);
        
		printf("Endereco: ");
		fgets(novo.endereco, sizeof(novo.endereco), stdin);
        novo.endereco[strcspn(novo.endereco, "\n")] = '\0';
        tirarespaco(novo.endereco);
        
        printf("Senha: ");
		fgets(novo.senha, sizeof(novo.senha), stdin);
        novo.senha[strcspn(novo.senha, "\n")] = '\0';
        tirarespaco(novo.senha);

		removercorretordoarquivo(pesq.id_corretor);

        arq = fopen("temp.txt", "a+");
        if (arq == NULL)
        {
            printf("\nErro ao gravar alteracao. Operacao cancelada.\n\n");
        }
        else
        {
    		fprintf(arq,"%s %i %s %s %s %s %s %s\n", novo.nome, novo.id_corretor, novo.cpf, novo.rg, novo.data, novo.tel, novo.endereco, novo.senha);
    		fclose(arq);
    		if (substituir_arquivo(ARQ_CORRETORES))
    			printf("\n\nAlteracao concluida\n\n");
        }
	}
	else
	{
		printf("Corretor nao cadastrado!\n\n");
	}
}

void atualizar_propostas(void)
{
	proposta novo, pesq;
	int id_propostapesq;
	int id_imovel, id_cliente;
	FILE* arq;

    printf("Alteracao de dados\n\n");

    printf("Informe a ID da proposta: ");
    scanf("%i", &id_propostapesq);
    limpar_buffer();

	pesq = pesquisar_propostas(id_propostapesq);
	
	if (pesq.id_proposta != -1)
	{
		printf("\n\n"); 
		imprimir_dados_propostas(pesq);

		printf("\n\nForneca novos dados para serem atualizados\n\n");
		
		novo.id_proposta = pesq.id_proposta;
		
		printf("Digite o ID do imovel para o qual deseja registrar a proposta: ");
    	scanf("%i", &id_imovel);
    	limpar_buffer();
    	
		printf("Digite o ID do cliente que esta fazendo a proposta: ");
    	scanf("%i", &id_cliente);
    	limpar_buffer();

    	printf("Digite o ID do corretor responsavel: ");
    	scanf("%i", &novo.id_corretor);
    	limpar_buffer();
    	
		printf("Digite o valor da proposta: ");
    	scanf("%f", &novo.valor_proposta);
    	limpar_buffer();
    	
    	printf("Digite o valor da entrada: ");
    	scanf("%f", &novo.entrada);
    	limpar_buffer();
    	
    	printf("Digite o valor da parcela: ");
    	scanf("%f", &novo.prestacao);
    	limpar_buffer();
    	
    	/* Valida imovel e cliente ANTES de remover o registro antigo */
    	imovel imv = pesquisar_imovel(id_imovel);
    	cliente cli = pesquisar_cliente(id_cliente);

    	if (imv.id_imovel == -1 || cli.id_cliente == -1)
    	{
    		printf("\nImovel ou cliente nao encontrado. Alteracao cancelada.\n\n");
    	}
    	else
		{
			removerpropostadoarquivo(pesq.id_proposta);
			arq = fopen("temp.txt", "a+");
			if (arq != NULL)
			{
    			fprintf(arq, "%i %i %i %i %.2f %.2f %.2f\n", novo.id_proposta, id_imovel, id_cliente, novo.id_corretor, novo.valor_proposta, novo.entrada, novo.prestacao);
        		fclose(arq);
        		if (substituir_arquivo(ARQ_PROPOSTAS))
        			printf("\n\nAlteracao concluida\n\n");
        	}
        	else
        	{
        		printf("\nErro ao gravar alteracao!\n\n");
        	}
		} 
	}	
}

void remover_imovel(void)
{
	imovel pesq;
	int id_imovelpesq;

    printf("Remover registro do imovel\n\n");

    printf("Informe a ID do imovel: ");
    scanf("%i", &id_imovelpesq);
    limpar_buffer();

	pesq = pesquisar_imovel(id_imovelpesq);
	
	if (pesq.id_imovel != -1)
	{
		printf("\n\n");
		imprimir_dados_imovel(pesq);

		removerimoveldoarquivo(pesq.id_imovel);

        printf("\n\nRegistro excluido!\n\n");

	}
	else
	{
		printf("Imovel nao cadastrado!\n\n");
	}
}

void remover_cliente(void)
{
	cliente pesq;
	int id_clientepesq;

    printf("Remover registro do cliente\n\n");

    printf("Informe a ID do cliente: ");
    scanf("%i", &id_clientepesq);
    limpar_buffer();

	pesq = pesquisar_cliente(id_clientepesq);
	
	if (pesq.id_cliente != -1)
	{
		printf("\n\n");
		imprimir_dados_cliente(pesq);

		removerclientedoarquivo(pesq.id_cliente);

        printf("\n\nRegistro excluido!\n\n");

	}
	else
	{
		printf("Cliente nao cadastrado!\n\n");
	}
}

void remover_corretor(void)
{
	corretor pesq;
	int id_corretorpesq;

    printf("Remover registro do corretor\n\n");

    printf("Informe a ID do corretor: ");
    scanf("%i", &id_corretorpesq);
    limpar_buffer();

	pesq = pesquisar_corretor(id_corretorpesq);
	
	if (pesq.id_corretor != -1)
	{
		printf("\n\n");
		imprimir_dados_corretor(pesq);

		removercorretordoarquivo(pesq.id_corretor);

        printf("\n\nRegistro excluido!\n\n");

	}
	else
	{
		printf("Corretor nao cadastrado!\n\n");
	}
}

void remover_propostas(void)
{
	proposta pesq;
	int id_propostapesq;

    printf("Remover registro de proposta\n\n");

    printf("Informe a ID da proposta de venda: ");
    scanf("%i", &id_propostapesq);
    limpar_buffer();

	pesq = pesquisar_propostas(id_propostapesq);
	
	if (pesq.id_proposta != -1)
	{
		printf("\n\n");
		imprimir_dados_propostas(pesq);

		removerpropostadoarquivo(pesq.id_proposta);

        printf("\n\nRegistro excluido!\n\n");

	}
	else
	{
		printf("Proposta nao cadastrada!\n\n");
	}
}

void listarimoveis(void)
{
	FILE *arq = abrir_leitura(ARQ_IMOVEIS);
	imovel tmp;
	if (arq == NULL) { printf("Nenhum imovel cadastrado.\n"); return; }
	
	
	while (fscanf(arq, "%s %i %f %i %i %i", tmp.endereco, &tmp.id_imovel, &tmp.preco, &tmp.qtd_quartos, &tmp.qtd_banheiros, &tmp.qtd_vagas_carro) == 6)
    {
    	colocarespaco(tmp.endereco);
        printf("Endereco: %s \n ID do imovel: %i \n Preco: %.2f \n Quantidade de quartos: %i \n Quantidade de banheiros: %i \n Quantidade de vagas para carro: %i \n\n", tmp.endereco, tmp.id_imovel, tmp.preco, tmp.qtd_quartos, tmp.qtd_banheiros, tmp.qtd_vagas_carro);
    }
    
    fclose(arq);
    
    printf("\n\n");
}

void listarclientes(void)
{
	FILE *arq = abrir_leitura(ARQ_CLIENTES);
	cliente tmp;
	if (arq == NULL) { printf("Nenhum cliente cadastrado.\n"); return; }
	
	while (fscanf(arq, "%s %i %s %s %s %s %s", tmp.nome, &tmp.id_cliente, tmp.cpf, tmp.rg, tmp.data, tmp.tel, tmp.endereco) == 7)
    {
    	colocarespaco(tmp.nome);
    	colocarespaco(tmp.cpf);
    	colocarespaco(tmp.rg);
    	colocarespaco(tmp.data);
    	colocarespaco(tmp.tel);
    	colocarespaco(tmp.endereco);
        printf("Nome: %s \n ID do Cliente: %i \n CPF: %s \n RG: %s \n Data de nascimento: %s \n Telefone: %s \n Endereco: %s \n\n", tmp.nome, tmp.id_cliente, tmp.cpf, tmp.rg, tmp.data, tmp.tel, tmp.endereco);
    }
    
    fclose(arq);
    
    printf("\n\n");
}

void listarcorretores(void)
{
	FILE *arq = abrir_leitura(ARQ_CORRETORES);
	corretor tmp;
	if (arq == NULL) { printf("Nenhum corretor cadastrado.\n"); return; }
	
	while (fscanf(arq, "%s %i %s %s %s %s %s %s", tmp.nome, &tmp.id_corretor, tmp.cpf, tmp.rg, tmp.data, tmp.tel, tmp.endereco, tmp.senha) == 8)
    {
    	colocarespaco(tmp.nome);
    	colocarespaco(tmp.cpf);
    	colocarespaco(tmp.rg);
    	colocarespaco(tmp.data);
    	colocarespaco(tmp.tel);
    	colocarespaco(tmp.endereco);
        printf("Nome: %s \n ID do Corretor: %i \n CPF: %s \n RG: %s \n Data de nascimento: %s \n Telefone: %s \n Endereco: %s \n\n", tmp.nome, tmp.id_corretor, tmp.cpf, tmp.rg, tmp.data, tmp.tel, tmp.endereco);
    }
    
    fclose(arq);
    
    printf("\n\n");
}

void listarpropostas(void)
{
    FILE *arq = abrir_leitura(ARQ_PROPOSTAS);
    proposta tmp;

    if (arq == NULL)
    {
        printf("Erro ao abrir arquivo de propostas.\n");
        return;
    }

    while (fscanf(arq, "%i %i %i %i %f %f %f",
                  &tmp.id_proposta, &tmp.id_imovel, &tmp.id_cliente, &tmp.id_corretor,
                  &tmp.valor_proposta, &tmp.entrada, &tmp.prestacao) == 7)
    {
        printf("ID da proposta: %i\n ID do imovel: %i\n ID do cliente: %i\n ID do corretor: %i\n Valor da proposta: %.2f\n Valor da entrada: %.2f\n Valor da prestacao: %.2f\n\n",
               tmp.id_proposta, tmp.id_imovel, tmp.id_cliente, tmp.id_corretor,
               tmp.valor_proposta, tmp.entrada, tmp.prestacao);
    }
    
    fclose(arq);
    
    printf("\n\n");
}

void calcular_financiamento()
{
	float taxa_juros, amortizacao, saldo_devedor, prestacao, juros;
	int meses, i;
	float preco_imovel = 0.0f;
	int sistema;

    printf("Escolha o sistema de amortizacao:\n\n");
    printf("1 - SAC\n");
    printf("2 - PRICE\n\n");
    printf("Opcao: ");
    scanf("%i", &sistema);

    switch (sistema)
    {
    	case 1:
    		printf("Informe o valor do imovel: ");
    		scanf("%f", &preco_imovel);

    		printf("Informe a taxa de juros mensal (%%): ");
    		scanf("%f", &taxa_juros);

    		printf("Informe o numero de meses: ");
    		scanf("%i", &meses);

    		if (meses <= 0 || preco_imovel <= 0)
    		{
    			printf("\nValores invalidos. Meses e valor devem ser maiores que zero.\n\n");
    			break;
    		}

    		taxa_juros = taxa_juros / 100;
    		amortizacao = preco_imovel / meses;
    		saldo_devedor = preco_imovel;

    		printf("Mes \t| Prestacao \t| Juros \t| Amortizacao \t| Saldo Devedor\n");

    		for (i = 1; i <= meses; i++)
			{
        		juros = saldo_devedor * taxa_juros;
        		prestacao = juros + amortizacao;
        		saldo_devedor -= amortizacao;
        		printf("%2d   \t| %.2f     \t| %.2f  \t| %.2f        \t| %.2f\n", i, prestacao, juros, amortizacao, saldo_devedor);
			}
			break;
		case 2:
			printf("Informe o valor do imovel: ");
    		scanf("%f", &preco_imovel);

    		printf("Informe a taxa de juros mensal (%%): ");
    		scanf("%f", &taxa_juros);

    		printf("Informe o numero de meses: ");
    		scanf("%i", &meses);

    		if (meses <= 0 || preco_imovel <= 0)
    		{
    			printf("\nValores invalidos. Meses e valor devem ser maiores que zero.\n\n");
    			break;
    		}

			taxa_juros = taxa_juros / 100;

    		if (taxa_juros == 0)
    		{
    			/* Com juros zero, PRICE se reduz a divisao simples */
    			prestacao = preco_imovel / meses;
    		}
    		else
    		{
    			prestacao = (preco_imovel * taxa_juros) / (1 - pow(1 + taxa_juros, -meses));
    		}
    		saldo_devedor = preco_imovel;
    		printf("Valor da prestacao: %.2f\n", prestacao);

    		printf("Mes \t| Prestacao \t| Juros \t| Amortizacao \t| Saldo Devedor\n");

    		for (i = 1; i <= meses; i++)
			{
        		juros = saldo_devedor * taxa_juros;
        		amortizacao = prestacao - juros;
        		saldo_devedor -= amortizacao;
        		printf("%2d   \t| %.2f     \t| %.2f  \t| %.2f        \t| %.2f\n", i, prestacao, juros, amortizacao, saldo_devedor);
    		}
		break;
		default:
			printf("Opcao invalida.\n");
    }
}

int fazer_login()
{
    int id_corretor;
    char senha[12];
    char ch;
    int i = 0;

    printf("Login\n");

    printf("Digite a ID do corretor: ");
    scanf("%i", &id_corretor);
    limpar_buffer();

    printf("Digite a senha: ");
    
    while (1)
    {
        ch = getch();

        if (ch == '\r' || ch == '\n') 
        {
            senha[i] = '\0';
            break;
        }
        else if ((ch == 8 || ch == 127) && i > 0)
        {
            i--;
            printf("\b \b");
        }
        else if (i < sizeof(senha) - 1)
        {
            senha[i++] = ch;
            printf("*");
        }
    }
    
    corretor pesq = pesquisar_corretor(id_corretor);

    if (pesq.id_corretor != -1 && strcmp(pesq.senha, senha) == 0)
	{
        printf("\nLogin realizado com sucesso!\n\n");
        return 1;
    } 
	else
	{
        printf("\nAcesso negado! Verifique suas credenciais.\n");
        
        return 0;
    }

}

void menucadastro()
{
	int escolha_cadastro;
	
	do 
	{
        printf("Menu Cadastro\n\n");
        printf("1 - Cadastrar Imovel\n");
        printf("2 - Cadastrar Cliente\n");
        printf("3 - Cadastrar Corretor\n");
        printf("4 - Cadastrar Proposta\n");
        printf("5 - Voltar\n\n");
        printf("Escolha a opcao: ");
        scanf("%i", &escolha_cadastro);
        
        system_cls();

        switch (escolha_cadastro) 
		{
            case 1:
                cadastrar_imovel();
                break;
            case 2:
                cadastrar_cliente();
                break;
            case 3:
                cadastrar_corretor();
                break;
            case 4:
                cadastrar_proposta();
                break;
            case 5:
                printf("Voltando ao menu principal.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } 
    
	while (escolha_cadastro != 5);
}

void menuconsulta()
{
	int escolha_consulta;
	
	do 
	{
        printf("Menu Consulta\n\n");
        printf("1 - Consultar Imovel\n");
        printf("2 - Consultar Cliente\n");
        printf("3 - Consultar Corretor\n");
        printf("4 - Consultar Proposta\n");
        printf("5 - Voltar\n\n");
        printf("Escolha a opcao: ");
        scanf("%i", &escolha_consulta);
        
        system_cls();

        switch (escolha_consulta) 
		{
            case 1:
                consultar_imovel();
                break;
            case 2:
                consultar_cliente();
                break;
            case 3:
                consultar_corretor();
                break;
            case 4:
                consultar_propostas();
                break;
            case 5:
                printf("Voltando ao menu principal.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } 
    
	while (escolha_consulta != 5);
}

void menuatualizacao()
{
	int escolha_atualiza;
	
	do 
	{
        printf("Menu Atualizacao\n\n");
        printf("1 - Atualizar Imovel\n");
        printf("2 - Atualizar Cliente\n");
        printf("3 - Atualizar Corretor\n");
        printf("4 - Atualizar Proposta\n");
        printf("5 - Voltar\n\n");
        printf("Escolha a opcao: ");
        scanf("%i", &escolha_atualiza);
        
        system_cls();

        switch (escolha_atualiza) 
		{
            case 1:
                atualizar_imovel();
                break;
            case 2:
                atualizar_cliente();
                break;
            case 3:
                atualizar_corretor();
                break;
            case 4:
                atualizar_propostas();
                break;
            case 5:
                printf("Voltando ao menu principal.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } 
    
	while (escolha_atualiza != 5);
}

void menuremover()
{
	int escolha_remove;
	
	do 
	{
        printf("Menu Remover\n\n");
        printf("1 - Remover Imovel\n");
        printf("2 - Remover Cliente\n");
        printf("3 - Remover Corretor\n");
        printf("4 - Remover Proposta\n");
        printf("5 - Voltar\n\n");
        printf("Escolha a opcao: ");
        scanf("%i", &escolha_remove);
        
        system_cls();

        switch (escolha_remove) 
		{
            case 1:
                remover_imovel();
                break;
            case 2:
                remover_cliente();
                break;
            case 3:
                remover_corretor();
                break;
            case 4:
                remover_propostas();
                break;
            case 5:
                printf("Voltando ao menu principal.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } 
    
	while (escolha_remove != 5);
}

void menulistar()
{
	int escolha_lista;
	
	do 
	{
        printf("Menu Listar\n\n");
        printf("1 - Listar Imoveis\n");
        printf("2 - Listar Clientes\n");
        printf("3 - Listar Corretores\n");
        printf("4 - Listar Propostas\n");
        printf("5 - Voltar\n\n");
        printf("Escolha a opcao: ");
        scanf("%i", &escolha_lista);
        
        system_cls();

        switch (escolha_lista) 
		{
            case 1:
                listarimoveis();
                break;
            case 2:
                listarclientes();
                break;
            case 3:
                listarcorretores();
                break;
            case 4:
                listarpropostas();
                break;
            case 5:
                printf("Voltando ao menu principal.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } 
    
	while (escolha_lista != 5);
}

int main() {
	
#ifdef _WIN32
    SetConsoleOutputCP(1252);
    SetConsoleCP(1252);
#endif
    setlocale(LC_ALL, "Portuguese");

	int opcao = 0;
    int escolha = 0;
    
    do {
        printf("\nSistema de Imobiliaria - Login e Cadastro\n");
        printf("1 - Cadastrar\n");
        printf("2 - Login\n");
        printf("3 - Sair\n");

        printf("Escolha a opcao: ");
        scanf("%d", &opcao);
        
        system_cls();

        switch (opcao) {
            case 1:
                cadastrar_corretor();
                break;
            case 2:
                if (fazer_login()) {
                    printf("\nBem-vindo ao sistema!\n\n");
                    do {
        				printf("Sistema de Imobiliaria\n\n");
        				printf("1 - Cadastro\n");
        				printf("2 - Consulta de informacoes\n");
    					printf("3 - Atualizacao de informacoes\n");
    					printf("4 - Deletar informacoes\n");
    					printf("5 - Listar\n");
    					printf("6 - Calcular Financiamento\n");
    					printf("7 - Sair\n\n");
    					printf("Escolha a opcao: ");
        				scanf("%i", &escolha);
        
        				system_cls();

        				switch (escolha) {
            				case 1:
                				menucadastro();
                				break;
            				case 2:
                				menuconsulta();
                				break;
            				case 3:
                				menuatualizacao();
                				break;
            				case 4:
                				menuremover();
                				break;
            				case 5:
            					menulistar();
            					break;
            				case 6:
            					calcular_financiamento();
            					break;
            				case 7:
            					printf("Saindo do sistema.\n");
            					break;
            					default:
                				printf("Opcao invalida. Tente novamente.\n");
        				}
    				} while (escolha != 7);
                }
                break;
            case 3:
                printf("Saindo do sistema.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 3);
    
    return 0;
}
