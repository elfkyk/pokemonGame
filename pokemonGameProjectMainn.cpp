#include <iostream>
#include <vector>
#include <clocale>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <string>
#include <algorithm> // shuffle
#include <random>    // random_device, mt19937

using namespace std;

class Move {
private:
    string name;

    int power;
public:
    Move();
    Move(string name, int power);

    string getName() const;
    int getPower() const;

    void setName(const string& name);
    void setPower(int power);
};

class DataManager;
class Pokemon;
class Pokeball;

class User {
protected:
    std::string username;
    std::string password;
    std::string userType; // "player" veya "admin"

public:
    User(const std::string& username, const std::string& password, const std::string& userType);
    virtual ~User() {}

    std::string getUsername() const;
    std::string getPassword() const;
    std::string getUserType() const;

    void setPassword(const std::string& newPassword);

    // Soyut metod � her kullan�c� kendi men�s�n� uygular
    virtual void showMenu() = 0;
};

class Player : public User {
	private:
	    DataManager* dataManager;
    std::vector<Pokemon*> myPokemons;
    std::vector<Pokeball*> myPokeballs;

    void encounterMenu(Pokemon& wild);
    void attackWild(Pokemon& wild);
    bool catchWild(Pokemon& wild);
    void runFromWild();
    void assignRandomMovesToPlayerPokemons();

public:
   
    Player(const std::string& username, const std::string& password, const std::string& userType, DataManager* dm);


    void showMenu() override;
    void listMyPokemons();
    void deleteMyPokemon();
    void performMove();
    void exitGame();
    
    void assignRandomMovesToPokemon(Pokemon& p);
};

class Admin : public User {
private:
    DataManager* dataManager;

public:
   
    
    Admin(const std::string& username, const std::string& password, const std::string& userType, DataManager* dm);


    void showMenu();

    void addPokemon();
    void deletePokemon();
    void listPokemons();
    void listMoves();
    void addMove();
    void deleteMove();
    void listPokeballs();
    void deletePokeball();
    void addPokeball();
    void addUser();
    void listUsers();
    void deleteUser();
    
    
};

class Pokemon {
private:
    string name;
    int hp;
    int attack;
    int defense;
    int speed;
    int level;
    int experience;
    vector<Move> moves;

public:
    Pokemon();
    Pokemon(string name, int hp, int attack, int defense, int speed);

    // Sald�r� ekleme
    void addMove(const Move& move);

    // Sava� mekanikleri
    void takeDamage(int amount);
    bool isDefeated() const;
    void gainExperience(int amount);
    void levelUpIfNeeded();

    // Getter'lar
    string getName() const;
    int getHP() const;
    int getAttack() const;
    int getDefense() const;
    int getSpeed() const;
    int getLevel() const;
    int getExperience() const;
    vector<Move> getMoves() const;

    // Setter'lar (gerekirse)
    void setHP(int hp);
    void setLevel(int level);
    void setExperience(int exp);
    void setMoves(const vector<Move>& moves);
};
class Pokeball {
private:
    string name;
    int catchRate;

public:
    Pokeball();
    Pokeball(string name, int catchRate);

    string getName() const;
    int getCatchRate() const;

    void setName(const string& name);
    void setCatchRate(int catchRate);
};

class DataManager {
public:
    std::vector<Pokemon*> pokemons;   // Pokemon pointer olarak tutulacak
    std::vector<Move> moves;
    std::vector<Pokeball> pokeballs;
    std::vector<User*> users;  // User pointer listesi, Player veya Admin olabilir

    ~DataManager() {
        for (Pokemon* p : pokemons) {
            delete p;  // Dinamik olarak olu�turulan pok�monlar� sil
        }
        for (User* user : users) {
            delete user;
        }
    }
    void loadUsers(const std::string& filename);


    void loadPokemons(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Pokemons dosyas� a��lamad�: " << filename << std::endl;
            return;
        }

        std::string line;
        std::getline(file, line); // Ba�l�k sat�r�n� atla

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string name;
            int hp, attack, defense, speed;

            if (!(iss >> name >> hp >> attack >> defense >> speed)) {
                continue; // Bozuk sat�r varsa atla
            }

            Pokemon* p = new Pokemon(name, hp, attack, defense, speed);
            pokemons.push_back(p);
        }

        file.close();
    }

    void loadMoves(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Moves dosyas� a��lamad�: " << filename << std::endl;
            return;
        }

        std::string line;
        std::getline(file, line); // Ba�l�k sat�r� atla

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string name;
            int power;

            if (!(iss >> name >> power)) continue;

            Move m(name, power);
            moves.push_back(m);
        }

        file.close();
    }

    void loadPokeballs(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Pokeballs dosyas� a��lamad�: " << filename << std::endl;
            return;
        }

        std::string line;
        std::getline(file, line); // Ba�l�k sat�r� atla

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string name;
            int captureRate;

            if (!(iss >> name >> captureRate)) continue;

            Pokeball pb(name, captureRate);
            pokeballs.push_back(pb);
        }

        file.close();
    }
 void savePokemons(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Pokemons dosyas� yazmak i�in a��lamad�: " << filename << std::endl;
            return;
        }
 file << "Name HP Attack Defense Speed\n";
        for (Pokemon* p : pokemons) {
            file << p->getName() << " "
                 << p->getHP() << " "
                 << p->getAttack() << " "
                 << p->getDefense() << " "
                 << p->getSpeed() << "\n";
        }

        file.close();
    }


void saveMoves(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Moves dosyas� kaydedilemedi: " << filename << std::endl;
        return;
    }

    file << "Move_Name Power\n"; // Ba�l�k sat�r�

    for (const Move& move : moves) {
        file << move.getName() << " " << move.getPower() << "\n";
    }

    file.close();
    std::cout << "Moves dosyas� kaydedildi: " << filename << "\n";
}


    void saveUsers(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Users dosyas� kaydedilemiyor: " << filename << std::endl;
            return;
        }

        file << "Kullanici_Adi Parola Kullanici_Tipi\n";

        for (User* user : users) {
            file << user->getUsername() << " " << user->getPassword() << " " << user->getUserType() << "\n";
        }

        file.close();
    }

void savePokeballs(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Pokeballs dosyas� kaydedilemedi: " << filename << std::endl;
        return;
    }

    file << "Pokeball_Name CatchRate\n";
    for (const Pokeball& pb : pokeballs) {
        file << pb.getName() << " " << pb.getCatchRate() << "\n";
    }

    file.close();
    std::cout << "Pokeballs dosyas� kaydedildi.\n";
}


};

User::User(const std::string& username, const std::string& password, const std::string& userType) {
    this->username = username;
    this->password = password;
    this->userType = userType;
}

std::string User::getUsername() const {
    return username;
}

std::string User::getPassword() const {
    return password;
}

std::string User::getUserType() const {
    return userType;
}

void User::setPassword(const std::string& newPassword) {
    password = newPassword;
}

Admin::Admin(const std::string& username, const std::string& password, const std::string& userType, DataManager* dm)
    : User(username, password, userType), dataManager(dm) {}



void Admin::showMenu() {
    while (true) {
        int choice;
        std::cout << "\n=== Admin Men� ===\n"
                  << "1. Pokemon Listele\n"
                  << "2. Pokemon Ekle\n"
                  << "3. Pokemon Sil\n"
                  << "4. Hareketleri Listele\n"
                  << "5. Hareket Ekle\n"
                  << "6. Hareket Sil\n"
                  << "7. Pokeball Listele\n"
                  << "8. Pokeball Ekle\n"
                  << "9. Pokeball Sil\n"
                  << "10. Kullan�c� Listele\n"
                  << "11. Kullan�c� Ekle\n"
                  << "12. Kullan�c� Sil\n"
                  << "0. ��k��\n"
                  << "Se�iminiz: ";

        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "L�tfen bir say� giriniz!\n";
            continue; // Hatal� giri�te tekrar men�ye d�n
        }

        switch (choice) {
            case 1: listPokemons(); break;
            case 2: addPokemon(); break;
            case 3: deletePokemon(); break;
            case 4: listMoves(); break;
            case 5: addMove(); break;
            case 6: deleteMove(); break;
            case 7: listPokeballs(); break;
            case 8: addPokeball(); break;
            case 9: deletePokeball(); break;
            case 10: listUsers(); break;
            case 11: addUser(); break;
            case 12: deleteUser(); break;
            case 0: std::cout << "��k�� yap�l�yor...\n"; return; // Men�den ��k, main'e d�n
            default:
                std::cout << "Ge�ersiz se�im!\n";
                break; // Ge�ersiz se�imde de tekrar sor
        }
    }
}


void Admin::listPokemons() {
    if (dataManager->pokemons.empty()) {
        std::cout << "Pokemon listesi bo�.\n";
        return;
    }

    std::cout << "\n--- Pokemon Listesi ---\n";
    for (size_t i = 0; i < dataManager->pokemons.size(); ++i) {
        Pokemon* p = dataManager->pokemons[i];
        std::cout << i + 1 << ". " << p->getName()
                  << "  HP: " << p->getHP()
                  << "  Sald�r�: " << p->getAttack()
                  << "  Savunma: " << p->getDefense()
                  << "  H�z: " << p->getSpeed()
                  << std::endl;
    }
}

void Admin::addPokemon() {
    std::string name;
    int hp, attack, defense, speed;

    std::cout << "Pokemon ad�: ";
    std::cin >> name;
    std::cout << "HP: ";
    std::cin >> hp;
    std::cout << "Sald�r�: ";
    std::cin >> attack;
    std::cout << "Savunma: ";
    std::cin >> defense;
    std::cout << "H�z: ";
    std::cin >> speed;

    Pokemon* newPokemon = new Pokemon(name, hp, attack, defense, speed);
    dataManager->pokemons.push_back(newPokemon);

    std::cout << "Pokemon eklendi: " << name << std::endl;

    // Yeni eklenen Pok�mon'u dosyaya kaydet
    dataManager->savePokemons("pokemons.txt");
}

void Admin::deletePokemon() {
    if (dataManager->pokemons.empty()) {
        std::cout << "Silinecek Pokemon yok." << std::endl;
        return;
    }

    std::cout << "Silmek istedi�iniz Pokemonun numaras�n� girin:" << std::endl;
    for (size_t i = 0; i < dataManager->pokemons.size(); ++i) {
        std::cout << i + 1 << ". " << dataManager->pokemons[i]->getName() << std::endl;
    }

    int choice;
    std::cin >> choice;
    if (choice < 1 || choice > (int)dataManager->pokemons.size()) {
        std::cout << "Ge�ersiz se�im!" << std::endl;
        return;
    }

    delete dataManager->pokemons[choice - 1];
    dataManager->pokemons.erase(dataManager->pokemons.begin() + (choice - 1));

    std::cout << "Pokemon silindi." << std::endl;

    // Dosyaya g�ncellenmi� listeyi yaz
    dataManager->savePokemons("pokemons.txt");
}

void Admin::listPokeballs() {
    if (dataManager->pokeballs.empty()) {
        std::cout << "Pokeball listesi bo�.\n";
        return;
    }

    std::cout << "\n--- Pokeball Listesi ---\n";
    for (size_t i = 0; i < dataManager->pokeballs.size(); ++i) {
        const Pokeball& pb = dataManager->pokeballs[i];
        std::cout << i + 1 << ". " << pb.getName()
                  << " | Yakalama Oran�: " << pb.getCatchRate() << "%\n";
    }
}

void Admin::addPokeball() {
    std::string name;
    int rate;

    std::cout << "Yeni Pokeball ad�: ";
    std::cin >> name;
    std::cout << "Yakalama oran� (%): ";
    std::cin >> rate;

    if (rate < 1 || rate > 100) {
        std::cout << "Ge�ersiz yakalama oran�! (1-100 aral���nda olmal�)\n";
        return;
    }

    for (const Pokeball& pb : dataManager->pokeballs) {
        if (pb.getName() == name) {
            std::cout << "Bu isimde bir Pokeball zaten var!\n";
            return;
        }
    }

    dataManager->pokeballs.push_back(Pokeball(name, rate));
    dataManager->savePokeballs("pokeballs.txt");

    std::cout << "Pokeball eklendi: " << name << " (" << rate << "%)\n";
}


void Admin::deletePokeball() {
    if (dataManager->pokeballs.empty()) {
        std::cout << "Silinecek Pokeball yok.\n";
        return;
    }

    listPokeballs();
    std::cout << "Silmek istedi�iniz pokeball'un numaras�n� girin: ";
    int idx;
    std::cin >> idx;

    if (idx < 1 || idx > static_cast<int>(dataManager->pokeballs.size())) {
        std::cout << "Ge�ersiz se�im!\n";
        return;
    }

    std::string name = dataManager->pokeballs[idx - 1].getName();
    dataManager->pokeballs.erase(dataManager->pokeballs.begin() + (idx - 1));
    std::cout << "Pokeball silindi: " << name << "\n";

    dataManager->savePokeballs("pokeballs.txt");
}



void Admin::addMove() {
    std::string moveName;
    int power;

    std::cout << "Hareket ad�: ";
    std::cin >> moveName;
    std::cout << "G�� (power): ";
    std::cin >> power;

    Move newMove(moveName, power);
    dataManager->moves.push_back(newMove);
    
    dataManager->saveMoves("moves.txt");
        std::cout << "Hareket eklendi: " << moveName << "\n";


}


Player::Player(const std::string& username, const std::string& password, const std::string& userType, DataManager* dm)
    : User(username, password, userType), dataManager(dm) {

    std::srand((unsigned)std::time(nullptr));
    // �lk 3 Pok�mon�u rastgele se�
    for (int i = 0; i < 3; ++i) {
        int idx = std::rand() % dataManager->pokemons.size();
        myPokemons.push_back(new Pokemon(*dataManager->pokemons[idx]));
    }
    // �lk 3 Pok�ball�u ver
    for (int i = 0; i < 3; ++i) {
        myPokeballs.push_back(new Pokeball("Pokeball", 25));
    }
}
void Admin::listMoves() {
    if (dataManager->moves.empty()) {
        std::cout << "Hareket listesi bo�.\n";
        return;
    }

    std::cout << "\n--- Hareket Listesi ---\n";
    for (size_t i = 0; i < dataManager->moves.size(); ++i) {
        const Move& m = dataManager->moves[i];
        std::cout << i + 1 << ". " << m.getName() << " | G��: " << m.getPower() << "\n";
    }
}




void Admin::deleteMove() {
    if (dataManager->moves.empty()) {
        std::cout << "Silinecek hareket yok.\n";
        return;
    }

    listMoves();
    std::cout << "Silmek istedi�iniz hareketin numaras�n� girin: ";
    int choice;
    std::cin >> choice;

    if (choice < 1 || choice > static_cast<int>(dataManager->moves.size())) {
        std::cout << "Ge�ersiz se�im!\n";
        return;
    }

    std::string deletedName = dataManager->moves[choice - 1].getName();
    dataManager->moves.erase(dataManager->moves.begin() + (choice - 1));

    dataManager->saveMoves("moves.txt");
        std::cout << "Hareket silindi: " << deletedName << "\n";


}

void Admin::listUsers() {
    if (dataManager->users.empty()) {
        std::cout << "Kullan�c� listesi bo�.\n";
        return;
    }

    std::cout << "\n--- Kullan�c� Listesi ---\n";
    for (size_t i = 0; i < dataManager->users.size(); ++i) {
        std::cout << i + 1 << ". " << dataManager->users[i]->getUsername()
                  << " | Tip: " << dataManager->users[i]->getUserType() << "\n";
    }
}

void Admin::addUser() {
    std::string username, password, type;

    std::cout << "Yeni kullan�c� ad�: ";
    std::cin >> username;
    std::cout << "Parola: ";
    std::cin >> password;
    std::cout << "Kullan�c� tipi (player/admin): ";
    std::cin >> type;

    if (type != "player" && type != "admin") {
        std::cout << "Ge�ersiz kullan�c� tipi!\n";
        return;
    }

    for (User* u : dataManager->users) {
        if (u->getUsername() == username) {
            std::cout << "Bu kullan�c� ad� zaten var!\n";
            return;
        }
    }

    if (type == "admin") {
        dataManager->users.push_back(new Admin(username, password, type, dataManager));
    } else {
        dataManager->users.push_back(new Player(username, password, type, dataManager));
    }

    dataManager->saveUsers("users.txt");

    std::cout << "Kullan�c� eklendi.\n";
}


void Admin::deleteUser() {
    if (dataManager->users.empty()) {
        std::cout << "Silinecek kullan�c� yok.\n";
        return;
    }

    listUsers();
    std::cout << "Silmek istedi�iniz kullan�c�n�n numaras�n� girin: ";
    int idx;
    std::cin >> idx;

    if (idx < 1 || idx > static_cast<int>(dataManager->users.size())) {
        std::cout << "Ge�ersiz se�im!\n";
        return;
    }

    User* user = dataManager->users[idx - 1];
    std::string uname = user->getUsername();

    delete user;
    dataManager->users.erase(dataManager->users.begin() + (idx - 1));

    dataManager->saveUsers("users.txt");

    std::cout << "Kullan�c� silindi: " << uname << "\n";
}


void Player::showMenu() {
	
	// Men� ba�lamadan �nce Pokemonlara hareket ata
    assignRandomMovesToPlayerPokemons();
    while (true) {
        int choice;
        std::cout << "\n=== Player Menu ===\n"
                  << "1. Pokemonlar�m� Listele\n"
                  << "2. Pokemon Sil\n"
                  << "3. Hareket Et\n"
                  << "4. ��k��\n"
                  << "Se�iminiz: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            // Hatal� giri� (harf, sembol vs)
            std::cin.clear(); // hata bayra��n� temizle
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // buffer'� temizle
            std::cout << "L�tfen bir say� giriniz!\n";
            continue; // men�ye tekrar d�n
        }

        switch (choice) {
            case 1: listMyPokemons();      break;
            case 2: deleteMyPokemon();     break;
            case 3: performMove();         break;
            case 4: 
                std::cout << "��k�� yap�l�yor...\n";
                return;  // Men�den ��k, main'e d�n
            default: 
                std::cout << "Ge�ersiz se�im!\n";
                break; // Ge�ersiz se�imde tekrar sor
        }
    }
}


void Player::listMyPokemons() {
    if (myPokemons.empty()) {
        std::cout << "\n Hi� Pok�monunuz yok.\n";
        return;
    }

    std::cout << "\n===  Pok�monlar�n�z ===\n";

    for (size_t i = 0; i < myPokemons.size(); ++i) {
        Pokemon* p = myPokemons[i];

        std::cout << "\n" << i + 1 << ". " << p->getName()
                  << " | HP: " << p->getHP()
                  << " | Level: " << p->getLevel()
                  << " | Deneyim(XP): " << p->getExperience() << "\n";

        std::cout << "Hareketler:";

        std::vector<Move> moves = p->getMoves();
        if (moves.empty()) {
            std::cout << " (Hi� hareket atanmad�)";
        } else {
            for (const Move& move : moves) {
                std::cout << " [" << move.getName()
                          << " (G��: " << move.getPower() << ")]";
            }
        }
        std::cout << "\n";
    }
}



void Player::deleteMyPokemon() {
    if (myPokemons.size() <= 1) {
        std::cout << "Son Pokemonunuzu silemezsiniz!\n";
        return;
    }
    listMyPokemons();
    std::cout << "Silmek istedi�iniz Pokemonun numaras�n� girin: ";
    int idx; std::cin >> idx;
    if (idx < 1 || idx > (int)myPokemons.size()) {
        std::cout << "Ge�ersiz se�im!\n"; return;
    }
    delete myPokemons[idx-1];
    myPokemons.erase(myPokemons.begin() + (idx-1));
    std::cout << "Pokemon silindi.\n";
}


void Player::performMove() {
    std::cout << "\nYolda ilerliyorsunuz...\n";
    int chance = std::rand() % 100;

    if (chance < 40) {
        int widx = std::rand() % dataManager->pokemons.size();
        Pokemon wild = *dataManager->pokemons[widx];

        // Rastgele 4 sald�r� ata (moves vekt�r� Move nesnesi i�eriyor)
        std::vector<Move> allMoves = dataManager->moves;

        // Modern C++ shuffle
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(allMoves.begin(), allMoves.end(), g);

        for (int i = 0; i < 4 && i < (int)allMoves.size(); ++i) {
            wild.addMove(allMoves[i]);
        }

        std::cout << "\n  Bir vah�i " << wild.getName() << " kar��na ��kt�!\n";
        std::cout << " �zellikler:\n";
        std::cout << "HP: " << wild.getHP()
                  << ", Sald�r�: " << wild.getAttack()
                  << ", Savunma: " << wild.getDefense()
                  << ", H�z: " << wild.getSpeed()
                  << ", Seviye: " << wild.getLevel()
                  << ", Deneyim: " << wild.getExperience() << "\n";

        std::cout << "\n Hareketler:\n";
        std::vector<Move> moves = wild.getMoves();
        for (size_t i = 0; i < moves.size(); ++i) {
            std::cout << "- " << moves[i].getName()
                      << " (G��: " << moves[i].getPower() << ")\n";
        }

        encounterMenu(wild);
    } else {
        std::cout << " Hi� kimseyle kar��la�mad�n�z...\n";
    }
}


void Player::encounterMenu(Pokemon& wild) {
    while (true) {
        std::cout << "\nNe yapmak istersiniz?\n";
        std::cout << "1. Sald�r\n";
        std::cout << "2. Pokeball At\n";
        std::cout << "3. Ka�\n";
        std::cout << "Se�iminiz: ";
        
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                attackWild(wild);
                break;
            case 2:
                if (catchWild(wild)) {
                 return; // Yakalad�ysa d�ng�den ��k 
                }
                break;
               
              
            case 3:
                runFromWild();
                return; // Ka��nca do�rudan men�den ��k
            default:
                std::cout << "Ge�ersiz se�im!\n";
                continue;
        }

        // Vah�i Pok�mon yakaland�ysa veya bay�ld�ysa men�den ��k
        if (wild.getHP() <= 0) {
            std::cout << " Vah�i " << wild.getName() << " etkisiz hale geldi.\n";
            return;
        }
    }
}


// Player s�n�f� i�in rastgele 4 hareket atama fonksiyonu
void Player::assignRandomMovesToPlayerPokemons() {
	
    std::random_device rd;
    std::mt19937 g(rd());

    for (Pokemon* p : myPokemons) {
        // �nce mevcut hareketleri temizle
        p->setMoves(std::vector<Move>());

        // T�m hareketleri kar��t�r
        std::vector<Move> shuffledMoves = dataManager->moves;
        std::shuffle(shuffledMoves.begin(), shuffledMoves.end(), g);

        // �lk 4 hareketi ata
        for (int i = 0; i < 4 && i < (int)shuffledMoves.size(); ++i) {
            p->addMove(shuffledMoves[i]);
        }
    }
}


void Player::attackWild(Pokemon& wild) {
    if (myPokemons.empty()) {
        std::cout << "Hi� Pokemonunuz yok!\n";
        return;
    }

    listMyPokemons();
    std::cout << "Sald�racak Pokemon numaras�n� girin: ";
    int idx;
    std::cin >> idx;
    
    if (std::cin.fail()) {
        std::cin.clear(); // hata bayra��n� temizler.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // tamponu temizle
        std::cout << "Ge�ersiz giri�! L�tfen bir say� girin.\n";
        return;
    }

    if (idx < 1 || idx > (int)myPokemons.size()) {
        std::cout << "Ge�ersiz se�im.\n";
        return;
    }

    Pokemon* attacker = myPokemons[idx - 1];
    std::vector<Move> moves = attacker->getMoves();

    if (moves.empty()) {
        std::cout << attacker->getName() << " hi� sald�r� bilmiyor!\n";
        return;
    }

    std::cout << attacker->getName() << " i�in sald�r� se�in:\n";
    for (size_t i = 0; i < moves.size(); ++i) {
        std::cout << i + 1 << ". " << moves[i].getName()
                  << " (G��: " << moves[i].getPower() << ")\n";
    }

    int moveChoice;
    std::cout << "Se�iminiz: ";
    std::cin >> moveChoice;

    if (moveChoice < 1 || moveChoice > (int)moves.size()) {
        std::cout << "Ge�ersiz sald�r� se�imi!\n";
        return;
    }

    Move chosenMove = moves[moveChoice - 1];
    int damage = chosenMove.getPower();
    wild.takeDamage(damage);

    std::cout << attacker->getName() << ", " << chosenMove.getName()
              << " sald�r�s�yla " << wild.getName() << " pokemonuna "
              << damage << " hasar verdi!\n";

    if (wild.getHP() <= 0) {
        std::cout << wild.getName() << " yenildi! +50 XP kazand�n�z.\n";
        attacker->gainExperience(50);
    } else {
        std::cout << "Vah�i " << wild.getName() << " kalan HP: " << wild.getHP() << "\n";
    }
}



bool Player::catchWild(Pokemon& wild) {
	std::random_shuffle(myPokeballs.begin(), myPokeballs.end());

    if (myPokeballs.empty()) {
        std::cout << "Hi� pokeballunuz yok!\n";
        return false;
    }

    std::cout << "Hangi Pokeball'u kullanmak istersiniz?\n";
    for (size_t i = 0; i < myPokeballs.size(); ++i) {
        std::cout << i + 1 << ". " << myPokeballs[i]->getName()
                  << " (Yakalama Oran�: " << myPokeballs[i]->getCatchRate() << "%)\n";
    }

    int idx;
    std::cin >> idx;
    if (idx < 1 || idx > (int)myPokeballs.size()) {
        std::cout << "Ge�ersiz se�im!\n";
        return false;
    }

    Pokeball* ball = myPokeballs[idx - 1];
    myPokeballs.erase(myPokeballs.begin() + (idx - 1));

    int roll = std::rand() % 100;
    if (roll < ball->getCatchRate()) {
        std::cout << wild.getName() << " yakaland�!\n";

        if (myPokemons.size() >= 6) {
            std::cout << "Zaten 6 Pok�mon sahibisiniz. �nce birini silmeniz gerekiyor.\n";
            deleteMyPokemon();
        }

        Pokemon* newPokemon = new Pokemon(wild);
        assignRandomMovesToPokemon(*newPokemon);
        myPokemons.push_back(newPokemon);

        delete ball;
        return true;  // Ba�ar�l� yakalama
    } else {
        std::cout << "Yakalama ba�ar�s�z oldu.\n";
        delete ball;
        return false; // Ba�ar�s�z yakalama
    }
}


void Player::assignRandomMovesToPokemon(Pokemon& p) {
    std::random_device rd;
    std::mt19937 g(rd());

    std::vector<Move> shuffledMoves = dataManager->moves;
    std::shuffle(shuffledMoves.begin(), shuffledMoves.end(), g);

    for (int i = 0; i < 4 && i < (int)shuffledMoves.size(); ++i) {
        p.addMove(shuffledMoves[i]);
    }
}


void Player::runFromWild() {
    std::cout << "Ka�may� ba�ard�n�z!\n";
}

void Player::exitGame() {
    std::cout << "Oyundan ��k�l�yor...\n";
}


Pokemon::Pokemon() {
    name = "";
    hp = 0;
    attack = 0;
    defense = 0;
    speed = 0;
    level = 1;
    experience = 0;
}

Pokemon::Pokemon(string name, int hp, int attack, int defense, int speed)
    : name(name), hp(hp), attack(attack), defense(defense), speed(speed), level(1), experience(0) {}

void Pokemon::addMove(const Move& move) {
    if (moves.size() < 4) {
        moves.push_back(move);
    }
}

void Pokemon::takeDamage(int amount) {
    hp -= amount;
    if (hp < 0) hp = 0;
}

bool Pokemon::isDefeated() const {
    return hp <= 0;
}

void Pokemon::gainExperience(int amount) {
    experience += amount;
    levelUpIfNeeded();
}

void Pokemon::levelUpIfNeeded() {
    while (experience >= 100) {
        level++;
        experience -= 100;
        cout << name << " seviye atlad�! Yeni seviye: " << level << endl;
    }
}

string Pokemon::getName() const {
    return name;
}

int Pokemon::getHP() const {
    return hp;
}


int Pokemon::getAttack() const {
    return attack;
}

int Pokemon::getDefense() const {
    return defense;
}

int Pokemon::getSpeed() const {
    return speed;
}

int Pokemon::getLevel() const {
    return level;
}

int Pokemon::getExperience() const {
    return experience;
}

vector<Move> Pokemon::getMoves() const {
    return moves;
}

void Pokemon::setHP(int hp) {
    this->hp = hp;
}

void Pokemon::setLevel(int level) {
    this->level = level;
}

void Pokemon::setExperience(int exp) {
    this->experience = exp;
}

void Pokemon::setMoves(const vector<Move>& newMoves) {
    this->moves = newMoves;
}

Move::Move() {
    name = "";
    power = 0;
}

Move::Move(string name, int power) {
    this->name = name;
    this->power = power;
}

string Move::getName() const {
    return name;
}

int Move::getPower() const {
    return power;
}

void Move::setName(const string& name) {
    this->name = name;
}

void Move::setPower(int power) {
    this->power = power;
}

Pokeball::Pokeball() {
    name = "";
    catchRate = 0;
}

Pokeball::Pokeball(string name, int catchRate) {
    this->name = name;
    this->catchRate = catchRate;
}

string Pokeball::getName() const {
    return name;
}

int Pokeball::getCatchRate() const {
    return catchRate;
}

void Pokeball::setName(const string& name) {
    this->name = name;
}

void Pokeball::setCatchRate(int catchRate) {
    this->catchRate = catchRate;
}

void DataManager::loadUsers(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Users dosyas� a��lamad�: " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Ba�l�k sat�r�n� atla

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string username, password, userType;

        if (!(iss >> username >> password >> userType)) continue;
        
        if (userType == "player") {
            users.push_back(new Player(username, password, userType, this));
        } else if (userType == "admin") {
            users.push_back(new Admin(username, password, userType, this));
        }
    }

    file.close();
}

bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

//**************** MA�N KISMI ********************
int main() {
	
setlocale(LC_ALL, "Turkish");
 std::srand(std::time(0)); 

    DataManager dataManager;
    
      vector<string> requiredFiles = {"pokemons.txt", "moves.txt", "pokeballs.txt", "users.txt"};
    for (const auto& file : requiredFiles) {
        if (!fileExists(file)) {
            cerr << "HATA: " << file << " dosyas� bulunamad�!\n";
            cerr << "�al��ma dizini: ";
            system("cd"); // Ge�erli dizini g�ster (Windows)
            return 1;
        }
    }
// Dosyalar� y�kle
    dataManager.loadPokemons("pokemons.txt");
    dataManager.loadMoves("moves.txt");
    dataManager.loadPokeballs("pokeballs.txt");
    dataManager.loadUsers("users.txt");
string basePath = "C:/Documents/";
dataManager.loadPokemons("pokemons.txt");




    User* loggedInUser = nullptr;

    while (true) {
        string username, password;
        cout << "Kullan�c� ad�: ";
        cin >> username;
        cout << "Parola: ";
        cin >> password;

        // Giri� kontrol�
        for (User* user : dataManager.users) {
            if (user->getUsername() == username && user->getPassword() == password) {
                if (user->getUserType() == "admin") {
                    loggedInUser = new Admin(username, password, user->getUserType(), &dataManager);
                } else if (user->getUserType() == "player") {
                    loggedInUser = new Player(username, password, user->getUserType(), &dataManager);
                }
                break;
            }
        }

if (loggedInUser) {
    cout << "Giri� ba�ar�l�! Ho� geldiniz " << loggedInUser->getUsername() << "!\n";
    loggedInUser->showMenu();
    
    delete loggedInUser;
    loggedInUser = nullptr;
    
    //cout << "Ana giri� ekran�na d�n�l�yor...\n\n";
    char choice;
            while (true) {
                cout << "\nDevam etmek istiyor musunuz? (e/h): ";
                cin >> choice;

                if (choice == 'e' || choice == 'E') {
                    cout << "\nAna giri� ekran�na d�n�l�yor...\n\n";
                    break; // while(true) d�ng�s�ne geri d�ner, giri� ekran� tekrar gelir
                } else if (choice == 'h' || choice == 'H') {
                    cout << "Programdan ��k�l�yor...\n";
                    return 0; // program sonlan�r
                } else {
                    cout << "Ge�ersiz se�im. L�tfen 'e' veya 'h' giriniz.\n";
                }
            }
        } else {
            cout << "Hatal� kullan�c� ad� veya �ifre. Tekrar deneyin.\n\n";
        }
    }

    
    
    
    

return 0;
}





