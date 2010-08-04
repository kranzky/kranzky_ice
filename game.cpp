//==============================================================================

#include <game.hpp>
#include <engine.hpp>
#include <entity.hpp>
#include <entity_manager.hpp>
#include <viewport.hpp>
#include <score.hpp>
#include <droplet.hpp>
#include <girder.hpp>

#include <hgeresource.h>

#include <algorithm>
#include <set>

namespace
{
    static const float FLOW( 0.01f );
    static const int MAX_COUNT( 250 );
}

//==============================================================================
Game::Game()
    :
    Context(),
    m_flow( FLOW ),
    m_count( 0 )
{
}

//------------------------------------------------------------------------------
Game::~Game()
{
}

//------------------------------------------------------------------------------
// public:
//------------------------------------------------------------------------------
void
Game::init()
{
    HGE * hge( Engine::hge() );
    b2World * b2d( Engine::b2d() );
    hgeResourceManager * rm( Engine::rm() );
    ViewPort * vp( Engine::vp() );

    Droplet::registerEntity();
    Girder::registerEntity();

    Engine::em()->init();

    vp->offset().x = 0.0f;
    vp->offset().y = 0.0f;
    vp->centre().x = 0.0f;
    vp->centre().y = 0.0f;
    vp->bounds().x = 1280.0f;
    vp->bounds().y = 720.0f;
    vp->setAngle( 0.0f );
    vp->setScale( 100.0f );

    _initArena();

    Engine::instance()->setMouse("cursor");
    Engine::instance()->showMouse();

    m_flow = FLOW;
    m_count = 0;
}

//------------------------------------------------------------------------------
void
Game::fini()
{
    Engine::instance()->hideMouse();
    Engine::em()->fini();
}

//------------------------------------------------------------------------------
bool
Game::update( float dt )
{
    const Controller & pad( Engine::instance()->getController() );
    HGE * hge( Engine::hge() );
    ViewPort * vp( Engine::vp() );

    if ( Engine::instance()->isPaused() || m_count >= MAX_COUNT )
    {
        return false;
    }

    // TODO: if mouse down, spawn droplet at certain rate
    m_flow -= dt;
    while ( m_flow  < 0.0f && m_count < MAX_COUNT )
    {
        m_flow += FLOW;
        m_count += 1;
        Droplet * droplet( static_cast< Droplet * >( Engine::em()->factory( Droplet::TYPE ) ) );
        droplet->setScale( 0.0015f );
        droplet->init();
        b2Vec2 position( Engine::hge()->Random_Float( -0.01f, 0.01f ) + 1.0f, 1.0f );
        droplet->getBody()->SetXForm( position, 0.0f );
    }

    return false;
}

//------------------------------------------------------------------------------
void
Game::render()
{
    hgeResourceManager * rm( Engine::rm() );
    hgeFont* font = Engine::rm()->GetFont("menu");
    ViewPort * vp( Engine::vp() );
    
    vp->setTransform();

    float scale( 1.0f );
    std::vector< Entity * > entities;
    for ( b2Body * body( Engine::b2d()->GetBodyList() ); body != NULL;
          body = body->GetNext() )
    {
        Entity * entity( static_cast< Entity * >( body->GetUserData() ) );
        if ( entity )
        {
            entity->render( scale );
        }
    }

    vp->reset();

    char message[8];
    sprintf_s( message, "%03d", m_count );
    font->printf( 100.0f, 10.0f, HGETEXT_CENTER, message );
}

//------------------------------------------------------------------------------
// private:
//------------------------------------------------------------------------------
void
Game::_initArena()
{
    b2Vec2 position( 0.0f, 0.0f );
    b2Vec2 dimensions( 0.0f, 0.0f );
    Entity * entity( 0 );

    for ( int i = 0; i < 4; ++i )
    {
        switch( i )
        {
            case 0:
            {
                dimensions.x = 128.0f;
                dimensions.y = 0.1f;
                position.x = 0.0f;
                position.y = -36.1f;
				continue;
                break;
            }
            case 1:
            {
                dimensions.x = 0.1f;
                dimensions.y = 72.0f;
                position.x = 64.1f;
                position.y = 0.0f;
                break;
            }
            case 2:
            {
                dimensions.x = 128.0f;
                dimensions.y = 0.1f;
                position.x = 0.0f;
                position.y = 36.1f;
                break;
            }
            case 3:
            {
                dimensions.x = 0.1f;
                dimensions.y = 72.0f;
                position.x = -64.1f;
                position.y = 0.0f;
                break;
            }
        }
        Girder * girder( static_cast< Girder * >(
            Engine::em()->factory( Girder::TYPE ) ) );
        girder->setScale( 1.0f );
        girder->setDimensions( dimensions );
        girder->init();
        girder->getBody()->SetXForm( 0.1f * position, 0.0f );
    }
}

//------------------------------------------------------------------------------
