//==============================================================================

#include <engine.hpp>
#include <entity_manager.hpp>
#include <splash.hpp>
#include <credits.hpp>
#include <menu.hpp>
#include <menu_item.hpp>
#include <tutorial.hpp>
#include <game.hpp>
#include <score.hpp>
#include <debug.hpp>
#include <entity.hpp>
#include <viewport.hpp>

#include <hgesprite.h>
#include <hgeresource.h>

#include "resource.h"

//------------------------------------------------------------------------------

Engine * Engine::s_instance( 0 );

//------------------------------------------------------------------------------
Engine::Engine()
    :
    m_rm( 0 ),
    m_pm( 0 ),
    m_hge( 0 ),
    m_b2d( 0 ),
    m_vp( 0 ),
    m_em( 0 ),
    m_dd( 0 ),
    m_overlay( 0 ),
    m_contexts(),
    m_state( STATE_NONE ),
    m_mouse(),
    m_controller(),
    m_config(),
    m_handled_key( false ),
    m_paused( false ),
    m_running( false ),
    m_show_mouse( false ),
    m_mouse_sprite( 0 ),
    m_time_ratio( 1.0f ),
    m_gui( 0 ),
    m_stick( false )
{
    m_vp = new ViewPort();
    m_em = new EntityManager();
}

//------------------------------------------------------------------------------
Engine::~Engine()
{
    m_config.fini();
    m_controller.fini();

    m_gui->DelCtrl( EC_CONTINUE );
    m_gui->DelCtrl( EC_QUIT );
    delete m_gui;

    std::vector< Context * >::iterator i;
    for ( i = m_contexts.begin(); i != m_contexts.end(); ++i )
    {
        Context * context( * i );
        context->fini();
        delete context;
    }
    m_contexts.clear();

    delete m_pm;
    m_pm = 0;

    m_rm->Purge();
    delete m_rm;
    m_rm = 0;

    if ( m_hge != 0 )
    {
        m_hge->System_Shutdown();
        m_hge->Release();
        m_hge = 0;
    }

    delete m_b2d;
    delete m_dd;
    delete m_overlay;
    delete m_vp;
    delete m_em;
}

//------------------------------------------------------------------------------
bool
Engine::handledKey()
{
    return m_handled_key;
}

//------------------------------------------------------------------------------
bool
Engine::isPaused()
{
    return m_paused;
}

//------------------------------------------------------------------------------
bool
Engine::isDebug()
{
    return m_dd->GetFlags() != 0;
}

//------------------------------------------------------------------------------
float
Engine::getTimeRatio()
{
    return m_time_ratio;
}

//------------------------------------------------------------------------------
void
Engine::error( const char * format, ... )
{
    char message[1024];

    va_list ap;
    va_start( ap, format );
    vsprintf_s( message, 1024, format, ap );
    va_end( ap );

    m_hge->System_Log( "Error: %s", message );
    MessageBox( NULL, message, "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
}

//------------------------------------------------------------------------------
void
Engine::start()
{
    m_contexts.push_back( new Splash() );
    m_contexts.push_back( new Menu() );
    m_contexts.push_back( new Tutorial() );
    m_contexts.push_back( new Game() );
    m_contexts.push_back( new Score() );
    m_contexts.push_back( new Credits() );

    m_pm = new hgeParticleManager();

    _initGraphics();
    _initPhysics();

    m_controller.init();

    if ( m_hge->System_Initiate() )
    {
        _loadData();
        init();
#ifdef _DEBUG
        switchContext( STATE_GAME );
#else
        switchContext( STATE_SPLASH );
#endif
        m_hge->System_Start();
    }
    else
    {
        MessageBox( NULL, m_hge->System_GetErrorMessage(), "Error",
                    MB_OK | MB_ICONERROR | MB_APPLMODAL );
    }
}

//------------------------------------------------------------------------------
void
Engine::init()
{
    hgeFont * font( m_rm->GetFont( "menu" ) );

    m_vp->restore();

    m_gui = new hgeGUI();
    float cx( 0.5f * m_vp->screen().x );
    float cy( 0.5f * m_vp->screen().y );

	m_navSnd = m_rm->GetEffect( "menu_nav" );
	m_selectSnd = m_rm->GetEffect( "menu_select" );

    m_gui->AddCtrl( new MenuItem( EC_CONTINUE, cx, cy - 15, "Continue",
                                  font, m_navSnd ) );

    m_gui->AddCtrl( new MenuItem( EC_QUIT, cx, cy + 15 , "Quit", font, m_navSnd ) );
    m_gui->SetNavMode( HGEGUI_UPDOWN );
    m_gui->SetFocus( 1 );
    m_gui->Enter();

    m_hge->Random_Seed();
}

//------------------------------------------------------------------------------
void
Engine::switchContext( EngineState state )
{
    m_running = false;

    if ( m_state != STATE_NONE )
    {
        m_contexts[m_state]->fini();
    }

    m_pm->KillAll();
    hgeInputEvent event;
    while ( m_hge->Input_GetEvent( & event ) );
    hideMouse();

    m_mouse.clear();
    m_controller.clear();

    m_stick = false;

    m_state = state;
    m_paused = false;
    m_handled_key = false;
    m_time_ratio = 1.0f;

    if ( m_state != STATE_NONE )
    {
        m_contexts[m_state]->init();
    }

    m_running = true;
}

//------------------------------------------------------------------------------
Context *
Engine::getContext()
{
    return m_contexts[m_state];
}

//------------------------------------------------------------------------------
void
Engine::showMouse()
{
    m_show_mouse = true;
}

//------------------------------------------------------------------------------
void
Engine::setMouse( const char * name )
{
    m_mouse_sprite = m_rm->GetSprite( name );
}

//------------------------------------------------------------------------------
void
Engine::hideMouse()
{
    m_show_mouse = false;
}

//------------------------------------------------------------------------------
const Mouse &
Engine::getMouse()
{
    return m_mouse;
}

//------------------------------------------------------------------------------
const Controller &
Engine::getController()
{
    return m_controller;
}

//------------------------------------------------------------------------------
Config &
Engine::getConfig()
{
    return m_config;
}

//------------------------------------------------------------------------------
int
Engine::updateGUI( float dt, hgeGUI * gui, int default_focus, int max )
{
    const Controller & pad( Engine::instance()->getController() );
    int select( 0 );

    if ( pad.isConnected() )
    {
        int focus( gui->GetFocus() );
        if ( pad.buttonDown( XPAD_BUTTON_START ) )
        {
            focus = default_focus;
            gui->SetFocus( focus );
        }
        if ( pad.buttonDown( XPAD_BUTTON_A ) ||
             pad.buttonDown( XPAD_BUTTON_START ) )
        {
            select = focus;
        }
        float dy( pad.getStick( XPAD_THUMBSTICK_LEFT ).y );
        if ( m_stick && dy < 0.8f && dy > -0.8f )
        {
            m_stick = false;
        }
        if ( ( ! m_stick && dy > 0.8f ||
               pad.buttonDown( XPAD_BUTTON_DPAD_UP ) ) && focus > 1 )
        {
            --focus;
            gui->SetFocus( focus );
            m_stick = true;
        }
        if ( ( ! m_stick && dy < -0.8f ||
               pad.buttonDown( XPAD_BUTTON_DPAD_DOWN ) ) && focus < max )
        {
            ++focus;
            gui->SetFocus( focus );
            m_stick = true;
        }
    }

    if ( select == 0 )
    {
        select = static_cast< Control >( gui->Update( dt ) );
    }

    return select;
}

//------------------------------------------------------------------------------
// physics:
//------------------------------------------------------------------------------
void
Engine::Violation( b2Body * body )
{
    m_hge->System_Log( "Body left world" );
    Entity * entity( static_cast<Entity *>( body->GetUserData() ) );
}

//------------------------------------------------------------------------------
void
Engine::Add( b2ContactPoint * point )
{
    Entity * entity1 =
        static_cast< Entity * >( point->shape1->GetBody()->GetUserData() );
    Entity * entity2 =
        static_cast< Entity * >( point->shape2->GetBody()->GetUserData() );
    entity1->collide( entity2, point );
    entity2->collide( entity1, point );
}

//------------------------------------------------------------------------------
void
Engine::Persist( b2ContactPoint * point )
{
}

//------------------------------------------------------------------------------
void
Engine::Remove( b2ContactPoint * point )
{
}

//------------------------------------------------------------------------------
bool
Engine::ShouldCollide( b2Shape * shape1, b2Shape * shape2 )
{
    if ( ! getContext()->handlesCollisions() )
    {
        return b2ContactFilter::ShouldCollide( shape1, shape2 );
    }
    Entity * entity1 =
        static_cast< Entity * >( shape1->GetBody()->GetUserData() );
    Entity * entity2 =
        static_cast< Entity * >( shape2->GetBody()->GetUserData() );
    return getContext()->shouldCollide( entity1, entity2 );
}

//------------------------------------------------------------------------------
//static:
//------------------------------------------------------------------------------
Engine *
Engine::instance()
{
    if ( s_instance == 0 )
    {
        s_instance = new Engine;
    }
    return s_instance;
}

//------------------------------------------------------------------------------
void
Engine::destroy()
{
    delete s_instance;
    s_instance = 0;
}

//------------------------------------------------------------------------------
HGE *
Engine::hge()
{
    return instance()->m_hge;
}

//------------------------------------------------------------------------------
b2World *
Engine::b2d()
{
    return instance()->m_b2d;
}

//------------------------------------------------------------------------------
ViewPort *
Engine::vp()
{
    return instance()->m_vp;
}

//------------------------------------------------------------------------------
EntityManager *
Engine::em()
{
    return instance()->m_em;
}

//------------------------------------------------------------------------------
hgeResourceManager *
Engine::rm()
{
    return instance()->m_rm;
}

//------------------------------------------------------------------------------
hgeParticleManager *
Engine::pm()
{
    return instance()->m_pm;
}

//------------------------------------------------------------------------------
DebugDraw *
Engine::dd()
{
    return instance()->m_dd;
}

//------------------------------------------------------------------------------
//private:
//------------------------------------------------------------------------------
bool
Engine::s_loseFocus()
{
    return s_instance->_loseFocus();
}

//------------------------------------------------------------------------------
bool
Engine::s_gainFocus()
{
    return s_instance->_gainFocus();
}

//------------------------------------------------------------------------------
bool
Engine::s_restore()
{
    return s_instance->_restore();
}

//------------------------------------------------------------------------------
bool
Engine::s_update()
{
    return s_instance->_update();
}

//------------------------------------------------------------------------------
bool
Engine::s_render()
{
    return s_instance->_render();
}

//------------------------------------------------------------------------------
bool
Engine::s_exit()
{
    return s_instance->_exit();
}

//------------------------------------------------------------------------------
bool
Engine::_loseFocus()
{
#ifndef _DEBUG
    m_paused = ( m_state == STATE_GAME || m_state == STATE_HELP );
#endif
    return false;
}

//------------------------------------------------------------------------------
bool
Engine::_gainFocus()
{
    return false;
}

//------------------------------------------------------------------------------
bool
Engine::_restore()
{
    _loseFocus();
    return m_vp->restore();
}

//------------------------------------------------------------------------------
bool
Engine::_update()
{
    float dt( m_hge->Timer_GetDelta() );

    if ( m_state == STATE_GAME || m_state == STATE_HELP )
    {
        if ( m_hge->Input_KeyDown( HGEK_P ) ||
             m_hge->Input_KeyDown( HGEK_ESCAPE ) )
        {
            m_handled_key = true;
            m_paused = ! m_paused;
        }
        if ( m_controller.buttonDown( XPAD_BUTTON_START ) ||
             m_controller.buttonDown( XPAD_BUTTON_BACK ) )
        {
            m_paused = ! m_paused;
            m_controller.clear();
        }
    }

#ifdef _DEBUG
    if ( m_hge->Input_KeyDown( HGEK_O ) && m_state != STATE_SCORE  )
    {
        m_handled_key = true;
        int flags( b2DebugDraw::e_shapeBit |
                   b2DebugDraw::e_aabbBit |
                   b2DebugDraw::e_obbBit );
        if ( m_dd->GetFlags() != 0 )
        {
            m_dd->ClearFlags( flags );
            m_time_ratio = 1.0f;
        }
        else
        {
            m_dd->SetFlags( flags );
        }
    }
#endif

    if ( m_dd->GetFlags() != 0 )
    {
        if ( m_hge->Input_KeyDown( HGEK_EQUALS ) )
        {
            m_time_ratio *= 0.5f;
        }
        if ( m_hge->Input_KeyDown( HGEK_MINUS ) )
        {
            m_time_ratio *= 2.0f;
        }
    }

    if ( m_dd->GetFlags() != 0 )
    {
        m_hge->Gfx_BeginScene();
        m_hge->Gfx_Clear( m_contexts[m_state]->getColour() );
        m_contexts[m_state]->render();
        m_vp->reset();
        _drawLetterbox();
    }      

    if ( m_paused )
    {
        switch ( static_cast< EngineControl >( updateGUI( dt, m_gui,
                                                          EC_CONTINUE, 2 ) ) )
        {
            case EC_CONTINUE:
            {
				m_hge->Effect_Play(m_selectSnd);
                m_paused = false;
                m_gui->SetFocus( 1 );
                break;
            }
            case EC_QUIT:
            {
				m_hge->Effect_Play(m_selectSnd);
                m_gui->SetFocus( 1 );
                switchContext( STATE_MENU );
                return false;
                break;
            }
        }
        dt = 0.0f;
    }
    else
    {
        dt *= m_time_ratio;
    }

    m_mouse.update( dt );
    m_controller.update( dt );

    m_b2d->Step( dt, 10 );
    bool retval( m_contexts[m_state]->update( dt ) );
    m_pm->Update( dt );

    if ( m_dd->GetFlags() != 0 )
    {
        _pauseOverlay();
        m_hge->Gfx_EndScene();
    }

    return retval;
}

//------------------------------------------------------------------------------
bool
Engine::_render()
{
    if ( ! m_running )
    {
        return false;
    }

    if ( m_dd->GetFlags() == 0 )
    {
        m_hge->Gfx_BeginScene();
        m_hge->Gfx_Clear( m_contexts[m_state]->getColour() );
        m_contexts[m_state]->render();
        m_vp->reset();
        _drawLetterbox();
        if ( m_show_mouse && m_mouse_sprite != 0 )
        {
            float x( 0.0f ); 
            float y( 0.0f );
            m_hge->Input_GetMousePos( & x, & y );
            m_mouse_sprite->Render( x, y );
        }
        _pauseOverlay();
        m_hge->Gfx_EndScene();
    }

    return false;
}

//------------------------------------------------------------------------------
bool
Engine::_exit()
{
    return false;
}

//------------------------------------------------------------------------------
void
Engine::_pauseOverlay()
{
    if ( ! m_paused && m_dd->GetFlags() == 0 && m_time_ratio == 1.0f )
    {
        return;
    }

    hgeFont * font( m_rm->GetFont( "menu" ) );

    float width( m_vp->screen().x );
    float height( m_vp->screen().y );

    if ( m_paused )
    {
        m_overlay->RenderStretch( 0.0f, 0.0f, width, height );
        m_gui->Render();
    }
    else if ( m_time_ratio != 1.0f )
    {
        font->SetColor( 0x88FFFFFF );
        font->printf( 0.5f * width, 0.0f, HGETEXT_CENTER,
                      "+++ %2.2f +++", m_time_ratio );
    }
    if ( m_dd->GetFlags() != 0 )
    {
        font->SetColor( 0x88FFFFFF );
        font->Render( 0.5f * width, height - font->GetHeight(), HGETEXT_CENTER,
                      "+++ D E B U G +++" );
    }
}

//------------------------------------------------------------------------------
void
Engine::_drawLetterbox()
{
    m_overlay->SetColor( m_contexts[m_state]->getBorder() );
    float width( m_vp->screen().x );
    float height( m_vp->screen().y );
    float dx( m_vp->getWidthOffset() );
    float dy( m_vp->getHeightOffset() );
    if ( dx > 0.0f )
    {
        m_overlay->RenderStretch( -dx, 0.0f, 0.0f, height + 2.0f * dy );
        m_overlay->RenderStretch( width, 0.0f, width + dx, height + 2.0f * dy );
    }
    if ( dy > 0.0f )
    {
        m_overlay->RenderStretch( 0.0f, -dy, width + 2.0f * dx, 0.0f );
        m_overlay->RenderStretch( 0.0f, height, width + 2.0f * dx, height + dy );
    }
    m_overlay->SetColor( 0xCC000000 );
}

//------------------------------------------------------------------------------
void
Engine::_initGraphics()
{
    m_hge = hgeCreate( HGE_VERSION );

    m_hge->System_SetState( HGE_LOGFILE, "kranzky.log" );
    m_hge->System_SetState( HGE_INIFILE, "kranzky.ini" );
    m_hge->System_SetState( HGE_FOCUSLOSTFUNC, s_loseFocus );
    m_hge->System_SetState( HGE_FOCUSGAINFUNC, s_gainFocus );
    m_hge->System_SetState( HGE_GFXRESTOREFUNC, s_restore );
    m_hge->System_SetState( HGE_FRAMEFUNC, s_update );
    m_hge->System_SetState( HGE_RENDERFUNC, s_render );
    m_hge->System_SetState( HGE_EXITFUNC, s_exit );
    m_hge->System_SetState( HGE_TITLE, "+++ P R O J E C T . I C E v0.1 +++" );
    m_hge->System_SetState( HGE_ICON, MAKEINTRESOURCE( IDI_ICON1 ) );
    m_hge->System_SetState( HGE_SHOWSPLASH, false );
    m_hge->System_SetState( HGE_FPS, HGEFPS_UNLIMITED );

    m_config.init();
    _setBestScreenMode();

    m_hge->System_SetState( HGE_SCREENWIDTH, m_config.screenWidth );
    m_hge->System_SetState( HGE_SCREENHEIGHT, m_config.screenHeight );
    m_hge->System_SetState( HGE_SCREENBPP, m_config.bpp );
    m_hge->System_SetState( HGE_USESOUND, m_config.sound );
    m_hge->System_SetState( HGE_WINDOWED, ! m_config.fullScreen );

    m_vp->screen().x = static_cast<float>( m_config.screenWidth );
    m_vp->screen().y = static_cast<float>( ( 9.0f * m_config.screenWidth ) / 16.0f);
    //m_vp->screen().y = static_cast<int>( m_config.screenHeight );

    m_overlay = new hgeSprite( 0, 0, 0, 1, 1 );
    m_overlay->SetColor( 0xCC000000 );
}

//------------------------------------------------------------------------------
void
Engine::_setBestScreenMode()
{
    int index = 0;
    DEVMODE dm;
    ZeroMemory( & dm, sizeof( dm ) );
    dm.dmSize = sizeof( dm );
    int bpp( 0 );
    float width( 0.0f );
    float height( 0.0f );
    float desired_ratio( 16.0f / 9.0f );
    int desired_bpp( m_config.bpp );
    while ( EnumDisplaySettings( NULL, index++, & dm ) != 0 )
    {
        // Don't consider anything less than 800x600
		if ( dm.dmPelsWidth < 800 || dm.dmPelsHeight < 600 )
		{
			continue;
		}
		// If it's the first time through the loop, or if we find an exact
        // match with the settings we want, then remember them.
        if ( width <= 0.0f || m_config.bpp == dm.dmBitsPerPel &&
                              m_config.screenWidth == dm.dmPelsWidth &&
                              m_config.screenHeight == dm.dmPelsHeight )
        {
            bool first( width <= 0.0f );
            bpp = static_cast<int>( dm.dmBitsPerPel );
            width = static_cast<float>( dm.dmPelsWidth );
            height = static_cast<float>( dm.dmPelsHeight );
            if ( ! first )
            {
                // No need to continue if we found a perfect match.
                break;
            }
        }
        // If we've found a partial match that has the precise width that we
        // wanted, then bail now (assumes ordered by increasing width).
        if ( width >= m_config.screenWidth &&
             static_cast<float>( dm.dmPelsWidth ) > width )
        {
            break;
        }
        // Check to see whether the current mode is better than what we're
        // currently going to use. This means that either the aspect ratio
        // is closer to ideal, or it's the same but the bpp is closer to
        // ideal, or that's the same too but the resolution is higher.
        float diff( fabs( desired_ratio -
                          static_cast<float>( dm.dmPelsWidth ) /
                          static_cast<float>( dm.dmPelsHeight ) ) -
                    fabs( desired_ratio - width / height ) );
        int dpp( abs( desired_bpp - static_cast<int>( dm.dmBitsPerPel ) ) -
                 abs( desired_bpp - bpp ) );
        if ( diff < 0.0f || diff == 0.0f && dpp <= 0 ||
             diff == 0.0f && dpp == 0 &&
             width > static_cast<float>( dm.dmPelsWidth ) )
        {
            bpp = static_cast<int>( dm.dmBitsPerPel );
            width = static_cast<float>( dm.dmPelsWidth );
            height = static_cast<float>( dm.dmPelsHeight );
        }
    }
    // Set the config to what we can support. This will be written to disk.
    m_config.bpp = bpp;
    m_config.screenWidth = static_cast<int>( width );
    m_config.screenHeight = static_cast<int>( height );
}

//------------------------------------------------------------------------------
void
Engine::_initPhysics()
{
    b2AABB worldAABB;
    worldAABB.lowerBound.Set( -2500.0f, -2500.0f );
    worldAABB.upperBound.Set( 2500.0f, 2500.0f );
    b2Vec2 gravity( 0.0f, 10.0f );
    m_b2d = new b2World( worldAABB, gravity, true );
    m_dd = new DebugDraw( m_hge, m_vp );
    m_b2d->SetDebugDraw( m_dd );
    m_b2d->SetListener( static_cast< b2ContactListener *>( this ) );
    m_b2d->SetListener( static_cast< b2BoundaryListener *>( this ) );
    m_b2d->SetFilter( static_cast< b2ContactFilter *>( this ) );
}

//------------------------------------------------------------------------------
void
Engine::_loadData()
{
    if ( ! m_hge->Resource_AttachPack( "resources.dat" ) )
    {
        error( "Cannot load '%s'", "resources.dat" );
    }

    m_rm = new hgeResourceManager( "data.res" );
    m_rm->Precache();

    m_hge->Resource_RemovePack( "resources.dat" );
}

//==============================================================================

