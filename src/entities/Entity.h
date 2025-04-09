#ifndef PROJET_CPP_ENTITY_H
#define PROJET_CPP_ENTITY_H

#include <string>

class Entity {
public:
    Entity(std::string name="Default Name", int hp=100);

    //Getters
    int getHp() const{ return hp; }
    std::string getName() const { return name; }

    //Setters
    void setHp(int new_hp) { hp = new_hp; }
    void setName(std::string new_name) { name = new_name; }

private:
    int hp;
    std::string name;


};


#endif //PROJET_CPP_ENTITY_H
