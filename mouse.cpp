//==============================================================================

#include <engine.hpp>
#include <mouse.hpp>
#include <viewport.hpp>
//------------------------------------------------------------------------------
Mouse::MouseButton::MouseButton()
    :
    m_state( MOUSE_START ),
    m_action( ACTION_NONE ),
    m_timer(),
    m_last( 0.0f, 0.0f ),
    m_delta( 0.0f, 0.0f ),
    m_total( 0.0f, 0.0f )
{
}

//------------------------------------------------------------------------------
Mouse::MouseButton::~MouseButton()
{
}

//------------------------------------------------------------------------------
void
Mouse::MouseButton::clear()
{
    m_state = MOUSE_START;
    m_action = ACTION_NONE;
    m_timer = 0.0f;
    Engine::hge()->Input_GetMousePos( & m_last.x, & m_last.y );
    m_delta.SetZero();
    m_total.SetZero();
}

//------------------------------------------------------------------------------
void
Mouse::MouseButton::update( float dt, bool state )
{
    m_timer += dt;

    b2Vec2 position( 0.0f, 0.0f );
    Engine::hge()->Input_GetMousePos( & position.x, & position.y );
    m_delta = position - m_last;
    m_total += m_delta;
    m_last = position;

    float offset( m_total.LengthSquared() );

    switch ( m_state )
    {
        case MOUSE_START:
        {
            if ( state )
            {
                m_timer = 0.0f;
                m_state = MOUSE_FIRST_DOWN;
                m_total.SetZero();
            }
            {
                m_action = ACTION_NONE;
            }
            break;
        }
        case MOUSE_FIRST_DOWN:
        {
            if ( ! state )
            {
                m_timer = 0.0f;
                m_state = MOUSE_FIRST_UP;
                m_total.SetZero();
            }
            else if ( m_timer > 0.2f || offset > 9.0f )
            {
                m_action = ACTION_DRAGGING;
                m_state = MOUSE_END;
            }
            break;
        }
        case MOUSE_FIRST_UP:
        {
            if ( state )
            {
                m_timer = 0.0f;
                m_state = MOUSE_SECOND_DOWN;
                m_total.SetZero();
            }
            else if ( m_timer > 0.1f || offset > 1.0f )
            {
                m_action = ACTION_CLICKED;
                m_state = MOUSE_START;
            }
            break;
        }
        case MOUSE_SECOND_DOWN:
        {
            if ( ! state )
            {
                m_timer = 0.0f;
                m_state = MOUSE_SECOND_UP;
                m_total.SetZero();
            }
            if ( m_timer > 0.2f || offset > 9.0f )
            {
                m_action = ACTION_DRAGGING;
                m_state = MOUSE_END;
            }
            break;
        }
        case MOUSE_SECOND_UP:
        {
            m_action = ACTION_DOUBLED;
            m_state = MOUSE_START;
            break;
        }
        case MOUSE_END:
        {
            if ( ! state )
            {
                m_action = ACTION_NONE;
                m_state = MOUSE_START;
            }
            break;
        }
    }
}

//------------------------------------------------------------------------------
bool
Mouse::MouseButton::dragging() const
{
    return m_action == ACTION_DRAGGING;
}

//------------------------------------------------------------------------------
bool
Mouse::MouseButton::clicked() const
{
    return m_action == ACTION_CLICKED;
}

//------------------------------------------------------------------------------
bool
Mouse::MouseButton::doubleClicked() const
{
    return m_action == ACTION_DOUBLED;
}

//------------------------------------------------------------------------------
const b2Vec2 &
Mouse::MouseButton::getDelta() const
{
    return m_delta;
}

//==============================================================================
Mouse::Mouse()
    :
    m_left(),
    m_right()
{
}

//------------------------------------------------------------------------------
Mouse::~Mouse()
{
}

//------------------------------------------------------------------------------
void
Mouse::clear()
{
    m_left.clear();
    m_right.clear();
}

//------------------------------------------------------------------------------
void
Mouse::update( float dt )
{
    m_left.update( dt, Engine::hge()->Input_GetKeyState( HGEK_LBUTTON ) ); 
    m_right.update( dt, Engine::hge()->Input_GetKeyState( HGEK_RBUTTON ) ); 
}

//------------------------------------------------------------------------------
const Mouse::MouseButton &
Mouse::getLeft()const
{
    return m_left;
}

//------------------------------------------------------------------------------
const Mouse::MouseButton &
Mouse::getRight()const
{
    return m_right;
}
//------------------------------------------------------------------------------
b2Vec2
Mouse::getMousePos() const
{
	b2Vec2 position;
	Engine::hge()->Input_GetMousePos(&position.x,&position.y);
	Engine::vp()->screenToWorld(position);
	return position;
}

//==============================================================================
