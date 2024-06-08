#ifndef CHUNK_H
#define CHUNK_H

// numero de Voxels dentro de un chunk
// CX significa Coordenada X, CY Coordenada Y, CZ Coordenada Z
#define CX 16
#define CY 16
#define CZ 16

// Numero de chunks rederizados al rededor del jugador en cada eje
#define SCX 48
#define SCY 4
#define SCZ 48

#include <glad/glad.h> 
#include <glm/gtc/type_ptr.hpp>
#include "noiseutils.h"
#include <iostream>
#include <sstream>
#include <string>

// indices de cada voxel / Cubo
struct CubeVertex {
    uint8_t x, y, z, w, nx, ny, nz;
    CubeVertex() {}
    
    // Constructor de la estructura
    // x, y, z, w: Coordenadas del voxel, w es el tipo de voxel
    // nx, ny, nz: Coordenadas de la normal del voxel
    // A partir de los : se inicializan los valores de las variables de la estructura

    CubeVertex(uint8_t x, uint8_t y, uint8_t z, uint8_t w, int8_t nx, int8_t ny, int8_t nz) : x(x), y(y), z(z), w(w), nx(nx), ny(ny), nz(nz) {}
};

// Lista con los tipos de Voxel
enum BlockType: uint8_t {
    BlockType_Empty = 0,
    Voxel_Nieve,
    Voxel_Piedra,
    Voxel_Tierra,
    Voxel_Pradera,
    Voxel_Arena,
    Voxel_Agua,
};

class Chunk {
private:
    // uint8_t = 1 byte | tipo de numero integral con longitud de 8 bits
    // lo usamos para guardar los Voxels del chunk
    uint8_t Chunk_voxels[CX][CY][CZ];
    // VAO y VBO para renderizar el chunk en OpenGL con shaders
    unsigned int VBO, VAO;
    // Numero de elementos a renderizar
    int elements;
    // Booleano para saber si el chunk ha cambiado
    bool changed;
public:
    Chunk *neighXN, *neighXP, *neighYN, *neighYP, *neighZN, *neighZP;

/*
    neighXN: Vecino en la dirección negativa del eje X
    neighXP: Vecino en la dirección positiva del eje X
    neighYN : Vecino en la dirección negativa del eje Y
    neighYP : Vecino en la dirección positiva del eje Y
    neighZN : Vecino en la dirección negativa del eje Z
    neighZP : Vecino en la dirección positiva del eje Z
*/

    // Posicion global de este chunk
    int posX, posY, posZ;
    Chunk(int i, int j, int k, utils::NoiseMap heightMap);
    ~Chunk();
    uint8_t GetNeighbourBlock(Chunk* neighbour, int x, int y, int z);
    void Update();
    void Render();
    void setNeighbours(Chunk* neighXN, Chunk* neighXP, Chunk* neighYN, Chunk* neighYP, Chunk* neighZN, Chunk* neighZP);
};
#endif