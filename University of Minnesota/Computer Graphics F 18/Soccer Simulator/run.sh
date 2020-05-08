#!/bin/bash

file="soccer"


g++ $file.cpp glad/glad.c -F/Library/Frameworks -framework SDL2 -framework OpenGL vector.cpp point.cpp -o $file
