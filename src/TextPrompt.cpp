#include "TextPrompt.h"

#include <string>
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <vector>

using namespace std;


    vector<sf::Text*> t;
    vector<sf::Clock*> c;
    vector<float> lm;
    extern sf::Font font;
    int y=0;

void TextPrompt::addPrompt(string s, float d){
    sf::Text *te = new sf::Text();
    te->setString(s);
    te->setFont(font);
    te->setCharacterSize(24);
    te->setColor(sf::Color::White);
    te->setStyle(sf::Text::Bold);
    sf::Clock *ce = new sf::Clock();
    t.push_back(te);
    c.push_back(ce);
    lm.push_back(d);
    te->setPosition(3,y);
    y+=te->getLocalBounds().height + 6;
}

void TextPrompt::draw(sf::RenderWindow &w){
    for(auto iter:t){
    w.draw(*iter);
    }
    for(int i=0;i<t.size();++i){
        if(c[i]->getElapsedTime().asSeconds()>lm[i])
        {
            int dy = t[i]->getLocalBounds().height+6;
            y-=dy;
            
            for(int j=i+1;j<t.size();++j){
            t[j]->move(0,-dy);
            }
            
            delete t[i];
            delete c[i];
            t.erase(t.begin()+i);
            c.erase(c.begin()+i);
            lm.erase(lm.begin()+i);
        }
    }
}
