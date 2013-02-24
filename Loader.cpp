/* 
 * File:   SPLoader.cpp
 * Author: radek
 * 
 * Created on 24 listopad 2012, 19:10
 */

#include "Loader.h"
#include "Utils.h"
//#include "TerrainGenerator.h"
#include <string>
#include <OGRE/OgreManualObject.h>
#include "OgreFramework.h"
/*#include "PolyVoxCore/Region.h"
#include "PolyVoxCore/SimpleVolume.h"
#include "PolyVoxCore/MaterialDensityPair.h"*/
#include <math.h>
#include <boost/thread.hpp>

Loader::Loader() {
    /*assert(!generationThread);
    generationThread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(runGenerates)));*/
    
}

Loader::~Loader() {
}

/*Ogre::Vector3 Loader::getSunPos(){
    //return Ogre::Vector3(660,800,660);
    return Ogre::Vector3(100,200,330);
}*/

void Loader::addChunk(int planetId, char side, int x, int y,int z){//request chunk and then add it
    //TODO chunk cache
   // Network::getNetwork()->requestChunk(x,y,z);
    //PlanetInfo planet = getPlanetInfo(planetId);
    //TerrainGenerator gen = TerrainGenerator::getGeneratorForSeed(planet.seed);//TODO get data from file or cache
    //PolyVox::SimpleVolume<PolyVox::MaterialDensityPair44> *volume = gen.getChunk(x,y,side);
    //TerrainGenerator::regionToMesh(volume,planet.sideSize,std::string()+side+x+y);
    //register it or so?
}

/*PlanetInfo Loader::getPlanetInfo(int planetId){
    PlanetInfo pi;
    pi.seed=planetId;
    pi.sideSize = 256;
    return pi;
}*/

/*std::vector<RenderableObject> Loader::getObjects(Position pos){
    //TODO getobjects
}*/
Loader* Loader::loader;
Loader* Loader::getLoader(){
    if(Loader::loader==NULL){
        Loader::loader=new Loader();
    }
    return Loader::loader;
}
/*
class ChunkGenerator : public Runnable {
public:
    ChunkGenerator(int x, int y,int z, int seed):Runnable(){
        xx=x;
        zz=z;
        yy=y;
        sseed=seed;
    }
    int xx,yy,zz,sseed;
    
    void run(){
        TerrainGenerator gen = TerrainGenerator::getGeneratorForSeed(sseed);//TODO get data from file or cache
        PolyVox::SimpleVolume<PolyVox::MaterialDensityPair44> *volume = gen.getChunk(xx,yy,zz,0);
        Ogre::SceneNode* node = TerrainGenerator::regionToMesh(volume,0,std::string("0")+(char)xx+(char)yy+(char)zz);
        node->setPosition(xx*CHUNK_SIZE,zz*CHUNK_SIZE,yy*CHUNK_SIZE);
        delete this;
    }
    
};*/

/*void generateChunk(int xx,int yy,int zz, int sseed){
        //std::cout<<"\nGeneration started\n\n";
        TerrainGenerator gen = TerrainGenerator::getGeneratorForSeed(sseed);//TODO get data from file or cache
        PolyVox::SimpleVolume<PolyVox::MaterialDensityPair44> *volume = gen.getChunk(xx,yy,zz,0);
        TerrainGenerator::regionToMesh(volume,0,xx,yy,zz);
        delete volume;
        //boost::unique_lock<boost::mutex> lock(OgreFramework::getSingletonPtr()->mutex);
        //node->setPosition(xx*CHUNK_SIZE,zz*(CHUNK_SIZE),yy*CHUNK_SIZE);
        //std::cout<<OgreFramework::getSingletonPtr()->m_pSceneMgr->getManualObject(std::string("0")+(char)xx+(char)yy+(char)zz)<<"\n\n\n\n";
        
        //std::cout<<"\nGeneration completed\t"<<xx<<" "<<yy<<" "<<zz<<"\n";
    }
*/
#include <queue>
#include <Poco/Any.h>

struct ChunkGenQueueElement{
    int x,y,z,seed;
};

boost::mutex chunkpool_mutex;
std::queue<ChunkGenQueueElement> chunkpool_queue;
/*
void runGenerates(){
    std::cout<<"Waiting for tasks(chunky)\n";
    while(true){
        //std::cout<<"imma alive!\n";
        try{
        boost::unique_lock<boost::mutex> lock(chunkpool_mutex);
        if(chunkpool_queue.size()>0){
        std::queue<ChunkGenQueueElement> myqueue;
        std::swap(myqueue,chunkpool_queue);
        chunkpool_queue = std::queue<ChunkGenQueueElement>();
        lock.unlock();
        while(myqueue.size()>0){
            ChunkGenQueueElement e = myqueue.front();
            myqueue.pop();
            try{
            generateChunk(e.x,e.y,e.z,e.seed);
            std::cout<<"generated "<<myqueue.size()+1<<"\n";
            }
            catch(...){
                std::cout<<"Somethin wrong happened in generation process\n";
            }

        }
                    Ogre::SceneNode::ChildNodeIterator it = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->getChildIterator();
             //       std::cout<<"\n\n\n\nList of all elements:\n\n";
           //         while (it.hasMoreElements())
         //           {
       //             std::cout<<"\""<<it.getNext()->getName()<<"\"\n";
        
                    //}
        }else{
            lock.unlock();
            //sleep(1);
        }
        
        }catch(...){
            std::cout<<"Something wrong happened in the thread\n";
        }
    }
}

void Loader::addChunkGen(int x, int y,int z, int seed){
    //th = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(generateChunk, x,y,z,seed)));
    //boost::thread thread(generateChunk,x,y,z,seed);//TODO threading - y u no work?
    //generateChunk(x,y,z,seed);
    ChunkGenQueueElement e;
    e.x=x;
    e.y=y;
    e.z=z;
    e.seed=seed;
    boost::unique_lock<boost::mutex> lock(chunkpool_mutex);
    chunkpool_queue.push(e);
}
*/
Ogre::Vector3 Loader::getPlayerPos(){
    //TODO
    
}

Ogre::MeshPtr Loader::getMeshFromMdl(unsigned int id){
    if(!models[id].isNull()){
        return models[id];
    }
    std::string name = "mesh"+Utils::toStr(id);
    std::ifstream stream;
    stream.open(("Models/"+name+".mdl").c_str(),std::ios::in|std::ios::binary);
    
    Ogre::ManualObject* obj = OgreFramework::getSingletonPtr()->m_pSceneMgr->createManualObject();
     unsigned int texture, normalmap, vertices, indices;
     short endian;
     stream.read((char*)&endian,sizeof(short));
     stream.read((char*)&texture,sizeof(unsigned int));
     stream.read((char*)&normalmap,sizeof(unsigned int));
     stream.read((char*)&vertices,sizeof(unsigned int));
     stream.read((char*)&indices,sizeof(unsigned int));
     float bounds[6];//{minX,minY,minZ,maxX,maxY,maxZ}
     stream.read((char*)bounds,sizeof(float)*6);
     std::string textureFName,normalMapFName;
     for(int i=0;i<texture;i++){
         char c;
         stream.read((char*)&c,sizeof(char));
         textureFName+=c;
     }
     if(normalmap>0){
        for(int i=0;i<texture;i++){
            char c;
            stream.read((char*)&c,sizeof(char));
            normalMapFName+=c;
        }
     }
     if(normalmap>0)
        obj->begin("NormalMapTexture");
     else
        obj->begin("PlainTexture");
    float * arrayV = new float[vertices*(3+3+2)];
    stream.read((char*)arrayV,sizeof(float)*(3+3+2)*vertices);//should also work - automatically grab ALL vertex data to array in one operation :) C++ is great
    
    for(int i=0;i<vertices;i++){
        int ptr = i*(3+3+2);
        obj->position(arrayV[ptr],arrayV[ptr+1],arrayV[ptr+2]);
        obj->normal(arrayV[ptr+3],arrayV[ptr+4],arrayV[ptr+5]);
        obj->textureCoord(arrayV[ptr+6],arrayV[ptr+7]);
    }
    
    delete [] arrayV;
    unsigned int * arrayI = new unsigned int[indices];
    stream.read((char*)arrayI,sizeof(unsigned int)*indices);
    
    for(int i=0;i<indices;i++){
        obj->index(arrayI[i]);
    }
    
    delete [] arrayI;
    
    obj->end();
    /*if(normalmap>0){
        obj->setMaterialName("NormalMapTexture");
        obj->addTextureAlias("normalmap",normalMapFName);
    }
    else{
        obj->setMaterialName("OneTexture");
    }
    obj->addTextureAlias("textura",textureFName);*/
    OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(obj);
    Ogre::MeshPtr mesh = obj->convertToMesh(name);
    mesh->getSubMesh(0)->addTextureAlias("textura",textureFName);//TODO normalmap
    return mesh;
    
    /*Ogre::MeshPtr meshM = Ogre::MeshManager::getSingletonPtr()->createManual(name,"General");
     unsigned int texture, normalmap, vertices, indices;
     short endian;
     stream.read((char*)&endian,sizeof(short));
     stream.read((char*)&texture,sizeof(unsigned int));
     stream.read((char*)&normalmap,sizeof(unsigned int));
     stream.read((char*)&vertices,sizeof(unsigned int));
     stream.read((char*)&indices,sizeof(unsigned int));
     float bounds[6];//{minX,minY,minZ,maxX,maxY,maxZ}
     stream.read((char*)bounds,sizeof(float)*6);
     std::string textureFName,normalMapFName;
     for(int i=0;i<texture;i++){
         char c;
         stream.read((char*)&c,sizeof(char));
         textureFName+=c;
     }
     if(normalmap>0){
        for(int i=0;i<texture;i++){
            char c;
            stream.read((char*)&c,sizeof(char));
            normalMapFName+=c;
        }
     }
     Ogre::SubMesh* mesh = meshM->createSubMesh();
        // We first create a VertexData
     Ogre::VertexData* data = new Ogre::VertexData();
    // Then, we link it to our Mesh/SubMesh :
     #ifdef SHARED_GEOMETRY
        meshM->sharedVertexData = data;
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

    stream.read((char*)arrayV,sizeof(float)*(3+3+2)*vertices);//should also work - automatically grab ALL vertex data to array in one operation :) C++ is great
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
    stream.read((char*)arrayI,sizeof(unsigned int)*indices);
    ibuf->writeData(0, ibuf->getSizeInBytes(), arrayI, true);
    delete [] arrayI;
    if(normalmap>0){
        mesh->setMaterialName("NormalMapTexture");
        mesh->addTextureAlias("normalmap",normalMapFName);
    }
    else{
        //mesh->setMaterialName("OneTexture");
    }
    mesh->addTextureAlias("textura",textureFName);
    meshM->_setBounds(Ogre::AxisAlignedBox(bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]));
    meshM->_setBoundingSphereRadius(std::max(bounds[3]-bounds[0], std::max(bounds[4]-bounds[1], bounds[5]-bounds[2]))/2.0f);
    meshM->load();//TODO need?
    return meshM;*/
}