//==============================================================================

#ifndef ArseMenu
#define ArseMenu

#pragma once

#include <hge.h>

#include <context.hpp>

class hgeFont;
class hgeGUI;

enum Control
{
    CTRL_NONE = 0,
    CTRL_HELP = 1,
    CTRL_START = 2,
    CTRL_SCORE = 3,
    CTRL_HOME = 4,
    CTRL_EXIT = 5,
};

//------------------------------------------------------------------------------
class Menu : public Context
{
  public:
    Menu();
    virtual ~Menu();

  private:
    Menu( const Menu & );
    Menu & operator=( const Menu & );

  public:
    virtual void init();
    virtual void fini();
    virtual bool update( float dt );
    virtual void render();

  private:
    hgeFont * m_font;
    hgeGUI * m_gui;
	HEFFECT m_navSnd;
	HEFFECT m_selectSnd;
};

#endif

//==============================================================================
