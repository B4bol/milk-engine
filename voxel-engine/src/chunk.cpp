#include "../headers/chunk.h"

Chunk::Chunk(int i, int j, int k, utils::NoiseMap heightMap) {
    // Inicializamos el array de vóxeles de chunk a 0
    memset(Chunk_voxels, 0, sizeof(Chunk_voxels));
    elements = 0; // asignamos el número de elementos a 0
    changed = true; // Una vez hemos generado el chunk lo podemos marcar como cambiado porque se acaba de generar

    // Generamos los buffers necesarios
    glGenVertexArrays(1, &VAO); // VAO (Vertex Array Object)
    glGenBuffers(1, &VBO); // VBO (Vertex Buffer Object)

    // Establecemos la posición del chunk en el mundo usando los índices i, j y k
    // los multiplicamos por el tamaño de cada chunk en cada eje
    posX = i * CX;
    posY = j * CY;
    posZ = k * CZ;

    float maxHeightValue = (CY * SCY - 1); // Valor máximo de altura

    // Generamos los vóxeles del chunk basado en el height map (perlin noise)
    for (int x = 0; x < CX; x++) {
        for (int z = 0; z < CZ; z++) {
            float heightmapValue = (heightMap.GetValue(x, z) + 1) / 2;
            float heightValue = heightmapValue * maxHeightValue - posY;
            if (heightValue > CY) heightValue = CY;
            if (posY == 0 && heightValue < 1) heightValue = 1;

            // Genera los Voxels hasta alcanzar el valor de altura
            for (int y = 0; y < heightValue; y++) {
                if ((y + posY) == maxHeightValue)
                    Chunk_voxels[x][y][z] = Voxel_Nieve; // Voxel de nieve
                else if ((y + posY) > (maxHeightValue * 0.75))
                    Chunk_voxels[x][y][z] = Voxel_Piedra; // Voxel de roca
                else if ((y + posY) > (maxHeightValue * 0.50))
                    Chunk_voxels[x][y][z] = Voxel_Tierra; // Voxel de tierra
                else if ((y + posY) > (maxHeightValue * 0.15))
                    Chunk_voxels[x][y][z] = Voxel_Pradera; // Voxel de césped
                else if ((y + posY) > (maxHeightValue * 0.05))
                    Chunk_voxels[x][y][z] = Voxel_Arena; // Voxel de arena
                else
                    Chunk_voxels[x][y][z] = Voxel_Agua; // Voxel de agua
            }
        }
    }
}


// Deconstructor
Chunk::~Chunk() {
    // Limpia los recursos de OpenGL eliminando el VAO y VBO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // limpiando los buffers podremos generar mas chunks con shaders diferentes
}


uint8_t Chunk::GetNeighbourBlock(Chunk* neighbour, int x, int y, int z) {
    if (neighbour) // poner solo neighbour es lo mismo que poner "if (neigbour == True)"
        return neighbour->Chunk_voxels[x][y][z]; // Devuelve el Voxel del vecino si existe
    return 0; // Si no hay vecino, devuelve 0
}


// Donde se crea la malla del chunk
void Chunk::Update() {
    changed = false; // Marca el chunk como no cambiado

    CubeVertex vertex[CX * CY * CZ * 2]; // Array de vértices para los cubos
    int i = 0; // Índice para el array de vértices

    for (int x = 0; x < CX; x++) {
        for (int y = 0; y < CY; y++) {
            for (int z = 0; z < CZ; z++) {
                uint8_t type = Chunk_voxels[x][y][z]; // Tipo de Voxel

                if (!type)
                    continue; // Si el Voxel está vacío, continúa al siguiente

                // Cara negativa en X
                // si x es 0 y no hay un vecino en la dirección negativa del eje X
                if ((x == 0 && !GetNeighbourBlock(neighXN, CX - 1, y, z)) ||
                    (x > 0 && !Chunk_voxels[x - 1][y][z])) {
                    // Triángulo inferior
                    vertex[i++] = CubeVertex(x, y, z, type, -1, 0, 0);
                    vertex[i++] = CubeVertex(x, y, z + 1, type, -1, 0, 0);
                    vertex[i++] = CubeVertex(x, y + 1, z, type, -1, 0, 0);
                    // Triángulo superior
                    vertex[i++] = CubeVertex(x, y + 1, z, type, -1, 0, 0);
                    vertex[i++] = CubeVertex(x, y, z + 1, type, -1, 0, 0);
                    vertex[i++] = CubeVertex(x, y + 1, z + 1, type, -1, 0, 0);
                }

                // Cara positiva en X
                if ((x == CX - 1 && !GetNeighbourBlock(neighXP, 0, y, z)) ||
                    (x < CX - 1 && !Chunk_voxels[x + 1][y][z])) {
                    // Triángulo inferior
                    vertex[i++] = CubeVertex(x + 1, y, z, type, 1, 0, 0);
                    vertex[i++] = CubeVertex(x + 1, y + 1, z, type, 1, 0, 0);
                    vertex[i++] = CubeVertex(x + 1, y, z + 1, type, 1, 0, 0);
                    // Triángulo superior
                    vertex[i++] = CubeVertex(x + 1, y + 1, z, type, 1, 0, 0);
                    vertex[i++] = CubeVertex(x + 1, y + 1, z + 1, type, 1, 0, 0);
                    vertex[i++] = CubeVertex(x + 1, y, z + 1, type, 1, 0, 0);
                }

                // Cara negativa en Y
                if ((y == 0 && !GetNeighbourBlock(neighYN, x, CY - 1, z)) ||
                    (y > 0 && !Chunk_voxels[x][y - 1][z])) {
                    // Triángulo inferior
                    vertex[i++] = CubeVertex(x, y, z, type, 0, -1, 0);
                    vertex[i++] = CubeVertex(x + 1, y, z, type, 0, -1, 0);
                    vertex[i++] = CubeVertex(x, y, z + 1, type, 0, -1, 0);
                    // Triángulo superior
                    vertex[i++] = CubeVertex(x + 1, y, z, type, 0, -1, 0);
                    vertex[i++] = CubeVertex(x + 1, y, z + 1, type, 0, -1, 0);
                    vertex[i++] = CubeVertex(x, y, z + 1, type, 0, -1, 0);
                }

                // Cara positiva en Y
                if ((y == CY - 1 && !GetNeighbourBlock(neighYP, x, 0, z)) ||
                    (y < CY - 1 && !Chunk_voxels[x][y + 1][z])) {
                    // Triángulo inferior
                    vertex[i++] = CubeVertex(x, y + 1, z, type, 0, 1, 0);
                    vertex[i++] = CubeVertex(x, y + 1, z + 1, type, 0, 1, 0);
                    vertex[i++] = CubeVertex(x + 1, y + 1, z, type, 0, 1, 0);
                    // Triángulo superior
                    vertex[i++] = CubeVertex(x + 1, y + 1, z, type, 0, 1, 0);
                    vertex[i++] = CubeVertex(x, y + 1, z + 1, type, 0, 1, 0);
                    vertex[i++] = CubeVertex(x + 1, y + 1, z + 1, type, 0, 1, 0);
                }

                // Cara negativa en Z
                if ((z == 0 && !GetNeighbourBlock(neighZN, x, y, CZ - 1)) ||
                    (z > 0 && !Chunk_voxels[x][y][z - 1])) {
                    // Triángulo inferior
                    vertex[i++] = CubeVertex(x, y, z, type, 0, 0, -1);
                    vertex[i++] = CubeVertex(x, y + 1, z, type, 0, 0, -1);
                    vertex[i++] = CubeVertex(x + 1, y, z, type, 0, 0, -1);
                    // Triángulo superior
                    vertex[i++] = CubeVertex(x, y + 1, z, type, 0, 0, -1);
                    vertex[i++] = CubeVertex(x + 1, y + 1, z, type, 0, 0, -1);
                    vertex[i++] = CubeVertex(x + 1, y, z, type, 0, 0, -1);
                }

                // Cara positiva en Z
                if ((z == CZ - 1 && !GetNeighbourBlock(neighZP, x, y, 0)) ||
                    (z < CZ - 1 && !Chunk_voxels[x][y][z + 1])) {
                    // Triángulo inferior
                    vertex[i++] = CubeVertex(x, y, z + 1, type, 0, 0, 1);
                    vertex[i++] = CubeVertex(x + 1, y, z + 1, type, 0, 0, 1);
                    vertex[i++] = CubeVertex(x, y + 1, z + 1, type, 0, 0, 1);
                    // Triángulo superior
                    vertex[i++] = CubeVertex(x, y + 1, z + 1, type, 0, 0, 1);
                    vertex[i++] = CubeVertex(x + 1, y, z + 1, type, 0, 0, 1);
                    vertex[i++] = CubeVertex(x + 1, y + 1, z + 1, type, 0, 0, 1);
                }
            }
        }
    }

    elements = i; // Actualiza el número de elementos en el buffer

    // Asignamos / linkeamos los atributos al vertex, 
    // esto es esencial a la hora de actualizar la pantalla, ya que es lo que renderiza los voxeles
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Carga los datos de los vértices en el buffer
    glBufferData(GL_ARRAY_BUFFER, elements * sizeof(*vertex), vertex, GL_STATIC_DRAW);

    // Atributo de posición
    glVertexAttribPointer(0, 3, GL_BYTE, GL_FALSE, sizeof(CubeVertex), (void*)0);
    glEnableVertexAttribArray(0);
    // Atributo del tipo de cubo
    glVertexAttribIPointer(1, 1, GL_BYTE, sizeof(CubeVertex), (void*)(3 * sizeof(uint8_t)));
    glEnableVertexAttribArray(1);
    // Atributo normal
    glVertexAttribPointer(2, 3, GL_BYTE, GL_FALSE, sizeof(CubeVertex), (void*)(4 * sizeof(uint8_t)));
    glEnableVertexAttribArray(2);
}

void Chunk::Render() {
    if (changed)
        Update(); // Actualiza el chunk si ha cambiado

    if (!elements)
        return; // Si no hay elementos, no se dibuja nada

    glEnable(GL_CULL_FACE); // Habilita el culling de caras
    glEnable(GL_DEPTH_TEST); // Habilita el test de profundidad
    glEnable(GL_MULTISAMPLE); // Habilita el multisampling

    glBindVertexArray(VAO); // Vincula el VAO

    // Dibuja los triángulos
    glDrawArrays(GL_TRIANGLES, 0, elements);
}

// Creditos de esta funcion a "Low Level Game Dev", 
// usando punteros de memoria podemos guardar chunks 
// en un espacio para comparar los ejes cardinales y asi saber si el chunk debe actualizarse o no

void Chunk::setNeighbours(Chunk* cXN, Chunk* cXP, Chunk* cYN, Chunk* cYP, Chunk* cZN, Chunk* cZP) {
    // Si los vecinos han cambiado, se marca el chunk como cambiado
    if (neighXN != cXN) {
        neighXN = cXN;
        changed = true;
    }
    if (neighXP != cXP) {
        neighXP = cXP;
        changed = true;
    }
    if (neighYN != cYN) {
        neighYN = cYN;
        changed = true;
    }
    if (neighYP != cYP) {
        neighYP = cYP;
        changed = true;
    }
    if (neighZN != cZN) {
        neighZN = cZN;
        changed = true;
    }
    if (neighZP != cZP) {
        neighZP = cZP;
        changed = true;
    }
}

/*
    cxn: Vecino en la dirección negativa del eje X
    cxp: Vecino en la dirección positiva del eje X
    cyn : Vecino en la dirección negativa del eje Y
    cyp : Vecino en la dirección positiva del eje Y
    czn : Vecino en la dirección negativa del eje Z
    czp : Vecino en la dirección positiva del eje Z
*/