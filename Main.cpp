#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <map>
#include <algorithm>
#include <vector>
#include <utility>
#include <set>
#include <ctime>

#include "Sprite.h"
#include "SpriteFactory.h"
#include "L.h"
#include "TextPrompt.h"

#define TIME_WIDTH 0.25

using namespace std;

int game_mode = 0;
int romEn = 0;
bool resized = false;
Sprite*** board;
int bSiz,vSiz,tileSiz;
sf::View view;
sf::Font font;

Sprite *player;
int tlx=0,tly=0,id=1;

sf::Text txtOS;
sf::Clock txtClk;
float txtLif=5;
int txtIsOn=0;

map<string, Sprite *> spritelist;


void drawText(string txt, int d = 8);
void loadRom(sf::RenderWindow &);
int strToInt(const string &);
string intToStr(int);
void makeViewPort(sf::RenderWindow &);
vector<string> split(const string &s, const string &delim, const bool keep_empty = false);
void loadMap(string file);


const char *ROMFILE = "rom.txt";


int valX(int x)
{
    return x>=0&&x<bSiz;
}
int valY(int y)
{
    return y>=0&&y<bSiz;
}

int main()
{
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(600, 600), "Programmer (Alpha Release)");
    window.setFramerateLimit(24);
    SpriteFactory::init();

    if(!font.loadFromFile("font.ttf")) cout<<"NO FONT\n";

    //NoRomLoaded
    sf::Text noRomLoaded;
    noRomLoaded.setFont(font);
    noRomLoaded.setString("NO ROM LOADED");
    noRomLoaded.setCharacterSize(30);
    noRomLoaded.setColor(sf::Color::Red);
    noRomLoaded.setStyle(sf::Text::Bold);

    sf::Clock clk;
    int tk = 0;
    int ptk = 0;

    while(window.isOpen())
        {
            if(!game_mode)
                {
                    sf::Event event;

                    while(window.pollEvent(event))
                        {
                            if(event.type == sf::Event::Closed)
                                window.close();

                            if(event.type == sf::Event::KeyPressed)
                                if(event.key.code == sf::Keyboard::R)
                                    loadRom(window);
                        }

                    window.clear();
                    window.draw(noRomLoaded);
                    window.display();

                    if(clk.getElapsedTime().asSeconds()>TIME_WIDTH)
                        {
                            clk.restart();
                            ++tk;

                            if(tk == 4) tk = 0;
                        }
                }
            else if(!romEn)
                {
                    cerr<<"\nROM NOT ENABLED! PLEASE RESTART FOR SMOOTH FUNCTIONING!\n";
                    game_mode = 0;
                }
            else
                {
                    if(tk!=ptk)
                        {
                            for(int i=0; i<bSiz; ++i)
                                {
                                    for(int j=0; j<bSiz; ++j)
                                        {
                                            if(board[i][j] == nullptr) continue;
                                            if(board[i][j]->hasT4()) l::parse(board[i][j]->getT4(),i,j);

                                            if((tk == 1 || tk == 3) && board[i][j]->hasT2())
                                                {
                                                    l::parse(board[i][j]->getT2(),i,j);
                                                }

                                            if((tk == 3) && board[i][j]->hasT1())
                                                {
                                                    l::parse(board[i][j]->getT1(),i,j);
                                                }
                                        }
                                }

                            ptk=tk;
                        }
                    sf::Event event;

                    while(window.pollEvent(event))
                        {
                            if(event.type == sf::Event::Closed)
                                window.close();

                            if(event.type == sf::Event::Resized)
                                {
                                    makeViewPort(window);
                                    resized = true;
                                }

                            if(event.type == sf::Event::KeyReleased)
                                {
                                    if(event.key.code == sf::Keyboard::Space)
                                        {
                                            int o = player->o;
                                            int y = player->y + (o==0?-1:(o==2?1:0));
                                            int x = player->x + (o==3?-1:(o==1?1:0));

                                            if(!(valX(x)&&valY(y)))
                                                {
                                                    cerr<<"\nPlayer cannot act outside the realm of his existence.";
                                                }
                                            else
                                                {
                                                    if(board[x][y]->hasOnAct())
                                                        l::parse(board[x][y]->getOnAct(),x,y);
                                                }
                                        }

                                    if(event.key.code == sf::Keyboard::W)
                                        {
                                            player->rotate(0);

                                            if(!valX(player->x) || !valY(player->y-1) || board[player->x][player->y-1]==nullptr) continue;

                                            if(!board[player->x][player->y-1]->isWall())
                                                {
                                                    view.move(0,-tileSiz);
                                                    player->move(0,-1);
                                                    --tly;

                                                    if(board[player->x][player->y]->hasOnHit()) l::parse(board[player->x][player->y]->getOnHit(), player->x, player->y);
                                                }
                                        }

                                    if(event.key.code == sf::Keyboard::S)
                                        {
                                            player->rotate(2);

                                            if(!valX(player->x) || !valY(player->y+1) || board[player->x][player->y+1]==nullptr) continue;

                                            if(!board[player->x][player->y+1]->isWall())
                                                {
                                                    view.move(0,tileSiz);
                                                    player->move(0,1);
                                                    ++tly;

                                                    if(board[player->x][player->y]->hasOnHit()) l::parse(board[player->x][player->y]->getOnHit(), player->x, player->y);
                                                }
                                        }

                                    if(event.key.code == sf::Keyboard::A)
                                        {
                                            player->rotate(3);

                                            if(!valX(player->x-1) || !valY(player->y) || board[player->x-1][player->y]==nullptr) continue;

                                            if(!board[player->x-1][player->y]->isWall())
                                                {
                                                    view.move(-tileSiz,0);
                                                    player->move(-1,0);
                                                    --tlx;

                                                    if(board[player->x][player->y]->hasOnHit()) l::parse(board[player->x][player->y]->getOnHit(), player->x, player->y);
                                                }
                                        }

                                    if(event.key.code == sf::Keyboard::D)
                                        {
                                            player->rotate(1);

                                            if(!valX(player->x+1) || !valY(player->y) || board[player->x+1][player->y]==nullptr) continue;

                                            if(!board[player->x+1][player->y]->isWall())
                                                {
                                                    view.move(tileSiz,0);
                                                    player->move(1,0);
                                                    ++tlx;

                                                    if(board[player->x][player->y]->hasOnHit()) l::parse(board[player->x][player->y]->getOnHit(), player->x, player->y);
                                                }
                                        }
                                }
                        }

                    window.clear();
                    window.setView(view);
                    int xd = max(tlx,0);
                    int yd = max(tly,0);

                    for(int i=xd; i<vSiz+tlx/*bSiz*/; ++i)
                        {
                            for(int j=yd; j<vSiz+tly/*bSiz*/; ++j)
                                {
                                    if(i>=bSiz || j>=bSiz || board[i][j]==nullptr) continue;

                                    board[i][j]->draw(window,tileSiz, resized);
                                }
                        }

                    //draw Player (LAST)
                    player->draw(window,tileSiz, resized);
                    //draw Text (UI Element, on top of even player)

                    window.setView(window.getDefaultView());
                    TextPrompt::draw(window);

                    window.display();

                    if(resized) resized = false;

                    if(clk.getElapsedTime().asSeconds()>TIME_WIDTH)
                        {
                            clk.restart();
                            ++tk;

                            if(tk == 4) tk = 0;
                        }
                }
        }

    return 0;
}


void loadRom(sf::RenderWindow &window)
{
    fstream rom(ROMFILE);
    string s;
    getline(rom,s);

    if(s!="ROMFILE")
        {
            cerr<<"CORRUPT ROM!\n";
            return;
        }
    else
        {
            game_mode = 1;
            int flags[] = {0,0};
            int mode;
            getline(rom,s);

            while(!rom.eof())
                {
                    if(s=="")
                        {
                            getline(rom,s);
                            continue;
                        }

                    if(s=="init")
                        {
                            mode = 1;
                            cout<<"\nInitializing";
                        }
                    else if(s=="bsl")
                        {
                            mode = 2;
                            cout<<"\nGetting Sprite List";
                        }
                    else if(mode==1)
                        {
                            if(s.substr(0,2)=="sb")
                                {
                                    if(flags[0])
                                        {
                                            cout<<"\nMultiple Set Board Calls, ignoring";
                                        }
                                    else
                                        {
                                            flags[0] = 1;
                                            int siz = strToInt(s.substr(3));
                                            bSiz = siz;
                                            cout<<"\nBoard size: "<<siz;
                                            board = (Sprite ** *) malloc(sizeof(Sprite **)*siz);
                                            board[0] = (Sprite **) malloc(sizeof(Sprite *)*siz*siz);

                                            for(int i=0; i<siz; ++i) board[i] = (*board + siz*i);

                                            for(int i=0; i<siz; ++i) for(int j=0; j<siz; ++j) board[i][j] = nullptr;

                                            romEn = flags[0] && flags[1];

                                            if(romEn) makeViewPort(window);
                                        }
                                }
                            else if(s.substr(0,2)=="sv")
                                {
                                    if(flags[1])
                                        {
                                            cout<<"\nMultiple Set View Calls, ignoring";
                                        }
                                    else
                                        {
                                            flags[1] = 1;
                                            int siz = strToInt(s.substr(3));
                                            vSiz = siz;
                                            cout<<"\nView size: "<<siz;
                                            romEn = flags[0] && flags[1];

                                            if(romEn) makeViewPort(window);
                                        }
                                }
                            else if(s.substr(0,2) == "pi")
                                {
                                    player = new Sprite("player", s.substr(3),tileSiz, 0);
                                    player->move(vSiz/2,vSiz/2);
                                }
                        }
                    else if(mode == 2)
                        {
                            if(s=="esl")
                                {
                                    mode = 3;
                                    getline(rom,s);
                                    continue;
                                }

                            string::size_type p = s.find(' ');

                            if(p==string::npos)
                                {
                                    cout<<"\nERROR in sprite list, "<<s;
                                    getline(rom,s);
                                    continue;
                                }

                            string nam = s.substr(0,p);
                            string loc = s.substr(p+1);
                            spritelist.insert(make_pair(nam, SpriteFactory::getSpriteFromFile(loc,nam,tileSiz, id++)));
                        }
                    else if(mode==3)
                        {
                            if(s.substr(0,2)=="ss")
                                {
                                    vector<string> toks = split(s.substr(3)," ",false);

                                    if(toks.size()!=3) cerr<<"\nSS has wrong size in "<<s;
                                    else
                                        {
                                            int x = strToInt(toks[0]);
                                            int y = strToInt(toks[1]);
                                            board[x][y] = new Sprite(spritelist[toks[2]]);
                                            board[x][y]->move(x,y);
                                        }
                                }

                            if(s.substr(0,1)=="m")
                                {
                                    string file = s.substr(2);
                                    loadMap(file);
                                }
                        }

                    getline(rom,s);
                }
        }
}

void makeViewPort(sf::RenderWindow &window)
{
    int minResoln = min(window.getSize().x,window.getSize().y);
    tileSiz = minResoln/vSiz;
    view = sf::View(sf::FloatRect(0, 0, tileSiz*vSiz, tileSiz*vSiz));
    view.setViewport(sf::FloatRect((window.getSize().x-tileSiz*vSiz)/(2.0*window.getSize().x), (window.getSize().y - tileSiz*vSiz)/(2.0*window.getSize().y),tileSiz*vSiz*1.0/window.getSize().x,tileSiz*vSiz*1.0/window.getSize().y));
    window.setView(view);
    cout<<"\nView Made";
}

void drawText(string txt, int d)
{
    TextPrompt::addPrompt(txt,d);
}

void loadMap(string file)
{
    fstream f(file);

    if(!f)
        {
            cerr<<"\nERROR OPENING MAP FILE "<<file<<"!";
            return;
        }

    string cl;
    getline(f,cl);

    if(cl!="MAP")
        {
            cerr<<"\nINCORRECTLY FORMATTED MAP FILE "<<file<<"!";
            return;
        }
        
    getline(f,cl);
    string cs;
    while(!f.eof())
        {
            //if(cl==""){getline(f,cl);
             //               continue;}
            if(cl.substr(0,2)=="s ")
                {
                    cs = cl.substr(2);
                }
            else
                {
                    vector<string> tmphdr = split(cl," ",false);

                    if(tmphdr.size()<2)
                        {
                            if(tmphdr.size()!=0)cerr<<"\nERROR, SIZE OF LINE "<<cl<<" IS LESS THAN 2 IN MAP "<<file;

                            getline(f,cl);
                            continue;
                        }
                    vector<int> coords;
                    for(auto iter : tmphdr)
                        {
                            coords.push_back(strToInt(iter));
                        }
                    int x = coords[0];
                    for(int y=1; (unsigned int)y<coords.size(); ++y)
                        {
                            board[x][coords[y]] = new Sprite(spritelist[cs]);
                            board[x][coords[y]]->move(x,coords[y]);
                        }
                }
            getline(f,cl);
        }
            cout<<"\nLOADED MAP "<<file;
}
