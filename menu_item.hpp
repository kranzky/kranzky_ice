//==============================================================================

#ifndef ArseMenuItem
#define ArseMenuItem

#pragma once

#include <hgegui.h>

class hgeFont;

//------------------------------------------------------------------------------
class MenuItem : public hgeGUIObject
{
  public:
    MenuItem( int id, float x, float y, const char * title, hgeFont * font, HEFFECT snd = 0 ); 

    virtual void    Render();
    virtual void    Update( float dt );
    virtual void    Enter();
    virtual void    Leave();
    virtual bool    IsDone();
    virtual void    Focus( bool focused );
    virtual void    MouseOver( bool over );
    virtual bool    MouseLButton( bool down );
    virtual bool    KeyClick( int key, int chr );

  private:
    hgeFont * m_font;
    const char * m_title;
    bool m_focused;
	HEFFECT m_snd;
};

#endif

//==============================================================================
