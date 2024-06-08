#include "../headers/process.h"

// Variables globales para el manejo de la cámara y el tiempo
extern float lastX, lastY, deltaTime;
extern unsigned int SCR_WIDTH, SCR_HEIGHT;
extern bool firstMouse;
extern Camera camera;

// Inicializa GLFW y GLAD, define la versión y el perfil de OpenGL, crea la ventana y establece el contexto
// Establece los callbacks y el modo de entrada del cursor, habilita la prueba de profundidad
GLFWwindow* setupWindow(int WIDTH, int HEIGHT, const char* TITLE) {
    // Inicializar GLFW
    glfwInit();
    // Configurar GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // MSAA 4x tambien conocido como anti-aliasing (suavizado de bordes)
    glfwWindowHint(GLFW_SAMPLES, 4);
    // crear la ventana
    GLFWwindow* GLwindow = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
    if (GLwindow == NULL) {
        std::cout << "Error al generar la ventana GLFW" << std::endl;
        glfwTerminate(); // Matamos el proceso de GLFW
        throw "Error al generar la ventana GLFW";
    }
    // creamos el contexto de la ventana como el contexto principal en el hilo actual
    // segun la documentacion de GLFW, esto es necesario para que las llamadas a OpenGL funcionen
    glfwMakeContextCurrent(GLwindow);
    /*
        Le decimos a GLFW que queremos llamar a esta función en cada cambio de tamaño de la ventana registrándola
        función de callback lambda en la ventana que se llama cada vez que se cambia el tamaño de la ventana
        cuando se muestra la ventana por primera vez, esto también se llama
    */
    glfwSetFramebufferSizeCallback(GLwindow, [](GLFWwindow* window, int width, int height) {
        // Le decimos a OpenGL el tamaño de la ventana en cada cambio
        glViewport(0, 0, width, height);
        });
    glfwSetCursorPosCallback(GLwindow, mouse_callback);
    glfwSetScrollCallback(GLwindow, scroll_callback);

    // Le decimos a GLFW que capture nuestro raton
    glfwSetInputMode(GLwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Por ultimos cargamos todas las funciones de OpenGL con GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Error al inicializar GLAD" << std::endl;
        throw "Error al inicializar GLAD";
    }
    // activamos la prueba de profundidad
    // esto sirve para que los objetos que esten mas cerca de la camara se rendericen primero
    glEnable(GL_DEPTH_TEST);

    return GLwindow; // Retornamos la ventana
}

// Procesa la entrada del teclado y el ratón en la ventana de GLFW y reacciona en consecuencia
void processInput(GLFWwindow* window) {
    // Variables para el manejo de la captura del ratón
    // las hacemos estaticas para acceder a ellas desde cualquier sitio
    static bool captMouse = false;
    static bool captMouseLock = true;

    // si se presiona la tecla ESC, cerramos la ventana
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Capturamos el ratón si se presiona la tecla T
    if (glfwGetKey(window, GLFW_KEY_T) != GLFW_PRESS)
        captMouseLock = false;
    if (glfwGetKey(window, GLFW_KEY_T) != GLFW_RELEASE && !captMouseLock) {
        if (captMouse) firstMouse = true;
        glfwSetInputMode(window, GLFW_CURSOR, (captMouse = !captMouse) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        captMouseLock = true;
    }
    // Movimiento de la camara con las teclas W, A, S, D
    // deltaTime es la diferencia de tiempo entre el frame actual y el anterior
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: siempre que el ratón se mueva, esta función callback es llamada
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) return;
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // invertimos el eje Y porque las coordenadas del ratón van de abajo a arriba

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: cada vez que la rueda del ratón se desplaza, esta función callback es llamada
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}