//==============================================================================

#include <tutorial.hpp>
#include <engine.hpp>
#include <entity.hpp>
#include <entity_manager.hpp>
#include <viewport.hpp>

#include <hgeresource.h>

#include <algorithm>
#include <set>

//==============================================================================
Tutorial::Tutorial()
    :
    Context()
{
}

//------------------------------------------------------------------------------
Tutorial::~Tutorial()
{
}

//------------------------------------------------------------------------------
// public:
//------------------------------------------------------------------------------
void
Tutorial::init()
{
    HGE * hge( Engine::hge() );
    b2World * b2d( Engine::b2d() );
    hgeResourceManager * rm( Engine::rm() );
    ViewPort * vp( Engine::vp() );

    Engine::em()->init();

    vp->offset().x = 0.0f;
    vp->offset().y = 0.0f;
    vp->centre().x = 0.0f;
    vp->centre().y = 0.0f;
    vp->bounds().x = 1280.0f;
    vp->bounds().y = 720.0f;
    vp->setAngle( 0.0f );
    vp->setScale( 10.0f );
}

//------------------------------------------------------------------------------
void
Tutorial::fini()
{
    Engine::instance()->getConfig().menu = 2;
    Engine::em()->fini();
}

//------------------------------------------------------------------------------
bool
Tutorial::update( float dt )
{
    const Controller & pad( Engine::instance()->getController() );
    HGE * hge( Engine::hge() );
    ViewPort * vp( Engine::vp() );

    Engine::em()->update( dt );

    if ( Engine::instance()->isPaused() )
    {
        return false;
    }

    return false;
}

//------------------------------------------------------------------------------
void
Tutorial::render()
{
    hgeResourceManager * rm( Engine::rm() );
    ViewPort * vp( Engine::vp() );
    
    vp->setTransform();

    float scale( 1.0f );
    for ( b2Body * body( Engine::b2d()->GetBodyList() ); body != NULL;
          body = body->GetNext() )
    {
        Entity * entity( static_cast< Entity * >( body->GetUserData() ) );
        if ( entity )
        {
            entity->render( scale );
        }
    }

    Engine::vp()->reset();
    vp->setTransform();
}

//==============================================================================
