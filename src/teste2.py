"""
- No diretorio deste ficheiro, ter a pasta meta1, com os casos de teste, e gocompiler.l

Corre todos os ficheiros .dgo, compara com o devido output esperado
e coloca os resultados na pasta outputs com o devido nome.

Imprime os testes em que falhou. Os outputs(diff entre os ficheiros) sao guardados em meta1/outputs

"""

import os

test_files = []
failed = 0

# Create outputs dir
if not os.path.exists("meta2/outputs"):
    print("Created path meta2/outputs")
    os.system("mkdir meta2/outputs")

#load all files in meta1 folder
for test in os.listdir('./meta2'):
    if '.dgo' in test:
        test_files.append(test[:-4])

#check if makefile exists and use it
if os.path.isfile("makefile"):
    print('Using makefile')
    os.system(f'make')
else: # compile without makefile
    print('No makefile found. Compiling with flex gocompiler.l && clang-7 -o gocompiler lex.yy.c')
    os.system('flex gocompiler.l')
    os.system('yacc gocompiler.y -d')
    os.system('clang-7 -o gocompiler y.tab.c lex.yy.c functions.c print_ast.c')

#run tests
for test in test_files:
    os.system(f'./gocompiler < meta2/{test}.dgo | diff meta2/{test}.out - > meta2/outputs/{test}.txt')
    
    #output result
    if os.stat(f"meta2/outputs/{test}.txt").st_size != 0:
        failed += 1
        print(f"{test}.dgo -> Failed")
    
if failed == 0:
    print("No Fails!")
