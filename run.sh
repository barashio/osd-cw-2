#!/bin/bash
#gcc -o main main.c
#gcc -I/usr/include/python3.9/ py.c -L/usr/python3.9/config-3.9-x86_64-linux-gnu -lpython3.9 -o py
gcc -I/usr/include/python3.9/ main.c -L/usr/python3.9/config-3.9-x86_64-linux-gnu -lpython3.9 -o main
