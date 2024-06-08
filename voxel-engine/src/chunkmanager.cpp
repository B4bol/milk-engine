#include "../headers/chunkmanager.h"

extern Camera camera;

ChunkManager::ChunkManager() {
    // Generamos un número aleatorio para la generación de nuestro mapa de altura
    // mt es un generador de números aleatorios de Mersenne Twister
    std::mt19937 mt(rd()); // rd() nos da un número aleatorio para inicializar el generador

    // Definimos los límites del mapa de altura y el módulo de ruido de Perlin
    std::uniform_real_distribution<double> dist(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    // SetSeed es un método de la clase Perlin que establece la semilla para la generación de ruido
    myModule.SetSeed(dist(mt));
    // Crear un mapa de altura aleatorio usando ruido de Perlin
    heightMapBuilder.SetSourceModule(myModule);
    // Asignamos el mapa de altura al generador de ruido
    heightMapBuilder.SetDestNoiseMap(heightMap);
    // Definimos el tamaño del mapa de altura
    heightMapBuilder.SetDestSize(CX, CZ);

    // Usamos Update para cargar los chunks al rededor de la camara / jugador
    Update(); 
}

// Destructor
ChunkManager::~ChunkManager() {
    // Liberar la memoria de los chunks creados
    for (ChunkMap::iterator iter = chunks.begin(); iter != chunks.end(); ++iter) {
        iter->second.reset();
    }
}

// Descargamos y cargamos los chunks al rededor de la camara / jugador
void ChunkManager::Update() {
    glm::vec3 key; // Clave para el hashmap de chunks
    // Obtenemos la posición del jugador en el mundo en base a la posición de la cámara
    int playerPosX = floor(camera.Position[0] / CX); // floor redondea hacia abajo
    int playerPosY = floor(camera.Position[1] / CY);
    int playerPosZ = floor(camera.Position[2] / CZ);
    lastPlayerPos = glm::vec3(playerPosX, playerPosY, playerPosZ); // Guardamos la última posición del jugador

    // Iteramos por todos los chunks y eliminamos aquellos que estén lejos del jugador

    // .begin() y .end() son iteradores que apuntan al primer y último elemento del hashmap
    for (ChunkMap::iterator iter = chunks.begin(); iter != chunks.end(); ++iter) {
        // Si el chunk está lejos del jugador, lo eliminamos del hashmap
        if (iter->second->posX / CX > playerPosX + SCX / 2 || iter->second->posX / CX < playerPosX + (-SCX / 2) ||
            iter->second->posZ / CZ > playerPosZ + SCZ / 2 || iter->second->posZ / CZ < playerPosZ + (-SCZ / 2)) {
            chunks.erase(iter);
        }
    }
 
    // Iteramos por todos los chunks al rededor del jugador y los generamos si no existen
    for (int x = playerPosX + (-SCX / 2); x <= playerPosX + SCX / 2; x++)
        for (int y = 0; y < SCY; y++)
            for (int z = playerPosZ + (-SCZ / 2); z <= playerPosZ + SCZ / 2; z++) {
                key.x = x; key.y = y; key.z = z;
                if (!chunks[key]) {
                    // El mapa de altura tiene datos continuos, solo necesitamos cambiar los límites para cada chunk
                    // Esto nos permite tener un terreno continuo en los bordes de los chunks
                    // dividimos x y z por 10.0f para que el terreno sea más plano
                    heightMapBuilder.SetBounds(x / 10.0f, x / 10.0f + 0.1, z / 10.0f, z / 10.0f + 0.1); 
                    heightMapBuilder.Build();
                    // Creamos un nuevo chunk y lo guardamos en el hashmap
                    chunks[key] = std::unique_ptr<Chunk>(new Chunk(x, y, z, heightMap));
                }
            }

    // Iteramos por todos los chunks y les asignamos sus vecinos
    // las ? son operadores ternarios, si el chunk existe en el hashmap, se asigna, si no, se asigna nullptr
    // es como un if en una sola linea
    for (ChunkMap::iterator iter = chunks.begin(); iter != chunks.end(); ++iter) {
        key.x = iter->second->posX; key.y = iter->second->posY; key.z = iter->second->posZ;
        iter->second->setNeighbours(chunks.count(glm::ivec3(key.x / CX - 1, key.y / CY, key.z / CZ)) ? chunks[glm::ivec3(key.x / CX - 1, key.y / CY, key.z / CZ)].get() : nullptr,
            chunks.count(glm::ivec3(key.x / CX + 1, key.y / CY, key.z / CZ)) ? chunks[glm::ivec3(key.x / CX + 1, key.y / CY, key.z / CZ)].get() : nullptr,
            chunks.count(glm::ivec3(key.x / CX, key.y / CY - 1, key.z / CZ)) ? chunks[glm::ivec3(key.x / CX, key.y / CY - 1, key.z / CZ)].get() : nullptr,
            chunks.count(glm::ivec3(key.x / CX, key.y / CY + 1, key.z / CZ)) ? chunks[glm::ivec3(key.x / CX, key.y / CY + 1, key.z / CZ)].get() : nullptr,
            chunks.count(glm::ivec3(key.x / CX, key.y / CY, key.z / CZ - 1)) ? chunks[glm::ivec3(key.x / CX, key.y / CY, key.z / CZ - 1)].get() : nullptr,
            chunks.count(glm::ivec3(key.x / CX, key.y / CY, key.z / CZ + 1)) ? chunks[glm::ivec3(key.x / CX, key.y / CY, key.z / CZ + 1)].get() : nullptr);
    }
}

void ChunkManager::Render(Shader shader, glm::mat4 &vp) {
    // Comprobamos la si la posicion del jugador cambió, para cargar/descargar chunks
    int playerPosX = floor(camera.Position[0] / CX);
    int playerPosY = floor(camera.Position[1] / CY);
    int playerPosZ = floor(camera.Position[2] / CZ);
    if (playerPosX != lastPlayerPos[0] || playerPosY != lastPlayerPos[1] || playerPosZ != lastPlayerPos[2]) {
        Update();
    }
    // Iteramos por todos los chunks y llamamos a su método Render
    glm::vec3 pos;
    if (!chunks.empty()) { // Si el hashmap de chunks no está vacío
        for (ChunkMap::iterator iter = chunks.begin(); iter != chunks.end(); ++iter) {
            // Cambiamos la posición de cada chunk para que no se apilen unos sobre otros
            pos.x = iter->second->posX; pos.y = iter->second->posY; pos.z = iter->second->posZ;
            glm::mat4 model = glm::translate(glm::mat4(1), pos); // Matriz de traslación para mover el chunk
            shader.setMat4("model", model);// Pasamos la matriz de modelo al shader

            // Transformamos el centro del chunk a coordenadas de pantalla para saber si está en la pantalla
            glm::vec4 center = (vp * model) * glm::vec4(CX / 2, CY / 2, CZ / 2, 1); // Multiplicamos la matriz de vista y proyección por la matriz de modelo
            float d = glm::length(center); // Calculamos la distancia del centro del chunk a la cámara

            // /= es un operador de división y asignación, es lo mismo que center.x = center.x / center.w
            center.x /= center.w;
            center.y /= center.w;

            // No renderizamos si el chunk está detrás de la cámara
            if (center.z < -CY / 2) // center.z es la coordenada z en coordenadas de pantalla y CY/2 es la mitad de la altura de un chunk
                continue; 
            // No renderizamos si el chunk está fuera de la pantalla
            if (fabsf(center.x) > 1 + fabsf(CY * 2 / center.w) || fabsf(center.y) > 1 + fabsf(CY * 2 / center.w))
                continue;   

            // "continue" salta a la siguiente iteración del bucle, ignorando el código que sigue
            // esto significa el que metodo render del chunk nunca se ejecutara

            iter->second->Render(); // Llamamos al método Render de cada chunk
        }
    }
}