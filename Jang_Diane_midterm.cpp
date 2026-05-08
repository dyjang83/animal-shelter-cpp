// Name: Diane Jang
// CS XL 32 Midterm Project: Animal Shelter Manager
// Bonus features included for extra credit:
//  * Third animal type: Rabbit
//  * Search animal by name (menu option 5)
//  * Undo last option (menu option 6)

#include <iostream>
#include <vector>
#include <memory>
#include <string>

using namespace std;

// AdoptionRecord

struct AdoptionRecord {
    string adopterName;
    unique_ptr<AdoptionRecord> previous; // older record

    AdoptionRecord(const string& name)
        : adopterName(name), previous(nullptr) {}

    // Recursive function that prints from most recent adopter to oldest.
    // Base case: previous == nullptr.
    // Recursive step: call printHistory() on previous.
    void printHistory() const {
        cout << adopterName << endl;
        if (previous) {
            cout << "Previously adopted by ";
            previous->printHistory(); // Recursive step
        }
    }
};

// Animal (abstract base class)

class Animal {
private: 
    string name;
    int age;

protected:
    unique_ptr<AdoptionRecord> history;

public:
    Animal(const string& n, int a)
        : name(n), age(a), history(nullptr) {}

    // Virtual destructor
    virtual ~Animal() = default;

    // Pure virtual function
    virtual void printInfo() const = 0;

    // Getters
    string getName() const {
        return name;
    }
    
    int getAge() const {
        return age;
    }

    // Adds a new adopter to the fron of the history chain
    void recordAdoption(const string& adopterName) {
        unique_ptr<AdoptionRecord> newRecord = make_unique<AdoptionRecord>(adopterName);
        newRecord->previous = move(history);
        history = move(newRecord);
    }

    // Prints full adoption history
    void printAdoptionHistory() const {
        if (!history) {
            cout << "[No adoption history yet.]" << endl;
        } else {
            history->printHistory();
        }
    }
};

// Dog (derived class)
class Dog : public Animal {
public:
    Dog(const string& name, int age)
        : Animal(name, age) {}

    void printInfo() const override {
        cout << getName() << " the dog (age " << getAge() << ")" << endl;
    }
};

// Cat (derived class)
class Cat : public Animal {
public:
    Cat(const string& name, int age) 
        : Animal(name, age) {}

    void printInfo() const override {
        cout << getName() << " the cat (age " << getAge() << ")" << endl;
    }
};

// Rabit (derived class)
class Rabbit : public Animal {
public:
    Rabbit(const string& name, int age) 
        : Animal(name, age) {}

    void printInfo() const override {
        cout << getName() << " the rabbit (age " << getAge() << ")" << endl;
    }
};

// Helper: find an animal in the shelter by name.
// Returns -1 if not found.

int findAnimalIndex(const vector<shared_ptr<Animal>>& shelter, const string& name) {
    for (unsigned int i = 0; i < shelter.size(); ++i) {
        if (shelter[i]->getName() == name) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// Print divider used between menu interactions

void printDivider() {
    cout << "------------------------" << endl;
}

// Print menu

void printMenu() {
    cout << "1. Intake Animal" << endl;
    cout << "2. Adopt Animal" << endl;
    cout << "3. View All Animals" << endl;
    cout << "4. View Adoption History" << endl;
    cout << "5. Search Animal by Name" << endl;
    cout << "6. Undo Last Adoption" << endl;
    cout << "0. Exit" << endl;
}

// Raw pointer demonstration for contrast with smart pointers.
// Why smart pointers are safer:
//  - With 'new', we must remember to call 'delete' exactly once. If we forget, memory leak happens.
//    If we delete twice, we get undefined behavior.
//  - If an exception is thrown between 'new' and 'delete', the delete is skipped and memory leaks.
//  - shared_ptr and unique_ptr automatically delete the object when they go out of scope. 
//    They also make ownership explicit (unique = sole owner, shared = reference counted)

void demonstrateRawPointer() {
    cout << "[Raw pointer demo]" << endl;
    Dog* rawDog = new Dog("RawRex", 5); // manual allocation
    rawDog->printInfo();
    delete rawDog; // Must manually free, which is easy to forget. Smart pointers would do this automatically.
    rawDog = nullptr; // avoid dangling pointer
}

int main() {
    // Shelter holds animals via shared_ptr<Animal> so we can store any subclass polymorphically.
    vector<shared_ptr<Animal>> shelter;

    // undoStack holds the most recently adopted animal so the user can undo the last adoption.
    vector<shared_ptr<Animal>> undoStack;

    cout << "Welcome to the Animal Shelter Manager" << endl;
    printDivider();

    // Run the raw pointer demo.
    demonstrateRawPointer();
    printDivider();

    int choice = -1;
    while (choice != 0) {
        printMenu();
        cin >> choice;
        cin.ignore(10000, '\n');

        if (choice == 1) {
            // Intake animal
            cout << "Enter animal type (dog/cat/rabbit): ";
            string type;
            getline(cin, type);

            cout << "Enter name: ";
            string name;
            getline(cin, name);

            cout << "Enter age: ";
            int age;
            cin >> age;
            cin.ignore(10000, '\n');

            // Readmission check: if an animal with this name was recently
            // adopted out, pull that same shared_ptr back into the shelter
            // so that its adoption history is preserved across the round-trip.
            int undoIdx = findAnimalIndex(undoStack, name);
            if (undoIdx != -1) {
                shared_ptr<Animal> returning = undoStack[undoIdx];
                undoStack.erase(undoStack.begin() + undoIdx);
                shelter.push_back(returning);
                cout << name << " has been readmitted to the shelter." << endl;
            } else {
                // New intake.
                // shared_ptr is stored as shared_ptr<Animal>, demonstrating polymorphism.
                shared_ptr<Animal> newAnimal;
                if (type == "dog") {
                    newAnimal = make_shared<Dog>(name, age);
                } else if (type == "cat") {
                    newAnimal = make_shared<Cat>(name, age);
                } else {
                    newAnimal = make_shared<Rabbit>(name, age);
                }
                shelter.push_back(newAnimal);
                cout << name << " the " << type << " has been added to the shelter." << endl;
            }
        } else if (choice == 2) {
            // Adopt animal
            if (shelter.empty()) {
                cout << "No animals available to adopt." << endl;
                printDivider();
                continue;
            }
            
            cout << "Enter the name of the animal to adopt: ";
            string animalName;
            getline(cin, animalName);
            
            int idx = findAnimalIndex(shelter, animalName);
            if (idx == -1) {
                cout << "No animal named " << animalName << "found in the shelter." << endl;
                printDivider();
                continue;
            }

            cout << "Enter adopter's name: ";
            string adopter;
            getline(cin, adopter);

            // Update the animal's recursive history chain.
            shelter[idx]->recordAdoption(adopter);

            // Save the adopted animal in the undo buffer, then remove it from the shelter.
            shared_ptr<Animal> adopted = shelter[idx];
            shelter.erase(shelter.begin() + idx);
            undoStack.push_back(adopted);

            cout << adopted->getName() << " has been adopted by " << adopter << "." << endl;
        } else if (choice == 3) {
            // View all animals
            cout << "Current Shelter Residents:" << endl;
            if (shelter.empty()) {
                cout << "[None - all animals have been adopted!]" << endl;
            } else {
                // Polymorphism: we call printInfo() through a shared_ptr<Animal>,
                // and the correct override is dispatched at runtime via the vtable.
                for (unsigned int i = 0; i < shelter.size(); ++i) {
                    cout << " " << (i + 1) << ". ";
                    shelter[i]->printInfo();
                }
            }
        } else if (choice == 4) {
            // View adoption history
            cout << "Enter the name of the animal to view history: ";
            string animalName;
            getline(cin, animalName);

            // Search the shelter and the undo stack.
            int idx = findAnimalIndex(shelter, animalName);
            shared_ptr<Animal> target = nullptr;

            if (idx != -1) {
                target = shelter[idx];
            } else {
                int undoIdx = findAnimalIndex(undoStack, animalName);
                if (undoIdx != -1) {
                    target = undoStack[undoIdx];
                }
            }

            if (!target) {
                cout << "No animal named \"" << animalName << "\" found." << endl;
            } else {
                cout << "Adoption History for " << target->getName() << ":" << endl;
                target->printAdoptionHistory();
            }
        } else if (choice == 5) {
            // Search for an animal without adopting
            cout << "Enter the name to search for: ";
            string animalName;
            getline(cin, animalName);

            int idx = findAnimalIndex(shelter, animalName);
            if (idx == -1) {
                cout << "No animal named \"" << animalName << "\" is currently in the shelter." << endl;
            } else {
                cout << "Found: ";
                shelter[idx]->printInfo();
            }
        } else if (choice == 6) {
            // Undo last adoption
            if (undoStack.empty()) {
                cout << "Nothing to undo - no recent adoptions." << endl;
            } else {
                shared_ptr<Animal> restored = undoStack.back();
                undoStack.pop_back();
                shelter.push_back(restored);
                cout << "Undid last adoption. " << restored->getName() << " is back in the shelter." << endl;
            }
        } else if (choice == 0) {
            cout << "Goodbye!" << endl;
        } else {
            cout << "Invalid menu choice. Please try again." << endl;
        }
        printDivider();
    }

    // No manual cleanup is needed because shared_ptr automatically frees all animals 
    // when 'shelter' and 'undoStack' go out of scope, and unique_ptr cleans up 
    // the entire AdoptionRecord chain for each animal. This shows why smart pointers 
    // are safer than raw pointers.
    return 0;
}