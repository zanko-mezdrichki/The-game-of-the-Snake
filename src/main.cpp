#include <fstream>
#include <iostream>
#include <vector>

#include "game.hpp"
#include "board.hpp"

int main() {
  std::ifstream domande("domande.txt");
  std::string Q;
  std::vector<std::string> v;
  while (getline(domande, Q)) {
    v.push_back(Q);
  }
  domande.close();  // necessario per poter generare il vettore di domande e risposte utilizzando un file esterno(nel
  // nostro caso domande.txt)
  std::vector<giocatore> vincitori;
  tabellone::infoeregolamento();
  long unsigned int ngiocatori = tabellone::richiestaGiocatori();
  int lunghezza = tabellone::richiestaLunghezza();
  std::vector<std::string> tabellone = tabellone::Creatabellone(static_cast<long unsigned int>(lunghezza));
  std::vector<giocatore> giocatori = tabellone::getplayers(ngiocatori);

  tabellone::Mostratabellone(lunghezza, ngiocatori, giocatori, tabellone);
  while (ngiocatori > 1) {
    for (size_t i = 0; i < ngiocatori; ++i) {
      gioco::movimento(giocatori, i, lunghezza, tabellone, ngiocatori, v);
      tabellone::Mostratabellone(lunghezza, ngiocatori, giocatori, tabellone);
      if (giocatori[i].posizione == lunghezza - 1) {
        std::cout << giocatori[i].nome << " Ha vinto!" << '\n';
        vincitori.push_back(giocatori[i]);
        tabellone::Mostratabellone(lunghezza, ngiocatori, giocatori, tabellone);
        giocatori.erase(giocatori.begin() + static_cast<long int>(i));
        --i;
        --ngiocatori;
        if (ngiocatori == 1) {
          std::cout << "\n Rimane solo un giocatore, Game Over! \n";
          break;
        } else {
          std::cout << "\n Volete continuare a giocare?(s/n) \n";
          std::string continuare;
          std::cin >> continuare;
          while (continuare != "s" && continuare != "n") {
            std::cout << "Scelta non valida, riprovare \n";
            std::cin >> continuare;
          }
          if (continuare == "n") {
            ngiocatori = 1;
            break;
          }
        }
      }
    }
  }
  std::cout << "\n La classifica finale recita: \n";
  for (size_t i = 0; i < vincitori.size(); ++i) {
    std::cout << "-" << i + 1 << "° posto, " << vincitori[i].nome << '\n';
  }
  std::cout << "-Gli altri giocatori non sono arrivati alla fine \n";
  tabellone::mostrapodio(vincitori);
}