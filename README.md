# Projeto Compiladores
## FCTUC - DEI - 2021/2022

### Repositório para o projeto da cadeira de Compiladores

Compilar com o comando

```
make && ./gocompiler [-l | -t | -s] < {nome_ficheiro}.dgo > {nome_ficheiro}.out
```

Usar o seguinte comando para comparar o output do compilador com um ficheiro
```
make && ./gocompiler [-l | -t | -s] < {nome_ficheiro}.dgo | diff {nome_ficheiro}.out - 

```

 - -l -> 1ª meta
 - -t -> 2ª meta
 - -s -> 3ª meta

### TODO

- [x] Meta 1 - Tokens
    - [x] Tokens
        - [x] Reserved words
        - [x] Literals (decimal, hexadecimal, octal)
        - [x] Emitir SEMICOLON se estiver em falta, em situações específicas
    - [x] Erros
        - [x] linha e coluna do erro
        - [x] motivo do erro

- [x] Meta 2 - Abstract Syntax Tree
    - [x] Gramática
    - [x] Erros
    - [x] AST 

- [ ] Meta 3 - Semantic Analysis
    - [x] Tabela de símbolos
        - [x] Global
        - [x] Funções
    - [ ] Anotações na AST
    - [ ] Erros Semânticos

- [ ] Meta 4 - Code Generation

