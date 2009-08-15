//==============================================================================

#ifndef ArseScore
#define ArseScore

#pragma once

#include <vector>
#include <string>

#include <hge.h>

#include <context.hpp>

class hgeSprite;

//------------------------------------------------------------------------------
enum ScoreState
{
    SCORE_CALCULATE = 0,
    SCORE_INPUT = 1,
    SCORE_SHOW = 2
};

//------------------------------------------------------------------------------
class Score : public Context
{
  public:
    Score();
    virtual ~Score();

  private:
    Score( const Score & );
    Score & operator=( const Score & );

  public:
    virtual void init();
    virtual void fini();
    virtual bool update( float dt );
    virtual void render();
    void setValue( int value );

  private:
    void _updateScore();
    void _insertScore( const char * name, int value );
    void _updateScoreDB();
    void _insertScoreDB( const char * name, int value );

  private:
    struct ScoreData
    {
        std::string timestamp;
        std::string name;
        int value;
        bool clipped;
    };

    hgeSprite * m_cursor;
    ScoreState m_state;
    float m_timer;
    float m_time_to_show;
    float m_time_to_post;
    std::vector< ScoreData > m_high_score;
    std::string m_name;
    int m_value;
    int m_highlight;
    float m_dy;
    bool m_error;
};

#endif

//==============================================================================
