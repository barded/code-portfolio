#!/bin/bash

file="maze"


g++ $file.cpp glad/glad.c -F/Library/Frameworks -framework SDL2 -framework OpenGL -o $file
