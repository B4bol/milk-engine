#pragma once
#include "SFML/Graphics.hpp"
using namespace std;

class Menu
{
public:
    sf::Text mainmenu[3];  // Array de 3 elementos de texto para el menú principal
    Menu(float width, float height);   // Constructor: inicializa el menú con las dimensiones dadas

    // Dibuja el menú en la ventana proporcionada
    void draw(sf::RenderWindow& window);

    // Mueve la selección del menú hacia arriba
    void MoveUp();

    // Mueve la selección del menú hacia abajo
    void MoveDown();

    // Establece el elemento seleccionado en el menú
    void setSelected(int n);

    // Devuelve el índice del elemento actualmente seleccionado
    int pressed() {
        return selected;
    }

    ~Menu(); // Destructor

private:
    int selected;   // Índice del elemento actualmente seleccionado
    sf::Font font;  // Fuente utilizada para el texto del menú
};