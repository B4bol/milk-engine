#include "SFML/Graphics.hpp"
#include "../headers/Menu.h"
using namespace std;
using namespace sf;

// Constructor del menú. Inicializa los elementos del menú (texto, posición, color)
Menu::Menu(float width, float height) {
    font.loadFromFile("assets/fonts/SuperPixel-m2L8j.ttf"); // Carga la fuente desde un archivo

    // Configuración del primer elemento del menú ("Generar Mundo")
    mainmenu[0].setFont(font);		   // Selecciona la fuente
    mainmenu[0].setFillColor(Color::Red);    // Lo resalta en rojo como elemento inicial
    mainmenu[0].setString("Generar Mundo"); // Texto del elemento
    mainmenu[0].setCharacterSize(70); 	  // Tamaño de la fuente
    mainmenu[0].setPosition(Vector2f(200, height / (4) + 50)); // Posición en la ventana

    // Configuración del segundo elemento del menú ("Información")
    mainmenu[1].setFont(font);
    mainmenu[1].setFillColor(Color::White);
    mainmenu[1].setString("Controles");
    mainmenu[1].setCharacterSize(70);
    mainmenu[1].setPosition(Vector2f(200, height / (4) + 250));

    // Configuración del tercer elemento del menú ("Salir")
    mainmenu[2].setFont(font);
    mainmenu[2].setFillColor(Color::White);
    mainmenu[2].setString("Salir");
    mainmenu[2].setCharacterSize(70);
    mainmenu[2].setPosition(Vector2f(200, height / (4) + 450));

    selected = 0; // Inicialmente, el primer elemento está seleccionado
}

// Destructor del menú (vacío en este caso)
Menu::~Menu() {}

// Establece el índice del elemento seleccionado en el menú
void Menu::setSelected(int n) {
    selected = n;
}

// Dibuja cada elemento del menú en la ventana
void Menu::draw(RenderWindow& window) {
    for (int i = 0; i < 3; i++) {
        window.draw(mainmenu[i]);
    }
}

// Mueve la selección hacia abajo, con manejo de ciclos
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

// Mueve la selección hacia arriba, con manejo de ciclos
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