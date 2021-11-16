"""
**Importante**: Usar makefile, pois simplifica os argumentos do programa

- No diretorio deste ficheiro, ter a pasta meta<num_meta>, com os casos de teste e os ficheiros necessários:
 - makefile
 - gocompiler.l
 - gocompiler.y
 - outros ficheiros necessários

Corre todos os ficheiros .dgo, compara com o devido output esperado
e coloca os resultados na pasta outputs com o devido nome.

Imprime os testes em que falhou. Os outputs(diff entre os ficheiros) sao guardados em meta<num_meta>/outputs

"""

import os
import sys


def run_tests():
    try:
        if (sys.argv[1] == '1'):
            print("---Meta 1---")
            meta = 1
        elif (sys.argv[1] == '2'):
            print("---Meta 2---")
            meta = 2
        elif (sys.argv[1] == '3'):
            print("---Meta 3---")
            meta = 3
        elif (sys.argv[1] == '4'):
            print("---Meta 4---")
            meta = 4
        else:
            print('Invalid Argument. Usage: test.py <num_meta>')
            return
    except:
        print('No Argument was passed. Usage: test.py <num_meta>')
        return
    
    test_files = []
    failed = 0
    passed = []
    flags = ['', '-l', '-t'] # used in flags[meta] to select flag

    # Create outputs dir
    if not os.path.exists(f"meta{meta}/outputs"):
        print(f"Created path meta{meta}/outputs")
        os.system(f"mkdir meta{meta}/outputs")

    #load all files in meta[num_meta] folder
    for test in os.listdir(f'./meta{meta}'):
        if '.dgo' in test:
            test_files.append(test[:-4])
                
    #check if makefile exists and use it
    if os.path.isfile("makefile"):
        print('Using makefile')
        os.system(f'make')
        
    else: # Don't compile if no makefile is found
        print('No makefile found. Please use a makefile')
        return

    #run tests
    for test in test_files:
        os.system(f"./gocompiler {flags[meta]} < meta{meta}/{test}.dgo | diff meta{meta}/{test}.out - > meta{meta}/outputs/{test}.txt")
        
        #output result
        if os.stat(f"meta{meta}/outputs/{test}.txt").st_size != 0:
            failed += 1
            print(f"{test}.dgo -> Failed")
        else:
            passed.append(f"{test}.dgo")
        
    if failed == 0:
        print("No Fails!")
    else:
        # only print tests that passed if at least 1 fails
        for i in passed:
            print(f"{i} -> Passed")


if __name__ == '__main__':
    run_tests()
