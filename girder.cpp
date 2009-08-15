//==============================================================================

#include <girder.hpp>
#include <engine.hpp>
#include <entity_manager.hpp>

#include <hgesprite.h>
#include <Box2D.h>
#include <sqlite3.h>
#include <Database.h>
#include <Query.h>

//==============================================================================
Girder::Girder( float scale )
    :
    Entity( scale ),
    m_dimensions(),
    m_shield( false )
{
}

//------------------------------------------------------------------------------
Girder::~Girder()
{
}

//------------------------------------------------------------------------------
void
Girder::collide( Entity * entity, b2ContactPoint * point )
{
}

//------------------------------------------------------------------------------
void
Girder::persistToDatabase()
{
    char * rows[] = { "x", "%f", "y", "%f", "angle", "%f", "scale", "%f",
                      "sprite_id", "%d" };
    m_id = Engine::em()->persistToDatabase( this, rows, m_body->GetPosition().x,
                                                        m_body->GetPosition().y,
                                                        m_body->GetAngle(),
                                                        m_scale, m_sprite_id );
}

//------------------------------------------------------------------------------
void
Girder::setDimensions( const b2Vec2 & dimensions )
{
    m_dimensions = dimensions;
}

//------------------------------------------------------------------------------
void
Girder::setShield( bool shield )
{
    m_shield = shield;
}

//------------------------------------------------------------------------------
bool
Girder::getShield() const
{
    return m_shield;
}

//------------------------------------------------------------------------------
// static:
//------------------------------------------------------------------------------
void
Girder::registerEntity()
{
    Engine::em()->registerEntity( Girder::TYPE, Girder::factory, "girders",
                                  "id, x, y, angle, scale, sprite_id" );
}

//------------------------------------------------------------------------------
// protected:
//------------------------------------------------------------------------------
void
Girder::doInit()
{
    b2PolygonDef shapeDef;
    shapeDef.SetAsBox( m_dimensions.x * m_scale, m_dimensions.y * m_scale );
    b2BodyDef bodyDef;
    bodyDef.userData = static_cast< void * >( this );
    m_body = Engine::b2d()->CreateStaticBody( & bodyDef );
    m_body->CreateShape( & shapeDef );
}

//------------------------------------------------------------------------------
void
Girder::doUpdate( float dt )
{
}

//------------------------------------------------------------------------------
void
Girder::doRender( float scale )
{
}

//------------------------------------------------------------------------------
void
Girder::initFromQuery( Query & query )
{
    b2Vec2 position( 0.0f, 0.0f );
    float angle( 0.0f );

    m_id = static_cast< sqlite_int64 >( query.getnum() );
    position.x = static_cast< float >( query.getnum() );
    position.y = static_cast< float >( query.getnum() );
    angle = static_cast< float >( query.getnum() );
    m_scale = static_cast< float >( query.getnum() );

    setSpriteID( static_cast< sqlite_int64 >( query.getnum() ) );

    init();

    m_body->SetXForm( position, angle );
}

//==============================================================================

