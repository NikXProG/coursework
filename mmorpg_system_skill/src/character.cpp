#include "../include/character.h"

void Character::InfoCharacter() const{

    std::cout << "equipment information:" << std::endl;

    for (const auto& item : this->getEquipment()) {
        std::cout << "Name: " << item->getName() << ", Defense: " << item->getDefense() << ", Attack: " << item->getAttack() << std::endl;
    }

    int sumDefense = 0;
    int sumAttack = 0;

    for (const auto& item : this->getEquipment()) {
        sumDefense += item->getDefense();
        sumAttack += item->getAttack();
    }

    std::cout << "Character Summary:" << std::endl;
    sumAttack += this->calculateDamage();

    std::cout << "Summary defense: " << sumDefense << ", Summary attack: " << sumAttack << std::endl << std::endl;

}