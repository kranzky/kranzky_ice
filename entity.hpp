//==============================================================================

#ifndef ArseThing
#define ArseThing

#pragma once

#include <sqlite3.h>

//------------------------------------------------------------------------------
struct b2ContactPoint;
class hgeSprite;
class Query;
class b2Body;
struct b2AABB;

//------------------------------------------------------------------------------
class Entity
{
  public:
    Entity( float scale = 1.0f );
    virtual ~Entity();

    void init();
    void update( float dt );
    void render( float scale );

    virtual void collide( Entity * entity, b2ContactPoint * point ) = 0;

    b2Body * getBody() const;
    b2AABB * getAABB() const;

    unsigned int getType() const;
    void setID( sqlite_int64 id );
    sqlite_int64 getID();

    float getScale() const;
    void setScale( float scale );
    void setSprite( const char * name );

    virtual void persistToDatabase() = 0;
    void deleteFromDatabase();

    void setZoom( int zoom );
    int getZoom() const;

    void setBlack( bool black );
    bool getBlack() const;

    void destroy();
    bool isGone() const;

  protected:
    Entity( const Entity & );
    Entity & operator=( const Entity & );

  protected:
    friend class EntityManager;

    void setType( unsigned int type );
    void setSpriteID( sqlite3_int64 sprite_id );
    virtual void onSetScale();
    virtual void doInit() = 0;
    virtual void doUpdate( float dt ) = 0;
    virtual void doRender( float scale ) = 0;
    virtual void initFromQuery( Query & query ) = 0;

  protected:
    int m_zoom;
    float m_scale;
    unsigned int m_type;
    sqlite_int64 m_id;
    b2Body * m_body;
    b2AABB * m_aabb;
    sqlite_int64 m_sprite_id;
    hgeSprite * m_sprite;
    bool m_black;
    bool m_destroy;
};

#endif

//==============================================================================
