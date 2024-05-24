#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <memory>
#include <character_class.h>

int main(){

    Defender defender("Arthur");
    std::cout << "character defender" << std::endl;
    defender.generateEquipment();
    defender.InfoCharacter();

    Healer healer("Genri");
    std::cout << "character healer" << std::endl;
    healer.generateEquipment();
    healer.InfoCharacter();

    Mage mage("Ignil");
    std::cout << "character mage" << std::endl;
    mage.generateEquipment();
    mage.InfoCharacter();

    BeastTamer BeastTamer("Robin");
    std::cout << "character swordman" << std::endl;
    BeastTamer.generateEquipment();
    BeastTamer.InfoCharacter();

    Berserkers berserk("Rufus");
    std::cout << "character berserk" << std::endl;
    berserk.generateEquipment();
    berserk.InfoCharacter();

    SwordMan SwordMan("Gvin");
    std::cout << "character swordman" << std::endl;
    SwordMan.generateEquipment();
    SwordMan.InfoCharacter();

    return 0;
}

