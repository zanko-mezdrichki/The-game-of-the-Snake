#ifndef STRUCT_HPP
#define STRUCT_HPP
#include <SFML/Graphics.hpp>
struct Pedina {
  sf::Texture texture;
  sf::Sprite sprite;
  int posizione;
};
struct giocatore {
  std::string nome;
  int posizione = 0;
  char colore;
  bool saltaturno = false;
  bool prigione = false;
  int turniinprigione = 3;
  bool bonus = true;
};
#endif