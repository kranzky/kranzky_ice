//==============================================================================

#ifndef ArseEngine
#define ArseEngine

#pragma once

#include <vector>

#include <hge.h>
#include <Box2D.h>

#include <mouse.hpp>
#include <controller.hpp>
#include <config.hpp>

class hgeResourceManager;
class hgeParticleManager;
class hgeSprite;
class hgeGUI;

class DebugDraw;
class Context;
class ViewPort;
class EntityManager;

//------------------------------------------------------------------------------
enum EngineState
{
    STATE_NONE = -1, 
    STATE_SPLASH = 0,
    STATE_MENU = 1,
    STATE_HELP = 2,
    STATE_GAME = 3,
    STATE_SCORE = 4,
    STATE_CREDITS
};

enum EngineControl
{
    EC_NONE = 0,
    EC_CONTINUE = 1,
    EC_QUIT = 2
};

//------------------------------------------------------------------------------
class Engine : public b2BoundaryListener, public b2ContactListener,
               public b2ContactFilter
{
  public:
    static Engine * instance();
    static void destroy();
    static HGE * hge();
    static b2World * b2d();
    static ViewPort * vp();
    static EntityManager * em();
    static hgeResourceManager * rm();
    static hgeParticleManager * pm();
    static DebugDraw * dd();

  private:
    static bool s_loseFocus();
    static bool s_gainFocus();
    static bool s_restore();
    static bool s_update();
    static bool s_render();
    static bool s_exit();
    
  protected:
    Engine();
    Engine( const Engine & );
    Engine & operator=( const Engine & );
    ~Engine();

  public:
    bool handledKey();
    bool isPaused();
    bool isDebug();
    float getTimeRatio();
    void error( const char * format, ... );
    void start();
    void init();
    void switchContext( EngineState state );
    Context * getContext();
    void showMouse();
    void setMouse( const char * name );
    void hideMouse();
    const Mouse & getMouse();
    const Controller & getController();
    Config & getConfig();
    int updateGUI( float dt, hgeGUI * gui, int default_focus, int max );
    virtual void Violation( b2Body * body );
    virtual void Add( b2ContactPoint * point );
    virtual void Persist( b2ContactPoint * point );
    virtual void Remove( b2ContactPoint * point );
    virtual bool ShouldCollide( b2Shape * shape1, b2Shape * shape2 );
    HCHANNEL getChannel() const { return m_channel; }

  private:
    bool _loseFocus();
    bool _gainFocus();
    bool _restore();
    bool _update();
    bool _render();
    bool _exit();
    void _pauseOverlay();
    void _drawLetterbox();
    void _initGraphics();
    void _setBestScreenMode();
    void _initPhysics();
    void _loadData();

  private:
    static Engine * s_instance;
    hgeResourceManager * m_rm;
    hgeParticleManager * m_pm;
    HGE * m_hge;
    b2World * m_b2d;
    ViewPort * m_vp;
    EntityManager * m_em;
    DebugDraw * m_dd;
    hgeSprite * m_overlay;
    std::vector< Context * > m_contexts;
    EngineState m_state;
    Mouse m_mouse;
    Controller m_controller;
    Config m_config;
    bool m_handled_key;
    bool m_paused;
    bool m_running;
    bool m_show_mouse;
    hgeSprite * m_mouse_sprite;
    float m_time_ratio;
    hgeGUI * m_gui;
    bool m_stick;
    HCHANNEL m_channel;
	HEFFECT m_navSnd;
	HEFFECT m_selectSnd;
};

#endif

//==============================================================================
