//==============================================================================

#include <windows.h>

#include <controller.hpp>

#pragma warning( disable : 4351 )

//------------------------------------------------------------------------------
Controller::Controller()
    :
    m_supported( false ),
    m_pad(),
    m_buttons(),
    m_stick(),
    m_duration( 0.0f )
{
    OSVERSIONINFO version;
    ZeroMemory( & version, sizeof( OSVERSIONINFO ) );
    version.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
    GetVersionEx( & version );
    // Windows 2000 is v5.0, and XP is 5.1.
    m_supported = version.dwMajorVersion > 5 || version.dwMinorVersion > 0;
}

//------------------------------------------------------------------------------
Controller::~Controller()
{
}

//------------------------------------------------------------------------------
void
Controller::clear()
{
    if ( ! m_supported )
    {
        return;
    }
    XenonPad()->UpdateState();
    m_pad = XenonPad()->GetState();
    for ( int i = 0; i < XPAD_NUM_BUTTONS; ++i )
    {
        m_buttons[i] = m_pad.button[i].isPressed;
    }
}

//------------------------------------------------------------------------------
void
Controller::init()
{
    if ( ! m_supported )
    {
        return;
    }
    XenonPad()->SetController( XPAD_CONTROLLER1 );
}

//------------------------------------------------------------------------------
void
Controller::fini()
{
    if ( ! m_supported )
    {
        return;
    }
    rumble( 0.0f, 0.0f, 0.0f );
    XenonPad()->Release();
}

//------------------------------------------------------------------------------
void
Controller::update( float dt )
{
    if ( ! m_supported )
    {
        return;
    }
    for ( int i = 0; i < static_cast<int>( XPAD_NUM_BUTTONS ); ++i )
    {
        m_buttons[i] = m_pad.button[i].isPressed;
    }
    XenonPad()->UpdateState();
    m_pad = XenonPad()->GetState();
    if ( m_duration > 0.0f )
    {
        m_duration -= dt;
        if ( m_duration <= 0.0f )
        {
            rumble( 0.0f, 0.0f, 0.0f );
        }
    }
}

//------------------------------------------------------------------------------
bool
Controller::isConnected() const
{
    if ( ! m_supported )
    {
        return false;
    }
    return m_pad.bIsConnected;
}

//------------------------------------------------------------------------------
bool
Controller::buttonDown( XPad_GAMEPADLAYOUT button ) const
{
    if ( ! m_supported )
    {
        return false;
    }
    return ! m_buttons[button] && m_pad.button[button].isPressed;
}

//------------------------------------------------------------------------------
bool
Controller::buttonUp( XPad_GAMEPADLAYOUT button ) const
{
    if ( ! m_supported )
    {
        return false;
    }
    return m_buttons[button] && ! m_pad.button[button].isPressed;
}

//------------------------------------------------------------------------------
bool
Controller::getButtonState( XPad_GAMEPADLAYOUT button ) const
{
    if ( ! m_supported )
    {
        return false;
    }
    return m_pad.button[button].isPressed;
}

//------------------------------------------------------------------------------
const b2Vec2 &
Controller::getStick( XPad_ANALOGUESTICK num ) const
{
    if ( ! m_supported )
    {
        return m_stick[num];
    }
    m_stick[num].x = m_pad.Stick[num].xAxis;
    m_stick[num].y = m_pad.Stick[num].yAxis;
    return m_stick[num];
}

//------------------------------------------------------------------------------
float
Controller::getTrigger( XPad_ANALOGUETRIGGER num ) const
{
    if ( ! m_supported )
    {
        return 0.0f;
    }
    return m_pad.Trigger[num].Value;
}

//------------------------------------------------------------------------------
void
Controller::rumble( float leftSpeed, float rightSpeed, float duration ) const
{
    if ( ! m_supported )
    {
        return;
    }
    m_duration = duration;
    XenonPad()->SetForceFeedback( leftSpeed, rightSpeed );
}

//==============================================================================
