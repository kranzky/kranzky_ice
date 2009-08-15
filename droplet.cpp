//==============================================================================

#include <droplet.hpp>
#include <engine.hpp>
#include <entity_manager.hpp>

#include <hgeSprite.h>
#include <hgeresource.h>
#include <Box2D.h>

//------------------------------------------------------------------------------
Droplet::Droplet( float scale, float temp )
    :
    Entity( scale ),
    m_temp( temp )
{
}

//------------------------------------------------------------------------------
Droplet::~Droplet()
{
}

//------------------------------------------------------------------------------
void
Droplet::collide( Entity * entity, b2ContactPoint * point )
{
}

//------------------------------------------------------------------------------
void
Droplet::persistToDatabase()
{
}

//------------------------------------------------------------------------------
void
Droplet::registerEntity()
{
    Engine::em()->registerEntity( Droplet::TYPE, Droplet::factory, "droplets",
                                  "x, y, angle, scale, sprite_id, temp" );
}

//------------------------------------------------------------------------------
// protected:
//------------------------------------------------------------------------------
void
Droplet::onSetScale()
{
    if ( m_body == 0 )
    {
        return;
    }
    b2Shape * shape;
    while ( shape = m_body->GetShapeList() )
    {
        m_body->DestroyShape( shape );
    }
    b2CircleDef shapeDef;
    shapeDef.radius = 0.5f * 0.5f * m_sprite->GetWidth() * m_scale;
    shapeDef.density = 1.0f;
    shapeDef.friction =0.0f;
    shapeDef.restitution = 0.3f;
    m_body->CreateShape( & shapeDef );
    m_body->SetMassFromShapes();
}

//------------------------------------------------------------------------------
void
Droplet::doInit()
{
    m_sprite = Engine::rm()->GetSprite( "droplet" );
    b2BodyDef bodyDef;
    bodyDef.allowSleep = true;
    bodyDef.userData = static_cast< void * >( this );
    m_body = Engine::b2d()->CreateDynamicBody( & bodyDef );
    m_body->m_linearDamping = 0.8f;
    onSetScale();
}

//------------------------------------------------------------------------------
void
Droplet::doUpdate( float dt )
{
}

//------------------------------------------------------------------------------
void
Droplet::doRender( float scale )
{
    b2Vec2 position( m_body->GetPosition() );
    float angle( m_body->GetAngle() );
    m_sprite->RenderEx( position.x, position.y, angle, m_scale );
}

//------------------------------------------------------------------------------
void
Droplet::initFromQuery( Query & query )
{
}

//==============================================================================
