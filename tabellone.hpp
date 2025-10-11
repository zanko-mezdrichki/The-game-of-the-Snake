#ifndef TABELLONE_HPP
#define TABELLONE_HPP
#include "struct.hpp"
namespace tabellone {

void infoeregolamento();
long unsigned int richiestaGiocatori();

int richiestaLunghezza();

std::vector<giocatore> getplayers(long unsigned int Ngiocatori);

bool sonoConsecutivi(const std::string a, const std::string b);
void scambiaNonConsecutivi(std::vector<std::string>& tabellone, size_t indice);
std::string Casella(int i, const int lunghezza);
std::vector<std::string> Creatabellone(long unsigned int lunghezza);

sf::Color ColoreCaselle(const std::string& tipo);
void ColorePedina(std::vector<giocatore>& giocatori, std::vector<Pedina>& pedine, long unsigned int i);
int coefficentesottrazionepedine(std::vector<giocatore>& giocatori, size_t i);
sf::Color Coloreleggenda(int i);
std::string Testoleggenda(int i);
void Mostratabellone(const int lunghezza, const long unsigned int ngiocatori, std::vector<giocatore>& giocatori,
                     const std::vector<std::string>& tabellone);

void mostrapodio(std::vector<giocatore>& vincitori);
}  // namespace tabellone

#endif