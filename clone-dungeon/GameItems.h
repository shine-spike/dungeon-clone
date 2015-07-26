//
//  GameItems.h
//  clone-dungeon
//
//  Created by Jack McKinney on 26/07/2015.
//  Copyright (c) 2015 Jack McKinney. All rights reserved.
//

#ifndef __clone_dungeon__GameItems__
#define __clone_dungeon__GameItems__

#include <stdio.h>
#include <array>
#include <SFML/Graphics.hpp>
#define WORLD_SIZE 20
#define TILE 32


std::array<int, 2> dirToArr(int dir);
int dirToInt(std::array<int,2 > dir);
class Block{
    int type;
    sf::Sprite sprite;
public:
    Block (int a, sf::Sprite b) {type=a; sprite=b;}
    Block () {}
    int getType(){
        return type;
    }
    sf::Sprite getSprite(){
        return sprite;
    }
    void setSprite(sf::Sprite set){
        sprite = set;
    }
    void setDirection(int dir){
        sprite.setRotation(dir * 90);
    }
    bool canAccess(){
        if (type == 10 || type == 20 || type == 40 || type == 50 || type == 70){
            return true;
        }
        else{
            return false;
        }
    }
};

class Mob{
protected:
    sf::Sprite sprite;
public:
    Mob (sf::Sprite a) {sprite = a;}
    Mob () { }
    
    void takeAction(char option, Block world[][WORLD_SIZE]){
        if (option == '0' || option == '1' || option == '2' || option == '3'){
            this->setDirection(option - '0');
            std::array<int, 2> dir = dirToArr(option - '0');
            if (world[this->getPosition().y + dir[1]][this->getPosition().x + dir[0]].canAccess()){
                this->move(dir[0], dir[1]);
            }
        }
        //        else if (option == 'A'){
        //
        //        }
    }
    
    sf::Sprite getSprite(){
        return sprite;
    }
    sf::Sprite setSprite(sf::Sprite a){
        sprite = a;
    }
    void move(float x, float y){
        sprite.setPosition(sprite.getPosition().x + x * TILE, sprite.getPosition().y + y * TILE);
    }
    void setDirection(int dir){
        sprite.setRotation(dir * 90);
    }
    sf::Vector2i getPosition(){
        return sf::Vector2i((sprite.getPosition().x - 16)/32, (sprite.getPosition().y - 16)/32);
    }
};
void load(Block world[][WORLD_SIZE], std::string world_name, sf::Texture* worldtextures);
void draw(sf::RenderWindow *window, Block world[][WORLD_SIZE]);

class Clone: public Mob{
public:
    int move_pos;
};

void runClones(Clone clones[], int clone_amount, char moves[], Block world[][WORLD_SIZE]);

Clone makeClone(int pos[], sf::Texture *mctexture);

#endif /* defined(__clone_dungeon__GameItems__) */
