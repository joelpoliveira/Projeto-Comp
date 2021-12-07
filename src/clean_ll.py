import re
import sys


"""
clang -S -emit-llvm teste.c -o /dev/stdout | python3 clean_ll.py > teste.ll
"""

data = [line for line in sys.stdin]

data = data[5:]

for line in data:
    
    if line[0] == '!':
        line = ""
        
    if "attributes #0 = {" in line:
        line = ""
    
    if " dso_local " in line:
        line = line.replace(" dso_local ", " ");
    
    if ", align " in line:
        line = re.split(", align [0-9]", line)[0]
        line += '\n'
        print(line, end = "")
    else:
        print(line, end = "")


