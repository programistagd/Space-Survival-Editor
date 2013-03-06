/* 
 * File:   MdlSerializer.h
 * Author: radek
 *
 * Created on 12 luty 2013, 14:13
 */

#ifndef MDLSERIALIZER_H
#define	MDLSERIALIZER_H

#include <OGRE/OgreSerializer.h>

#define SHARED_GEOMETRY //TODO?

class Mdl; // forward declaration
 
 class MdlSerializer : public Ogre::Serializer
 {
 public:
     MdlSerializer();
     virtual ~MdlSerializer();
     
     void exportMdl(const Mdl *mdl, const Ogre::String &fileName);
     void importMdl(Ogre::DataStreamPtr &stream, Mdl *pDest);
 };

#endif	/* MDLSERIALIZER_H */

