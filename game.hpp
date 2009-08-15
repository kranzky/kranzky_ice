//==============================================================================

#ifndef ArseGame
#define ArseGame

#pragma once

#include <context.hpp>
#include <list>

class hgeSprite;

//------------------------------------------------------------------------------
class Game : public Context
{
  public:
    Game();
    virtual ~Game();

  private:
    Game( const Game & );
    Game & operator=( const Game & );

  public:
    virtual void init();
    virtual void fini();
    virtual bool update( float dt );
    virtual void render();

  private:
    void clearArena();
    void clearPiece();
    void checkBorders();
    bool checkLeft();
    bool checkRight();
    bool checkTop();
    bool checkBottom();
    bool blankBelow();
    void addRows();
    void insertRow( int row );
    void removePiece();
    bool isClear();

    bool m_buffer[10][20];
    bool m_arena[10][20];
    bool m_piece[4][4];
    int m_index;
    int m_rotate;
    int m_col;
    int m_row;
    int m_replay[1000][4];
    int m_num;
    bool m_replaying;
    float m_timer;
};

#endif

//==============================================================================
