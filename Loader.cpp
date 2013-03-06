/* 
 * File:   SPLoader.cpp
 * Author: radek
 * 
 * Created on 24 listopad 2012, 19:10
 */

#include "Loader.h"
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