## 🐍 **The Game of the Snake**

A C++ console board game inspired by The Game of the Goose. Players roll dice to move across a dynamic board filled with traps, bonuses, and random events.

## 📦 **Installation**

🛠️ **Prerequisites**

Ensure you have the following installed:

A C++ compiler (e.g., GCC, Clang, MSVC)

CMake (for building the project)

SFML

📥 **Clone the Repository**

Clone the repository to your local machine:
```bash
git clone https://github.com/Zazza2003/The-game-of-the-Snake.git
```
🧱 **Build the Project**

Before starting, change the name of the assets directory in "immagini_e_font".

Navigate to the project directory:
```bash
cd The-game-of-the-Snake
```
Run CMake to configure the project:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYP
cmake --build build -j$(nproc)E=Release
```
## 🎮 **How to Play**

Run the game executable:
```bash
./build/bin/gioco
```
Follow the on-screen instructions to start a new game. (Language:Italian)
## 🧪 **Running Tests**

The project includes unit tests using the doctest
 framework.

To run tests:
```bash
cmake --build build --target test
```
## 📄 **Documentation**

Detailed documentation is available in the relazione folder(in italian). 

Some names have been changed, additionally the part about bonuses/maluses and the final podium is missing, which were added later.

It was a group project carried out during the Programming course of the Bachelor's degree in Physics at UNIBO. 

I improved the code and implemented new features to make the gaming experience more enjoyable.

# File structure
```plaintext
The-game-of-the-Snake/               
├── assets/
|   └── ...                  
├── docs/                   
│   ├── relazione
│         └──RelazioneProgrammazione  
├── src/                         
│   ├── board.cpp
│   ├── game.cpp
│   └── main.cpp
├── include/                         
│   ├── board.hpp
│   ├── game.hpp
|   ├── doctest.h
│   └── struct.hpp
├── tests/
|   ├── functiontest.cpp                  
│   └── tabellonetest.cpp
├── CMakeLists.txt              
├── .clang-format                
├── .gitignore
├── domande                  
├── LICENSE                      
└── README.md
```


## 📄 **License**

This project is licensed under the [MIT License](./LICENSE). See the LICENSE file for more details.
