#include "Sprite.h"
#include <iostream>
using namespace std;

Sprite::Sprite(string nam, std::string s, int siz, int id)
{
    tex = new sf::Texture();
    tex->loadFromFile(s);
    spr = new sf::Sprite();
    spr->setTexture(*tex);
    spr->setOrigin(tex->getSize().x/2.0, tex->getSize().y/2.0);
    spr->setPosition(siz/2.0, siz/2.0);
    spr->setScale(siz*1.0/tex->getSize().x,siz*1.0/tex->getSize().y);
    this->siz = siz;
    vars = map<string, int>();
    locked = 0;

    for(int i=0; i<5; ++i) flags[i] = 0;

    x=0; y=0;
    w=0;
    o=0;
    name = nam;
    sid = id;
}

Sprite::Sprite(Sprite *from)
{
    tex = from->tex;
    siz = from->siz;
    w=from->w;
    oh = from->oh;
    oa = from->oa;
    t1 = from->t1;
    t2 = from->t2;
    t4 = from->t4;
    vars = map<string, int>();
    o=0;

    for(auto iter:from->vars)
        {
            vars.insert(iter);
        }

    locked=0;

    for(int i=0; i<5; ++i) flags[i] = from->flags[i];

    spr = new sf::Sprite();
    spr->setTexture(*tex);
    spr->setOrigin(tex->getSize().x/2.0, tex->getSize().y/2.0);
    spr->setPosition(siz/2.0, siz/2.0);
    spr->setScale(siz*1.0/tex->getSize().x,siz*1.0/tex->getSize().y);
    x=0;
    y=0;
    this->name = from->name;
}

Sprite::~Sprite()
{
    delete spr;
}

void Sprite::resize(int siz)
{
    spr->setScale(siz*1.0/tex->getSize().x,siz*1.0/tex->getSize().y);
    spr->setPosition(x*siz+siz/2.0,y*siz+siz/2.0);
    this->siz = siz;
}

void Sprite::draw(sf::RenderWindow &w, int siz, bool resized)
{
    if(resized) resize(siz);

    locked = 0;
    w.draw(*spr);
}

void Sprite::move(int x, int y)
{
    this->x += x;
    this->y += y;
    this->spr->move(x*siz, y*siz);
}

int Sprite::isWall()
{
    return w;
}

void Sprite::setWall(int _w)
{
    w=_w;
}

void Sprite::setOnHit(list<string> *t)
{
    if(t==nullptr) return;

    oh = t;
    flags[0] = 1;
}

void Sprite::setOnAct(list<string> *t)
{
    if(t==nullptr) return;

    oa = t;
    flags[4] = 1;
}

void Sprite::setT1(list<string> *t)
{
    if(t==nullptr) return;

    t1 = t;
    flags[1] = 1;
}
void Sprite::setT2(list<string> *t)
{
    if(t==nullptr) return;

    t2 = t;
    flags[2] = 1;
}
void Sprite::setT4(list<string> *t)
{
    if(t==nullptr) return;

    t4 = t;
    flags[3] = 1;
}
int Sprite::hasOnHit()
{
    return flags[0]&&(!locked);
}
int Sprite::hasOnAct()
{
    return flags[4]&&(!locked);
}
int Sprite::hasT1()
{
    return flags[1]&&(!locked);
}
int Sprite::hasT2()
{
    return flags[2]&&(!locked);
}
int Sprite::hasT4()
{
    return flags[3]&&(!locked);
}
list<string> *Sprite::getOnHit()
{
    return oh;
}
list<string> *Sprite::getOnAct()
{
    return oa;
}
list<string> *Sprite::getT1()
{
    return t1;
}
list<string> *Sprite::getT2()
{
    return t2;
}
list<string> *Sprite::getT4()
{
    return t4;
}
void Sprite::lock()
{
    locked = 1;
}
void Sprite::setVar(string var, int val)
{
    cout<<"\nSetting variable "<<var<<" to "<<val;

    if(vars.find(var)==vars.end())
        {
            vars[var]=val;
        }
    else
        {
            vars[var] = val;
        }
}
int Sprite::getVar(string var)
{

    if(!hasVar(var))
        {
            cerr<<"\nTRIED TO ACCESS UNDEFINED VARIABLE "<<var<<" in sprite "<<name;
            return 0;
        }
    else
        {
            return vars[var];
        }
}

int Sprite::hasVar(string var)
{
    if(vars.find(var)==vars.end())
        {
            return 0;
        }

    return 1;
}

void Sprite::rotate(int on)
{
    spr->setRotation(90*on);
    o=on;
}
