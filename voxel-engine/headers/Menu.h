#pragma once
#include "SFML/Graphics.hpp"
using namespace std;

class Menu
{
public:
    sf::Text mainmenu[3];  // Array de 3 elementos de texto para el men� principal
    Menu(float width, float height);   // Constructor: inicializa el men� con las dimensiones dadas

    // Dibuja el men� en la ventana proporcionada
    void draw(sf::RenderWindow& window);

    // Mueve la selecci�n del men� hacia arriba
    void MoveUp();

    // Mueve la selecci�n del men� hacia abajo
    void MoveDown();

    // Establece el elemento seleccionado en el men�
    void setSelected(int n);

    // Devuelve el �ndice del elemento actualmente seleccionado
    int pressed() {
        return selected;
    }

    ~Menu(); // Destructor

private:
    int selected;   // �ndice del elemento actualmente seleccionado
    sf::Font font;  // Fuente utilizada para el texto del men�
};