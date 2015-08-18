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
#include <iostream>
#define WORLD_SIZE 20
#define TILE 32

class Block;
class Mob;
class Clone;
class Special;

Special makeSpec(int posx, int posy, int type, sf::Texture *worldtextures, int id);
Clone makeClone(int pos[], sf::Texture *mctexture);
std::array<int, 2> dirToArr(int dir);
int dirToInt(std::array<int,2 > dir);
Block makeBlock(int type, int pos[]);
bool accessible(int x, int y, Block world[][WORLD_SIZE], Clone clones[], int clone_amount, Special specs[], int spec_amount, Mob mc);


class Block{
protected:
    int type;
    sf::Sprite sprite;
    bool touching;
public:
    Block (int a, sf::Sprite b) {type=a; sprite=b; touching=false;}
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
    sf::Vector2i getPosition(){
        return sf::Vector2i((sprite.getPosition().x - 16)/32, (sprite.getPosition().y - 16)/32);
    }
    bool canAccess(){
        if (type == 10 || type == 20 || type == 40 || type == 50 || type == 70 || type == 11 || type == 31 || type == 1){
            return true;
        }
        else{
            return false;
        }
    }
    bool getSteppedOn(){
        return touching;
    }
    void setSteppedOn(bool yolo){
        touching = yolo;
    }
};

class Mob{
protected:
    sf::Sprite sprite;
public:
    Mob (sf::Sprite a) {sprite = a;}
    Mob () { }
    
    void takeAction(char option, Block world[][WORLD_SIZE], Special specs[], int spec_amount, Clone clones[], int clone_amount, Mob mc){
        if (option == '0' || option == '1' || option == '2' || option == '3'){
            this->setDirection(option - '0');
            std::array<int, 2> dir = dirToArr(option - '0');
            if (accessible(this->getPosition().x + dir[0], this->getPosition().y + dir[1], world, clones, clone_amount, specs, spec_amount, mc)){
                this->move(dir[0], dir[1]);
            }
        }
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
    int getDirection(){
        return this->sprite.getRotation();
    }
    sf::Vector2i getPosition(){
        return sf::Vector2i((sprite.getPosition().x - 16)/32, (sprite.getPosition().y - 16)/32);
    }
    sf::Vector2i getnPosition(){
        int dir[2];
        dir[0] = dirToArr(this->getDirection() / 90)[0];
        dir[1] = dirToArr(this->getDirection() / 90)[1];
        return sf::Vector2i(this->getPosition().x + dir[0], this->getPosition().y + dir[1]);
    }
};

class Clone: public Mob{
public:
    int move_pos;
};

class Special: public Block{
    int id;
public:
    int getID(){
        return id;
    }
    Special (int a, sf::Sprite b, int c) {type=a; sprite=b; id = c;}
    Special () {}
    void steppedOn(Special specs[], int spec_count, Clone clones[], int *clone_count, sf::Texture*worldtextures){
        if (this->getType() == 11){
            for (int i = 0; i < spec_count; i++){
                if (specs[i].getType() == 90 && specs[i].getID() == this->id){
                    int pos[2] = {specs[i].getPosition().x, specs[i].getPosition().y};
                    clones[*clone_count] = makeClone(pos, worldtextures);
                    *clone_count += 1;
                }
                else if (specs[i].getType() == 21 && specs[i].getID() == this->id){
                    specs[i] = makeSpec(specs[i].getPosition().y, specs[i].getPosition().x, 31, worldtextures, specs[i].getID());
                }
                else if (specs[i].getType() == 31 && specs[i].getID() == this->id){
                    specs[i] = makeSpec(specs[i].getPosition().y, specs[i].getPosition().x, 21, worldtextures, specs[i].getID());
                }
            }
        }
        if (this->getType() == 1){
            for (int i = 0; i < spec_count; i++){
                if (specs[i].getType() == 21 && specs[i].getID() == this->id){
                    specs[i] = makeSpec(specs[i].getPosition().y, specs[i].getPosition().x, 31, worldtextures, specs[i].getID());
                }
            }
        }
    }
    void steppedOff(Special specs[], int spec_count, Clone clones[], int *clone_count, sf::Texture*worldtextures){
        if (this->getType() == 1){
            for (int i = 0; i < spec_count; i++){
                if (specs[i].getType() == 31 && specs[i].getID() == this->id){
                    specs[i] = makeSpec(specs[i].getPosition().y, specs[i].getPosition().x, 21, worldtextures, specs[i].getID());
                }
            }
        }
    }
};

class MessageBox{
    std::string message;
    
public:
    
};

int load_fg(Special world[], std::string world_name, sf::Texture* worldtextures);
void load_bg(Block world[][WORLD_SIZE], std::string world_name, sf::Texture* worldtextures);
void draw(sf::RenderWindow *window, Block world[][WORLD_SIZE], Special worlds[], int worlds_amount);
void runClones(Clone clones[], int clone_amount, char moves[], Block world[][WORLD_SIZE], Special specs[], int spec_amount, Mob mc);
#endif /* defined(__clone_dungeon__GameItems__) */
