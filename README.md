# Projeto Compiladores
## FCTUC - DEI - 2021/2022

### Repositório para o projeto da cadeira de Compiladores

Compilar com o comando
```
make && ./gocompiler -l < {nome_ficheiro}.dgo > {nome_ficheiro}.out
```
Usar o seguinte comando para comparar o output do compilador com um ficheiro
```
make && ./gocompiler -l < {nome_ficheiro}.dgo | diff {nome_ficheiro}.out - 

```

### TODO

[ ] Meta 1
- [ ] Tokens
    - [x] Reserved words
    - [x] Literals (decimal, hexadecimal, octal)
    - [ ] Emitir SEMICOLON se estiver em falta, em situações específicas
- [ ] Erros
    - [ ] linha e coluna do erro
    - [ ] motivo do erro

[ ] Meta 2

[ ] Meta 3

[ ] Meta 4

