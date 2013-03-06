/* 
 * File:   Mdl.cpp
 * Author: radek
 * 
 * Created on 12 luty 2013, 14:07
 */

#include <OGRE/OgreMesh.h>

#include "Mdl.h"
#include "MdlSerializer.h"
 
 Mdl::Mdl(Ogre::ResourceManager* creator, const Ogre::String &name, 
                     Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual, 
                     Ogre::ManualResourceLoader *loader) :
 Ogre::Resource(creator, name, handle, group, isManual, loader)
 {
     /* If you were storing a pointer to an object, then you would set that pointer to NULL here.
     */
 
     /* For consistency with StringInterface, but we don't add any parameters here
     That's because the Resource implementation of StringInterface is to
     list all the options that need to be set before loading, of which 
     we have none as such. Full details can be set through scripts.
     */ 
     createParamDictionary("Mdl");
 }
 
 Mdl::~Mdl()
 {
     unload();
 }
 
 // farm out to MdlSerializer
 void Mdl::loadImpl()
 {
     MdlSerializer serializer;
     Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(mName, mGroup, true, this);
     serializer.importMdl(stream, this);
 }
 
 void Mdl::unloadImpl()
 {
     /* If you were storing a pointer to an object, then you would check the pointer here,
     and if it is not NULL, you would destruct the object and set its pointer to NULL again.
     */
 
 }
 
 size_t Mdl::calculateSize() const
 {
     return mesh->getSize();
 }