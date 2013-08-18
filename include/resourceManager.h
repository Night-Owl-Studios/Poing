/* 
 * File:   resourceManager.h
 * Author: hammy
 *
 * Created on July 12, 2013, 11:56 PM
 */

#ifndef __POING_RESOURCE_MGR_H__
#define	__POING_RESOURCE_MGR_H__

#include <map>
#include <vector>
#include <string>

#include "utils/hash.h"

#include "harbinger.h"

enum resourceType : int {
    RESOURCE_INVALID    = -1,
    RESOURCE_BITMAP     = 0,
    RESOURCE_CUBEMAP,
    RESOURCE_FONT,
    RESOURCE_MESH,
    RESOURCE_SHADER
};

class resourceManager {
    public:
        struct resource {
            std::string     fileName    = "";
            unsigned long   hashId      = 0;
            resourceType    type        = RESOURCE_INVALID;
            hge::resource*  pResource   = nullptr;
        };
        
        typedef hamLibs::utils::hashVal_t           resourceId_t;
        typedef std::vector< resource >             resourceList_t;
        typedef std::map< resourceId_t, hge::resource* > resourceMap_t;
        
    private:
        resourceMap_t   resourceMap;
        resourceList_t  resourceList;
    
    public:
        resourceManager ();
        ~resourceManager() { clear(); }
        
        resourceId_t    hasResource     ( const std::string& ) const;
        hge::resource*  getResource     ( resourceId_t );
        
        void            unloadResource  ( resourceId_t );
        void            clear           ();
        
        void            setPoolSize     ( unsigned poolSize )   { resourceList.reserve( poolSize ); }
        unsigned        getPoolSize     () const                { return resourceList.capacity(); }
        unsigned        getNumResources () const                { return resourceList.size(); }
        
        resourceId_t    loadResource    ( resourceType, const std::string&, int flags );
};

#endif	/* __POING_RESOURCE_MGR_H__ */

