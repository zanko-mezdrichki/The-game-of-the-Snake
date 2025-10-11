#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <algorithm>
#include <random>
#include <vector>

#include "doctest.h"
#include "struct.hpp"
void movimento(std::vector<giocatore>& giocatori, size_t j, const int lunghezza,
               const std::vector<std::string>& tabellone, const long unsigned int ngiocatori,
               const std::vector<std::string>& v);

int GiocoSFC();
void EffettoSCF(size_t j, std::vector<giocatore>& giocatori);

bool quiz(const std::vector<std::string>& v);
void domanda(std::vector<giocatore>& giocatori, const std::vector<std::string>& vecdomande, size_t j);

void movimento(std::vector<giocatore>& giocatori, size_t j, const int lunghezza,
               const std::vector<std::string>& tabellone, const long unsigned int ngiocatori,
               const std::vector<std::string>& v) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(1, 6);
  std::string dado;
  if (giocatori[j].saltaturno == true) {  // necessario per casella pausa
    giocatori[j].saltaturno = false;
    return;
  } else if (giocatori[j].prigione == true) {  // necessario per casella prigione
    std::cin >> dado;
    int tiro = dist(gen);
    if (tiro == 5 || tiro == 6) {
      giocatori[j].prigione = false;
    } else {
      giocatori[j].turniinprigione--;
      if (giocatori[j].turniinprigione > 0) {
        return;
      } else if (giocatori[j].turniinprigione == 0) {
        giocatori[j].prigione = false;
      }
    }
  }  // se il giocatore non è in pausa o in prigione può muoversi
  std::cin >> dado;
  int numeroCasuale = dist(gen);
  giocatori[j].posizione += numeroCasuale;
  if (giocatori[j].posizione > lunghezza - 1) {  // in questo modo se si supera l'ultima casella si vince comunque e
                                                 // nella grafica ci si trova sulla casella fine
    giocatori[j].posizione = lunghezza - 1;
  }
  std::string tipo = tabellone[static_cast<size_t>(giocatori[j].posizione)];
  if (tipo == "Pausa") {
    giocatori[j].saltaturno = true;
  }
  if (tipo == "Scambio") {  // puoi scambiarti con qualsiasi giocatore(se un giocatore in prigione viene
                            // scambiato, cambia posizione ma deve comunque uscire di prigione)
    size_t altro_giocatore;
    do {
      std::uniform_int_distribution<size_t> scambio(0, giocatori.size() - 1);
      altro_giocatore = scambio(gen);
    } while (altro_giocatore == j);

    std::swap(giocatori[j].posizione, giocatori[altro_giocatore].posizione);
  }
  if (tipo == "Prigione") {
    giocatori[j].prigione = true;
    giocatori[j].turniinprigione = 3;
  }
  if (tipo == "Bonus") {
    movimento(giocatori, j, lunghezza, tabellone, ngiocatori, v);
  }
  if (tipo == "SCF") {
    EffettoSCF(j, giocatori);
  }
  if (tipo == "Serpente") {
    bool serpenteTrovato = false;
    for (int i = giocatori[j].posizione + 1; i < lunghezza; ++i) {
      if (tabellone[static_cast<size_t>(i)] == "Serpente") {
        giocatori[j].posizione = i;
        serpenteTrovato = true;
        break;
      }
    }
    if (!serpenteTrovato) {
    }
  }
  if (tipo == "Domanda") {
    domanda(giocatori, v, j);
  }
}
int GiocoSFC() {
  // 1=pareggio; 2=vittoria; 3=sconfitta
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(1, 3);
  int numero = dist(gen);
  std::string scelta;
  std::cin >> scelta;
  while (scelta != "sasso" && scelta != "carta" && scelta != "forbici") {
    std::cin >> scelta;
  }

  switch (numero) {
    case 1:  // sasso del computer
      if (scelta == "sasso") {
        return 1;
      } else if (scelta == "carta") {
        return 2;
      } else if (scelta == "forbici") {
        return 3;
      }
      break;
    case 2:  // forbici del computer
      if (scelta == "sasso") {
        return 3;
      } else if (scelta == "carta") {
        return 2;

      } else if (scelta == "forbici") {
        return 1;
      }
      break;
    case 3:  // carta del computer
      if (scelta == "sasso") {
        return 3;
      }
      if (scelta == "carta") {
        return 1;
      }
      if (scelta == "forbici") {
        return 2;
      }
      break;
  }
  return 0;
}
void EffettoSCF(size_t j, std::vector<giocatore>& giocatori) {
  int scf = GiocoSFC();
  if (scf == 1) {
  } else if (scf == 2) {
    giocatori[j].posizione += 2;
  } else if (scf == 3) {
    giocatori[j].posizione -= 1;
  }
}
bool quiz(const std::vector<std::string>& v) {
  // generatore di numeri casuali
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(0, static_cast<int>(v.size() / 8 - 1));

  int c = dist(gen);
  for (int i = c * 8 + 1; i < c * 8 + 6; ++i) {  // per come sono strutturate le domande nel file domande.txt
    std::cout << v[static_cast<long unsigned int>(i)] << "\n";
  }

  std::string risp;
  std::cin >> risp;

  while (risp != "A" && risp != "B" && risp != "C" && risp != "D") {
    std::cin >> risp;
  }
  std::string fRISP = "[Risposta " + risp + "]";
  if (v[static_cast<long unsigned int>(c * 8 + 6)] == fRISP) {
    return true;
  } else {
    return false;
  }
}
void domanda(std::vector<giocatore>& giocatori, const std::vector<std::string>& vecdomande, size_t j) {
  if (quiz(vecdomande)) {
    giocatori[j].posizione += 2;

  } else {
    giocatori[j].posizione -= 1;
  }
}
TEST_CASE("check funzioni domanda e pausa") {
  giocatore P1;
  P1.nome = "p1";
  P1.colore = 'R';
  giocatore P2;
  P2.nome = "p2";
  P2.colore = 'B';
  std::vector<giocatore> Players{P1, P2};
  std::vector<std::string> tabellone{"Via",    "Niente",  "Pausa", "Serpente", "Serpente", "Prigione", "Pausa",
                                     "Niente", "Scambio", "Bonus", "SCF",      "Domanda",  "Fine"};

  std::vector<std::string> dom1{
      "Domanda 1", "Quanti pianeti ci sono nel sistema solare?", "A 10", "B 5", "C 9", "D 8", "[Risposta D]", " "};
  SUBCASE("controllare fine Pausa") {
    std::cout << "check pausa\n\n";
    Players[0].saltaturno = true;
    movimento(Players, 0, static_cast<int>(tabellone.size()), tabellone, 2, dom1);
    CHECK(Players[0].saltaturno == false);
    std::cout << "\n";
  }
  SUBCASE("controllare domanda e quiz 1") {
    std::cout << "check dom1\n\n";
    Players[0].posizione = 1;
    std::istringstream input("D");
    std::cin.rdbuf(input.rdbuf());
    domanda(Players, dom1, 0);
    CHECK(Players[0].posizione == 3);
    std::cout << "\n";
  }
  SUBCASE("controllare domanda e quiz 2") {
    std::cout << "check dom2\n\n";
    Players[0].posizione = 1;
    std::istringstream input("A");
    std::cin.rdbuf(input.rdbuf());
    domanda(Players, dom1, 0);
    CHECK(Players[0].posizione == 0);
    std::cout << "\n";
  }
}
TEST_CASE("check prigione") {
  giocatore P1;
  P1.nome = "p1";
  P1.colore = 'R';
  giocatore P2;
  P2.nome = "p2";
  P2.colore = 'B';
  std::vector<giocatore> Players{P1, P2};
  std::vector<std::string> tabellone{"Via",      "Niente",  "Pausa", "Serpente", "Serpente", "Pausa", "Niente",
                                     "Prigione", "Scambio", "Bonus", "SCF",      "Domanda",  "Fine"};

  std::vector<std::string> dom1{
      "Domanda 1", "Quanti pianeti ci sono nel sistema solare?", "A 10", "B 5", "C 9", "D 8", "[Risposta D]", " "};
  SUBCASE("controllare termine Prigione") {
    std::cout << "check termine prigione\n\n";
    Players[0].prigione = true;
    Players[0].turniinprigione = 1;
    std::istringstream input("d");
    std::cin.rdbuf(input.rdbuf());
    movimento(Players, 0, static_cast<int>(tabellone.size()), tabellone, 2, dom1);

    CHECK(Players[0].posizione >= 1);
    CHECK(Players[0].posizione <= 6);
    CHECK(Players[0].posizione != 3);
    CHECK(Players[0].prigione == false);
    std::cout << "\n";
  }
  SUBCASE("controllare 3 turni di Prigione") {
    std::cout << "check 3 turni di Prigione\n\n";
    Players[0].prigione = true;
    Players[0].turniinprigione = 3;
    std::istringstream input("d");
    std::cin.rdbuf(input.rdbuf());
    movimento(Players, 0, static_cast<int>(tabellone.size()), tabellone, 2, dom1);
    bool prova;
    if (Players[0].prigione == true) {
      prova = (Players[0].posizione == 0);
    } else {
      prova = ((Players[0].posizione >= 1) && (Players[0].posizione <= 6) && (Players[0].posizione != 3));
    }
    CHECK(prova);
    std::cout << "\n";
  }
  SUBCASE("controllare 2 turni di Prigione") {
    std::cout << "check 2 turni di Prigione\n\n";
    Players[0].prigione = true;
    Players[0].turniinprigione = 2;
    std::istringstream input("d");
    std::cin.rdbuf(input.rdbuf());
    movimento(Players, 0, static_cast<int>(tabellone.size()), tabellone, 2, dom1);
    bool prova;
    if (Players[0].prigione == true) {
      prova = (Players[0].posizione == 0);
    } else {
      prova = ((Players[0].posizione >= 1) && (Players[0].posizione <= 6) && (Players[0].posizione != 3));
    }
    CHECK(prova);
    std::cout << "\n";
  }
}
TEST_CASE("check movimento") {
  giocatore P1;
  P1.nome = "p1";
  P1.colore = 'R';
  giocatore P2;
  P2.nome = "p2";
  P2.colore = 'B';
  std::vector<giocatore> Players{P1, P2};
  std::vector<std::string> tabellone{"Via",    "Niente",  "Pausa", "Serpente", "Serpente", "Prigione", "Pausa",
                                     "Niente", "Scambio", "Bonus", "SCF",      "Domanda",  "Fine"};

  std::vector<std::string> dom1{
      "Domanda 1", "Quanti pianeti ci sono nel sistema solare?", "A 10", "B 5", "C 9", "D 8", "[Risposta D]", " "};
  SUBCASE("controllare movimento") {
    std::cout << "check movimento\n\n";
    std::istringstream input("d");
    std::cin.rdbuf(input.rdbuf());
    movimento(Players, 0, static_cast<int>(tabellone.size()), tabellone, 2, dom1);
    CHECK(Players[0].posizione >= 1);
    CHECK(Players[0].posizione <= 6);
    CHECK(Players[0].posizione != 3);
    std::cout << "\n";
  }
}
TEST_CASE("check scambio") {
  giocatore P1;
  P1.nome = "p1";
  P1.colore = 'R';
  giocatore P2;
  P2.nome = "p2";
  P2.colore = 'B';
  std::vector<giocatore> Players{P1, P2};
  std::vector<std::string> tabellone{"Via", "Scambio", "Scambio", "Scambio", "Scambio", "Scambio", "Scambio"};

  std::vector<std::string> dom1{
      "Domanda 1", "Quanti pianeti ci sono nel sistema solare?", "A 10", "B 5", "C 9", "D 8", "[Risposta D]", " "};
  SUBCASE("controllare scambio") {
    std::cout << "test Scambio\n\n";
    std::istringstream input("d");
    std::cin.rdbuf(input.rdbuf());
    movimento(Players, 0, static_cast<int>(tabellone.size()), tabellone, 2, dom1);
    CHECK(Players[0].posizione == 0);
    CHECK(Players[1].posizione >= 1);
    std::cout << "\n";
  }
}
TEST_CASE("check Serpente") {
  giocatore P1;
  P1.nome = "p1";
  P1.colore = 'R';
  giocatore P2;
  P2.nome = "p2";
  P2.colore = 'B';
  std::vector<giocatore> Players{P1, P2};
  std::vector<std::string> tabellone{"Via",      "Serpente", "Serpente", "Serpente",
                                     "Serpente", "Serpente", "Serpente", "Serpente"};

  std::vector<std::string> dom1{
      "Domanda 1", "Quanti pianeti ci sono nel sistema solare?", "A 10", "B 5", "C 9", "D 8", "[Risposta D]", " "};
  SUBCASE("controllare serpente") {
    std::cout << "test serpente\n\n";
    std::istringstream input("d");
    std::cin.rdbuf(input.rdbuf());
    movimento(Players, 0, static_cast<int>(tabellone.size()), tabellone, 2, dom1);
    CHECK(Players[0].posizione >= 2);
    CHECK(Players[0].posizione <= 7);
    std::cout << "\n";
  }
}
