#include "SpriteFactory.h"

#include <string>
#include <fstream>
#include <iostream>
#include <map>

#include "Sprite.h"
#include "utils.h"
#include "L.h"

using namespace std;

map<string,pair<int, int> > cmd;

extern Sprite*** board;

void SpriteFactory::init()
{
    cmd["ss"]   = make_pair(1,3);
    cmd["gs"]   = make_pair(2,2);
    cmd["pgx"]  = make_pair(3,0);
    cmd["pgy"]  = make_pair(4,0);
    cmd["pm"]   = make_pair(5,2);
    cmd["end"]  = make_pair(6,0);
    cmd["if"]   = make_pair(7,0);
    cmd["isp"]  = make_pair(8,1);
    cmd["ssp"]  = make_pair(9,1);
    cmd["add"]  = make_pair(10,0);
    cmd["sub"]  = make_pair(11,0);
    cmd["mul"]  = make_pair(12,0);
    cmd["dev"]  = make_pair(13,0);
    cmd["eq"]   = make_pair(14,0);
    cmd["seq"]  = make_pair(15,0);
    cmd["gt"]   = make_pair(16,0);
    cmd["lt"]   = make_pair(17,0);
    cmd["la"]   = make_pair(18,0);
    cmd["lo"]   = make_pair(19,0);
    cmd["ln"]   = make_pair(19,0);
    cmd["else"] = make_pair(20,0);
    cmd[";"]    = make_pair(21,0);
    cmd["cs"]   = make_pair(22,4);
    cmd["mod"]  = make_pair(23,0);
    cmd["sv"]   = make_pair(24,4);
    cmd["gv"]   = make_pair(25,3);
    cmd["sgv"]  = make_pair(26,2);
    cmd["ggv"]  = make_pair(27,1);
    cmd["ssv"]  = make_pair(28,3);
    cmd["ssgv"] = make_pair(29,1);
    cmd["rand"] = make_pair(30,0);
    cmd["print"]= make_pair(31,1);
    cmd["sw"]   = make_pair(32,3);
}

Sprite *SpriteFactory::getSpriteFromFile(string file, string name, int siz, int id)
{
    fstream f(file);

    if(!f)
        {
            cerr<<"\nERROR LOADING FILE "<<file<<" FOR SPRITE "<<name<<", expect improper working";
            return nullptr;
        }

    string cl;
    getline(f,cl);

    if(cl!="SPRITE")
        {
            cerr<<"\nSPRITE HEADER NOT FOUND IN FILE "<<file<<" FOR SPRITE "<<name<<", expect improper working";
            return nullptr;
        }

    getline(f,cl);

    if(cl!=name)
        {
            cerr<<"\nINCORRECTLY NAMED SPRITE FOUND IN FILE "<<file<<" FOR SPRITE "<<name<<", expect improper working";
        }

    getline(f,cl);
    int i=0,w=0,ro = 0;
    string img;
    list<string> *tList;
    list<string> *a[]= {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
    int mode = 0;

    while(!f.eof())
        {
            if(cl.empty())
                {
                    getline(f,cl);
                    continue;
                }

            if(cl.substr(0,2)=="i ")
                {
                    //Image locn;
                    i=1;
                    img = cl.substr(2);
                }
            else if(cl.substr(0,2)=="w ")
                {
                    w=strToInt(cl.substr(2));
                }
            else if(cl=="oh" || cl=="t1" || cl=="t2" || cl=="t4" || cl == "ro"|| cl=="#" || cl=="oa")
                {
                    if(mode != 0)
                        {
                            a[mode-1] = tList;

                            if(mode == 5) ro = 1;

                            mode = 0;
                        }

                    if(cl == "oh") mode = 1;

                    if(cl == "t1") mode = 2;

                    if(cl == "t2") mode = 3;

                    if(cl == "t4") mode = 4;

                    if(cl == "ro") mode = 5;

                    if(cl == "oa") mode = 6;

                    tList = new list<string>();
                }
            else if(mode)
                {
                    compileCmds(cl,name, tList);
                }

            getline(f,cl);
        }

    if(!i)
        {
            cerr<<"IMAGELESS SPRITE NOT POSSIBLE IN FILE "<<file<<" FOR SPRITE "<<name<<", expect improper working";
            return nullptr;
        }

    cout<<"\nMade Sprite "<<name<<" with i: "<<img<<" and w: "<<w;
    Sprite *ret = new Sprite(name,img,siz,id);
    ret->setWall(w);
    ret->setOnHit(a[0]);
    ret->setOnAct(a[5]);
    ret->setT1(a[1]);
    ret->setT2(a[2]);
    ret->setT4(a[3]);
    
    Sprite *prevB = board[0][0];
    board[0][0] = ret;
    if(ro) l::parse(a[4],0,0);
    board[0][0] = prevB;

    return ret;
}

void SpriteFactory::compileCmds(string cl,string name, list<string> *tList)
{
    vector<string> toks = split(cl," ",false);

    if(cmd.find(toks[0])==cmd.end())
        {
            cerr<<"\nIn function of sprite "<<name<<", line "<<cl<<", command not found.";
        }
    else if(toks[0]=="print" && toks.size()!=1){
        tList->push_back(toks[0]);
        tList->push_back(cl.substr(5));
    }
    else if((unsigned int)(cmd[toks[0]].second+1)!=toks.size())
        {
            cerr<<"\nIn function of sprite "<<name<<", line "<<cl<<", command has incorrect number of parameters.";
        }
    else
        {
            for(auto iter : toks)
                {
                    tList->push_back(iter);
                }
        }
}
