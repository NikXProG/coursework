#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>

class Character {

    std::string name_;
    std::vector<int> equipmentSlots_;

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

    struct A{
        int accuracy;
        int luck;
        int mastery;
    } secondarySkill;

protected:

    int limit_skill = 50;
    int maxCountDamage;
    int SummaryCharacterSkill;
 
protected:

    Character(std::string name)  : name_(name) {}

public:

    virtual int calculateDamage() = 0; // may be equivalent to heal
    virtual void generateEquipment() = 0; 

};