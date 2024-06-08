// ################################################################
// #           Mario L�pez Llamas                                 #
// #           04/04/2024                                         #
// #           Desarrollo de aplicaciones multiplataforma         #
// #           Emprende CampusFP Humanes	          	          #
// ################################################################

#ifndef MAIN_H
#define MAIN_H

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>


#include "shader.h"
#include "camera.h"
#include "process.h"
#include "chunkmanager.h"
#include "Menu.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main();
#endif