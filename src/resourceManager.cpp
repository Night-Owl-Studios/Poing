/* 
 * File:   resourceManager.cpp
 * Author: hammy
 * 
 * Created on July 12, 2013, 11:56 PM
 */

#include <iostream>
#include "resourceManager.h"

///////////////////////////////////////////////////////////////////////////////
//      RESOURCE MANAGER CONSTRUCTORS, OPERATORS, AND DESTRUCTORS
///////////////////////////////////////////////////////////////////////////////
resourceManager::resourceManager() {
}

///////////////////////////////////////////////////////////////////////////////
//      RESOURCE MANAGER UNLOADING
///////////////////////////////////////////////////////////////////////////////
void resourceManager::unloadResource( resourceId_t id ) {
    for ( unsigned i = 0; i < resourceList.size(); ++i ) {
        if ( id == resourceList[ i ].hashId ) {
            resource* pResource = &resourceList[ i ];
            pResource->fileName.clear();
            pResource->hashId = 0;
            pResource->type = RESOURCE_INVALID;

            delete pResource->pResource;
            pResource->pResource = nullptr;
            
            resourceMap_t::iterator mapIter = resourceMap.find( id );
            resourceMap.erase( mapIter );
            resourceList.erase( resourceList.begin() + i );
            
            break;
        }
    }
}

void resourceManager::clear() {
    for ( unsigned i = 0; i < resourceList.size(); ++i ) {
        resource* pResource = &resourceList[ i ];
        pResource->fileName.clear();
        pResource->hashId = 0;
        pResource->type = RESOURCE_INVALID;
        
        delete pResource->pResource;
        pResource->pResource = nullptr;
    }
    resourceList.clear();
    resourceMap.clear();
}

///////////////////////////////////////////////////////////////////////////////
//      RESOURCE MANAGER DATA ACQUITISION
///////////////////////////////////////////////////////////////////////////////
resourceManager::resourceId_t resourceManager::hasResource( const std::string& id ) const {
    for ( unsigned i = 0; i < resourceList.size(); ++i ) {
        if ( resourceList[ i ].fileName == id )
            return resourceList[ i ].hashId;
    }
    return RESOURCE_INVALID;
}

hge::resource* resourceManager::getResource( resourceId_t id ) {
    resourceMap_t::iterator iter = resourceMap.find( id );
    
    return ( iter != resourceMap.end() )
        ? iter->second
        : nullptr;
}

///////////////////////////////////////////////////////////////////////////////
//      RESOURCE MANAGER LOADING
///////////////////////////////////////////////////////////////////////////////
resourceManager::resourceId_t resourceManager::loadResource(
    resourceType        resType,
    const std::string&  fileName,
    int                 flags
) {
    resourceId_t hashedId = hamLibs::utils::hashFNV1( fileName.c_str() );
    hge::resource* res = nullptr;
    
    switch( resType ) {
        case RESOURCE_BITMAP:   res = new hge::bitmap;  break;
        case RESOURCE_CUBEMAP:  res = new hge::cubemap; break;
        case RESOURCE_FONT:     res = new hge::font;    break;
        case RESOURCE_MESH:     res = new hge::mesh;    break;
        case RESOURCE_SHADER:   res = new hge::shader;  break;
        default:                return 0;
    }
    
    if ( !hge::resource::fileExists( fileName.c_str() ) ) {
        std::cerr
            << "Error: Attempted to load a nonexistent file "
            << fileName << '.'
            << std::endl;
        return 0;
    }
    if ( !res->load( fileName.c_str(), flags ) ) {
        std::cerr
            << "Error: Unable to load " << fileName
            << ". Please check that the file has not been corrupted or altered "
            << " by another program while being loaded."
            << std::endl;
        return 0;
    }
    
    // All went well. add the new resource to the manager
    resourceList.emplace_back( resource() );
    resource& r = resourceList.back();
    r.fileName  = fileName;
    r.hashId    = hashedId;
    r.type      = resType;
    r.pResource = res;
    
    resourceMap[ hashedId ] = resourceList.back().pResource;
    
    return hashedId;
}
