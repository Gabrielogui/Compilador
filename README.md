# COMPILADOR - LINGUAGEM PROC

Este projeto é um compilador para a linguagem de programação **Proc**, com todas as suas especificações, desenvolvido em **C** utilizando o ambiente de desenvolvimento **CodeBlocks**. Ele realiza a análise léxica, 
sintática e semântica, além de gerar o código intermediário para programas escritos na linguagem **Proc**.
Esse projeto universitário foi parte da disciplina de Compiladores. A linguagem Proc foi idealizada pelo doscente da disciplina.

## ESTRUTURA DO PROJETO

O projeto está organizado da seguinte forma:

- **AFD - proc/**: Pasta que onde está localizada a imagem e o arquivo do Autômato Finito Determinístico(AFD) referente a linguagem. Importante e fundamental para a etapa de análise léxica.
- **compilador/**: Pasta onde está o código fonte até a etapa da análise sintática.
- **compiladorTeste/**: Pasta onde está o código fonte completo do compilador, compreendendo todas as etpas: léxica, sintática, semântica e geração do código intermediário.
- **especificacao - proc/**: Pasta onde está localizada a especificação da linguagem proc.

## ⚙️ Funcionalidades

O compilador da linguagem **Proc** inclui:

- **Análise Léxica**: Identificação dos tokens válidos na linguagem.
- **Análise Sintática**: Verificação da estrutura gramatical do programa.
- **Análise Semântica**: Garantia de consistência e coerência dos tipos e símbolos.
- **Geração de Código Intermediário**: Produção de uma representação intermediária do programa - Máquina de Pilha - que pode ser usada para posterior tradução ou execução.
