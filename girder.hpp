//==============================================================================

#ifndef ArseGirder
#define ArseGirder

#pragma once

#include <entity.hpp>

#include <Box2D.h>

//------------------------------------------------------------------------------
class Girder : public Entity
{
  public:
    static const unsigned int TYPE = 3;
    static Entity * factory() { return new Girder(); }

    Girder( float scale = 1.0f );
    virtual ~Girder();

    virtual void collide( Entity * entity, b2ContactPoint * point );
    void setShield( bool shield );
    bool getShield() const;

    virtual void persistToDatabase();

    void setDimensions( const b2Vec2 & dimensions );

    static void registerEntity();

  protected:
    Girder( const Girder & );
    Girder & operator=( const Girder & );

    virtual void doInit();
    virtual void doUpdate( float dt );
    virtual void doRender( float scale );
    virtual void initFromQuery( Query & query );

  private:
    b2Vec2 m_dimensions;
    bool m_shield;
};

#endif

//==============================================================================
