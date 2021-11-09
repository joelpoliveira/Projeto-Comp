# Projeto Compiladores
## FCTUC - DEI - 2021/2022

### Repositório para o projeto da cadeira de Compiladores

Compilar com o comando
```
make && ./gocompiler [-l | -t] < {nome_ficheiro}.dgo > {nome_ficheiro}.out
```
Usar o seguinte comando para comparar o output do compilador com um ficheiro
```
make && ./gocompiler [-l | -t] < {nome_ficheiro}.dgo | diff {nome_ficheiro}.out - 

```

 - -l -> 1ª meta
 - -t -> 2ª meta

### TODO

- [x] Meta 1
    - [x] Tokens
        - [x] Reserved words
        - [x] Literals (decimal, hexadecimal, octal)
        - [x] Emitir SEMICOLON se estiver em falta, em situações específicas
    - [x] Erros
        - [x] linha e coluna do erro
        - [x] motivo do erro

- [ ] Meta 2
    - [x] Gramática
    - [ ] Erros
    - [ ] AST 

- [ ] Meta 3

- [ ] Meta 4

