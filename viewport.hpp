//==============================================================================

#ifndef ArseView
#define ArseView

#pragma once

#include <Box2D.h>

//------------------------------------------------------------------------------
class ViewPort
{
  public:
    ViewPort();
    ~ViewPort();
    float angle();
    b2Vec2 & offset();
    b2Vec2 & centre();
    b2Vec2 & bounds();
    b2Vec2 & screen();
    void screenToWorld( b2Vec2 & point );
    float hscale() const;
    float vscale() const;
    void setScale( float scale );
    void setAngle( float angle );
    void setTransform( float scale = 1.0f );
    void reset();
    bool restore();
    float getWidthOffset() const { return m_dx; }
    float getHeightOffset() const { return m_dy; }

  private:
    void _updateRatios() const;

    b2Vec2 m_offset;
    b2Vec2 m_centre;
    b2Vec2 m_bounds;
    b2Vec2 m_screen;
    mutable float m_hscale;
    mutable float m_vscale;
    float m_angle;
    float m_dx;
    float m_dy;
};

#endif

//==============================================================================
