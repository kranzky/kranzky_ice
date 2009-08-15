//==============================================================================

#include <config.hpp>
#include <engine.hpp>

#include <hge.h>

//------------------------------------------------------------------------------
Config::Config()
    :
    screenWidth( 800 ),
    screenHeight( 600 ),
    bpp( 32 ),
#ifdef _DEBUG
    fullScreen( false ),
#else
    fullScreen( false ),
#endif
    sound( true ),
    userName( "Lloyd" ),
    menu( 1 ),
#ifdef _DEBUG
    vibrate( false ),
#else
    vibrate( true ),
#endif
    leaderboard( false )
{
}

//------------------------------------------------------------------------------
Config::~Config()
{
}

//------------------------------------------------------------------------------
void
Config::init()
{
    screenWidth = Engine::hge()->Ini_GetInt( "kranzky", "width",
                                             screenWidth );
    screenHeight = Engine::hge()->Ini_GetInt( "kranzky","height",
                                              screenHeight );
    bpp = Engine::hge()->Ini_GetInt( "kranzky","bpp", bpp );
    fullScreen = Engine::hge()->Ini_GetInt( "kranzky", "fullscreen",
                                            fullScreen ? 1 : 0 ) == 1;
    sound = Engine::hge()->Ini_GetInt( "kranzky", "sound", sound ? 1 : 0 ) == 1;
    userName = Engine::hge()->Ini_GetString( "kranzky", "username",
                                             userName.c_str() );
    menu = Engine::hge()->Ini_GetInt( "kranzky", "menu", menu );
    vibrate = Engine::hge()->Ini_GetInt( "kranzky", "vibrate",
                                         vibrate ? 1 : 0 ) == 1;
    leaderboard = Engine::hge()->Ini_GetInt( "kranzky", "leaderboard",
                                             leaderboard ? 1 : 0 ) == 1;
}

//------------------------------------------------------------------------------
void
Config::fini()
{
    Engine::hge()->Ini_SetInt( "kranzky", "width", screenWidth );
    Engine::hge()->Ini_SetInt( "kranzky","height", screenHeight );
    Engine::hge()->Ini_SetInt( "kranzky","bpp", bpp );
    Engine::hge()->Ini_SetInt( "kranzky", "fullscreen", fullScreen ? 1 : 0 );
    Engine::hge()->Ini_SetInt( "kranzky", "sound", sound ? 1 : 0 );
    Engine::hge()->Ini_SetString( "kranzky", "username", userName.c_str() );
    Engine::hge()->Ini_SetInt( "kranzky", "menu", menu );
    Engine::hge()->Ini_SetInt( "kranzky", "vibrate", vibrate ? 1 : 0 );
    Engine::hge()->Ini_SetInt( "kranzky", "leaderboard", leaderboard ? 1 : 0 );
}

//==============================================================================
