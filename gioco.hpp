#ifndef GIOCO_HPP
#define GIOCO_HPP
#include "struct.hpp"
namespace gioco {
void movimento(std::vector<giocatore>& giocatori, size_t j, const int lunghezza,
               const std::vector<std::string>& tabellone, const long unsigned int ngiocatori,
               const std::vector<std::string>& v);

int GiocoSFC();
void EffettoSCF(size_t j, std::vector<giocatore>& giocatori);

bool quiz(const std::vector<std::string>& v);
void domanda(std::vector<giocatore>& giocatori, const std::vector<std::string>& vecdomande, size_t j);
void usajolly(std::vector<giocatore>& giocatori, size_t j, const int lunghezza);
}  // namespace gioco
#endif