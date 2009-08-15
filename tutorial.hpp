//==============================================================================

#ifndef ArseTute
#define ArseTute

#pragma once

#include <context.hpp>

//------------------------------------------------------------------------------
class Tutorial : public Context
{
  public:
    Tutorial();
    virtual ~Tutorial();

  private:
    Tutorial( const Tutorial & );
    Tutorial & operator=( const Tutorial & );

  public:
    virtual void init();
    virtual void fini();
    virtual bool update( float dt );
    virtual void render();

  private:
};

#endif

//==============================================================================
