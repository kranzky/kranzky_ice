//==============================================================================

#ifndef ArseSplash
#define ArseSplash

#pragma once

#include <context.hpp>

//------------------------------------------------------------------------------
class Splash : public Context
{
  public:
    Splash();
    virtual ~Splash();

  private:
    Splash( const Splash & );
    Splash & operator=( const Splash & );

  public:
    virtual void init();
    virtual void fini();
    virtual bool update( float dt );
    virtual void render();

  private:
    float m_timer;
    float m_mute;
};

#endif

//==============================================================================
