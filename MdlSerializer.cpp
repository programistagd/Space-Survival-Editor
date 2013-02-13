/* 
 * File:   MdlSerializer.cpp
 * Author: radek
 * 
 * Created on 12 luty 2013, 14:13
 */

#include <OGRE/OgreMesh.h>
#include <OGRE/OgreMeshManager.h>
#include <OGRE/OgreHardwareBufferManager.h>
#include "Mdl.h"
#include "MdlSerializer.h"
#include <OGRE/OgreSubMesh.h>

 MdlSerializer::MdlSerializer()
 {
 }
 
 MdlSerializer::~MdlSerializer()
 {
 }
 
 void MdlSerializer::exportMdl(const Mdl *pText, const Ogre::String &fileName)
 {
     //TODO
     throw -1;
 }

 void MdlSerializer::importMdl(Ogre::DataStreamPtr &stream, Mdl *pDest)
 {
     pDest->mesh = Ogre::MeshManager::getSingletonPtr()->createManual(stream->getName(),"General");
     unsigned int texture, vertices, indices;
     stream->read(&texture,sizeof(unsigned int));
     stream->read(&vertices,sizeof(unsigned int));
     stream->read(&indices,sizeof(unsigned int));
     float bounds[6];//{minX,minY,minZ,maxX,maxY,maxZ}
     stream->read(bounds,sizeof(float)*6);
     std::string textureFName;
     for(int i=0;i<texture;i++){
         char c;
         stream->read(&c,sizeof(char));
         textureFName+=c;
     }
     Ogre::SubMesh* mesh = pDest->mesh->createSubMesh();
        // We first create a VertexData
     Ogre::VertexData* data = new Ogre::VertexData();
    // Then, we link it to our Mesh/SubMesh :
     #ifdef SHARED_GEOMETRY
        pDest->mesh->sharedVertexData = data;
     #else
        mesh->useSharedVertices = false; // This value is 'true' by default
        mesh->vertexData = data;
     #endif
    // We have to provide the number of verteices we'll put into this Mesh/SubMesh
     data->vertexCount = vertices;
    // Then we can create our VertexDeclaration
     Ogre::VertexDeclaration* decl = data->vertexDeclaration;
     size_t offset = 0;
    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
    decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);//you can't animate because one buffer
    Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        decl->getVertexSize(0),                     // This value is the size of a vertex in memory
        vertices,                                 // The number of vertices you'll put into this buffer
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY // Properties
    );
    float * arrayV = new float[vertices*(3+3+2)];
    /*for(int i=0;i<vertices;i++){
        int aPos = i*(3+3+2);
        stream->read(&arrayV[aPos],sizeof(float)*(3+3+2));//theoretically it should load all floats at once; 3POS, 3NORMAL, 2UV
    }*/
    stream->read(arrayV,sizeof(float)*(3+3+2)*vertices);//should also work - automatically grab ALL vertex data to array in one operation :) C++ is great
    vbuf->writeData(0, vbuf->getSizeInBytes(), arrayV, true);
    delete [] arrayV;
    // "data" is the Ogre::VertexData* we created before
    Ogre::VertexBufferBinding* bind = data->vertexBufferBinding;
    bind->setBinding(0, vbuf);
    Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
        Ogre::HardwareIndexBuffer::IT_16BIT,        // You can use several different value types here
        indices,                                  // The number of indices you'll put in that buffer
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY // Properties
    );
    mesh->indexData->indexBuffer = ibuf;     // The pointer to the index buffer
    mesh->indexData->indexCount = indices; // The number of indices we'll use
    mesh->indexData->indexStart = 0;
    unsigned int * arrayI = new unsigned int[indices];
    stream->read(arrayI,sizeof(unsigned int)*indices);
    ibuf->writeData(0, ibuf->getSizeInBytes(), arrayI, true);
    delete [] arrayI;
    pDest->mesh->_setBounds(Ogre::AxisAlignedBox(bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]));
    pDest->mesh->_setBoundingSphereRadius(std::max(bounds[3]-bounds[0], std::max(bounds[4]-bounds[1], bounds[5]-bounds[2]))/2.0f);
    pDest->mesh->load();//TODO need?
 }