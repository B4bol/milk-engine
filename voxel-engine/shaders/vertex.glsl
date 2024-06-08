#version 330 core // version 3.3 of OpenGL

// input del vertex shader, differente para cada ejecución de este shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in uint aType;
layout (location = 2) in vec3 aNormal;

// output del vertex shader, se pasa al fragment shader
out vec3 Color;
out vec3 Normal;

// Esto son uniformes, sirven para pasar información desde la CPU al shader
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Tipos de Voxels
const uint Voxel_Nieve = uint(1);
const uint Voxel_Piedra = uint(2);
const uint Voxel_Tierra = uint(3);
const uint Voxel_Pradera = uint(4);
const uint Voxel_Arena = uint(5);
const uint Voxel_Agua = uint(6);

void main()
{
	// Aqui asignamos el color de los cubos, dependiendo del tipo seran uno u otro
	// se establecen con vectores, la division separa el maximo valor de color (255) entre el valor que queremos
	if (aType == Voxel_Nieve)
	    Color = vec3(255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f);
	else if (aType == Voxel_Piedra)
	    Color = vec3(142.0f/255.0f, 142.0f/255.0f, 142.0f/255.0f);
	else if (aType == Voxel_Tierra)
	    Color = vec3(113.0f/255.0f, 79.0f/255.0f, 54.0f/255.0f);
	else if (aType == Voxel_Pradera)
	    Color = vec3(11.0f/255.0f, 128.0f/255.0f, 0.0f/255.0f);
	else if (aType == Voxel_Arena)
	    Color = vec3(240.0f/255.0f, 240.0f/255.0f, 64.0f/255.0f);
	else
	    Color = vec3(0.0f/255.0f, 0.0f/255.0f, 255.0f/255.0f);

	// se calcula la posición final del fragmento para pasarla al fragment shader
	vec3 FragPos = vec3(model * vec4(aPos.xyz, 1.0));
    Normal = aNormal;  
    
	// se multiplica por la matriz de proyección, vista y modelo para obtener la posición final
    gl_Position = projection * view * vec4(FragPos, 1.0);
}