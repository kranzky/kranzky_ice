//==============================================================================

#ifndef ArseDrop
#define ArseDrop

#pragma once

#include <entity.hpp>

//------------------------------------------------------------------------------
class Droplet : public Entity
{
  public:
    static const unsigned int TYPE = 1;
    static Entity * factory() { return new Droplet(); }

    Droplet( float scale = 1.0f, float temp = 26.0f );
    virtual ~Droplet();

    virtual void collide( Entity * entity, b2ContactPoint * point );

    virtual void persistToDatabase();

    static void registerEntity();

  protected:
    Droplet( const Droplet & );
    Droplet & operator=( const Droplet & );

  protected:
    friend class EntityManager;

    virtual void onSetScale();
    virtual void doInit();
    virtual void doUpdate( float dt );
    virtual void doRender( float scale );
    virtual void initFromQuery( Query & query );

  private:
    float m_temp;
};

#endif

//==============================================================================
