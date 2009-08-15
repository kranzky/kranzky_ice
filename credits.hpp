//==============================================================================

#ifndef ArseAbout
#define ArseAbout

#pragma once

#include <context.hpp>

//------------------------------------------------------------------------------
class Credits : public Context
{
  public:
    Credits();
    virtual ~Credits();

  private:
    Credits( const Credits & );
    Credits & operator=( const Credits & );

  public:
    virtual void init();
    virtual void fini();
    virtual bool update( float dt );
    virtual void render();

  private:
    bool m_interact;
};

#endif

//==============================================================================
