#ifndef PROCESS_H
#define PROCESS_H

#include "main.h"


// Heaader para declaracion de las funciones encargadas de procesar el input del usuario

GLFWwindow* setupWindow(int WIDTH, int HEIGHT, const char* TITLE);

void processInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
#endif 