#ifndef SPRITE_H
#define SPRITE_H

#include "SFML/Graphics.hpp"
#include <string>
#include <list>

using namespace std;

class Sprite
{
        sf::Texture *tex;
        sf::Sprite *spr;
        int siz;
        list<string> *oa,*oh,*t1,*t2,*t4;
        int flags[5];

        int w;
        int locked;
        
        map<string,int> vars;
        
        Sprite();
    public:
        std::string name;
        int x, y, sid, o;

        Sprite(std::string nam, std::string s,int siz, int id);
        Sprite(Sprite *from);
        ~Sprite();
        void resize(int siz);
        void move(int x, int y);
        void draw(sf::RenderWindow &w, int siz = 0, bool resized = false);
        int isWall();
        void setWall(int _w);
        void setOnHit(list<string> *t);
        void setOnAct(list<string> *t);
        void setT1(list<string> *t);
        void setT2(list<string> *t);
        void setT4(list<string> *t);
        int hasOnHit();
        int hasOnAct();
        int hasT1();
        int hasT2();
        int hasT4();
        list<string> *getOnHit();
        list<string> *getOnAct();
        list<string> *getT1();
        list<string> *getT2();
        list<string> *getT4();
        void lock();
        void setVar(string var, int val);
        int getVar(string var);
        int hasVar(string var);
        void rotate(int on);
        
    protected:
    private:
};

#endif // SPRITE_H
