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

    // undoStack holds the most recently adopted
}