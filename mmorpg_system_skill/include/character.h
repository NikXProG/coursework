#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <memory>
#include "Equipment.h"

class Character {

    std::string name_;

protected:

    // main characteristics
    int health;
    int armor;

    // main ability skill
    struct MainSkill{
        int strength;
        int intelligence;
        int dexterity;
    } mainSkill;

    // secoudary characteristics
    struct SecondarySkill{
        int accuracy;
        int luck;
        int mastery;
    } secondarySkill;

    std::vector<std::shared_ptr<Equipment>> equipment_;

protected:

    int limit_skill = 50;
    int maxCountDamage;
    int SummaryCharacterSkill;

protected:

    Character(std::string name)  : name_(name) {
        std::random_device rd;
        std::srand(rd());
    }

public:

    virtual int calculateDamage() const = 0; // may be equivalent to heal
    virtual void generateEquipment() = 0; 

public:

    void InfoCharacter() const;
    const std::vector<std::shared_ptr<Equipment>>& getEquipment() const { return equipment_; } ;

};