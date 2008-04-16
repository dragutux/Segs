/*
 * Super Entity Game Server Project
 * http://segs.sf.net/
 * Copyright (c) 2006 Super Entity Game Server Team (see Authors.txt)
 * This software is licensed! (See License.txt for details)
 *
 * $Id: MapServer.cpp 319 2007-01-26 17:03:18Z nemerle $
 */

//#include <ace/SOCK_Stream.h>
#include "ServerManager.h"
#include <ace/Message_Block.h>
#include "InterfaceManager.h"
#include "MapServer.h"
#include "ConfigExtension.h"
#include "MapServerEndpoint.h"
//#include "Net.h"
#include "AdminServerInterface.h"
#include "MapClient.h"

#include "SEGSMap.h"

// Template instantiation
template class ClientStore<MapClient>;

MapServer::MapServer(void) : 
m_id(0),
m_endpoint(NULL),
m_online(false),
m_max_maps(2)
{
	
}
MapServer::~MapServer(void)
{
	if(ACE_Reactor::instance())
	{
		ACE_Reactor::instance()->remove_handler(m_endpoint,ACE_Event_Handler::READ_MASK);
		delete m_endpoint;
	}
}
bool MapServer::Run(void)
{
	if(m_endpoint)
	{
		ACE_DEBUG((LM_WARNING,ACE_TEXT("(%P|%t) Game server already running\n") ));
		return true;
	}
	m_handled_worlds[0]=new SEGSMap("City_00_01");
	ACE_ASSERT(m_max_maps>0); // we have to have a world to run
	m_endpoint = new MapServerEndpoint(m_listen_point,this);
	if (ACE_Reactor::instance()->register_handler(m_endpoint,ACE_Event_Handler::READ_MASK) == -1)
		ACE_ERROR_RETURN ((LM_ERROR, "ACE_Reactor::register_handler"),false);
	if(!startup())
		return false;
	m_online = true;
	return true;
}
/**
 * @return bool (false means an error occurred )
 * @param  inipath Doc at RoamingServer::ReadConfig
 */
bool MapServer::ReadConfig(const std::string &inipath)
{
	StringsBasedCfg config;
	ACE_Ini_ImpExp	config_importer(config);
	ACE_Configuration_Section_Key root;
	string world_name;

	if(m_endpoint)
	{
		ACE_DEBUG((LM_WARNING,ACE_TEXT("(%P|%t) MapServer already initialized and running\n") ));
		return true;
	}
	if(!RoamingServer::ReadConfig(inipath))
		return false;

	if (config.open () == -1)
		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT ("(%P|%t) MapServer: %p\n"), ACE_TEXT ("config")),false);
	if (config_importer.import_config (inipath.c_str()) == -1)
		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT ("(%P|%t) MapServer: Unable to open config file : %s\n"), inipath.c_str()),false);
	if(-1==config.open_section(config.root_section(),"MapServer",1,root))
		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT ("(%P|%t) MapServer: Config file %s is missing [MapServer] section\n"), inipath.c_str()),false);

	config.get_addr(root,ACE_TEXT("listen_addr"),m_listen_point,ACE_INET_Addr(7002,"0.0.0.0"));
	config.get_addr(root,ACE_TEXT("location_addr"),m_location,ACE_INET_Addr(7002,"127.0.0.1"));
	m_online = false;
	return true;
}
bool MapServer::ShutDown(const std::string &reason)
{
	if(!m_endpoint)
	{
		ACE_DEBUG((LM_WARNING,ACE_TEXT("(%P|%t) Server not running yet\n") ));
		return true;
	}
	m_online = false;
	ACE_DEBUG((LM_WARNING,ACE_TEXT ("(%P|%t) Shutting down map server because : %s\n"), reason.c_str()));
	if (ACE_Reactor::instance()->remove_handler(m_endpoint,ACE_Event_Handler::READ_MASK) == -1)
	{
		delete m_endpoint;
		ACE_ERROR_RETURN ((LM_ERROR, "ACE_Reactor::remove_handler"),false);
	}
	delete m_endpoint;
	return true;
}
// This method is called by Game Server, to notify this MapServer that a client 
// with given source ip/port,id and access_level will be connecting here soon
// In return caller gets an unique client identifier. which is used later on to retrieve appropriate 
// client object
u32 MapServer::ExpectClient(const ACE_INET_Addr &from,u64 id,u16 access_level)
{
	return 2+m_clients.ExpectClient(from,id,access_level); // 2 is because 0,1 returns are reserved for : InvalidName,DatabaseProblem
}

MapClient * MapServer::ClientExpected(ACE_INET_Addr &from,u32 cookie)
{
	MapClient * res = m_clients.getExpectedByCookie(cookie-2); // reversing cookie incremental
	return res;
}
/**
* @return int Maximum number of handled Map threads
*/
int MapServer::getMaxHandledMaps()
{
	return m_max_maps;
}

/**
* Processing according to MapServerStartup sequence diagram.
*/
bool MapServer::startup()
{
	return true;
	AuthServerInterface *i_auth;
	AdminServerInterface *i_admin;
	GameServerInterface *i_game;

	ServerHandle<IMapServer> h_me(m_listen_point,m_id);

	i_auth = getAuthServer(); // connect to AuthServer
	i_admin= InterfaceManager::instance()->get(i_auth->AuthenticateMapServer(h_me,MAPSERVER_VERSION,"some_password"));// Authenticate
	if(!i_admin)
	{
		//Auth failure, Errors could be passed here by async AuthServer->MapServer calls
		return false;
	}
#if 0
	m_i_game=InterfaceManager::instance()->get(i_admin->RegisterMapServer(h_me));
	if(!m_i_game)
	{
		//Couldn't register self with mapserver
		return false;
	}
	return m_i_game->MapServerReady(h_me); // inform game server that we are ready.
#endif
}
void MapServer::AssociatePlayerWithMap(u64 player_id,int map_number)
{
	MapClient *client=m_clients.getById(player_id);
	client->setCurrentMap(getMapByNum(map_number));
}
SEGSMap * MapServer::getMapByNum(int num)
{
	return m_handled_worlds[num];
}