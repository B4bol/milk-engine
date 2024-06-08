#include "SFML/Graphics.hpp"
#include "../headers/Menu.h"
using namespace std;
using namespace sf;

// Constructor del men�. Inicializa los elementos del men� (texto, posici�n, color)
Menu::Menu(float width, float height) {
    font.loadFromFile("assets/fonts/SuperPixel-m2L8j.ttf"); // Carga la fuente desde un archivo

    // Configuraci�n del primer elemento del men� ("Generar Mundo")
    mainmenu[0].setFont(font);		   // Selecciona la fuente
    mainmenu[0].setFillColor(Color::Red);    // Lo resalta en rojo como elemento inicial
    mainmenu[0].setString("Generar Mundo"); // Texto del elemento
    mainmenu[0].setCharacterSize(70); 	  // Tama�o de la fuente
    mainmenu[0].setPosition(Vector2f(200, height / (4) + 50)); // Posici�n en la ventana

    // Configuraci�n del segundo elemento del men� ("Informaci�n")
    mainmenu[1].setFont(font);
    mainmenu[1].setFillColor(Color::White);
    mainmenu[1].setString("Controles");
    mainmenu[1].setCharacterSize(70);
    mainmenu[1].setPosition(Vector2f(200, height / (4) + 250));

    // Configuraci�n del tercer elemento del men� ("Salir")
    mainmenu[2].setFont(font);
    mainmenu[2].setFillColor(Color::White);
    mainmenu[2].setString("Salir");
    mainmenu[2].setCharacterSize(70);
    mainmenu[2].setPosition(Vector2f(200, height / (4) + 450));

    selected = 0; // Inicialmente, el primer elemento est� seleccionado
}

// Destructor del men� (vac�o en este caso)
Menu::~Menu() {}

// Establece el �ndice del elemento seleccionado en el men�
void Menu::setSelected(int n) {
    selected = n;
}

// Dibuja cada elemento del men� en la ventana
void Menu::draw(RenderWindow& window) {
    for (int i = 0; i < 3; i++) {
        window.draw(mainmenu[i]);
    }
}

// Mueve la selecci�n hacia abajo, con manejo de ciclos
void Menu::MoveDown() {
    if (selected + 1 <= 3) {         // Evita desbordamiento
        mainmenu[selected].setFillColor(Color::White); // Deselecciona el actual
        selected++;
        if (selected == 3) {       // Si llega al final, vuelve al inicio
            selected = 0;
        }
        mainmenu[selected].setFillColor(Color::Red);    // Selecciona el nuevo
    }
}

// Mueve la selecci�n hacia arriba, con manejo de ciclos
void Menu::MoveUp() {
    if (selected - 1 >= -1) {        // Evita desbordamiento
        mainmenu[selected].setFillColor(Color::White); // Deselecciona el actual
        selected--;
        if (selected == -1) {       // Si llega al inicio, vuelve al final
            selected = 2;
        }
        mainmenu[selected].setFillColor(Color::Red);    // Selecciona el nuevo
    }
}