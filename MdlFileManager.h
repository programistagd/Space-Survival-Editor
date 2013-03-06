/* 
 * File:   MdlFileManager.h
 * Author: radek
 *
 * Created on 12 luty 2013, 14:15
 */

#ifndef MDLFILEMANAGER_H
#define	MDLFILEMANAGER_H

#include <OGRE/OgreResourceManager.h>
#include <OGRE/OgreSingleton.h>
#include "Mdl.h"

 class MdlManager : public Ogre::ResourceManager, public Ogre::Singleton<MdlManager>
 {
 protected:
 
     // must implement this from ResourceManager's interface
     Ogre::Resource *createImpl(const Ogre::String &name, Ogre::ResourceHandle handle, 
         const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader, 
         const Ogre::NameValuePairList *createParams);
 
 public:
 
     MdlManager();
     virtual ~MdlManager();
 
     virtual MdlPtr load(const Ogre::String &name, const Ogre::String &group);
 
     static MdlManager &getSingleton();
     static MdlManager *getSingletonPtr();
 };

#endif	/* MDLFILEMANAGER_H */

