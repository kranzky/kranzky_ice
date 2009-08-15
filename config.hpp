//==============================================================================

#ifndef ArseConfig
#define ArseConfig

#pragma once

#include <string>

class Config
{
  public:
    Config();
    virtual ~Config();

    void init();
    void fini();

    int screenWidth;
    int screenHeight;
    int bpp;
    bool fullScreen;
    bool sound;
    std::string userName;
    int menu;
    bool vibrate;
    bool leaderboard;
};

#endif

//==============================================================================
