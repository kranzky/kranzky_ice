//==============================================================================

#include <viewport.hpp>
#include <debug.hpp>

#include <hge.h>

//------------------------------------------------------------------------------
DebugDraw::DebugDraw( HGE * hge, ViewPort * viewport )
    :
    m_hge( hge ),
    m_viewport( viewport )
{
}

//------------------------------------------------------------------------------
DebugDraw::~DebugDraw()
{
}

//------------------------------------------------------------------------------
void
DebugDraw::DrawPolygon( const b2Vec2 * vertices, int32 vertexCount,
                        const b2Color & color )
{
    for ( int32 i = 0; i < vertexCount - 1; ++i )
    {
        DrawSegment( vertices[i], vertices[i + 1], color );
    }
    DrawSegment( vertices[vertexCount - 1], vertices[0], color );
}
 
//------------------------------------------------------------------------------
void
DebugDraw::DrawSolidPolygon( const b2Vec2 * vertices, int32 vertexCount,
                             const b2Color & color )
{
    b2Vec2 average( 0.0f, 0.0f );
    for ( int i = 0; i < vertexCount; ++ i )
    {
        average.x += vertices[i].x;
        average.y += vertices[i].y;
    }
    average.x /= static_cast< float >( vertexCount );
    average.y /= static_cast< float >( vertexCount );
    for ( int i = 0; i < vertexCount; ++ i )
    {
        int j = i + 1;
        if ( j >= vertexCount )
        {
            j = 0;
        }
        hgeTriple triple;
        _setVertex( triple.v[0], vertices[i], color );
        _setVertex( triple.v[1], average, color );
        _setVertex( triple.v[2], vertices[j], color );
        triple.tex = 0;
        triple.blend = BLEND_ALPHABLEND;
        m_hge->Gfx_RenderTriple( & triple );
    }
}
 
//------------------------------------------------------------------------------
void
DebugDraw::DrawCircle( const b2Vec2 & center, float32 radius,
                       const b2Color & color ) 
{
    b2Vec2 vertices[16];
    _getCircleVertices( vertices, center, radius );
    DrawPolygon( vertices, 16, color );
}
 
//------------------------------------------------------------------------------
void
DebugDraw::DrawSolidCircle( const b2Vec2 & center, float32 radius,
                            const b2Vec2 & axis, const b2Color & color ) 
{
    b2Vec2 vertices[16];
    _getCircleVertices( vertices, center, radius );
    DrawSolidPolygon( vertices, 16, color );
}
 
//------------------------------------------------------------------------------
void
DebugDraw::DrawSegment( const b2Vec2 & p1, const b2Vec2 & p2,
                        const b2Color & color ) 
{
    m_hge->Gfx_RenderLine( p1.x, p1.y, p2.x, p2.y, _hgeColor( color ) );
}
 
//------------------------------------------------------------------------------
void
DebugDraw::DrawXForm( const b2XForm & xf ) 
{
}

//------------------------------------------------------------------------------
// private:
//------------------------------------------------------------------------------
DWORD
DebugDraw::_hgeColor( b2Color color )
{
    return ARGB( 0x88,
                 static_cast<int>( 0xFF * color.r ),
                 static_cast<int>( 0xFF * color.g ),
                 static_cast<int>( 0xFF * color.b ) );
}

//------------------------------------------------------------------------------
void
DebugDraw::_setVertex( hgeVertex & vertex, const b2Vec2 & vector,
                       const b2Color & color )
{
    vertex.x = vector.x;
    vertex.y = vector.y;
    vertex.z = 0;
    vertex.col = _hgeColor( color );
    vertex.tx = 0;
    vertex.ty = 0;
}

//------------------------------------------------------------------------------
void
DebugDraw::_getCircleVertices( b2Vec2 ( & vertices )[16], const b2Vec2 & center,
                               float radius )
{
    float angle( 0.0f );
    float step( 2.0f * M_PI / 15.0f );
    vertices[0].x = 0.0f;
    vertices[0].y = radius;
    b2Mat22 rotation;
    for ( int i = 1; i < 16; ++i )
    {
        angle += step;
        rotation.Set( angle );
        vertices[i] = b2Mul( rotation, vertices[0] ) + center;
    }
    vertices[0] += center;
}

//==============================================================================
