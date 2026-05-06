# Animal Shelter Manager

C++ midterm project for CS XL 32 (UCLA Extension).

A console-based animal shelter management system demonstrating:
- Abstract base class with polymorphism (Animal → Dog, Cat, Rabbit)
- Smart pointers (shared_ptr for shelter storage, unique_ptr for adoption history)
- Recursive linked structure for tracking adoption chains
- Menu-driven user interface

## Build & Run

```bash
g++ -std=c++17 -Wall -Wextra -o shelter Jang_Diane_midterm.cpp
./shelter
```

## Bonus Features
- Third animal type: Rabbit
- Search animal by name
- Undo last adoption