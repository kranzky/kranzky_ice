//==============================================================================

#include <credits.hpp>
#include <engine.hpp>
#include <viewport.hpp>

#include <hgeresource.h>

//------------------------------------------------------------------------------
Credits::Credits()
    :
    Context(),
    m_interact( false )
{
}

//------------------------------------------------------------------------------
Credits::~Credits()
{
}

//------------------------------------------------------------------------------
// public:
//------------------------------------------------------------------------------
void
Credits::init()
{
    ViewPort * vp( Engine::vp() );

    vp->offset().x = 0.0f;
    vp->offset().y = 0.0f;
    vp->centre().x = 0.0f;
    vp->centre().y = 0.0f;
    vp->bounds().x = 80.0f;
    vp->bounds().y = 60.0f;
    vp->setAngle( 0.0f );

    m_interact = false;
}

//------------------------------------------------------------------------------
void
Credits::fini()
{
}

//------------------------------------------------------------------------------
bool
Credits::update( float dt )
{
    HGE * hge( Engine::hge() );
    ViewPort * vp( Engine::vp() );
    const Controller & pad( Engine::instance()->getController() );

    if ( vp->bounds().x < 800.0f && ! m_interact )
    {
        vp->bounds() *= ( 1.0f + dt * 2.0f );
        return false;
    }

    if ( hge->Input_GetKey() != 0 &&
         ! Engine::instance()->handledKey() )
    {
        Engine::instance()->switchContext( STATE_MENU );
        return false;
    }
    if ( pad.buttonDown( XPAD_BUTTON_A ) ||
         pad.buttonDown( XPAD_BUTTON_B ) ||
         pad.buttonDown( XPAD_BUTTON_START ) ||
         pad.buttonDown( XPAD_BUTTON_BACK ) )
    {
        Engine::instance()->switchContext( STATE_MENU );
        return false;
    }

    if ( pad.buttonDown( XPAD_BUTTON_BUTTON_Y ) )
    {
        m_interact = true;
    }

    if ( m_interact )
    {
        b2Vec2 offset( pad.getStick( XPAD_THUMBSTICK_RIGHT ) );
        float length( offset.Normalize() );
        if ( length > 0.9f )
        {
            b2Vec2 vertical( 0.0f, 1.0f );
            float angle( acosf( b2Dot( offset, vertical ) ) );
            if ( b2Cross( offset, vertical ) < 0.0f )
            {
                angle = -angle;
            }
            vp->setAngle( angle );
        }
        vp->centre().x += pad.getStick( XPAD_THUMBSTICK_LEFT ).x * dt * 50.0f;
        vp->centre().y += pad.getStick( XPAD_THUMBSTICK_LEFT ).y * dt * 50.0f;
        if ( vp->bounds().x > 80.0f )
        {
            vp->bounds() *= ( 1.0f - pad.getTrigger( XPAD_TRIGGER_LEFT ) * dt * 10.0f );
        }
        if ( vp->bounds().x < 1600.0f )
        {
            vp->bounds() *= 1.0f / ( 1.0f - pad.getTrigger( XPAD_TRIGGER_RIGHT ) * dt * 10.0f );
        }
    }

    return false;
}

//------------------------------------------------------------------------------
void
Credits::render()
{
    hgeResourceManager * rm( Engine::rm() );
    hgeSprite * sprite( 0 );
    ViewPort * vp( Engine::vp() );
    vp->setTransform();

    setColour( 0xFF555555 );

    hgeFont * font( rm->GetFont( "menu" ) );

    font->SetColor( 0xFF000000 );

    font->printf( 0.0, -250.0f, HGETEXT_CENTER,
                  "+++ B R O W N | C L O U D v1.2 +++" );
    font->printf( 0.0, -220.0f, HGETEXT_CENTER,
                  "By The Kranzky Bros." );
    font->printf( 0.0, -150.0f, HGETEXT_CENTER,
                  "Jason Hutchens" );
    font->printf( 0.0, -120.0f, HGETEXT_CENTER,
                  "Robert \"Dodgy\" Barnett" );
    font->printf( 0.0, -90.0f, HGETEXT_CENTER,
                  "Nigel Budden" );
    font->printf( 0.0, -60.0f, HGETEXT_CENTER,
                  "Simon Boxer" );
    font->printf( 0.0, -30.0f, HGETEXT_CENTER,
                  "Simon Wittber" );

    font->printf( 0.0, 40.0f, HGETEXT_CENTER,
                  "Made for Global Game Jam 2009" );

    font->printf( 0.0, 120.0f, HGETEXT_CENTER,
                  "PERTH SPONSORS" );

    rm->GetSprite( "murdoch" )->RenderEx( -300.0f, 220.0f, 0.0f, 0.5f );
    rm->GetSprite( "subversive" )->RenderEx( -100.0f, 220.0f, 0.0f, 0.5f );
    rm->GetSprite( "transmin" )->RenderEx( 100.0f, 220.0f, 0.0f, 0.5f );
    rm->GetSprite( "rv" )->RenderEx( 300.0f, 220.0f, 0.0f, 0.5f );
}

//==============================================================================
