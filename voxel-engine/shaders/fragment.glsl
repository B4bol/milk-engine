// Vertex Shader
// El vertex shader se encarga de transformar las coordenadas de los vértices de los objetos en coordenadas de la pantalla.
// También se encarga de pasar los datos necesarios al fragment shader

// Este vertex shader lo ha desarrollado "OGL-Tutorials"
// en la pagina web tiene varios ejemplos de VAO y VBO

#version 330 core // versión de OpenGL

// Este struct es para pasar los datos de la luz al fragment shader
struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
};

out vec4 FragColor; // Color del fragmento

in vec3 Color; // Color del objeto
in vec3 Normal; // Normal del objeto, un normal es un vector perpendicular a la superficie del objeto

uniform vec3 viewPos; // Posición de la cámara
uniform Light light; // Luz


// Datos de entrada del vertex shader (posición y textura)
void main() {
    // Ambiente
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light.ambient;
  	
    // Diffusión 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse;
    
    // Resultado final del color
    vec3 result = (ambient + diffuse) * Color;
    FragColor = vec4(result, 1.0);
} 