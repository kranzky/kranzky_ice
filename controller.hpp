//==============================================================================

#ifndef ArseController
#define ArseController

#pragma once

#include <rpc.h>

#include <XenonPad.h>
#include <Box2D.h>

class Controller
{
  public:
    Controller();
    virtual ~Controller();

  private:
    Controller( const Controller & );
    Controller & operator=( const Controller & );

  public:
    void clear();
    void init();
    void fini();
    void update( float dt );
    bool isConnected() const;
    bool buttonDown( XPad_GAMEPADLAYOUT button ) const;
    bool buttonUp( XPad_GAMEPADLAYOUT button ) const;
    bool getButtonState( XPad_GAMEPADLAYOUT button ) const;
    const b2Vec2 & getStick( XPad_ANALOGUESTICK num ) const;
    float getTrigger( XPad_ANALOGUETRIGGER num ) const;
    void rumble( float leftSpeed, float rightSpeed,
                 float duration = 0.1f ) const;

  private:
    bool m_supported;
    XPad_ControllerState m_pad;
    bool m_buttons[XPAD_NUM_BUTTONS];
    mutable b2Vec2 m_stick[2];
    mutable float m_duration;
};

#endif

//==============================================================================
