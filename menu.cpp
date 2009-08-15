//==============================================================================

#include <menu.hpp>
#include <menu_item.hpp>
#include <engine.hpp>
#include <viewport.hpp>
#include <config.hpp>

#include <hgeresource.h>
#include <hgefont.h>

//------------------------------------------------------------------------------
Menu::Menu()
    :
    Context(),
    m_font( 0 ),
    m_gui( 0 )
{
}

//------------------------------------------------------------------------------
Menu::~Menu()
{
}

//------------------------------------------------------------------------------
// public:
//------------------------------------------------------------------------------
void
Menu::init()
{
	

    hgeResourceManager * rm( Engine::rm() );
    ViewPort * vp( Engine::vp() );
    Config & config( Engine::instance()->getConfig() );

    char * label = config.leaderboard ? "Leaderboard" : "High Scores";

    m_font = rm->GetFont( "menu" );
    m_gui = new hgeGUI();
    float cx( 0.5f * vp->screen().x );
    float cy( 0.5f * vp->screen().y );
    float dy( 0.15f * cy );
    float ty( cy - 1.5f * dy );

	m_navSnd = rm->GetEffect( "menu_nav" );
	m_selectSnd = rm->GetEffect( "menu_select" );

    m_gui->AddCtrl( new MenuItem( CTRL_HELP, cx, ty, "Tutorial", m_font, m_navSnd ) );
    ty += dy;
    m_gui->AddCtrl( new MenuItem( CTRL_START, cx, ty, "Start", m_font, m_navSnd ) );
    ty += dy;
    m_gui->AddCtrl( new MenuItem( CTRL_SCORE, cx, ty, label, m_font, m_navSnd ) );
    ty += dy;
    m_gui->AddCtrl( new MenuItem( CTRL_HOME, cx, ty, "Home Page", m_font, m_navSnd ) );
    ty += dy;
    m_gui->AddCtrl( new MenuItem( CTRL_EXIT, cx, ty, "Exit", m_font, m_navSnd ) );

	
    m_gui->SetNavMode( HGEGUI_UPDOWN );
    m_gui->SetFocus( Engine::instance()->getConfig().menu );
    m_gui->Enter();
}

//------------------------------------------------------------------------------
void
Menu::fini()
{
    m_gui->DelCtrl( CTRL_HELP );
    m_gui->DelCtrl( CTRL_START );
    m_gui->DelCtrl( CTRL_SCORE );
    m_gui->DelCtrl( CTRL_HOME );
    m_gui->DelCtrl( CTRL_EXIT );
    delete m_gui;
    m_gui = 0;
    m_font = 0;
}

//------------------------------------------------------------------------------
bool
Menu::update( float dt )
{
    HGE * hge( Engine::hge() );
    Engine * engine( Engine::instance() );

    if ( hge->Input_KeyDown( HGEK_ESCAPE ) )
    {
        return true;
    }

    switch ( static_cast< Control >( engine->updateGUI( dt, m_gui,
                                     engine->getConfig().menu, 5 ) ) )
    {
        case CTRL_HELP:
        {
			hge->Effect_Play(m_selectSnd);
            Engine::instance()->switchContext( STATE_HELP );
            break;
        }
        case CTRL_START:
        {
			hge->Effect_Play(m_selectSnd);
            Engine::instance()->switchContext( STATE_GAME );
            break;
        }
        case CTRL_SCORE:
        {
			hge->Effect_Play(m_selectSnd);
            Engine::instance()->switchContext( STATE_SCORE );
            break;
        }
        case CTRL_HOME:
        {
			hge->Effect_Play(m_selectSnd);
            hge->System_Launch( "http://rockethands.com/ProjectIce" );
        }
        case CTRL_EXIT:
        {
            return true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
void
Menu::render()
{
    hgeResourceManager * rm( Engine::rm() );
    ViewPort * vp( Engine::vp() );

    m_gui->Render();
    float cx( 0.5f * vp->screen().x );
    rm->GetSprite( "title" )->Render( cx, 60.0f );

    hgeFont * font( rm->GetFont( "menu" ) );
    font->SetColor( 0xFFFFFFFF );
    font->printf( cx, 110.0f, HGETEXT_CENTER, "A RocketHands Experiment by Lloyd Kranzky" );
    font->printf( cx, vp->screen().y - 70.0f, HGETEXT_CENTER, "Copyright (c) 2009 RocketHands Pty. Ltd.  All rights reserved." );
    font->printf( cx, vp->screen().y - 40.0f, HGETEXT_CENTER, "http://rockethands.com/ProjectIce" );
}

//==============================================================================
