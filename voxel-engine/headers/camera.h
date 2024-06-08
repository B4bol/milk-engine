#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Define varias opciones para el movimiento de la c嫥ara
// Usado como abstracci鏮 para evitar m彋odos de entrada espec璗icos del sistema de ventanas
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Clase abstracta que procesa el input y calcula los 聲gulos de Euler, Vectores y Matrices correspondientes para usar en OpenGL
class Camera {
private:

    // Valores por defecto de la camara
    const float SPEED = 80.0f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM = 45.0f;

    // Atributos de clase (angulos de la camara)
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // 聲gulos de Euler (yaw, pitch)
    float Yaw;             // 聲gulo de rotaci鏮 horizontal
    float Pitch;           // 聲gulo de rotaci鏮 vertical

    // Calcula el vector frontal a partir de los 聲gulos de Euler (actualizados)
    void updateCameraVectors() {
        // Calcular el nuevo vector frontal
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front); // Normalizar para mantener la longitud

        // Tambi幯 recalcular los vectores Right y Up
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Producto vectorial 
        Up = glm::normalize(glm::cross(Right, Front));       // Producto vectorial
    }
public:
    // Opciones de la c嫥ara
    glm::vec3 Position;        // Posici鏮 de la c嫥ara
    float MovementSpeed;       // Velocidad de movimiento actual
    float MouseSensitivity;    // Sensibilidad del mouse actual
    float Zoom;                // Nivel de zoom actual

    // Constructor con vectores
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.f, float pitch = 0.f) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // Constructor con valores escalables
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Devuelve la matriz de vista calculada usando 聲gulos de Euler y LookAt
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // Funcion para procesar el movimiento de la camara en base a la tecla que se pulse
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // Funcion para procesar el movimiento del raton
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // Si el Pitch supera los limites lo reseteamos a unos valores para que no se nos de la vuelta la camara
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Actualizar la visualizacion de la camara con los nuevos parametros para obtener los angulos correctos
        updateCameraVectors();
    }

    // Gestion rueda del raton (zoom)
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }
};
#endif 