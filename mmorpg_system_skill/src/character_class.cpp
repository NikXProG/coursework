#include "../include/character_class.h"


Defender::Defender(std::string name) : Character(name) {
    health = 14;
    armor = 10;
    mainSkill.strength = 8;
    mainSkill.intelligence = 3;
    mainSkill.dexterity = 4;
    secondarySkill.accuracy = 3;
    secondarySkill.luck = 3;
    secondarySkill.mastery = 4;
}

int Defender::calculateDamage() const {
    // the more mastery and health, the more strength
    return (health * secondarySkill.mastery * mainSkill.strength/30 );
}

void Defender::generateEquipment() {
    equipment_.clear();
    equipment_.push_back(std::make_shared<BattleEquipment>("Sword", 30 + std::rand()  % 50));      
    equipment_.push_back(std::make_shared<ArmorEquipment>("Helmet", 60 + std::rand() % 50));      
    equipment_.push_back(std::make_shared<ArmorEquipment>("Cuirass", 80 + std::rand()  % 50));
    equipment_.push_back(std::make_shared<ArmorEquipment>("Vambraces", 50 + std::rand()  % 50));      
    equipment_.push_back(std::make_shared<ArmorEquipment>("Cuisses", 40 + std::rand() % 50));            
    equipment_.push_back(std::make_shared<ShieldEquiment>("Shield", 100 + std::rand()  % 50, 20 + std::rand() % 50 ));     
    
}




Healer::Healer(std::string name) : Character(name) {
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
int Healer::calculateDamage() const{
    // the more skill and luck, the more intelligence
    return (secondarySkill.luck * secondarySkill.mastery * mainSkill.intelligence/30 );
}

void Healer::generateEquipment() {
    equipment_.clear();
    equipment_.push_back(std::make_shared<BattleEquipment>("Staff", 70 + std::rand()  % 100));      
    equipment_.push_back(std::make_shared<ArmorEquipment>("Healer hat", 30 + std::rand() % 30));      
    equipment_.push_back(std::make_shared<ArmorEquipment>("Healer costume", 40 + std::rand()  % 30));   
    equipment_.push_back(std::make_shared<ShieldEquiment>("Healer barrier", 30 + std::rand()  % 50, 0));     
    
}


MeleeFighters::MeleeFighters(std::string name) : Character(name){
    health = 7;
    armor = 6;
    mainSkill.intelligence = 5;
    secondarySkill.accuracy = 6;
    secondarySkill.luck = 5;
    secondarySkill.mastery = 6;
}

void MeleeFighters::generateEquipment() {
    equipment_.clear();
    equipment_.push_back(std::make_shared<BattleEquipment>("Sword", 90 + std::rand()  % 50));      
    equipment_.push_back(std::make_shared<ArmorEquipment>("Helmet", 30 + std::rand() % 50));      
    equipment_.push_back(std::make_shared<ArmorEquipment>("Cuirass", 50 + std::rand()  % 50));
    equipment_.push_back(std::make_shared<ArmorEquipment>("Vambraces", 20 + std::rand()  % 50));      
    equipment_.push_back(std::make_shared<ArmorEquipment>("Cuisses", 30 + std::rand() % 50));            
    equipment_.push_back(std::make_shared<ShieldEquiment>("Shield", 50 + std::rand()  % 50, 0));      
    
}

Berserkers::Berserkers(std::string name) : MeleeFighters(name) {
    mainSkill.strength = 15;
    mainSkill.dexterity = 8;
}

int Berserkers::calculateDamage() const {
    // the more dexterity and health, the more x damage.
    return (mainSkill.dexterity * secondarySkill.mastery * mainSkill.strength/30 );
}



SwordMan::SwordMan(std::string name) : MeleeFighters(name) {
    mainSkill.strength = 8;
    mainSkill.dexterity = 15;
}

int SwordMan::calculateDamage() const{
    // the more dexterity and health, the more x damage.
    return (mainSkill.strength * secondarySkill.mastery * mainSkill.dexterity/30 );
}
RangedFighters::RangedFighters (std::string name) : Character(name) {
        health = 11;
        armor = 6;
        mainSkill.strength = 3;
        mainSkill.intelligence = 14;
        mainSkill.dexterity = 6;
        secondarySkill.accuracy = 3;
        secondarySkill.luck = 5;
        secondarySkill.mastery = 10;        
}

Mage::Mage (std::string name) : RangedFighters(name) {}

void Mage::generateEquipment() {
    equipment_.clear();
    equipment_.push_back(std::make_shared<BattleEquipment>("Staff", 140 + std::rand()  % 100));      
    equipment_.push_back(std::make_shared<ArmorEquipment>("Magic hat", 30 + std::rand() % 30));      
    equipment_.push_back(std::make_shared<ArmorEquipment>("Magic costume", 35 + std::rand()  % 30));      
    equipment_.push_back(std::make_shared<ShieldEquiment>("Magic barrier", 30 + std::rand()  % 50, std::rand()  % 40));          
    
}

int Mage::calculateDamage() const {
    // the more accuracy and mastery, the more intelligence
    return (secondarySkill.accuracy * secondarySkill.mastery * mainSkill.intelligence/30 );

}

BeastTamer::BeastTamer(std::string name) : RangedFighters(name) {}

void BeastTamer::generateEquipment() {
    equipment_.clear();
    equipment_.push_back(std::make_shared<BattleEquipment>("Whip", 40 + std::rand()  % 100));   
    equipment_.push_back(std::make_shared<ArmorEquipment>("Magic hat", 20 + std::rand() % 30));      
    equipment_.push_back(std::make_shared<ArmorEquipment>("Magic costume", 35 + std::rand()  % 30));   
    equipment_.push_back(std::make_shared<ShieldEquiment>("Pet", 200 + std::rand()  % 50, 70 +  + std::rand()  % 50));        
    
}
int BeastTamer::calculateDamage() const {
    // the more accuracy and mastery, the more intelligence
    return (secondarySkill.accuracy * secondarySkill.mastery * mainSkill.intelligence/30 );

}
/*class MeleeFighters : public Character {

public:

    MeleeFighters(std::string name) : Character(name){
        health = 7;
        armor = 6;
        mainSkill.intelligence = 5;
        secondarySkill.accuracy = 6;
        secondarySkill.luck = 5;
        secondarySkill.mastery = 6;
    }

    void generateEquipment() override {
        equipment_.clear();
        equipment_.push_back(std::make_shared<BattleEquipment>("Sword", 90 + std::rand()  % 50));      
        equipment_.push_back(std::make_shared<ArmorEquipment>("Helmet", 30 + std::rand() % 50));      
        equipment_.push_back(std::make_shared<ArmorEquipment>("Cuirass", 50 + std::rand()  % 50));
        equipment_.push_back(std::make_shared<ArmorEquipment>("Vambraces", 20 + std::rand()  % 50));      
        equipment_.push_back(std::make_shared<ArmorEquipment>("Cuisses", 30 + std::rand() % 50));            
        equipment_.push_back(std::make_shared<ShieldEquiment>("Shield", 50 + std::rand()  % 50, 0));      
       
    }

};

class Berserkers : public MeleeFighters{

public:

    Berserkers (std::string const &name) : MeleeFighters(name) {
        mainSkill.strength = 15;
        mainSkill.dexterity = 8;
    }

    int calculateDamage() override {
        // the more dexterity and health, the more x damage.
        return (mainSkill.dexterity * secondarySkill.mastery * mainSkill.strength/30 );
    }

};

class SwordMan : public MeleeFighters{

public:

    SwordMan (std::string const &name) : MeleeFighters(name) {
        mainSkill.strength = 8;
        mainSkill.dexterity = 15;
    }
    
    int calculateDamage() override {
        // the more dexterity and health, the more x damage.
        return (mainSkill.strength * secondarySkill.mastery * mainSkill.dexterity/30 );
    }

};


class RangedFighters : public Character{

protected:

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


};

class Mage : public RangedFighters{

public:

    Mage (std::string name) : RangedFighters(name) {}

    void generateEquipment() override {
        equipment_.clear();
        equipment_.push_back(std::make_shared<BattleEquipment>("Staff", 140 + std::rand()  % 100));      
        equipment_.push_back(std::make_shared<ArmorEquipment>("Magic hat", 30 + std::rand() % 30));      
        equipment_.push_back(std::make_shared<ArmorEquipment>("Magic costume", 35 + std::rand()  % 30));      
        equipment_.push_back(std::make_shared<ShieldEquiment>("Magic barrier", 30 + std::rand()  % 50, std::rand()  % 40));          
       
    }

    int calculateDamage() override {
        // the more accuracy and mastery, the more intelligence
        return (secondarySkill.accuracy * secondarySkill.mastery * mainSkill.intelligence/30 );

    }

};

class BeastTamer : public RangedFighters{

public:

    BeastTamer(std::string name) : RangedFighters(name) {}

    void generateEquipment() override {
        equipment_.clear();
        equipment_.push_back(std::make_shared<BattleEquipment>("Whip", 40 + std::rand()  % 100));   
        equipment_.push_back(std::make_shared<ArmorEquipment>("Magic hat", 20 + std::rand() % 30));      
        equipment_.push_back(std::make_shared<ArmorEquipment>("Magic costume", 35 + std::rand()  % 30));   
        equipment_.push_back(std::make_shared<ShieldEquiment>("Pet", 200 + std::rand()  % 50, 70 +  + std::rand()  % 50));        
       
    }
    int calculateDamage() override {
        // the more accuracy and mastery, the more intelligence
        return (secondarySkill.accuracy * secondarySkill.mastery * mainSkill.intelligence/30 );

    }

};*/