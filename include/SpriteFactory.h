#ifndef SPRITEFACTORY_H
#define SPRITEFACTORY_H

#include <string>
#include "Sprite.h"

using namespace std;

namespace SpriteFactory
{
    
    void init();
    Sprite *getSpriteFromFile(string file, string name, int siz, int id);
    void compileCmds(string cl,string name, list<string> *tList);

}

#endif // SPRITEFACTORY_H
