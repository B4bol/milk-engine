#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

// Incluyendo las librerías necesarias para el manejo de chunks y generación de terreno con ruido de Perlin
#include "chunk.h"
#include "main.h"
#include <math.h>
#include <memory>
#include <unordered_map>
#include "glm/gtx/hash.hpp"
#include <noise/noise.h>
#include <random>
#include <limits>

using namespace noise;

// Definimos un alias para el hashmap de chunks que se encuentran en la pantalla
// La clave es un vector de 3 dimensiones y el valor es un puntero único a un chunk
using ChunkMap = std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>>;

class ChunkManager {
private:
	// Variables para el manejo de chunks y generación de terreno
	glm::vec3 lastPlayerPos;
	module::Perlin myModule; // Módulo de ruido de Perlin
	// Mapa de altura
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	// Generador de números aleatorios
	std::random_device rd;
public:
	// Hashmap para guardar los chunks visibles 
	ChunkMap chunks;
	ChunkManager(); // Constructor
	~ChunkManager(); // Destructor
	void Update();
	void Render(Shader shader, glm::mat4 &vp); // Renderizar los chunks visibles en la pantalla
};
#endif