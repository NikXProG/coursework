#include "character.h"

class Defender : public Character {
    
    Defender(std::string name) : Character(name) {
        health = 14;
        armor = 10;
        mainSkill.strength = 8;
        mainSkill.intelligence = 3;
        mainSkill.dexterity = 4;
        secondarySkill.accuracy = 3;
        secondarySkill.luck = 3;
        secondarySkill.mastery = 4;
    }

    int calculateDamage() override {
        // the more mastery and health, the more strength
        maxCountDamage = (health * secondarySkill.mastery * mainSkill.strength/50 );
    }

    void generateEquipment() override {
        
        equipmentSlots_.push_back()
    }

};

class Healer : public Character {

    Healer(std::string name) : Character(name) {
        health = 11;
        armor = 6;
        mainSkill.strength = 3;
        mainSkill.intelligence = 15;
        mainSkill.dexterity = 6;
        secondarySkill.accuracy = 3;
        secondarySkill.luck = 3;
        secondarySkill.mastery = 4;
    }

    // damage and heal equals
    int calculateDamage() override {
        // the more skill and luck, the more intelligence
        maxCountDamage = (secondarySkill.luck * secondarySkill.mastery * mainSkill.intelligence/50 );
    }

};

class MeleeFighters : public Character {

    MeleeFighters(std::string name) : Character(name){
        health = 7;
        armor = 6;
        mainSkill.strength = 15;
        mainSkill.intelligence = 5;
        mainSkill.dexterity = 11;
        secondarySkill.accuracy = 6;
        secondarySkill.luck = 5;
        secondarySkill.mastery = 6;
    }

    int calculateDamage() override {
        // the more dexterity and health, the more x damage.
        maxCountDamage = (mainSkill.dexterity * secondarySkill.mastery * mainSkill.strength/50 );
    }

};

class RangedFighters : public Character{

    RangedFighters (std::string name) : Character(name) {
        health = 11;
        armor = 6;
        mainSkill.strength = 3;
        mainSkill.intelligence = 14;
        mainSkill.dexterity = 6;
        secondarySkill.accuracy = 3;
        secondarySkill.luck = 5;
        secondarySkill.mastery = 10;        
    }

    int calculateDamage() override {
        // чем больше защиты и здоровья тем больше иксы урона.
        maxCountDamage = (secondarySkill.accuracy * secondarySkill.mastery * mainSkill.intelligence/50 );
    }

};