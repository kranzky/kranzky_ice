//==============================================================================

#ifndef ArseHurt
#define ArseHurt

#pragma once

#include <hge.h>

struct b2Vec2;

//------------------------------------------------------------------------------
class Damageable
{
  public:
    Damageable( float max_strength );
    virtual ~Damageable();

    void updateDamageable( float dt );
    void renderDamageable( const b2Vec2 & position, float scale, DWORD colour );
    void addStrength( float amount );
    void takeDamage( float amount );
    bool isDestroyed();
    bool isHealthy();

  protected:
    Damageable( const Damageable & );
    Damageable & operator=( const Damageable & );

  private:
    float m_max_strength;
    float m_strength;
    float m_damage;
    float m_health;
};

#endif

//==============================================================================
