#include "character.h"

class Defender : public Character {

public:

    Defender(std::string name) ;

    int calculateDamage() const override;

    void generateEquipment() override;


};

class Healer : public Character {

public:
 
    Healer(std::string name);

    // damage and heal equals
    int calculateDamage() const override;

    void generateEquipment() override;
};

class MeleeFighters : public Character {

public:

    MeleeFighters(std::string name);
    
    void generateEquipment() override;

};

class Berserkers : public MeleeFighters{

public:

    Berserkers (std::string name);

    int calculateDamage() const override;

};

class SwordMan : public MeleeFighters{

public:

    SwordMan (std::string name);
    
    int calculateDamage() const override;

};


class RangedFighters : public Character{

protected:

    RangedFighters (std::string name);


};

class Mage : public RangedFighters{

public:

    Mage (std::string name);

    void generateEquipment() override;

    int calculateDamage() const override;

};

class BeastTamer : public RangedFighters{

public:

    BeastTamer(std::string name) ;

    void generateEquipment() override;
    int calculateDamage() const override;

};