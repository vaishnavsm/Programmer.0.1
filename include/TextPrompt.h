#ifndef TEXTPROMPT_H
#define TEXTPROMPT_H
#include <string>
#include <SFML\Graphics.hpp>

using namespace std;

namespace TextPrompt{
    
    void addPrompt(string s, float d);
    void draw(sf::RenderWindow &w);
};

#endif // TEXTPROMPT_H
