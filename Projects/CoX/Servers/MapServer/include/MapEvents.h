/*
 * Super Entity Game Server Project
 * http://segs.sf.net/
 * Copyright (c) 2006 Super Entity Game Server Team (see Authors.txt)
 * This software is licensed! (See License.txt for details)
 *
 * $Id$
 */

#pragma once
#include <ace/Assert.h>
#include "LinkLevelEvent.h"
#include "BitStream.h"
#include "CRUD_Link.h"
#include "CRUD_Events.h"
#include "MapLink.h"
class Entity;
typedef CRUDLink_Event<MapLink> MapLinkEvent;

class MapEventTypes : public CRUD_EventTypes
{
public:
    BEGINE_EVENTS(CRUD_EventTypes);
    EVENT_DECL(evEntityEnteringMap      ,0);
    EVENT_DECL(evMapInstanceConnected   ,1);
    EVENT_DECL(evShortcutsRequest       ,2);
    EVENT_DECL(evShortcuts              ,3);
    EVENT_DECL(evSceneRequest           ,4);
    EVENT_DECL(evScene                  ,5);
    EVENT_DECL(evEntitiesRequest        ,6);
    EVENT_DECL(evEntitites              ,7);
    EVENT_DECL(evInputState             ,8);
    END_EVENTS(500);
};
//////////////////////////////////////////////////////////////////////////
// Client -> Server
#include "Events/NewEntity.h"

class MapUnknownRequest : public MapLinkEvent
{
public:
    MapUnknownRequest():MapLinkEvent(MapEventTypes::evUnknownEvent)
    {

    }
    void serializeto(BitStream &) const
    {
    }
    void serializefrom(BitStream &)
    {
    }
};
class ShortcutsRequest : public MapLinkEvent
{
public:
    ShortcutsRequest():MapLinkEvent(MapEventTypes::evShortcutsRequest)
    {}
    void serializeto(BitStream &bs) const
    {
        bs.StorePackedBits(1,4); // opcode
    }
    void serializefrom(BitStream &)
    {
    }
};

class SceneRequest : public MapLinkEvent
{
public:
    SceneRequest():MapLinkEvent(MapEventTypes::evSceneRequest)
    {}
    void serializeto(BitStream &bs) const
    {
        bs.StorePackedBits(1,3); // opcode
    }
    void serializefrom(BitStream &)
    {
    }
};

class EntitiesRequest : public MapLinkEvent
{
public:
    EntitiesRequest():MapLinkEvent(MapEventTypes::evEntitiesRequest)
    {}
    void serializeto(BitStream &bs) const
    {
        bs.StorePackedBits(1,5); // opcode
    }
    void serializefrom(BitStream &)
    {
    }

};

class InputState : public MapLinkEvent
{
public:
    InputState() : MapLinkEvent(MapEventTypes::evInputState)
    {}
    void serializeto(BitStream &) const
    {
    }
    void serializefrom(BitStream &)
    {
    }
};

class MapEventFactory : public CRUD_EventFactory<MapLinkEvent>
{
public:
    static MapLinkEvent *EventFromStream(BitStream &bs);
};
//////////////////////////////////////////////////////////////////////////
// Server -> Client events
class MapInstanceConnected : public MapLinkEvent
{
public:
    MapInstanceConnected():MapLinkEvent(MapEventTypes::evMapInstanceConnected)
    {
    }
    MapInstanceConnected(EventProcessor *evsrc,u32 resp,const std::string &err) : MapLinkEvent(MapEventTypes::evMapInstanceConnected),m_resp(resp),m_fatal_error(err)
    {
    }
    void serializeto(BitStream &bs) const
    {
        bs.StorePackedBits(1,5); //opcode
        bs.StorePackedBits(1,m_resp);
        if(m_resp)
            bs.StoreString(m_fatal_error);
    }
    void serializefrom(BitStream &src)
    {
        m_resp = src.GetPackedBits(1);
        if(m_resp==0)
            src.GetString(m_fatal_error);
    }
    u32         m_resp;
    std::string m_fatal_error;

};
#include "NetCommandManager.h"
class Shortcuts : public MapLinkEvent
{
public:
    Shortcuts():MapLinkEvent(MapEventTypes::evShortcuts)
    {
    }
    void serializeto(BitStream &bs) const
    {
        bs.StorePackedBits(1,4); // opcode
        NetCommandManagerSingleton::instance()->SendCommandShortcuts(m_client,bs,m_commands2);
    }
    void serializefrom(BitStream &src)
    {
        ACE_ASSERT(!"TODO");
    }
    u32 m_num_shortcuts2;
    //vector<NetCommand *> m_commands;
    vector<NetCommand *> m_commands2;  // m_commands2 will get filled after we know more about them
    vector<std::string>  m_shortcuts2;
    MapClient *m_client;
};
#include "MapRef.h"
class Scene : public MapLinkEvent
{
    virtual void dependent_dump(void)
    {
        ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%IpktSC_SceneResp\n%I{\n")));
        ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I    undos_PP 0x%08x\n"),undos_PP));
        ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I    var_14 0x%08x\n"),var_14));
        if(var_14)
        {
            ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I    m_outdoor_map 0x%08x\n"),m_outdoor_map));
            ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I    m_map_number 0x%08x\n"),m_map_number));
            ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I    unkn1 0x%08x\n"),unkn1));
            ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I    m_map_desc %s\n"),m_map_desc.c_str()));
        }
        ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I    flag_dword_151D5D8 0x%08x\n"),current_map_flags));
        ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I    num_base_elems 0x%08x\n"),num_base_elems));
        for(size_t i=0; i<num_base_elems; i++)
        {
            ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I    unkn2 0x%08x\n"),unkn2));
            ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I    m_trays[i] %s\n"),m_trays[i].c_str()));
            ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I    m_crc[i] 0x%08x\n"),m_crc[i]));
            ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I    unkn2 0x%08x\n"),unkn2));
            ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I    unkn1 0x%08x\n"),unkn1));
        }
        if(num_base_elems!=0)
        {
            ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I    unkn2 0x%08x\n"),unkn2));
            ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I    m_trays[0] %s\n"),m_trays[0].c_str()));
            ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I    m_crc[0] 0x%08x\n"),m_crc[0]));
            ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I    unkn2 0x%08x\n"),unkn2));
            ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I    unkn1 0x%08x\n"),unkn1));
        }
        for(size_t i=0; i<m_refs.size(); i++)
        {
            m_refs[i].dump();
        }
        ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%I}\n")));
    }
public:
    Scene():MapLinkEvent(MapEventTypes::evScene)
    {
    }

    void getGrpElem(BitStream &src,int idx)
    {
        if(src.GetBits(1))
        {
            src.GetString(m_trays[idx]);
            m_crc[idx] = src.GetBits(32);
            if(!src.GetBits(1))
            {
                int var_14=0;
                int var_10;
                while(var_10=src.GetPackedBits(1)-1>=0)
                {
                    //					if(!var_14)
                    //						var_14=syb_599950(var_4);
                    groupnetrecv_5927C0(src,var_10,var_14);
                }
            }
            else
                reqWorldUpdateIfPak(src);
        }
    }
    void reqWorldUpdateIfPak(BitStream &src)
    {
        //src.GetBits(1);
        ACE_ASSERT(0);
    }
    void groupnetrecv_5927C0(BitStream &src,int a,int b)
    {
        if(!src.GetBits(1))
            return;
        ACE_ASSERT(0);
        string def_filename;
        src.GetString(def_filename);
    }

    void serializefrom(BitStream &src)
    {
        unkn1=false;
        bool IAmAnArtist;
        undos_PP = src.GetPackedBits(1);
        var_14 = src.GetBits(1);
        if(var_14)
        {
            m_outdoor_map = src.GetBits(1);
            m_map_number = src.GetPackedBits(1);
            unkn1 = src.GetPackedBits(1);
            if(unkn1)
            {
                IAmAnArtist=false;
                //IAmAnArtist=isDevelopmentMode())
            }
            src.GetString(m_map_desc);
        }
        current_map_flags = src.GetBits(1); // is beaconized
        num_base_elems = src.GetPackedBits(1);
        m_crc.resize(num_base_elems);
        m_trays.resize(num_base_elems);
        for(size_t i=1; i<num_base_elems; i++)
        {
            getGrpElem(src,i);
        }
        if(num_base_elems!=0)
        {
            getGrpElem(src,0);
        }
        MapRef r;
        do
        {
            r.serializefrom(src);
            m_refs.push_back(r);
        } while(r.m_idx>=0);
        src.GetPackedBits(1); //unused
        ref_count = src.GetPackedBits(1);
        ACE_ASSERT(ref_count==m_refs.size());
        src.GetBits(32); //unused - crc ?
        ref_crc=src.GetBits(32); // 0x3f6057cf
    }
    void serializeto(BitStream &tgt) const
    {
        tgt.StorePackedBits(1,6); // opcode
        tgt.StorePackedBits(1,undos_PP);
        tgt.StoreBits(1,var_14);
        if(var_14)
        {
            tgt.StoreString(m_map_desc);
            tgt.StoreBits(1,m_outdoor_map);
            tgt.StorePackedBits(1,m_map_number);
        }
        //tgt.StoreBits(1,current_map_flags);
        //tgt.StorePackedBits(1,m_trays.size());
        u32 hashes[] = {0x00000000,0xAFD34459,0xE63A2B76,0xFBBAD9D4,
            0x9AE0A9D4,0x06BDEF70,0xA47A21F8,0x5FBF835D,
            0xFF25F3F6,0x70E6C422,0xF1CCC459,0xCBD35A55,
            0x64CCCC31,0x535B08CC};

        //			ACE_TRACE(!"Hold yer horses!");
        // overriding defs
        tgt.StorePackedBits(1,-1); // finisher
        // overriding groups
        tgt.StorePackedBits(1,-1); // fake it all the way
        return;
        if(0)
        {
            for(size_t i=1; i<m_refs.size(); i++)
            {
                //			ACE_TRACE(!"Hold yer horses!");
                tgt.StorePackedBits(1,0); // next element idx 
                tgt.StoreBits(1,0);
                if(0)
                {
                    // old coh use it that way, first string of the 2 is optional (path), second is mandatory
                    //NetStructure::storeStringConditional(tgt,m_trays[i]);
                    //tgt.StoreString("");
                    NetStructure::storeStringConditional(tgt,"");
                    tgt.StoreString(m_trays[i]);
                    if(i<12)
                        tgt.StoreBits(32,hashes[i]); // crc ? 
                    else
                        tgt.StoreBits(32,0); // crc ? 
                    //tgt.StoreBits(1,0);
                    tgt.StorePackedBits(1,0);
                    tgt.StorePackedBits(1,0);
                }
            }
            tgt.StorePackedBits(1,-1); // finisher
        }
        if(0)
        {
            for(size_t i=0; i<m_refs.size(); i++)
            {
                tgt.StorePackedBits(1,0); // next element idx 
                m_refs[i].serializeto(tgt);
            }
        }
        tgt.StorePackedBits(1,~0); // finishing marker,-1
        tgt.StorePackedBits(1,0xD8); //unused
        tgt.StorePackedBits(1,ref_count);
        tgt.StoreBits(32,0); //unused - crc ?
        tgt.StoreBits(32,ref_crc); // 0x3f6057cf
    }
    string m_map_desc;
    int ref_count;
    int ref_crc;
    int var_4;
    bool m_outdoor_map;
    bool current_map_flags;
    size_t num_base_elems;
    int undos_PP;
    bool var_14;
    vector<string> m_trays;
    vector<u32> m_crc;
    vector<MapRef> m_refs;
    int unkn1; 
    int m_map_number;
    bool unkn2;
};