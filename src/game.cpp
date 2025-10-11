
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "gioco.hpp"
#include "tabellone.hpp"
namespace gioco {
void movimento(std::vector<giocatore>& giocatori, size_t j, const int lunghezza,
               const std::vector<std::string>& tabellone, const long unsigned int ngiocatori,
               const std::vector<std::string>& v) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(1, 6);
  std::string dado;
  if (giocatori[j].saltaturno == true) {  // necessario per casella pausa
    std::cout << giocatori[j].nome << " è in pausa e salta un turno" << '\n';
    giocatori[j].saltaturno = false;
    return;
  } else if (giocatori[j].prigione == true) {  // necessario per casella prigione
    std::cout << giocatori[j].nome << " è in prigione, lancia il dado per provare ad uscire" << '\n';
    std::cin >> dado;
    int tiro = dist(gen);
    std::cout << "Hai fatto " << tiro << '\n';
    if (tiro == 5 || tiro == 6) {
      std::cout << "Sei uscito di prigione!" << '\n';
      giocatori[j].prigione = false;
    } else {
      giocatori[j].turniinprigione--;
      if (giocatori[j].turniinprigione > 0) {
        std::cout << "Devi stare in prigione per altri " << giocatori[j].turniinprigione << " turni" << '\n';
        return;
      } else if (giocatori[j].turniinprigione == 0) {
        std::cout << "Sei uscito di prigione perchè hai scontato i turni !" << '\n';
        giocatori[j].prigione = false;
      }
    }
  }  // se il giocatore non è in pausa o in prigione può muoversi
  if (giocatori[j].bonus == true) {
    usajolly(giocatori, j, lunghezza);
    if (giocatori[j].bonus == false) {
      tabellone::Mostratabellone(lunghezza, ngiocatori, giocatori, tabellone);
    }
  }
  std::cout << "Tocca a " << giocatori[j].nome << " clicca un tasto qualsiasi e invia per lanciare il dado\n";
  std::cin >> dado;
  int numeroCasuale = dist(gen);
  std::cout << giocatori[j].nome << " ha fatto " << numeroCasuale << '\n';
  giocatori[j].posizione += numeroCasuale;
  if (giocatori[j].posizione <= lunghezza - 1) {
    std::cout << giocatori[j].nome << " si trova a " << giocatori[j].posizione << '\n';
  }
  if (giocatori[j].posizione > lunghezza - 1) {
    int eccesso = giocatori[j].posizione - (lunghezza - 1);
    std::cout << giocatori[j].nome << " ha superato " << lunghezza - 1 << '\n';
    giocatori[j].posizione = (lunghezza - 1) - eccesso;
    std::cout << giocatori[j].nome << " torna a " << giocatori[j].posizione << '\n';
  }
  std::string tipo = tabellone[static_cast<size_t>(giocatori[j].posizione)];
  if (tipo == "Pausa") {
    giocatori[j].saltaturno = true;
    std::cout << giocatori[j].nome << " è atterrato su una casella pausa e salta un turno" << '\n';
  }
  if (tipo == "Scambio") {  // puoi scambiarti con qualsiasi giocatore(se un giocatore in prigione viene
                            // scambiato, cambia posizione ma deve comunque uscire di prigione)
    size_t altro_giocatore;
    do {
      std::uniform_int_distribution<size_t> scambio(0, giocatori.size() - 1);
      altro_giocatore = scambio(gen);
    } while (altro_giocatore == j);

    std::cout << giocatori[j].nome << " scambia la posizione con " << giocatori[altro_giocatore].nome << '\n';
    std::cout << "Le vecchie posizioni erano: " << giocatori[j].nome << "-casella: " << giocatori[j].posizione << '\n';
    std::cout << giocatori[altro_giocatore].nome << "-casella: " << giocatori[altro_giocatore].posizione << '\n';
    std::swap(giocatori[j].posizione, giocatori[altro_giocatore].posizione);
    std::cout << "Le nuove posizioni sono: " << giocatori[j].nome << "-casella: " << giocatori[j].posizione << '\n';
    std::cout << giocatori[altro_giocatore].nome << "-casella: " << giocatori[altro_giocatore].posizione << '\n';
  }
  if (tipo == "Prigione") {
    giocatori[j].prigione = true;
    giocatori[j].turniinprigione = 3;
    std::cout << giocatori[j].nome
              << " è atterrato su una prigione e resta in prigione per 3 turni o fino a che non "
                 "tira 5 o 6"
              << '\n';
  }
  if (tipo == "Bonus") {
    std::cout << "Sei finito su una casella bonus, puoi lanciare nuovamente i dadi!" << '\n';
    tabellone::Mostratabellone(lunghezza, ngiocatori, giocatori, tabellone);
    movimento(giocatori, j, lunghezza, tabellone, ngiocatori, v);
  }
  if (tipo == "SCF") {
    std::cout << "Sei finito su una casella Sasso, Carta, Forbici" << '\n';
    tabellone::Mostratabellone(lunghezza, ngiocatori, giocatori, tabellone);
    EffettoSCF(j, giocatori);
    if (giocatori[j].posizione >= lunghezza - 1) {
      std::cout << "Hai raggiunto o superato la casella fine, verrai riportato indietro\n";
      giocatori[j].posizione -= 2;
    }
  }
  if (tipo == "Serpente") {
    std::cout << "Sei finito su una casella serpente, verrai portato a quella successiva!" << '\n';
    tabellone::Mostratabellone(lunghezza, ngiocatori, giocatori, tabellone);
    bool serpenteTrovato = false;
    for (int i = giocatori[j].posizione + 1; i < lunghezza; ++i) {
      if (tabellone[static_cast<size_t>(i)] == "Serpente") {
        giocatori[j].posizione = i;
        std::cout << "Sei finito sulla casella numero " << giocatori[j].posizione << '\n';
        serpenteTrovato = true;
        break;
      }
    }
    if (!serpenteTrovato) {
      std::cout << "Non ci sono caselle serpente disponibili!" << '\n';
    }
  }
  if (tipo == "Domanda") {
    std::cout << "Sei finito su una casella Domanda" << '\n';
    tabellone::Mostratabellone(lunghezza, ngiocatori, giocatori, tabellone);
    domanda(giocatori, v, j);
    if (giocatori[j].posizione >= lunghezza - 1) {
      std::cout << "Hai raggiunto o superato la casella fine, verrai riportato indietro\n";
      giocatori[j].posizione -= 3;
    }
  }
}
int GiocoSFC() {
  // 1=pareggio; 2=vittoria; 3=sconfitta
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(1, 3);
  int numero = dist(gen);
  std::string scelta;
  std::cout << "Sasso, carta o forbici?(scrivi la parola in minuscolo) \n";
  std::cin >> scelta;
  while (scelta != "sasso" && scelta != "carta" && scelta != "forbici") {
    std::cout << "Sasso, carta o forbici? \n";
    std::cin >> scelta;
  }

  switch (numero) {
    case 1:  // sasso del computer
      if (scelta == "sasso") {
        std::cout << "La mia scelta è sasso. \n";
        std::cout << "Abbiamo pareggiato!!! \n";
        return 1;
      } else if (scelta == "carta") {
        std::cout << "La mia scelta è sasso. \n";
        std::cout << "Hai vinto!!!\n";
        return 2;
      } else if (scelta == "forbici") {
        std::cout << "La mia scelta è sasso. \n";
        std::cout << "Hai perso!!!\n";
        return 3;
      }
      break;
    case 2:  // forbici del computer
      if (scelta == "sasso") {
        std::cout << "la mia scelta è forbici. \n";
        std::cout << "Hai perso!!!\n";
        return 3;
      } else if (scelta == "carta") {
        std::cout << "La mia scelta è forbici. \n";
        std::cout << "Hai vinto!!!\n";
        return 2;

      } else if (scelta == "forbici") {
        std::cout << "La mia scelta è forbici. \n";
        std::cout << "Abbiamo pareggiato!!! \n";
        return 1;
      }
      break;
    case 3:  // carta del computer
      if (scelta == "sasso") {
        std::cout << "La mia scelta è carta. \n";
        std::cout << "Hai perso!!!\n";
        return 3;
      }
      if (scelta == "carta") {
        std::cout << "La mia scelta è carta. \n";
        std::cout << "Abbiamo pareggiato!!! \n";
        return 1;
      }
      if (scelta == "forbici") {
        std::cout << "La mia scelta è carta. \n";
        std::cout << "Hai vinto!!!\n";
        return 2;
      }
      break;
  }
  return 0;
}
void EffettoSCF(size_t j, std::vector<giocatore>& giocatori) {
  int scf = GiocoSFC();
  if (scf == 1) {
    std::cout << "La tua posizione non è cambiata, è rimasta " << giocatori[j].posizione << '\n';
  } else if (scf == 2) {
    giocatori[j].posizione += 2;
    std::cout << "La tua nuova posizione è " << giocatori[j].posizione << '\n';
  } else if (scf == 3) {
    giocatori[j].posizione -= 1;
    std::cout << "La tua nuova posizione è " << giocatori[j].posizione << '\n';
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
  std::cout << "\n Inserisci la tua risposta (in maiuscolo)\n";
  std::cin >> risp;

  while (risp != "A" && risp != "B" && risp != "C" && risp != "D") {
    std::cout << "La risposta inserita non è tra le opzioni, oppure è stata inserita in modo "
                 "incorretto\n";
    std::cout << "Inserisci la tua risposta (in maiuscolo)\n";
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
    std::cout << "corretto\n";
    giocatori[j].posizione += 3;
    std::cout << "La tua nuova posizione è " << giocatori[j].posizione << '\n';
  } else {
    std::cout << "sbagliato\n";
    giocatori[j].posizione -= 1;
    std::cout << "La tua nuova posizione è " << giocatori[j].posizione << '\n';
  }
}
void usajolly(std::vector<giocatore>& giocatori, size_t j, const int lunghezza) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> jollynum(1, 9);
  std::cout << "Tocca a " << giocatori[j].nome << " vuoi utilizzare il jolly?(s/n) \n";
  char jolly;
  std::cin >> jolly;
  while (jolly != 's' && jolly != 'n' && !std::cin.fail()) {
    std::cout << "Scelta non valida, riprovare \n";
    std::cin >> jolly;
  }
  if (jolly == 'n') {
    return;
  }
  if (jolly == 's') {
    std::cout << "Quale jolly vuoi utlizzare?(b per bonus, m per malus) \n";
    char jollyscelta;
    std::cin >> jollyscelta;
    while (jollyscelta != 'b' && jolly != 'm' && !std::cin.fail()) {
      std::cout << "Scelta non valida, riprovare \n";
      std::cin >> jollyscelta;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    if (jollyscelta == 'b') {
      std::cout << "Per lanciare il dado bonus, " << giocatori[j].nome << ", prema un tasto qualsiasi e invio \n";
      std::string dado;
      std::cin >> dado;
      int numerobonus = jollynum(gen);
      giocatori[j].posizione += numerobonus;
      if (giocatori[j].posizione >= lunghezza - 1) {
        std::cout << "Hai raggiunto o superato la casella fine, il bonus viene annullato \n";
        giocatori[j].posizione -= numerobonus;
        giocatori[j].bonus = false;

      } else {
        std::cout << "Hai fatto " << numerobonus << '\n';
        std::cout << "La tua nuova posizione è " << giocatori[j].posizione << '\n';
        giocatori[j].bonus = false;
      }
    }
    if (jollyscelta == 'm') {
      std::cout << "Scegli un altro dei giocatori, il quale dovrà subire il malus \n";
      for (giocatore player : giocatori) {
        std::cout << "-" << player.nome << " ";
      }
      std::cout << std::endl;
      std::string nomegiocatore;
      std::cin >> nomegiocatore;
      auto it = std::find_if(giocatori.begin(), giocatori.end(),
                             [&](const giocatore& player) { return player.nome == nomegiocatore; });
      while (it == giocatori.end() || it->nome == giocatori[j].nome) {
        if (it == giocatori.end()) {
          std::cout << "Giocatore non trovato, riprova \n";
        } else if (it->nome == giocatori[j].nome) {
          std::cout << "Non puoi selezionare te stesso, riprova \n";
        }
        std::cin >> nomegiocatore;
        it = std::find_if(giocatori.begin(), giocatori.end(),
                          [&](const giocatore& player) { return player.nome == nomegiocatore; });
      }
      int malusjolly = jollynum(gen);
      std::cout << it->nome << " dovrà indietreggiare di " << malusjolly << " caselle \n";
      it->posizione -= malusjolly;
      if (it->posizione < 0) {
        it->posizione = 0;
      }
      std::cout << "La sua nuova posizione è " << it->posizione << '\n';
      giocatori[j].bonus = false;
    }
  }
}
}  // namespace gioco