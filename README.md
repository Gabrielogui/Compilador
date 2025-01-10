# COMPILADOR - LINGUAGEM PROC

Este projeto é um compilador para a linguagem de programação **Proc**, com todas as suas especificações, desenvolvido em **C** utilizando o ambiente de desenvolvimento **CodeBlocks**. Ele realiza a análise léxica, 
sintática e semântica, além de gerar o código intermediário (Máquina de pilha) para programas escritos na linguagem **Proc**.
Esse projeto universitário foi parte da disciplina Fundamentos de Compiladores. A linguagem Proc foi idealizada pelo doscente da disciplina.

## 📂 ESTRUTURA DO PROJETO

O projeto está organizado da seguinte forma:

- **AFD - proc/**: Pasta que onde está localizada a imagem e o arquivo do Autômato Finito Determinístico(AFD) referente a linguagem. Importante e fundamental para a etapa de análise léxica.
- **compilador/**: Pasta onde está o código fonte até a etapa da análise sintática.
- **compiladorTeste/**: Pasta onde está o código fonte completo do compilador, compreendendo todas as etpas: léxica, sintática, semântica e geração do código intermediário.
- **especificacao - proc/**: Pasta onde está localizada a especificação da linguagem proc.

## ⚙️ FUNCIONALIDADES

O compilador da linguagem **Proc** inclui:

- **Análise Léxica**: Identificação dos tokens válidos na linguagem.
- **Análise Sintática**: Verificação da estrutura gramatical do programa.
- **Análise Semântica**: Garantia de consistência e coerência dos tipos e símbolos.
- **Geração de Código Intermediário**: Produção de uma representação intermediária do programa - Máquina de Pilha - que pode ser usada para posterior tradução ou execução.

## 🛠️ REQUISITOS

- **Compilador GCC**: Integrado ao CodeBlocks para compilar o código C.
- **Sistema Operacional**: Windows, Linux ou macOS.

> [!NOTE]
> O projeto foi desenvolvido utilizando a IDE Code::Blocks 20.03.

## 🚀 CONFIGURAR E USAR

1. Clone o repositório
git clone https://github.com/Gabrielogui/Compilador

2. Abra o arquivo 'compiladorTeste'

3. Digite um código em proc no arquivo 'codigo.txt' (Pode utilizar os exemplos, que estão em 'especificacao - proc/exemplos_codigos_proc.txt')

4. Compile o projeto com o gcc

5. O código intermediário da máquina de pilha será gerado e pode ser encontrado no arquivo 'maquina_de_pilha.txt'

Utilizando o Code::Blocks:

2. Abra o projeto 'compiladorTeste.cbp'

3. Digite um código em proc no arquivo 'codigo.txt' (Pode utilizar os exemplos, que estão em 'especificacao - proc/exemplos_codigos_proc.txt')

4. Compile o projeto

5. O código intermediário da máquina de pilha será gerado e pode ser encontrado no arquivo 'maquina_de_pilha.txt'

Caso dê conflito com pastas, faça dessa forma:

2. Crie um projeto console (console application)

3. Importe os arquivos: main.c ; analex.c ; analex.h ; anasint.c ; anasint.h ; codigo.txt

4. Digite um código em proc no arquivo 'codigo.txt' (Pode utilizar os exemplos, que estão em 'especificacao - proc/exemplos_codigos_proc.txt')

5. Compile o projeto

6. O código intermediário da máquina de pilha será gerado e pode ser encontrado no arquivo 'maquina_de_pilha.txt'

## 📖 ESPECIFICAÇÃO DA LÍNGUAGEM

A especificação completa da linguagem está em um arquivo na pasta **especificacao - proc**
Consulte o documento para entender os detalhes sobre:

- Regras léxicas
- Regras sintáticas
- Gramática
- Estruturas de controles e tipos de dados
- Regras semânticas

## 🧑‍💻 COLABORADORES

- Gabriel Rodrigues:
    - Github: https://github.com/Gabrielogui
    - Linkedin: https://www.linkedin.com/in/gabriel-rodrigues-a551012a2

- Luiz Vinícius:
    - Github: https://github.com/luizvn
    - Linkedin: https://www.linkedin.com/in/luizvn

## 📈 MELHORIAS FUTURAS:

- Implementação de um gerador de código para máquina virtual.
- Otimização do código intermediário gerado (Máquina de Pilha).