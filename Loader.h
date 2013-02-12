/* 
 * File:   SPLoader.h
 * Author: radek
 *
 * Created on 24 listopad 2012, 19:10
 */

#ifndef SPLOADER_H
#define	SPLOADER_H
#include "Loader.h"
#include <OGRE/OgreVector3.h>
#include <vector>
#include "Position.h"

struct GeneratorInfo{
    float seed;
    //TOOD add other options
};

void runGenerates();

class Loader{
    //friend class Network;
public:
    static Loader* getLoader();
    virtual ~Loader();
    //std::vector<RenderableObject> getObjects(Position pos);
    void addChunk(int planetId,char side, int x,int y,int z);
  //  void setScene(GeneratorInfo info);
    void setScene(int mapId);
    void updatePlayerPos(Ogre::Vector3 pos);
    Ogre::Vector3 getPlayerPos();
  //  Ogre::Vector3 getSunPos();
   // static PlanetInfo getPlanetInfo(int planetId);
    void addChunkGen(int x,int y, int z,int seed);
private:
    
    void loadHeightMap(int x,int y,int lod);
    int mapId;
    boost::shared_ptr<boost::thread> generationThread;
    Loader();
    static Loader* loader;
};

#endif	/* SPLOADER_H */

