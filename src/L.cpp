#include "L.h"
#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <fstream>

#include "utils.h"
#include "Sprite.h"
#include "SpriteFactory.h"

using namespace std;

extern Sprite ** *board;
extern map<string, Sprite *> spritelist;
extern void drawText(string txt, int d = 8);
extern Sprite *player;
extern int bSiz, tileSiz, vSiz;
extern sf::View view;

extern int tlx,tly;

stack<string> sstk;
stack<int> istk;
int stackmode = 0;

void l::parse(list<string> *obj, int h, int k)
{
    int r=1,b=0;

    if(obj == nullptr)
        {
            cerr<<"\nLIST DOES NOT EXIST";
            return;
        }

    if(obj->empty())
        {
            cerr<<"\nCANNOT PARSE EMPTY LIST";
            return;
        }

    for(auto iter = obj->begin(); iter!=obj->end(); ++iter)
        {
            if(*iter==";")
                {
                    r=1;
                    b=0;
                }

            if(*iter == "else")
                {
                    r=!r;
                }

            if(r)
                {
                    if(*iter=="end")
                        {
                            cout<<"\n\nYOU'RE DEAD, too bad!\n\n";
                            player->x=-1;player->y=-1;
                        }
                    else if(*iter=="ss")
                        {
                            int x = strToInt(*(++iter)) + h;
                            int y = strToInt(*(++iter)) + k;
                            string s = *(++iter);

                            if(!(x>=0&&x<bSiz && y>=0&&y<bSiz))
                                {
                                    cerr<<"\nTried to modify "<<x<<", "<<y<<", which is out of bounds. Ignoring.";
                                }
                            else
                                {
                                    delete board[x][y];
                                    board[x][y] = new Sprite(spritelist[s]);
                                    board[x][y]->move(x,y);
                                    board[x][y]->lock();
                                }
                        }
                    else if(*iter == "pm")
                        {
                            int x = strToInt(*(++iter));
                            int y = strToInt(*(++iter));

                            if(!(x+player->x>=0&&x+player->x<bSiz && y+player->y>=0&&y+player->y<bSiz))
                                {
                                    cerr<<"\nTried to move player to "<<x<<", "<<y<<", which is out of bounds. Ignoring.";
                                }
                            else
                                {
                                    view.move(x*tileSiz,y*tileSiz);
                                    player->move(x,y);
                                    tlx += x;
                                    tly += y;
                                }
                        }
                    else if(*iter=="gs")
                        {
                            int x = strToInt(*(++iter))+h;
                            int y = strToInt(*(++iter))+k;

                            if(!(x>=0&&x<bSiz && y>=0&&y<bSiz))
                                {
                                    cerr<<"\nTried to get "<<x<<", "<<y<<", which is out of bounds. Ignoring.";
                                    sstk.push("");
                                }
                            else
                                {
                                    sstk.push(board[x][y]->name);
                                    //cout<<"\nPushed to top: "<<sstk.top();
                                }
                        }
                    else if(*iter=="pgx")
                        {
                            istk.push(player->x - h);
                            //cout<<"\nPushed to top: "<<istk.top();
                        }
                    else if(*iter=="pgy")
                        {
                            istk.push(player->y - k);
                            //cout<<"\nPushed to top: "<<istk.top();
                        }
                    else if(*iter=="isp")
                        {
                            istk.push(strToInt(*(++iter)));
                            //cout<<"\nPushed to top: "<<istk.top();
                        }
                    else if(*iter=="ssp")
                        {
                            sstk.push(*(++iter));
                            //cout<<"\nPushed to top: "<<sstk.top();
                        }
                    else if(*iter=="add")
                        {
                            if(istk.size()<2)
                                {
                                    cerr<<"\nInvalid addition, less than 2 numbers in stack!";
                                }
                            else
                                {
                                    int a = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    //cout<<"\nPopped int stack";
                                    int b = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    //cout<<"\nPopped int stack";
                                    istk.push(a+b);
                                    //cout<<"\nPushed to top: "<<istk.top();
                                }
                        }
                    else if(*iter=="sub")
                        {
                            if(istk.size()<2)
                                {
                                    cerr<<"\nInvalid subtraction, less than 2 numbers in stack!";
                                }
                            else
                                {
                                    int a = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    int b = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    istk.push(a-b);
                                    //cout<<"\nPushed to top: "<<istk.top();
                                }
                        }
                    else if(*iter=="mul")
                        {
                            if(istk.size()<2)
                                {
                                    cerr<<"\nInvalid multiply, less than 2 numbers in stack!";
                                }
                            else
                                {
                                    int a = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    int b = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    istk.push(a*b);
                                    //cout<<"\nPushed to top: "<<istk.top();
                                }
                        }
                    else if(*iter=="dev")
                        {
                            if(istk.size()<2)
                                {
                                    cerr<<"\nInvalid division, less than 2 numbers in stack!";
                                }
                            else
                                {
                                    int a = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    int b = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    istk.push(a/b);
                                    //cout<<"\nPushed to top: "<<istk.top();
                                }
                        }
                    else if(*iter=="eq")
                        {
                            if(istk.size()<2)
                                {
                                    cerr<<"\nInvalid equality check, less than 2 numbers in stack!";
                                }
                            else
                                {
                                    int a = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    int b = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    istk.push(a==b);
                                    //cout<<"\nPushed to top: "<<istk.top();
                                }
                        }
                    else if(*iter=="seq")
                        {
                            if(sstk.size()<1)
                                {
                                    cerr<<"\nInvalid equality check, less than 2 strings in stack!";
                                }
                            else
                                {
                                    string a = sstk.top();
                                    sstk.pop();//cout<<"\nPopped str stack";
                                    string b = sstk.top();
                                    sstk.pop();//cout<<"\nPopped str stack";
                                    istk.push(a==b);
                                    //cout<<"\nPushed to top: "<<istk.top();
                                }
                        }
                    else if(*iter=="gt")
                        {
                            if(istk.size()<2)
                                {
                                    cerr<<"\nInvalid greater than, less than 2 numbers in stack!";
                                }
                            else
                                {
                                    int a = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    int b = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    istk.push(a>b);
                                    //cout<<"\nPushed to top: "<<istk.top();
                                }
                        }
                    else if(*iter=="lt")
                        {
                            if(istk.size()<2)
                                {
                                    cerr<<"\nInvalid lesser than, less than 2 numbers in stack!";
                                }
                            else
                                {
                                    int a = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    int b = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    istk.push(a<b);
                                    //cout<<"\nPushed to top: "<<istk.top();
                                }
                        }
                    else if(*iter=="la")
                        {
                            if(istk.size()<2)
                                {
                                    cerr<<"\nInvalid logical AND, less than 2 numbers in stack!";
                                }
                            else
                                {
                                    int a = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    int b = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    istk.push(a&&b);
                                    //cout<<"\nPushed to top: "<<istk.top();
                                }
                        }
                    else if(*iter=="lo")
                        {
                            if(istk.size()<2)
                                {
                                    cerr<<"\nInvalid logical OR, less than 2 numbers in stack!";
                                }
                            else
                                {
                                    int a = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    int b = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    istk.push(a||b); //cout<<"\nPushed to top: "<<istk.top();
                                }
                        }
                    else if(*iter=="ln")
                        {
                            if(istk.size()<1)
                                {
                                    cerr<<"\nInvalid logical NOT, empty stack!";
                                }
                            else
                                {
                                    int a = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    istk.push(!a); //cout<<"\nPushed to top: "<<istk.top();
                                }
                        }
                    else if(*iter=="cs")
                        {
                            int x = strToInt(*(++iter))+h;
                            int y = strToInt(*(++iter))+k;
                            string typ = *(++iter);
                            string fil = *(++iter);

                            if(!(x>=0&&x<bSiz && y>=0&&y<bSiz))
                                {
                                    cerr<<"\nTried to modify script of "<<x<<", "<<y<<", which is out of bounds. Ignoring.";
                                }
                            else
                                {
                                    fstream f(fil);

                                    if(!f)
                                        {
                                            cerr<<"\nERROR OPENING SCRIPT FILE FOR cs IN "<<board[h][k]->name<<", Expect improper working!";
                                        }
                                    else
                                        {
                                            string cl;
                                            list<string> *tList = new list<string>();
                                            getline(f,cl);

                                            if(cl!="SCRIPT")
                                                {
                                                    cerr<<"\nIMPROPER SCRIPT FILE FOR cs IN "<<board[h][k]->name<<", Expect improper working!";
                                                }
                                            else
                                                {
                                                    getline(f,cl);

                                                    while(!f.eof())
                                                        {
                                                            SpriteFactory::compileCmds(cl,board[h][k]->name,tList);
                                                            getline(f,cl);
                                                        }

                                                    if(typ == "oh") board[x][y]->setOnHit(tList);

                                                    if(typ == "t1") board[x][y]->setT1(tList);

                                                    if(typ == "t2") board[x][y]->setT1(tList);

                                                    if(typ == "t4") board[x][y]->setT1(tList);

                                                    if(typ == "ro") l::parse(tList,x,y);

                                                    if(typ == "oa") board[x][y]->setOnAct(tList);
                                                }
                                        }
                                }
                        }
                    else if(*iter=="sv")
                        {
                            int x = strToInt(*(++iter))+h;
                            int y = strToInt(*(++iter))+k;
                            string var = *(++iter);
                            int val = strToInt(*(++iter));

                            if(!(x>=0&&x<bSiz && y>=0&&y<bSiz))
                                {
                                    cerr<<"\nTried to modify variables of "<<x<<", "<<y<<", which is out of bounds. Ignoring.";
                                }
                            else
                                {
                                    board[x][y]->setVar(var,val);
                                }
                        }
                    else if(*iter=="gv")
                        {
                            int x = strToInt(*(++iter))+h;
                            int y = strToInt(*(++iter))+k;
                            string var = *(++iter);

                            if(!(x>=0&&x<bSiz && y>=0&&y<bSiz))
                                {
                                    cerr<<"\nTried to get variables of "<<x<<", "<<y<<", which is out of bounds. Ignoring.";
                                    istk.push(0);
                                }
                            else
                                {
                                    if(!board[x][y]->hasVar(var))
                                        {
                                            cerr<<"\nTried to get variable "<<var<<" of "<<x<<", "<<y<<", which does not exist. Ignoring.";
                                            istk.push(0);
                                        }
                                    else
                                        {
                                            istk.push(board[x][y]->getVar(var));
                                        }
                                }
                        }
                    else if(*iter=="sgv")
                        {
                            string var = *(++iter);
                            int val = strToInt(*(++iter));
                            player->setVar(var,val);
                        }
                    else if(*iter=="ggv")
                        {
                            string var = *(++iter);

                            if(!player->hasVar(var))
                                {
                                    cerr<<"\nTried to get global variable "<<var<<" which does not exist. Ignoring.";
                                    istk.push(0);
                                }
                            else
                                {
                                    istk.push(player->getVar(var));
                                }
                        }
                    else if(*iter=="ssv")
                        {
                            int x = strToInt(*(++iter))+h;
                            int y = strToInt(*(++iter))+k;
                            string var = *(++iter);

                            if(!(x>=0&&x<bSiz && y>=0&&y<bSiz))
                                {
                                    cerr<<"\nTried to modify variables of "<<x<<", "<<y<<", which is out of bounds. Ignoring.";
                                    istk.pop();
                                }
                            else
                                {
                                    board[x][y]->setVar(var,istk.top());
                                    //cout<<"\nPopping "<<istk.top()<<" from int stack";
                                    istk.pop();
                                }
                        }
                    else if(*iter=="ssgv")
                        {
                            string var = *(++iter);
                            player->setVar(var,istk.top());
                            //cout<<"\nPopping "<<istk.top()<<" from int stack";
                            istk.pop();
                        }
                    else if(*iter=="mod")
                        {
                            if(istk.size()<2)
                                {
                                    cerr<<"\nInvalid modulo, less than 2 numbers in stack!";
                                }
                            else
                                {
                                    int a = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    int b = istk.top();
                                    istk.pop(); //cout<<"\nPopped int stack";
                                    istk.push(a%b);
                                    //cout<<"\nPushed to top: "<<istk.top();
                                }
                        }
                    else if(*iter=="rand")
                        {
                            istk.push(rand());
                            //cout<<"\nPushed to top: "<<istk.top();
                        }
                    else if(*iter=="print")
                        {
                            string txt = *(++iter);
                            drawText(txt);
                        }
                    else if(*iter=="sw")
                        {
                            int x = strToInt(*(++iter))+h;
                            int y = strToInt(*(++iter))+k;
                            int p = strToInt(*(++iter));

                            if(!(x>=0&&x<bSiz && y>=0&&y<bSiz))
                                {
                                    cerr<<"\nTried to set wall of "<<x<<", "<<y<<", which is out of bounds. Ignoring.";
                                }
                            else
                                {
                                    board[x][y]->setWall(p);
                                }
                        }
                    else if(*iter=="if")
                        {
                            if(istk.size()<1)
                                {
                                    cerr<<"\nInvalid if, empty stack!";
                                }
                            else
                                {
                                    int c = istk.top();
                                    istk.pop();
                                    //cout<<"\nPopped int stack";

                                    if(c)
                                        {
                                            r=1;
                                        }
                                    else
                                        {
                                            r=0;
                                        }

                                    b=1;
                                }
                        }
                }
        }
}
