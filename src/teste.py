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
if not os.path.exists("meta1/outputs"):
    print("Created path meta1/outputs")
    os.system("mkdir meta1/outputs")

#load all files in meta1 folder
for test in os.listdir('./meta1'):
    if '.dgo' in test:
        test_files.append(test[:-4])

#check if makefile exists and use it
if os.path.isfile("makefile"):
    print('Using makefile')
    os.system(f'make')
else: # compile without makefile
    print('No makefile found. Compiling with flex gocompiler.l && clang -o gocompiler lex.yy.c')
    os.system('flex gocompiler.l')
    os.system('clang -o gocompiler lex.yy.c')

#run tests
for test in test_files:
    os.system(f'./gocompiler -l < meta1/{test}.dgo | diff meta1/{test}.out - > meta1/outputs/{test}.txt')
    
    #output result
    if os.stat(f"meta1/outputs/{test}.txt").st_size != 0:
        failed += 1
        print(f"{test}.dgo -> Failed")
    
if failed == 0:
    print("No Fails!")
