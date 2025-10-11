
#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <vector>


#include "tabellone.hpp"

namespace tabellone {
void infoeregolamento() {
  std::cout << "                           ____________________________________________________\n";
  std::cout << "                           |                                                  |\n";
  std::cout << "                           |             § IL GIOCO DEL SERPENTE §            |\n";
  std::cout << "                           |                    creato da                     |\n";
  std::cout << "                           |    Z. V. Mezdrichki, L. Marchetti, A. Rondini    |\n";
  std::cout << "                           |__________________________________________________|\n";
  std::cout << "                           ~~~~~~~~~~~~~~~~~~~REGOLE e INFO~~~~~~~~~~~~~~~~~~~~\n";
  std::cout << "1.Bisogna giocare in minimo 2 giocatori, massimo 4.\n";
  std::cout << "2.Seguite le istruzioni che compariranno a schermo sul terminale man mano che si "
               "procede nel gioco.\n";
  std::cout << "3.Ogni giocatore possiede un jolly, può essere utilizzato in qualsiasi momento della partita e "
               "permette di avanzare tra 1 a 9 caselle \no far indietreggiare un giocatore a scelta dello stesso "
               "numero di caselle.\n Dopo il jolly gli effetti delle caselle non vengono attivati.\n ";
  std::cout << "4.Tutti gli input dovranno essere inseriti dal terminale.\n";
  std::cout << "5.Ogni volta che si finisce su una casella che modifica  la propria posizione, "
               "si è immuni all'effetto della casella di arrivo.\n";
  std::cout << "6.La finestra contente l'immagine del tabellone  aggiornato, dopo 3 secondi si "
               "chiuderà in automatico,  se non è gia stata chiusa.\n";
  std::cout
      << "7.Il gioco termina quando un giocatore arriva precisamente all'ultima casella\n(non si può terminare il "
         "gioco con il dado bonus oppure con l'effetto di una casella).\n";
  std::cout << "                       Effetti caselle:\n";
  std::cout << "-Domanda:se rispondi correttamente avanzi di 3, se sbagli indietreggi di 1.\n";
  std::cout << "-SCF:sasso, carta, forbici con il computer, se vinci avanzi di 2, se sbagli "
               "indietreggi di 1.\n";
  std::cout << "-Scambio:scambia la posizione con un giocatore casuale.\n";
  std::cout << "-Pausa:salta un turno.\n";
  std::cout << "-Prigione:Rimani in prigione per 3 turni oppure fino a quando non fai 5 o 6.\n "
               "Quando sei in prigione non puoi muoverti.\n";
  std::cout << "-Bonus:Ritira i dadi.\n";
  std::cout << "-Serpente:Finisci alla successiva casella serpente se è presente.\n";

  std::cout << "\n        BUON DIVERIMENTO E BUONA FORTUNA!!!            \n\n";
}
long unsigned int richiestaGiocatori() {
  long unsigned int ngiocatori;
  std::cout << "Inserisci il numero di giocatori(da 2 a 4): ";
  std::cin >> ngiocatori;
  while (std::cin.fail() || (ngiocatori != 2 || ngiocatori != 3 ||
                             ngiocatori != 4)) {  // cin.fail() è true quando l input da parte dell'utente non è
                                                  // dello stesso tipo dell'input richiesto
    if (ngiocatori == 2 || ngiocatori == 3 || ngiocatori == 4) {
      break;
    } else {
      std::cout << "Valore non valido. Inserisci 2, 3, o 4: ";
      std::cin.clear();  //
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                      '\n'); /* std::cin.ignore(): Questo metodo estrae e scarta il resto della
            linea corrente dal buffer di input. In altre parole, ignora tutto ciò che rimane sulla stessa riga
            dopo l’ultimo input letto(arriva a a capo e finisce). std::numeric_limits<std::streamsize>::max():
            Questo imposta il numero massimo di caratteri da ignorare. In questo caso, stai dicendo a cin di
            ignorare tutti i caratteri rimanenti sulla linea, senza limiti specifici.*/
      std::cin >> ngiocatori;
    }
  }
  return ngiocatori;
}

int richiestaLunghezza() {
  int lunghezza;
  std::cout << "Inserisci il numero di caselle(32-43-54-65): ";
  std::cin >> lunghezza;
  while (std::cin.fail() || (lunghezza != 32 || lunghezza != 43 || lunghezza != 54 ||
                             lunghezza != 65)) {  // cin.fail() è true quando l input da parte dell'utente non è
                                                  // dello stesso tipo dell'input richiesto
    if (lunghezza == 32 || lunghezza == 43 || lunghezza == 54 || lunghezza == 65) {
      break;
    } else {
      std::cout << "Valore non valido. Inserisci 32, 43, 54 o 65: ";
      std::cin.clear();  //
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cin >> lunghezza;
    }
  }
  return lunghezza;
}

std::vector<giocatore> getplayers(long unsigned int Ngiocatori) {
  std::vector<char> PedineDisponibili{'R', 'B', 'V', 'G'};  // Vettore con i colori disponibili
  std::vector<giocatore> giocatori;
  std::vector<std::string> frasi{"-R per rosso  ", "-B per blu  ", "-V per verde  ", "-G per giallo"};
  for (size_t i = 0; i < Ngiocatori; ++i) {
    giocatore g;
    char ColoreTemp;

    std::cout << "nome giocatore " << i + 1 << "?" << '\n';
    std::cin >> g.nome;

    // Ciclo per scegliere un colore disponibile
    bool colore_scelto = false;
    do {
      for (std::string frase : frasi) {
        std::cout << frase;
      }
      std::cout << std::endl;
      std::cin >> ColoreTemp;
      std::cin.ignore(256, '\n');  // scarta 256 caratteri della linea corrente dal buffer di input

      // Controlla se il colore è ancora disponibile
      bool colore_disponibile = false;
      for (size_t j = 0; j < PedineDisponibili.size(); ++j) {
        if (ColoreTemp == PedineDisponibili[j]) {
          colore_disponibile = true;
          PedineDisponibili.erase(PedineDisponibili.begin() +
                                  static_cast<long int>(j));  // Rimuove il colore scelto dal vettore
          frasi.erase(frasi.begin() + static_cast<long int>(j));
          break;
        }
      }
      if (!colore_disponibile) {
        std::cout << "Scelta non valida" << '\n';
      } else {
        colore_scelto = true;
      }
    } while (!colore_scelto);  // Termina il ciclo se il colore è stato scelto correttamente
    g.colore = ColoreTemp;
    giocatori.push_back(g);
  }
  return giocatori;
}

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
std::vector<std::string> Creatabellone(long unsigned int lunghezza) {
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

sf::Color ColoreCaselle(const std::string& tipo) {
  if (tipo == "Via") {
    return sf::Color::Red;
  } else if (tipo == "Niente") {
    return sf::Color::White;
  } else if (tipo == "Scambio") {
    return sf::Color(255, 128, 0);  // arancione
  } else if (tipo == "Serpente") {
    return sf::Color::Yellow;
  } else if (tipo == "Prigione") {
    return sf::Color::Black;
  } else if (tipo == "Bonus") {
    return sf::Color::Cyan;
  } else if (tipo == "Pausa") {
    return sf::Color::Green;
  } else if (tipo == "Domanda") {
    return sf::Color::Magenta;
  } else if (tipo == "SCF") {
    return sf::Color::Blue;
  } else {
    return sf::Color::Red;
  }
}
void ColorePedina(std::vector<giocatore>& giocatori, std::vector<Pedina>& pedine, long unsigned int i) {
  sf::Texture Rosso;  // modo per implementare le texture
  if (!Rosso.loadFromFile("immagini_e_font/pedinaserperosso.png")) {
  }
  sf::Texture Blu;
  if (!Blu.loadFromFile("immagini_e_font/pedinaserpeblu.png")) {
  }
  sf::Texture Giallo;
  if (!Giallo.loadFromFile("immagini_e_font/pedinaserpegiallo.png")) {
  }
  sf::Texture Verde;
  if (!Verde.loadFromFile("immagini_e_font/pedinaserpeverde.png")) {
  }
  if (giocatori[i].colore == 'R') {
    pedine[i].texture = Rosso;

  } else if (giocatori[i].colore == 'B') {
    pedine[i].texture = Blu;

  } else if (giocatori[i].colore == 'G') {
    pedine[i].texture = Giallo;

  } else if (giocatori[i].colore == 'V') {
    pedine[i].texture = Verde;
  }
}
int coefficentesottrazionepedine(std::vector<giocatore>& giocatori, size_t i) {
  if (giocatori[i].posizione < 10) {
    return 0;
  } else if (giocatori[i].posizione > 10 && giocatori[i].posizione < 21) {
    return 1;
  } else if (giocatori[i].posizione > 21 && giocatori[i].posizione < 32) {
    return 2;
  } else if (giocatori[i].posizione > 32 && giocatori[i].posizione < 43) {
    return 3;
  } else if (giocatori[i].posizione > 43 && giocatori[i].posizione < 54) {
    return 4;
  } else {
    return 5;
  }  // necessario per far tornare i conti per il movimento delle pedine
}
sf::Color Coloreleggenda(int i) {
  if (i == 0) {
    return sf::Color::Red;
  } else if (i == 1) {
    return sf::Color::White;
  } else if (i == 2) {
    return sf::Color(255, 128, 0);  // arancione
  } else if (i == 3) {
    return sf::Color::Yellow;
  } else if (i == 4) {
    return sf::Color::Black;
  } else if (i == 5) {
    return sf::Color::Cyan;
  } else if (i == 6) {
    return sf::Color::Green;
  } else if (i == 7) {
    return sf::Color::Magenta;
  } else {
    return sf::Color::Blue;
  }
}
std::string Testoleggenda(int i) {
  if (i == 0) {
    return "Via o fine!";
  } else if (i == 1) {
    return "Niente!";
  } else if (i == 2) {
    return "Scambio!";
  } else if (i == 3) {
    return "Serpente!";
  } else if (i == 4) {
    return "Prigione!";
  } else if (i == 5) {
    return "Bonus!";
  } else if (i == 6) {
    return "Pausa!";
  } else if (i == 7) {
    return "Domanda!";
  } else {
    return "SCF!";
  }
}
void Mostratabellone(const int lunghezza, const long unsigned int ngiocatori, std::vector<giocatore>& giocatori,
                     const std::vector<std::string>& tabellone) {
  // Creazione della finestra
  sf::RenderWindow finestragrafica(sf::VideoMode(800, 600), "Gioco del serpente");
  finestragrafica.setSize(sf::Vector2u(924, 768));
  finestragrafica.setPosition(sf::Vector2i(850, 100));
  sf::Clock timer;
  // Ciclo principale del gioco
  while (finestragrafica.isOpen()) {
    // Gestione degli eventi
    sf::Event event;
    while (finestragrafica.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        finestragrafica.close();
      }
    }
    sf::Time tempotrascorso = timer.getElapsedTime();  // dopo 10 s il tabellone si chiude automaticamente
    if (tempotrascorso.asSeconds() >= 4.0f) {
      finestragrafica.close();
    }
    finestragrafica.clear();
    sf::RectangleShape sfondo(sf::Vector2f(800, 600));
    sf::Texture Sfondo;
    if (!Sfondo.loadFromFile("immagini_e_font/sfondo7.png")) {
    }
    sf::Sprite immagine(Sfondo);
    finestragrafica.draw(immagine);
    for (int colonne = 0; colonne < 10; ++colonne) {
      if (colonne > 0 && colonne < 9) {  // le 8 caselle centrali di ogni riga che si crea a 2 a 2
        for (int righe = 0; righe < 2 * (lunghezza / 10); righe = righe + 2) {
          // Creazione del rettangolo per la casella
          sf::RectangleShape casellagrafica(sf::Vector2f(49, 49));
          casellagrafica.setPosition(
              static_cast<float>(180.2 + 50 * colonne),
              static_cast<float>(29.2 + 50 * righe));  // Posizione leggermente spostata per centrare i quadrati

          if (righe % 4 == 0) {
            std::string tipo = tabellone[static_cast<long unsigned int>(righe * 5 + colonne + ((righe * 5) / 10))];
            casellagrafica.setFillColor(ColoreCaselle(tipo));
          }
          if (righe % 4 != 0) {
            std::string tipo =
                tabellone[static_cast<long unsigned int>(righe * 5 + (9 - colonne) + ((righe * 5) / 10))];
            casellagrafica.setFillColor(ColoreCaselle(tipo));
          }  // in questo modo la posizione della casella in base a righe e colonne coincide con
             // la posizione della stringa scelta nel vettore tabellone

          // Creazione del rettangolo per il bordo
          sf::RectangleShape bordocasellag(sf::Vector2f(49, 49));
          bordocasellag.setPosition(casellagrafica.getPosition());  // Posizione uguale al quadrato
          bordocasellag.setFillColor(sf::Color::Transparent);       // Trasparente per non coprire il quadrato
          bordocasellag.setOutlineColor(sf::Color::Black);          // Colore del bordo nero
          bordocasellag.setOutlineThickness(2);                     // Spessore del bordo
                                                                    // Disegno del bordo e del quadrato
          finestragrafica.draw(bordocasellag);
          finestragrafica.draw(casellagrafica);
        }
      }
      if (colonne == 9) {  // Caso separato perchè presenta più caselle
        for (int righe = 0; righe < 2 * (lunghezza / 10) - 1; ++righe) {
          if ((righe + 1) % 4 != 0) {  // crea 3 caselle e ne salta una
            // Creazione del rettangolo per la casella
            sf::RectangleShape casellagrafica(sf::Vector2f(49, 49));
            casellagrafica.setPosition(
                static_cast<float>(180.2 + 50 * colonne),
                static_cast<float>(29.2 + 50 * righe));  // Posizione leggermente spostata per centrare il quadrato

            std::string tipo = tabellone[static_cast<long unsigned int>(
                (righe - (righe % 4)) * 5 + (((righe - (righe % 4)) * 5) / 10) + 9 + righe % 4)];
            casellagrafica.setFillColor(ColoreCaselle(tipo));

            // in questo modo la posizione della casella in base a righe e colonne coincide con la
            // posizione della stringa scelta nel vettore tabellone

            // Creazione del rettangolo per il bordo
            sf::RectangleShape bordocasellag(sf::Vector2f(49, 49));
            bordocasellag.setPosition(casellagrafica.getPosition());  // Posizione uguale al quadrato
            bordocasellag.setFillColor(sf::Color::Transparent);       // Trasparente per non coprire il quadrato
            bordocasellag.setOutlineColor(sf::Color::Black);          // Colore del bordo nero
            bordocasellag.setOutlineThickness(2);                     // Spessore del bordo
                                                                      // Disegno del bordo e del quadrato
            finestragrafica.draw(bordocasellag);
            finestragrafica.draw(casellagrafica);
          }
        }
      }

      if (colonne == 0) {
        for (int righe = 0; righe < 2 * (lunghezza / 10) - 1; ++righe) {
          if ((righe - 1) % 4 != 0) {  // crea 3 caselle e ne salta una, salta dalla seconda riga
                                       // perchè la prima riga ha valore 0
            // Creazione del rettangolo per la casella
            sf::RectangleShape casellagrafica(sf::Vector2f(49, 49));
            casellagrafica.setPosition(static_cast<float>(180.2 + 50 * colonne), static_cast<float>(29.2 + 50 * righe));
            // Posizione leggermente spostata per centrare il quadrato
            if (righe % 4 == 0) {
              std::string tipo = tabellone[static_cast<long unsigned int>(righe * 5 + ((righe * 5) / 10))];
              casellagrafica.setFillColor(ColoreCaselle(tipo));
            }
            if (righe % 4 == 3 || righe % 4 == 2) {
              std::string tipo = tabellone[static_cast<long unsigned int>(
                  (righe + (4 - (righe % 4))) * 5 + (((righe + (4 - (righe % 4))) * 5) / 10) - (4 - (righe % 4)))];
              casellagrafica.setFillColor(ColoreCaselle(tipo));
            }

            // Creazione del rettanggolo per il bordo
            sf::RectangleShape bordocasellag(sf::Vector2f(49, 49));
            bordocasellag.setPosition(casellagrafica.getPosition());  // Posizione uguale al quadrato
            bordocasellag.setFillColor(sf::Color::Transparent);       // Trasparente per non coprire il quadrato
            bordocasellag.setOutlineColor(sf::Color::Black);          // Colore del bordo nero
            bordocasellag.setOutlineThickness(2);                     // Spessore del bordo
                                                                      // Disegno del bordo e del quadrato
            finestragrafica.draw(bordocasellag);
            finestragrafica.draw(casellagrafica);
          }
        }
      }
    }

    std::vector<Pedina> pedine(ngiocatori);
    for (size_t i = 0; i < pedine.size(); i++) {
      switch (i) {
        case 0:
          ColorePedina(giocatori, pedine, i);
          break;
        case 1:
          ColorePedina(giocatori, pedine, i);
          break;
        case 2:
          ColorePedina(giocatori, pedine, i);
          break;
        case 3:
          ColorePedina(giocatori, pedine, i);
          break;
      }
      pedine[i].sprite.setTexture(pedine[i].texture);
      pedine[i].sprite.setScale(
          sf::Vector2f(0.025f, 0.025f));  // serve per rendere la dimensione della foto 1/4 di casella
    }
    for (size_t i = 0; i < ngiocatori; ++i) {
      // 2 pedine in alto
      if ((giocatori[i].posizione + 1) % 11 == 0 && (giocatori[i].posizione + 1) % 22 != 0) {
        pedine[i].sprite.setPosition(
            179.8f + static_cast<float>(25 * (i % 2)) + 9 * 50.f,
            29.0f + static_cast<float>(i / 2) * 25.0f + static_cast<float>((giocatori[i].posizione / 6) * 50));
        finestragrafica.draw(pedine[i].sprite);
      } else if ((giocatori[i].posizione + 1) % 22 == 0) {
        pedine[i].sprite.setPosition(
            179.8f + static_cast<float>(25 * (i % 2)),
            29.0f + static_cast<float>(i / 2) * 25.0f + static_cast<float>((giocatori[i].posizione / 6) * 50));
        finestragrafica.draw(pedine[i].sprite);
      }

      else if (((((giocatori[i].posizione) - (giocatori[i].posizione / 10)) / 10) % 2) == 0) {
        pedine[i].sprite.setPosition(
            179.8f + static_cast<float>(25 * (i % 2)) +
                static_cast<float>((((giocatori[i].posizione) - coefficentesottrazionepedine(giocatori, i)) % 10)) *
                    50.f,
            29.0f + static_cast<float>(i / 2) * 25.0f +
                static_cast<float>((((giocatori[i].posizione) - (giocatori[i].posizione / 10)) / 10) * 100));
        finestragrafica.draw(pedine[i].sprite);
      } else if (((((giocatori[i].posizione) - (giocatori[i].posizione / 10)) / 10) % 2) != 0) {
        pedine[i].sprite.setPosition(
            179.8f + static_cast<float>(25 * (i % 2)) + 9 * 50.f -
                static_cast<float>(
                    ((((giocatori[i].posizione) - coefficentesottrazionepedine(giocatori, i)) % 10) * 50)),
            29.0f + static_cast<float>(i / 2) * 25.0f +
                static_cast<float>((((giocatori[i].posizione) - (giocatori[i].posizione / 10)) / 10) * 100));
        finestragrafica.draw(pedine[i].sprite);

      }  // formule permettono alla pedina di muoversi sul tabellone a forma di serpente centrando
         // sempre le caselle
    }
    sf::RectangleShape bordolegenda(sf::Vector2f(150, 400));
    bordolegenda.setPosition(static_cast<float>(10), static_cast<float>(29.2));
    bordolegenda.setFillColor(sf::Color::White);
    bordolegenda.setOutlineColor(sf::Color::Black);
    bordolegenda.setOutlineThickness(2);
    finestragrafica.draw(bordolegenda);
    sf::Font font;
    if (!font.loadFromFile("immagini_e_font/arial.ttf")) {
    }  // modo per poter inserire un font per scrivere
    for (int i = 0; i < 9; ++i) {
      sf::RectangleShape casellalegenda(sf::Vector2f(20, 30));
      casellalegenda.setPosition(static_cast<float>(20), static_cast<float>(49.2 + 40 * i));
      casellalegenda.setFillColor(Coloreleggenda(i));
      sf::RectangleShape bordocasella(sf::Vector2f(20, 30));
      bordocasella.setPosition(casellalegenda.getPosition());  // Posizione uguale al quadrato
      bordocasella.setFillColor(sf::Color::Transparent);       // Trasparente per non coprire il quadrato
      bordocasella.setOutlineColor(sf::Color::Black);          // Colore del bordo nero
      bordocasella.setOutlineThickness(2);
      sf::Text testo;
      testo.setFont(font);
      testo.setString(Testoleggenda(i));
      testo.setCharacterSize(16);  // in pixel, non in punti!
      testo.setFillColor(sf::Color::Black);
      testo.setPosition(50, static_cast<float>(54.2 + 40 * i));
      finestragrafica.draw(casellalegenda);
      finestragrafica.draw(bordocasella);
      finestragrafica.draw(testo);
    }
    // mostra la finestra grafica
    finestragrafica.display();
  }
}
void mostrapodio(std::vector<giocatore>& vincitori) {
  sf::RenderWindow immaginepodio(sf::VideoMode(626, 626), "Podio");
  immaginepodio.setSize(sf::Vector2u(924, 808));
  immaginepodio.setPosition(sf::Vector2i(850, 100));

  // Ciclo principale del gioco
  while (immaginepodio.isOpen()) {
    // Gestione degli eventi
    sf::Event event;
    while (immaginepodio.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        immaginepodio.close();
      }
    }
    immaginepodio.clear();
    sf::RectangleShape podio(sf::Vector2f(626, 626));
    sf::Texture Sfondo;
    if (!Sfondo.loadFromFile("immagini_e_font/podio.png")) {
    }
    sf::Sprite immagine(Sfondo);
    immaginepodio.draw(immagine);

    std::vector<Pedina> pedine(vincitori.size());
    for (size_t i = 0; i < pedine.size(); i++) {
      switch (i) {
        case 0:
          ColorePedina(vincitori, pedine, i);
          break;
        case 1:
          ColorePedina(vincitori, pedine, i);
          break;
        case 2:
          ColorePedina(vincitori, pedine, i);
          break;
        case 3:
          ColorePedina(vincitori, pedine, i);
          break;
      }
      pedine[i].sprite.setTexture(pedine[i].texture);
      pedine[i].sprite.setScale(sf::Vector2f(0.075f, 0.075f));
    }
    sf::Font font;
    if (!font.loadFromFile("immagini_e_font/arial.ttf")) {
    }
    for (size_t j = 0; j < pedine.size(); ++j) {
      sf::Text testo;
      testo.setFont(font);
      testo.setString(vincitori[j].nome);
      testo.setCharacterSize(26);  // in pixel, non in punti!
      testo.setFillColor(sf::Color::Black);
      if (j % 2 != 0) {
        pedine[j].sprite.setPosition(275.f - static_cast<float>(j) * 160.f, 310.0f + (static_cast<float>(j) * 25.f));
        immaginepodio.draw(pedine[j].sprite);
        testo.setPosition(pedine[j].sprite.getPosition().x, pedine[j].sprite.getPosition().y - 35.f);
        immaginepodio.draw(testo);
      }

      if (j % 2 == 0) {
        pedine[j].sprite.setPosition(275.f + static_cast<float>(j) * 80.f, 310.0f + (static_cast<float>(j) * 25.f));
        immaginepodio.draw(pedine[j].sprite);
        testo.setPosition(pedine[j].sprite.getPosition().x, pedine[j].sprite.getPosition().y - 35.f);
        immaginepodio.draw(testo);
      }
    }
    immaginepodio.display();
  }
}
}  // namespace tabellone