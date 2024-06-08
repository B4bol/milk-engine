// ################################################################
// #           Mario L�pez Llamas                                 #
// #           04/04/2024                                         #
// #           Desarrollo de aplicaciones multiplataforma         #
// #           Emprende CampusFP Humanes	          	          #
// ################################################################

#include "../headers/main.h"



// Constantes para el ancho y alto de la ventana
const unsigned int SCR_WIDTH = 1280, SCR_HEIGHT = 720;
// Variables para seguimiento del rat�n
float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;  // Posici�n previa del rat�n
float deltaTime = 0.0f, lastFrame = 0.0f;   // Para calcular el tiempo transcurrido entre frames
bool firstMouse = true;  // Bandera para controlar el primer movimiento del rat�n
Camera camera(glm::vec3(50.0f, 85.0f, 100.0f));  // C�mara inicial (posici�n)

int main() {
    // Configuraci�n de la ventana del men� principal
    sf::RenderWindow menu_window(sf::VideoMode(1920, 1080), "Milk Engine");
    Menu menu(1920, 1080);  // Objeto para manejar el men�

    // Carga de la imagen de fondo del men�
    sf::Texture mainmenubg;
    mainmenubg.loadFromFile("assets/img/menu_bg.png");
    sf::Sprite bg;
    bg.setTexture(mainmenubg);

    // Carga de la imagen de los controles
    sf::Texture controles;
    controles.loadFromFile("assets/img/controles.png");
	sf::Sprite controlesSprite;
    controlesSprite.setTexture(controles);

    // Bucle principal del men� (se ejecuta hasta que se cierra)
    while (true) {
        // Bucle de eventos del men� (maneja interacciones del usuario)
        while (menu_window.isOpen()) {
            sf::Event event;
            while (menu_window.pollEvent(event)) {  // Obtener eventos de la cola
                if (event.type == sf::Event::Closed) {  // Cerrar ventana si se pulsa la X
                    menu_window.close();
                    break; // Salir del bucle de eventos
                }

                if (event.type == sf::Event::KeyPressed) {  // Manejo de teclas
                    if (event.key.code == sf::Keyboard::Up) {
                        menu.MoveUp();  // Mover selecci�n del men� hacia arriba
                    }
                    if (event.key.code == sf::Keyboard::Down) {
                        menu.MoveDown();  // Mover selecci�n del men� hacia abajo
                    }

                    // Manejo de la tecla Enter (Return)
                    if (event.key.code == sf::Keyboard::Return) {
                        // Ventana temporal para mostrar informaci�n (podr�a no ser necesaria)
                        sf::RenderWindow Info(sf::VideoMode(1920, 1080), "Milk Engine - Controles");

                        int x = menu.pressed();  // Opci�n seleccionada en el men�
                        if (x == 0) { // Si se elige la primera opci�n (Generar mundo)
                            Info.close();  // Cerrar la ventana de informaci�n

                            // Configuraci�n de la ventana principal y el contexto OpenGL
                            GLFWwindow* window = setupWindow(SCR_WIDTH, SCR_HEIGHT, "Voxel Terrain");
                            ChunkManager chunkManager;   // Administrador de chunks (terreno)
                            Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");  // Shaders

                            // Configuraci�n de Dear ImGui (interfaz de usuario)
                            IMGUI_CHECKVERSION();
                            ImGui::CreateContext();
                            ImGuiIO& io = ImGui::GetIO(); (void)io;
                            ImGui::StyleColorsDark(); // Estilo visual de ImGui
                            // Inicializaci�n de ImGui para OpenGL
                            ImGui_ImplGlfw_InitForOpenGL(window, true);
                            ImGui_ImplOpenGL3_Init("#version 130");

                            // Variables de estado de ImGui
                            ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // Color de fondo
                            bool wireframeCheckBox = false;  // Checkbox para mostrar rejilla
                            float frustumFarBoundary = 500.0f; // Distancia de dibujado

                            // Activar el shader principal
                            shader.use();

                            // Bucle principal del juego
                            while (!glfwWindowShouldClose(window)) {
                                // Actualizaci�n de ImGui cada frame
                                ImGui_ImplOpenGL3_NewFrame();
                                ImGui_ImplGlfw_NewFrame();
                                ImGui::NewFrame();

                                // Creaci�n de la ventana de control de ImGui
                                ImGui::SetNextWindowSize(ImVec2((float)320, (float)210));
                                {
                                    ImGui::Begin("Panel de Control");  // T�tulo de la ventana
                                    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Pulsar T para Voxelar o liberar el raton");
                                    ImGui::Text("Media FPS: %.2f", ImGui::GetIO().Framerate);
                                    ImGui::Text("Pos camara. X: %.1f | Y: %.1f | Z: %.1f", camera.Position[0], camera.Position[1], camera.Position[2]);
                                    ImGui::Text("Chunk Pos camara. X: %.1f | Y: %.1f | Z: %.1f", floor(camera.Position[0] / CX), floor(camera.Position[1] / CY), floor(camera.Position[2] / CZ));
                                    ImGui::Text("Voxels por chunk %dx%dx%d", CX, CY, CZ);
                                    ImGui::Text("Chunks en el mundo: %dx%dx%d", SCX, SCY, SCZ);
                                    ImGui::Checkbox("Rejilla", &wireframeCheckBox);
                                    ImGui::Text("Distancia de Frustum: ");
                                    ImGui::SliderFloat("float", &frustumFarBoundary, 0.1f, 2000.0f);
                                    ImGui::End();
                                }

                                // Renderizar con o sin la rejilla?
                                glPolygonMode(GL_FRONT_AND_BACK, wireframeCheckBox ? GL_LINE : GL_FILL);

                                // Logica por-fotograma (Calculo del tiempo entre fotogramas), obtenemos el tiempo con GLFW
                                float currentFrame = glfwGetTime();
                                deltaTime = currentFrame - lastFrame;
                                lastFrame = currentFrame;

                                // recogemos el input en cada fotograma
                                processInput(window);

                                // Tambien renderizamos ImGui cada fotogframa
                                ImGui::Render();

                                // Establece el color de borrado a un tono claro de verde azulado (Cielo)
                                glClearColor(55.0f / 255.0f, 184.0f / 255.0f, 1.0f, 1.0f);

                                // Borra el buffer de color (lo llena con el color teal) y el buffer de profundidad (reinicia la informaci�n de profundidad)
                                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                                // Propiedades luminicas
                                shader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
                                shader.setVec3("viewPos", camera.Position);
                                shader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f);
                                shader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);

                                // Pass projection matrix to shader (note that in this case it could change every frame)
                                // These last two parameters specify the lower and farest boundaries of our frustrum culling

                                // Pasamos la matriz de proyecci�n al shader (puede cambiar en cada frame)
                                // Estos dos �ltimos par�metros especifican los l�mites m�s bajos y m�s lejanos de nuestro culling de frustrum
                                glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, frustumFarBoundary);
                                shader.setMat4("projection", projection);

                                // Transladamos la c�mara a la posici�n correcta
                                glm::mat4 view = camera.GetViewMatrix();
                                shader.setMat4("view", view);

                                // Matriz de modelo-vista-proyecci�n
                                // se encarga de transformar los v�rtices de los objetos en coordenadas de pantalla
                                glm::mat4 vp = projection * view;
                                chunkManager.Render(shader, vp);

                                // Renderizamos imgui lo ultimo para que apareza por encima de todo
                                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                                // glfw : barajamos los buffers y procesamos los eventos para el input
                                glfwSwapBuffers(window);
                                glfwPollEvents();
                            }
                            glfwTerminate(); // Liberar recursos de GLFW
                        }
                        else if (x == 1) {  // Si se elige la segunda opci�n (probablemente "Informaci�n")
                            while (Info.isOpen()) {  // Mantener la ventana de informaci�n abierta
                                sf::Event aevent;
                                while (Info.pollEvent(aevent)) {
                                    if (aevent.type == sf::Event::Closed || aevent.type == sf::Event::KeyPressed) {
                                        Info.close(); // Cerrar al pulsar una tecla o la X
                                    }
                                }
                                Info.clear();
                                Info.draw(controlesSprite);  // Dibujar la imagen de los controles
                                Info.display();
                            }
                        }
                        else if (x == 2) {  // Si se elige la tercera opci�n (probablemente "Salir")
                            menu_window.close(); // Cerrar la ventana del men�
                            break; // Salir del bucle del men�
                        }
                    }
                }
            }

            // Limpiar la ventana del men� y dibujar los elementos
            menu_window.clear();
            menu_window.draw(bg);
            menu.draw(menu_window);
            menu_window.display();
        }
    }
    return 0;
}
