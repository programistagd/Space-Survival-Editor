/* 
 * File:   Mdl.h
 * Author: radek
 *
 * Created on 12 luty 2013, 14:07
 */

#ifndef MDL_H
#define	MDL_H

#include <OGRE/OgreResourceManager.h>
#include <OGRE/OgreMesh.h> 

 class Mdl : public Ogre::Resource
 {
     
 protected:
 
     // must implement these from the Ogre::Resource interface
     void loadImpl();
     void unloadImpl();
     size_t calculateSize() const;
     
 public:
     Ogre::MeshPtr mesh;
     Mdl(Ogre::ResourceManager *creator, const Ogre::String &name, 
         Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual = false, 
         Ogre::ManualResourceLoader *loader = 0);
 
     virtual ~Mdl();
 };
 
 class MdlPtr : public Ogre::SharedPtr<Mdl> 
 {
 public:
     MdlPtr() : Ogre::SharedPtr<Mdl>() {}
     explicit MdlPtr(Mdl *rep) : Ogre::SharedPtr<Mdl>(rep) {}
     MdlPtr(const MdlPtr &r) : Ogre::SharedPtr<Mdl>(r) {} 
     MdlPtr(const Ogre::ResourcePtr &r) : Ogre::SharedPtr<Mdl>()
     {
         if( r.isNull() )
             return;
         // lock & copy other mutex pointer
         OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
         OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
         pRep = static_cast<Mdl*>(r.getPointer());
         pUseCount = r.useCountPointer();
         useFreeMethod = r.freeMethod();
         if (pUseCount)
         {
             ++(*pUseCount);
         }
     }
 
     /// Operator used to convert a ResourcePtr to a MdlPtr
     MdlPtr& operator=(const Ogre::ResourcePtr& r)
     {
         if(pRep == static_cast<Mdl*>(r.getPointer()))
             return *this;
         release();
         if( r.isNull() )
             return *this; // resource ptr is null, so the call to release above has done all we need to do.
         // lock & copy other mutex pointer
         OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
         OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
         pRep = static_cast<Mdl*>(r.getPointer());
         pUseCount = r.useCountPointer();
         useFreeMethod = r.freeMethod();
         if (pUseCount)
         {
             ++(*pUseCount);
         }
         return *this;
     }
 };

#endif	/* MDL_H */

