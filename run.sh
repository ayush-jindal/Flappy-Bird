#!/bin/bash
g++ -c main.cpp headers/Window.cpp headers/shaders.cpp headers/LightShaders.cpp headers/Bird.cpp
g++ -o swap *.o -lGL -lGLEW -lglfw
./swap
rm *.o
