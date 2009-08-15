//==============================================================================

#include <game.hpp>
#include <engine.hpp>
#include <entity.hpp>
#include <entity_manager.hpp>
#include <viewport.hpp>
#include <score.hpp>

#include <hgeresource.h>

#include <algorithm>
#include <set>

namespace
{
    int PIECE[7][8] = {
        { 0,0,1,0,2,0,3,0 },    // I
        { 0,0,0,1,1,1,2,1 },    // J
        { 1,1,2,1,3,1,3,0 },    // L
        { 1,0,1,1,2,0,2,1 },    // O
        { 0,1,1,1,1,0,2,0 },    // S
        { 0,0,1,0,2,0,1,1 },    // T
        { 1,0,2,0,2,1,3,1 }     // Z
    };
}

//==============================================================================
Game::Game()
    :
    Context(),
    m_arena(),
    m_buffer(),
    m_piece(),
    m_index( 0 ),
    m_rotate( 0 ),
    m_col( 6 ),
    m_row( 1 ),
    m_replay(),
    m_num( 0 ),
    m_replaying( false ),
    m_timer( 0.0f )
{
}

//------------------------------------------------------------------------------
Game::~Game()
{
}

//------------------------------------------------------------------------------
// public:
//------------------------------------------------------------------------------
void
Game::init()
{
    HGE * hge( Engine::hge() );
    b2World * b2d( Engine::b2d() );
    hgeResourceManager * rm( Engine::rm() );
    ViewPort * vp( Engine::vp() );

    Engine::em()->init();

    vp->offset().x = 0.0f;
    vp->offset().y = 0.0f;
    vp->centre().x = 0.0f;
    vp->centre().y = 0.0f;
    vp->bounds().x = 1280.0f;
    vp->bounds().y = 720.0f;
    vp->setAngle( 0.0f );
    vp->setScale( 10.0f );
    
    clearArena();
    clearPiece();

    m_index = 0;
    m_rotate = 0;
    m_col = 6;
    m_row = 1;
    m_num = 0;
    m_replaying = false;
    m_timer = 0.0f;
}

//------------------------------------------------------------------------------
void
Game::fini()
{
    Engine::em()->fini();
}

//------------------------------------------------------------------------------
bool
Game::update( float dt )
{
    const Controller & pad( Engine::instance()->getController() );
    HGE * hge( Engine::hge() );
    ViewPort * vp( Engine::vp() );

    if ( Engine::instance()->isPaused() )
    {
        return false;
    }

    if ( m_replaying )
    {
        m_timer += dt;
        if ( m_timer < 0.2f )
        {
            return false;
        }
        if ( m_row == 1 )
        {
            if ( m_num == 0 )
            {
                m_replaying = false;
            }
            else
            {
                --m_num;
                m_index = m_replay[m_num][0];
                m_rotate = m_replay[m_num][1];
                m_col = m_replay[m_num][3];
                ++m_row;
            }
        }
        else
        {
            ++m_row;
        }
    }
    else if ( pad.isConnected() )
    {
        if ( pad.buttonDown( XPAD_BUTTON_BUTTON_Y ) )
        {
            m_replaying = true;
            m_row = 1;
            clearArena();
        }
        if ( pad.buttonDown( XPAD_BUTTON_X ) )
        {
            m_index = ( m_index + 6 ) % 7;
        }
        if ( pad.buttonDown( XPAD_BUTTON_B ) )
        {
            m_index = ( m_index + 1 ) % 7;
        }
        if ( pad.buttonDown( XPAD_BUTTON_LEFT_SHOULDER ) )
        {
            m_rotate = ( m_rotate + 3 ) % 4;
        }
        if ( pad.buttonDown( XPAD_BUTTON_RIGHT_SHOULDER ) )
        {
            m_rotate = ( m_rotate + 1 ) % 4;
        }
        if ( pad.buttonDown( XPAD_BUTTON_DPAD_UP ) )
        {
            m_row = ( m_row + 22 ) % 23;
        }
        if ( pad.buttonDown( XPAD_BUTTON_DPAD_DOWN ) )
        {
            m_row = ( m_row + 1 ) % 23;
        }
        if ( pad.buttonDown( XPAD_BUTTON_DPAD_LEFT ) )
        {
            m_col = ( m_col + 12 ) % 13;
        }
        if ( pad.buttonDown( XPAD_BUTTON_DPAD_RIGHT ) )
        {
            m_col = ( m_col + 1 ) % 13;
        }
    }
    else
    {
        if ( Engine::hge()->Input_KeyDown( HGEK_R ) )
        {
            m_replaying = true;
            m_row = 1;
            clearArena();
        }
        if ( Engine::hge()->Input_KeyDown( HGEK_A ) )
        {
            m_index = ( m_index + 6 ) % 7;
        }
        if ( Engine::hge()->Input_KeyDown( HGEK_D ) )
        {
            m_index = ( m_index + 1 ) % 7;
        }
        if ( Engine::hge()->Input_KeyDown( HGEK_W ) )
        {
            m_rotate = ( m_rotate + 3 ) % 4;
        }
        if ( Engine::hge()->Input_KeyDown( HGEK_S ) )
        {
            m_rotate = ( m_rotate + 1 ) % 4;
        }
        if ( Engine::hge()->Input_KeyDown( HGEK_UP ) )
        {
            m_row = ( m_row + 22 ) % 23;
        }
        if ( Engine::hge()->Input_KeyDown( HGEK_DOWN ) )
        {
            m_row = ( m_row + 1 ) % 23;
        }
        if ( Engine::hge()->Input_KeyDown( HGEK_LEFT ) )
        {
            m_col = ( m_col + 12 ) % 13;
        }
        if ( Engine::hge()->Input_KeyDown( HGEK_RIGHT ) )
        {
            m_col = ( m_col + 1 ) % 13;
        }
    }

    clearPiece();

    for ( int i = 0; i < 8; i += 2 )
    {
        int x( PIECE[m_index][i] ), y( PIECE[m_index][i+1] + 1 );
        for ( int j = 0; j < m_rotate; ++j )
        {
            b2Swap( x, y );
            y = 3 - y;
        }
        m_piece[x][y]= true;
    }

    for ( int x = 0; x < 10; ++x )
    {
        for ( int y = 0; y < 20; ++y )
        {
            m_buffer[x][y] = m_arena[x][y];
        }
    }

    if ( m_replaying )
    {
        if ( m_row == m_replay[m_num][2] )
        {
            m_row = 1;
            for ( int y = 0; y < 4; ++y )
            {
                for ( int x = 0; x < 4; ++x )
                {
                    if ( ! m_piece[x][y] )
                    {
                        continue;
                    }
                    m_arena[m_col + x - 3][m_row + y - 3] = true;
                }
            }
        }
        return false;
    }

    checkBorders();
   
    // If the piece has space beneath it, then fall down until we either hit the
    // bottom or there is something below us.
    while( ! checkBottom() && blankBelow() )
    {
        m_row += 1;
    }

    // Add rows, starting at the bottom, to accomodate the piece
    addRows();

    // If the user presses the button, check if the piece has nothing blocking
    // it above. If it doesn't, remove it.
    if ( pad.isConnected() )
    {
        if ( pad.buttonDown( XPAD_BUTTON_A ) )
        {
            removePiece();
        }
    }
    else
    {
        if ( Engine::hge()->Input_KeyDown( HGEK_SPACE ) )
        {
            removePiece();
        }
    }

    return false;
}

//------------------------------------------------------------------------------
void
Game::render()
{
    hgeResourceManager * rm( Engine::rm() );
    hgeFont* font = Engine::rm()->GetFont("menu");
    ViewPort * vp( Engine::vp() );
    
    vp->setTransform();

    float scale( 1.0f );
    std::vector< Entity * > entities;
    for ( b2Body * body( Engine::b2d()->GetBodyList() ); body != NULL;
          body = body->GetNext() )
    {
        Entity * entity( static_cast< Entity * >( body->GetUserData() ) );
        if ( entity )
        {
            entity->render( scale );
        }
    }

    Engine::vp()->reset();
    vp->setTransform();

    hgeSprite * black( rm->GetSprite( "empty" ) );
    hgeSprite * white( rm->GetSprite( "tile" ) );

    white->SetColor( 0xFFFFFFFF );
    for ( int y = 0; y < 20; ++y )
    {
        int count( 0 );
        for ( int x = 0; x < 10; ++x )
        {
            if ( ! m_buffer[x][y] )
            {
                break;
            }
            ++count;
        }
        if ( count == 10 )
        {
            white->SetColor( 0x8888FFFF );
        }
        else
        {
            white->SetColor( 0xFFFFFFFF );
        }
        for ( int x = 0; x < 10; ++x )
        {
            hgeSprite * sprite( m_buffer[x][y] ? white : black );
            sprite->RenderEx( ( x - 4.5f  ) * 3.2f,
                              ( y - 9.5f ) * 3.2f,
                              0.0f, 0.1f );
        }
    }
    white->SetColor( 0xFFFFFF00 );
    for ( int x = 0; x < 4; ++x )
    {
        for ( int y = 0; y < 4; ++y )
        {
            hgeSprite * sprite( m_piece[x][y] ? white : black );
            sprite->RenderEx( ( x - 1.5f + 12.0f ) * 3.2f,
                              ( y - 1.5f ) * 3.2f,
                              0.0f, 0.1f );
        }
    }
    white->SetColor( 0xCCFFFF00 );
    for ( int x = 0; x < 4; ++x )
    {
        for ( int y = 0; y < 4; ++y )
        {
            hgeSprite * sprite( m_piece[x][y] ? white : black );
            if ( ! m_piece[x][y] )
            {
                continue;
            }
            sprite->RenderEx( ( m_col + x - 7.5f ) * 3.2f,
                              ( m_row + y - 12.5f ) * 3.2f,
                              0.0f, 0.1f );
        }
    }
}

//------------------------------------------------------------------------------
//private:
//------------------------------------------------------------------------------
void
Game::clearArena()
{
    for ( int x = 0; x < 10; ++x )
    {
        for ( int y = 0; y < 20; ++y )
        {
            m_arena[x][y] = false;
            m_buffer[x][y] = false;
            /*
            if ( y > 15 )
            {
                m_arena[x][y] = ( hge->Random_Int( 0, 1 ) == 0 );
            }
            */
        }
    }
}

//------------------------------------------------------------------------------
void
Game::clearPiece()
{
    for ( int x = 0; x < 4; ++x )
    {
        for ( int y = 0; y < 4; ++y )
        {
            m_piece[x][y] = false;
        }
    }
}

//------------------------------------------------------------------------------
void
Game::checkBorders()
{
    checkLeft();
    checkRight();
    checkTop();
    checkBottom();
}

//------------------------------------------------------------------------------
bool
Game::checkLeft()
{
    if ( m_col >= 3 )
    {
        return false;
    }
    for ( int x = 0; x < 3 - m_col; ++x )
    {
        for ( int y = 0; y < 4; ++y )
        {
            if ( m_piece[x][y] )
            {
                m_col += ( 3 - m_col - x );
                return true;
            }
        }
    }
    return false;
}

//------------------------------------------------------------------------------
bool
Game::checkTop()
{
    if ( m_row >= 3 )
    {
        return false;
    }
    for ( int x = 0; x < 4; ++x )
    {
        for ( int y = 0; y < 3 - m_row; ++y )
        {
            if ( m_piece[x][y] )
            {
                m_row += ( 3 - m_row - y );
                return true;
            }
        }
    }
    return false;
}

//------------------------------------------------------------------------------
bool
Game::checkRight()
{
    if ( m_col <= 9 )
    {
        return false;
    }
    for ( int x = 3; x > 12 - m_col; --x )
    {
        for ( int y = 0; y < 4; ++y )
        {
            if ( m_piece[x][y] )
            {
                m_col -= ( x - 12 + m_col );
                return true;
            }
        }
    }
    return false;
}

//------------------------------------------------------------------------------
bool
Game::checkBottom()
{
    if ( m_row <= 19 )
    {
        return false;
    }
    for ( int x = 0; x < 4; ++x )
    {
        for ( int y = 3; y > 22 - m_row; --y )
        {
            if ( m_piece[x][y] )
            {
                m_row -= ( y - 22 + m_row );
                return true;
            }
        }
    }
    return false;
}

//------------------------------------------------------------------------------
bool
Game::blankBelow()
{
    int low( 0 );
    for ( int x = 0; x < 4; ++x )
    {
        for ( int y = 0; y < 4; ++y )
        {
            if ( m_piece[x][y] )
			{
                if ( y > low )
                {
                    low = y;
                }
            }
        }
    }

    for ( int x = 0; x < 10; ++x )
    {
        if ( m_buffer[x][m_row + low - 2] )
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
void
Game::addRows()
{
    for ( int y = 3; y >= 0; --y )
    {
        for ( int x = 0; x < 4; ++x )
        {
            if ( ! m_piece[x][y] || m_buffer[m_col + x - 3][m_row + y - 3] )
            {
                continue;
            }
            insertRow( m_row + y - 3 );
            break;
        }
    }
}

//------------------------------------------------------------------------------
void
Game::insertRow( int row )
{
    for ( int y = 1; y <= row; ++y )
    {
        for ( int x = 0; x < 10; ++x )
        {    
            m_buffer[x][y-1] = m_buffer[x][y];
        }
    }
    for ( int x = 0; x < 10; ++x )
    {
        m_buffer[x][row] = true;
    }
}

//------------------------------------------------------------------------------
void
Game::removePiece()
{
    if ( ! isClear() )
    {
        return;
    }
    m_replay[m_num][0] = m_index;
    m_replay[m_num][1] = m_rotate;
    m_replay[m_num][2] = m_row;
    m_replay[m_num][3] = m_col;
    m_num += 1;
    for ( int x = 0; x < 4; ++x )
    {
        for ( int y = 0; y < 4; ++y )
        {
            if ( ! m_piece[x][y] )
            {
                continue;
            }
            m_buffer[m_col + x - 3][m_row + y - 3] = false;
        }
    }
    for ( int y = 19; y > 0; --y )
    {
        int count( 0 );
        for ( int x = 0; x < 10; ++x )
        {
            if ( m_buffer[x][y] )
            {
                break;
            }
            ++count;
        }
        if ( count < 10 )
        {
            continue;
        }
        for ( int x = 0; x < 10; ++x )
        {
            for ( int i = y; i > 0; --i )
            {
                m_buffer[x][i] = m_buffer[x][i-1];
            }
        }
    }
    int count( 0 );
    for ( int x = 0; x < 10; ++x )
    {
        for ( int y = 0; y < 20; ++y )
        {
            m_arena[x][y] = m_buffer[x][y];
            if ( m_arena[x][y] )
            {
                count += 1;
            }
        }
    }
    if ( count == 0 )
    {
        m_replaying = true;
        m_row = 1;
        clearArena();
    }
}

//------------------------------------------------------------------------------
bool
Game::isClear()
{
    for ( int x = 0; x < 4; ++x )
    {
        for ( int y = 0; y < 4; ++y )
        {
            if ( ! m_piece[x][y] )
            {
                continue;
            }
            for ( int i = m_row + y - 4; i > 0; --i )
            {
                if ( m_buffer[m_col + x - 3][i] )
                {
                    return false;
                }
            }
            break;
        }
    }

    return true;
}

//==============================================================================
