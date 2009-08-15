//==============================================================================

#include <entity_manager.hpp>
#include <entity.hpp>
#include <engine.hpp>

#include <hgeResource.h>

#include <sstream>
#include <cstdarg>

//------------------------------------------------------------------------------
EntityManager::EntityManager()
    :
    m_registry(),
    m_entities(),
    m_sprites(),
    m_names(),
    m_new_entities()
{
}

//------------------------------------------------------------------------------
EntityManager::~EntityManager()
{
    fini();
}

//------------------------------------------------------------------------------
void
EntityManager::init()
{
}

//------------------------------------------------------------------------------
void
EntityManager::update( float dt )
{
    std::vector< Entity * >::iterator i( m_entities.begin() );
    while ( i != m_entities.end() )
    {
        ( * i )->update( dt );
        if ( ( * i )->isGone() )
        {
            delete ( * i );
            i = m_entities.erase( i );
        }
        else
        {
            ++i;
        }
    }
    while ( m_new_entities.size() > 0 )
    {
        m_entities.push_back( m_new_entities.back() );
		m_new_entities.pop_back();
    }
}

//------------------------------------------------------------------------------
void
EntityManager::fini()
{
    while ( m_entities.size() > 0 )
    {
        delete m_entities.back();
        m_entities.pop_back();
    }
    while ( m_new_entities.size() > 0 )
    {
        delete m_new_entities.back();
        m_new_entities.pop_back();
    }
    m_registry.clear();
    m_sprites.clear();
    m_names.clear();
}

//------------------------------------------------------------------------------
void
EntityManager::registerEntity( unsigned int type, Entity * ( * factory )(),
                               const char * table, const char * query )
{
    std::map< unsigned int, EntityData >::iterator i( m_registry.find( type ) );
    if ( i != m_registry.end() )
    {
        Engine::hge()->System_Log( "Cannot register same entity twice." );
        return;
    }
    EntityData data;
    data.m_factory = factory;
    data.m_table = table;
    data.m_query = query;
    m_registry.insert( std::pair< unsigned int, EntityData >( type, data ) );
}

//------------------------------------------------------------------------------
sqlite_int64
EntityManager::registerSprite( const char * cname )
{
    std::string name( cname );
    std::map< std::string, sqlite_int64 >::iterator i( m_names.find( name ) );
    if ( i != m_names.end() )
    {
        return m_names[name];
    }
    sqlite_int64 id( 0 );
    for ( id = 0; m_sprites.find( id ) != m_sprites.end(); ++id );
    m_names[name] = id;
    m_sprites[id] = Engine::rm()->GetSprite( cname );
    return id;
}

//------------------------------------------------------------------------------
Entity *
EntityManager::factory( unsigned int type, bool add )
{
    std::map< unsigned int, EntityData >::iterator i( m_registry.find( type ) );
    if ( i == m_registry.end() )
    {
        Engine::hge()->System_Log( "Tried to create unregistered entity." );
        return 0;
    }
    Entity * entity( i->second.m_factory() );
    entity->setType( type );
    if ( add )
    {
        m_new_entities.push_back( entity );
    }
    return entity;
}

//------------------------------------------------------------------------------
std::vector< Entity * >
EntityManager::databaseFactory( unsigned int type )
{
    std::vector< Entity * > retval;
    return retval;
}

//------------------------------------------------------------------------------
sqlite_int64
EntityManager::persistToDatabase( Entity * entity, char * rows[], ... )
{
    return 0;
}

//------------------------------------------------------------------------------
void
EntityManager::deleteFromDatabase( Entity * entity )
{
}

//------------------------------------------------------------------------------
hgeSprite *
EntityManager::getSprite( sqlite_int64 sprite_id )
{
    std::map< sqlite_int64,hgeSprite * >::iterator i(m_sprites.find(sprite_id));
    if ( i == m_sprites.end() )
    {
        Engine::hge()->System_Log( "Cannot find referenced sprite." );
        return 0;
    }
    return m_sprites[sprite_id];
}

//------------------------------------------------------------------------------
std::vector<Entity*> 
EntityManager::getEntities(unsigned int type)
{
    std::vector< Entity * > entities;

	std::vector< Entity * >::iterator i;
    for ( i = m_entities.begin(); i != m_entities.end(); ++i )
    {
		Entity* ent = (*i);
		if (ent->getType() == type)
		{
			entities.push_back(ent);
		}
	}
	return entities;
}


//==============================================================================
