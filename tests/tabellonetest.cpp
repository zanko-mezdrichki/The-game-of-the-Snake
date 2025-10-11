#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <algorithm>
#include <random>
#include <vector>

#include "doctest.h"
#include "struct.hpp"
bool sonoConsecutivi(const std::string a, const std::string b) { return a == b; }
void scambiaNonConsecutivi(std::vector<std::string>& tabellone, size_t indice) {
  for (size_t i = 1; i != tabellone.size() - 1; ++i) {
    if (!sonoConsecutivi(tabellone[indice], tabellone[i]) && !sonoConsecutivi(tabellone[indice - 1], tabellone[i])) {
      std::swap(tabellone[indice], tabellone[i]);
      break;
    }
  }
}
std::string Casella(int i, const int lunghezza) {  // per dividere le caselle
  std::string tipo;
  if (i == 0) {
    tipo = "Via";
  }
  if (i > 0 && i <= ((lunghezza - 2) * 3) / 15) {
    tipo = "Niente";
  }

  if (i > ((lunghezza - 2) * 3) / 15 && i <= ((lunghezza - 2) * 4) / 15) {
    tipo = "Scambio";
  }
  if (i > ((lunghezza - 2) * 4) / 15 && i <= ((lunghezza - 2) * 5) / 15) {
    tipo = "Serpente";
  }
  if (i > ((lunghezza - 2) * 5) / 15 && i <= ((lunghezza - 2) * 6) / 15) {
    tipo = "Prigione";
  }
  if (i > ((lunghezza - 2) * 6) / 15 && i <= ((lunghezza - 2) * 7) / 15) {
    tipo = "Bonus";
  }
  if (i > ((lunghezza - 2) * 7) / 15 && i <= ((lunghezza - 2) * 8) / 15) {
    tipo = "Pausa";
  }
  if (i > ((lunghezza - 2) * 8) / 15 && i <= ((lunghezza - 2) * 10) / 15) {
    tipo = "Domanda";
  }
  if (i > ((lunghezza - 2) * 10) / 15 && i <= ((lunghezza - 2) * 12) / 15) {
    tipo = "SCF";
  }

  if (i > ((lunghezza - 2) * 12) / 15 && i < lunghezza - 1) {
    tipo = "Niente";
  }
  if (i == lunghezza - 1) {
    tipo = "Fine";
  }
  return tipo;
}
std::vector<std::string> Creatabellone(const long unsigned int lunghezza) {
  std::random_device rd;
  std::mt19937 g(rd());
  std::vector<int> caselle(lunghezza);
  std::iota(caselle.begin(), caselle.end(), 0);  // genera un vettore di interi in ordine crescente
  std::vector<std::string> tabellone(lunghezza);
  for (size_t i = 0; i < lunghezza; ++i) {
    std::string tipo = Casella(caselle[i], static_cast<int>(lunghezza));
    tabellone[i] = tipo;
  }
  for (auto it = tabellone.begin(); it != tabellone.end(); ++it) {
    do {
      std::shuffle(tabellone.begin() + 1, tabellone.end() - 1,
                   g);  // mischia casualmente gli elementi(no il via e la fine)
      for (size_t i = 1; i < lunghezza - 1; ++i) {
        if (sonoConsecutivi(tabellone[i], tabellone[i - 1])) {
          scambiaNonConsecutivi(tabellone, i);
        }
      }
    } while (std::adjacent_find(tabellone.begin(), tabellone.end(), sonoConsecutivi) !=
             tabellone.end());  // adjacent_find in questo caso cerca da begin a end se due valori
                                // adiacenti del vettore rispettano la condizione
                                // "sonoConsecutivi", ponendolo poi diverso da end significa che
                                // per nessuna coppia di valori adiacenti vale sonoConsecutivi
  }
  return tabellone;
}
TEST_CASE("Check lunghezza tabellone") {
  long unsigned int lunghezza;
  std::vector<std::string> tabellone;
  SUBCASE("lunghezza=32") {
    lunghezza = 32;
    tabellone = Creatabellone(lunghezza);
    CHECK(tabellone.size() == 32);
  }
  SUBCASE("lunghezza=43") {
    lunghezza = 43;
    tabellone = Creatabellone(lunghezza);
    CHECK(tabellone.size() == 43);
  }
  SUBCASE("lunghezza=54") {
    lunghezza = 54;
    tabellone = Creatabellone(lunghezza);
    CHECK(tabellone.size() == 54);
  }
  SUBCASE("lunghezza=65") {
    lunghezza = 65;
    tabellone = Creatabellone(lunghezza);
    CHECK(tabellone.size() == 65);
  }
}
TEST_CASE("Check numero caselle diverse") {
  long unsigned int lunghezza = 32;
  std::vector<std::string> tabellone = Creatabellone(lunghezza);
  int conteggioniente = 0;
  int conteggiobonus = 0;
  int conteggiopausa = 0;
  int conteggioscf = 0;
  int conteggiodomanda = 0;
  int conteggioprigione = 0;
  int conteggioscambio = 0;
  int conteggioserpente = 0;
  for (std::string caselle : tabellone) {
    if (caselle == "Niente") {
      conteggioniente += 1;
    }
    if (caselle == "Bonus") {
      conteggiobonus += 1;
    }
    if (caselle == "Pausa") {
      conteggiopausa += 1;
    }
    if (caselle == "Prigione") {
      conteggioprigione += 1;
    }
    if (caselle == "Serpente") {
      conteggioserpente += 1;
    }
    if (caselle == "Domanda") {
      conteggiodomanda += 1;
    }
    if (caselle == "SCF") {
      conteggioscf += 1;
    }
    if (caselle == "Scambio") {
      conteggioscambio += 1;
    }
  }
  CHECK(conteggioniente == 12);
  CHECK(conteggiobonus == 2);
  CHECK(conteggiopausa == 2);
  CHECK(conteggioprigione == 2);
  CHECK(conteggioserpente == 2);
  CHECK(conteggiodomanda == 4);
  CHECK(conteggioscf == 4);
  CHECK(conteggioscambio == 2);
}
TEST_CASE("Check non consecutivi") {
  long unsigned int lunghezza = 54;
  std::vector<std::string> tabellone = Creatabellone(lunghezza);
  bool nonconsecutivi;
  for (size_t i = 0; i < tabellone.size() - 1; ++i) {
    if (tabellone[i] == tabellone[i + 1]) {
      nonconsecutivi = false;
    } else {
      nonconsecutivi = true;
    }
  }
  CHECK(nonconsecutivi == true);
}
