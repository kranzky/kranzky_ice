//==============================================================================

#include <engine.hpp>
#include <damageable.hpp>

#include <hgefont.h>
#include <hgeresource.h>
#include <Box2D.h>

//------------------------------------------------------------------------------
Damageable::Damageable( float strength )
    :
    m_max_strength( strength ),
    m_strength( 0.5f * strength + 0.5f ),
    m_damage( 0.0f ),
    m_health( 0.0f )
{
}

//------------------------------------------------------------------------------
Damageable::~Damageable()
{
}

//------------------------------------------------------------------------------
void
Damageable::updateDamageable( float dt )
{
    if ( m_damage > m_max_strength )
    {
        m_strength = -1.0f;
    }
    if ( m_damage > 0.0f )
    {
        float dd( 100.0f * dt );
        if ( dd > m_damage )
        {
            dd = m_damage;
        }
        m_damage -= dd;
        m_strength -= dd;
        if ( m_damage < 0.0f )
        {
            m_damage = 0.0f;
        }
    }

    if ( m_health > 0.0f )
    {
        float dd( 100.0f * dt );
        if ( dd > m_health )
        {
            dd = m_health;
        }
        m_health -= dd;
        m_strength += dd;
        if ( m_health < 0.0f )
        {
            m_health = 0.0f;
        }
    }

    if ( m_strength > m_max_strength )
    {
        m_strength = m_max_strength;
        m_damage = 0.0f;
        m_health = 0.0f;
    }

    if ( m_strength < 0.0f )
    {
        m_strength = 0.0f;
        m_damage = 0.0f;
        m_health = 0.0f;
    }
}

//------------------------------------------------------------------------------
void
Damageable::renderDamageable( const b2Vec2 & position, float scale,
                              DWORD colour )
{
    hgeFont* font = Engine::rm()->GetFont("tiny");
    font->SetScale( scale );
    font->SetColor( colour );
    font->printf( position.x, position.y, HGETEXT_CENTER, "%02d",
                  static_cast<int>(m_strength) );
    font->SetScale( 1.0f );
}

//------------------------------------------------------------------------------
void
Damageable::addStrength( float amount )
{
    m_health += amount;
}

//------------------------------------------------------------------------------
void
Damageable::takeDamage( float amount )
{
    m_damage += amount;
}

//------------------------------------------------------------------------------
bool
Damageable::isDestroyed()
{
    return m_strength <= 0.0f;
}

//------------------------------------------------------------------------------
bool
Damageable::isHealthy()
{
    return m_strength >= m_max_strength;
}

//==============================================================================
