#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>

class Equipment {

public:
    std::string name;
    int defense;
    int attack;

    const std::string& getName() const { return name; }
    int getDefense() const { return defense; }
    int getAttack() const { return attack; }

    Equipment(const std::string& name, int defense, int attack)
        : name(name), defense(defense), attack(attack) {}
    
    virtual ~Equipment() = default;
};


class BattleEquipment : public Equipment {
public:
    BattleEquipment(const std::string& name, int attack)
        : Equipment(name, 0, attack) {}
};

class ArmorEquipment : public Equipment {
public:
    ArmorEquipment(const std::string& name, int defense)
        : Equipment(name, defense, 0) {}
};

class ShieldEquiment : public Equipment {
public:
    ShieldEquiment(const std::string& name, int defense, int attack)
        : Equipment(name, defense, attack) {}
};


