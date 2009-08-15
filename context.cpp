//==============================================================================

#include <context.hpp>
#include <entity.hpp>

//------------------------------------------------------------------------------
Context::Context()
    :
    m_handles_collisions( false ),
    m_colour( 0xFF000000 ),
    m_border( 0xFFFFFFFF )
{
}

//------------------------------------------------------------------------------
Context::~Context()
{
}

//------------------------------------------------------------------------------
void
Context::notifyOnCollision( bool notify )
{
    m_handles_collisions = notify;
}

//------------------------------------------------------------------------------
bool
Context::handlesCollisions()
{
    return m_handles_collisions;
}

//------------------------------------------------------------------------------
bool
Context::shouldCollide( Entity * left, Entity * right )
{
    return false;
}

//------------------------------------------------------------------------------
DWORD
Context::getColour()
{
    return m_colour;
}

//------------------------------------------------------------------------------
DWORD
Context::getBorder()
{
    return m_border;
}

//------------------------------------------------------------------------------
void
Context::setColour( DWORD colour )
{
    m_colour = colour;
}

//------------------------------------------------------------------------------
void
Context::setBorder( DWORD border )
{
    m_border = border;
}

//==============================================================================
