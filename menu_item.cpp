//==============================================================================

#include <menu_item.hpp>
#include <Engine.hpp>
#include <hgeresource.h>
#include <hgefont.h>

//------------------------------------------------------------------------------
MenuItem::MenuItem( int control, float x, float y, const char * title,
                    hgeFont * font, HEFFECT snd )
    :
    hgeGUIObject(),
    m_title( title ),
    m_font( font ),
    m_focused( false ),
	m_snd( snd )
{
    id = control;

    bStatic = false;
    bVisible = true;
    bEnabled = true;

    float width( m_font->GetStringWidth( title ) );
    rect.Set( x - width / 2, y, x + width / 2, y + m_font->GetHeight() );
}
 
//------------------------------------------------------------------------------
void
MenuItem::Render()
{
    if ( m_focused )
    {
        m_font->SetColor( 0xFFFFFFFF );
    }
    else
    {
        m_font->SetColor( 0x55888888 );
    }
    m_font->Render( rect.x1, rect.y1, HGETEXT_LEFT, m_title );
}

//------------------------------------------------------------------------------
void
MenuItem::Update( float dt )
{
}

//------------------------------------------------------------------------------
void
MenuItem::Enter()
{
}

//------------------------------------------------------------------------------
void
MenuItem::Leave()
{
}

//------------------------------------------------------------------------------
bool
MenuItem::IsDone()
{
    return true;
}

//------------------------------------------------------------------------------
#include <iostream>
void
MenuItem::Focus( bool focused )
{
    m_focused = focused;

	std::cout<<"Menu Snd"<<m_snd;
	if(m_focused && m_snd)
	{
		Engine::hge()->Effect_Play(m_snd);
	}
}

//------------------------------------------------------------------------------
void
MenuItem::MouseOver( bool over )
{
}

//------------------------------------------------------------------------------
bool
MenuItem::MouseLButton( bool down )
{
    return false;
}

//------------------------------------------------------------------------------
bool
MenuItem::KeyClick( int key, int chr )
{
    return ( key == HGEK_ENTER || key == HGEK_SPACE );
}

//==============================================================================
