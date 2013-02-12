/* 
 * File:   MdlFileManager.cpp
 * Author: radek
 * 
 * Created on 12 luty 2013, 14:15
 */

#include "MdlFileManager.h"

template<> MdlManager *Ogre::Singleton<MdlManager>::ms_Singleton = 0;
 
 MdlManager *MdlManager::getSingletonPtr()
 {
     return ms_Singleton;
 }
 
 MdlManager &MdlManager::getSingleton()
 {  
     assert(ms_Singleton);  
     return(*ms_Singleton);
 }
 
 MdlManager::MdlManager()
 {
     mResourceType = "Mdl";
 
     // low, because it will likely reference other resources
     mLoadOrder = 30.0f;
 
     // this is how we register the ResourceManager with OGRE
     Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
 }
 
 MdlManager::~MdlManager()
 {
     // and this is how we unregister it
     Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
 }
 
 MdlPtr MdlManager::load(const Ogre::String &name, const Ogre::String &group)
 {
     MdlPtr mdl = getByName(name);
 
     if (mdl.isNull())
         mdl = create(name, group);
 
     mdl->load();
     return mdl;
 }
 
 Ogre::Resource *MdlManager::createImpl(const Ogre::String &name, Ogre::ResourceHandle handle, 
                                             const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader, 
                                             const Ogre::NameValuePairList *createParams)
 {
     return new Mdl(this, name, handle, group, isManual, loader);
 }