# Sistema de Imobiliária / Real Estate Management System

> Projeto acadêmico desenvolvido na **Universidade Paulista (UNIP)** — Curso de Análise e Desenvolvimento de Sistemas, 2023.
>
> Academic project developed at **Universidade Paulista (UNIP)** — Systems Analysis and Development, 2023.

---

## 🇧🇷 Português

### Descrição

Sistema de gerenciamento de imobiliária desenvolvido em linguagem C. Permite cadastrar e gerenciar imóveis, clientes, corretores e propostas de compra, além de calcular financiamentos pelos sistemas SAC e PRICE.

### Funcionalidades

- Cadastro, consulta, atualização e remoção de:
  - Imóveis
  - Clientes
  - Corretores
  - Propostas de compra
- Login com senha para acesso ao sistema
- Cálculo de financiamento (SAC e PRICE) com tabela de amortização
- Persistência de dados em arquivos `.txt`

### Como compilar

**Linux / macOS**
```bash
gcc -o sistema Sistema_Imobiliaria.c -lm
./sistema
```

**Windows (MinGW)**
```bash
gcc -o sistema.exe Sistema_Imobiliaria.c -lm
sistema.exe
```

### Como usar

1. Na tela inicial, escolha **1 - Cadastrar** para criar o primeiro corretor
2. Faça login com **2 - Login** usando o ID e a senha cadastrados
3. No menu principal, acesse as opções de cadastro, consulta, atualização, remoção, listagem e financiamento

### Arquivos de dados

O sistema cria automaticamente os seguintes arquivos na pasta do executável:

| Arquivo | Conteúdo |
|---|---|
| `imoveis.txt` | Imóveis cadastrados |
| `clientes.txt` | Clientes cadastrados |
| `corretores.txt` | Corretores cadastrados |
| `propostas.txt` | Propostas de compra |

### Autor

**William Shizuo Figueiredo Hattori**  
Análise e Desenvolvimento de Sistemas — UNIP, 2023

---

## 🇺🇸 English

### Description

A real estate management system written in C. It allows registering and managing properties, clients, real estate agents and purchase proposals, as well as calculating loan amortization using the SAC and PRICE methods.

### Features

- Create, read, update and delete:
  - Properties
  - Clients
  - Real estate agents
  - Purchase proposals
- Password-protected login
- Loan amortization calculator (SAC and PRICE) with full payment schedule
- Data persistence using `.txt` files

### How to build

**Linux / macOS**
```bash
gcc -o sistema Sistema_Imobiliaria.c -lm
./sistema
```

**Windows (MinGW)**
```bash
gcc -o sistema.exe Sistema_Imobiliaria.c -lm
sistema.exe
```

### How to use

1. On the start screen, choose **1 - Cadastrar** to register the first agent
2. Log in with **2 - Login** using the agent's ID and password
3. Use the main menu to access all features

### Data files

The system automatically creates the following files in the executable's folder:

| File | Content |
|---|---|
| `imoveis.txt` | Registered properties |
| `clientes.txt` | Registered clients |
| `corretores.txt` | Registered agents |
| `propostas.txt` | Purchase proposals |

### Author

**William Shizuo Figueiredo Hattori**  
Systems Analysis and Development — UNIP, 2023
