//==============================================================================

#include <engine.hpp>
#include <viewport.hpp>

//------------------------------------------------------------------------------
ViewPort::ViewPort()
    :
    m_offset(),
    m_centre(),
    m_bounds(),
    m_screen(),
    m_hscale( 0.0f ),
    m_vscale( 0.0f ),
    m_angle( 0.0f ),
    m_dx( 0.0f ),
    m_dy( 0.0f )
{
}

//------------------------------------------------------------------------------
ViewPort::~ViewPort()
{
}

//------------------------------------------------------------------------------
float
ViewPort::angle()
{
    return m_angle;
}

//------------------------------------------------------------------------------
b2Vec2 &
ViewPort::offset()
{
    return m_offset;
}

//------------------------------------------------------------------------------
b2Vec2 &
ViewPort::centre()
{
    return m_centre;
}

//------------------------------------------------------------------------------
b2Vec2 &
ViewPort::bounds()
{
    return m_bounds;
}

//------------------------------------------------------------------------------
b2Vec2 &
ViewPort::screen()
{
    return m_screen;
}

//------------------------------------------------------------------------------
void
ViewPort::screenToWorld( b2Vec2 & point )
{
    _updateRatios();
	float tempOffsetX =0;
	float tempOffsetY = 0;
	
    float dx( 0.5f * m_screen.x - m_centre.x * m_hscale );
    float dy( 0.5f * m_screen.y - m_centre.y * m_vscale );
    float maxx( 0.5f * m_screen.x * m_hscale );
    float minx( m_screen.x - 0.5f * m_screen.x * m_hscale );
    float maxy( 0.5f * m_screen.y * m_vscale );
    float miny( m_screen.y - 0.5f * m_screen.y * m_vscale );

    if ( dx > maxx )
    {
        dx = maxx;
    }
    if ( dx < minx )
    {
        dx = minx;
    }
    if ( dy > maxy )
    {
        dy = maxy;
    }
    if ( dy < miny )
    {
        dy = miny;
    }

    dx = 0.5f * m_screen.x;
    dy = 0.5f * m_screen.y;

    point.x -= dx;
    point.y -= dy;
    point.x /= m_hscale;
    point.y /= m_vscale;
}

//------------------------------------------------------------------------------
float
ViewPort::hscale() const
{
    _updateRatios();
    return m_hscale;
}

//------------------------------------------------------------------------------
float
ViewPort::vscale() const
{
    _updateRatios();
    return m_vscale;
}

//------------------------------------------------------------------------------
void
ViewPort::setScale( float scale )
{
    m_bounds = ( 1.0f / scale ) * m_bounds;
    _updateRatios();
}

//------------------------------------------------------------------------------
void
ViewPort::setAngle( float angle )
{
    m_angle = angle;
}

//------------------------------------------------------------------------------
void
ViewPort::setTransform( float scale )
{
    _updateRatios();
    float dx( 0.5f * m_screen.x - m_centre.x * m_hscale );
    float dy( 0.5f * m_screen.y - m_centre.y * m_vscale );
    float maxx( 0.5f * m_screen.x * m_hscale );
    float minx( m_screen.x - 0.5f * m_screen.x * m_hscale );
    float maxy( 0.5f * m_screen.y * m_vscale );
    float miny( m_screen.y - 0.5f * m_screen.y * m_vscale );
    if ( dx > maxx )
    {
        dx = maxx;
    }
    if ( dx < minx )
    {
        dx = minx;
    }
    if ( dy > maxy )
    {
        dy = maxy;
    }
    if ( dy < miny )
    {
        dy = miny;
    }
    dx = 0.5f * m_screen.x + m_dx;
    dy = 0.5f * m_screen.y + m_dy;
    Engine::hge()->Gfx_SetTransform( m_offset.x, m_offset.y, dx, dy,
                                     m_angle,
                                     scale * m_hscale, scale * m_vscale );
}

//------------------------------------------------------------------------------
void
ViewPort::reset()
{
    Engine::hge()->Gfx_SetTransform( 0.0f, 0.0f, m_dx, m_dy, 0.0f, 1.0f, 1.0f );
}

//------------------------------------------------------------------------------
bool
ViewPort::restore()
{
    int width( 0 );
    int height( 0 );
    if ( Engine::hge()->System_GetState( HGE_WINDOWED ) )
    {
        width = Engine::hge()->System_GetState( HGE_SCREENWIDTH );
        height = Engine::hge()->System_GetState( HGE_SCREENHEIGHT );
    }
    else
    {
        width = GetSystemMetrics( SM_CXSCREEN );
        height = GetSystemMetrics( SM_CYSCREEN );
    }
    float ratio( static_cast<float>(width)/static_cast<float>(height) );
    if ( ratio > 16.0f / 9.0f )
    {
        m_screen.y = static_cast<float>( height );
        m_screen.x = ( 16.0f * m_screen.y ) / 9.0f;
        m_dx = 0.5f * ( static_cast<float>( width ) - m_screen.x );
        m_dy = 0.0f;
    }
    else
    {
        m_screen.x = static_cast<float>( width );
        m_screen.y = ( 9.0f * m_screen.x ) / 16.0f;
        m_dx = 0.0f;
        m_dy = 0.5f * ( static_cast<float>( height ) - m_screen.y );
    }
	return true;
}

//------------------------------------------------------------------------------
// private:
//------------------------------------------------------------------------------
void
ViewPort::_updateRatios() const
{
    m_hscale = m_screen.x / m_bounds.x;
    m_vscale = m_screen.y / m_bounds.y;
}

//==============================================================================
