"""
Corre todos os ficheiros .dgo e compara com o devido output esperado
e coloca os resultados na pasta outputs com o devido nome

"""

import os

test_files = []
failed = 0

if not os.path.exists("meta1/outputs"):
    print("Created path")
    os.system("mkdir meta1/outputs")


for test in os.listdir('./meta1'):
    if '.dgo' in test:
        test_files.append(test[:-4])


for test in test_files:
    os.system(f'make && ./gocompiler -l < meta1/{test}.dgo | diff meta1/{test}.out - > meta1/outputs/{test}.txt')
    
    if os.stat(f"meta1/outputs/{test}.txt").st_size != 0:
        failed += 1
        print(f"{test}.dgo -> Failed")
    

if failed == 0:
    print("No Fails!")
