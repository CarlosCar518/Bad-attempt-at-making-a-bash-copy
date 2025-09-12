#!/bin/bash

gcc -Wall -Wextra ./src/*.c -o main -I"C:/msys64/mingw64/include" -I."/include" -L"C:/msys64/mingw64/lib" -lcurl -g