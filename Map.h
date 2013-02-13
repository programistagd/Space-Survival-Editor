/* 
 * File:   Map.h
 * Author: radek
 *
 * Created on 1 luty 2013, 21:42
 */

#ifndef MAP_H
#define	MAP_H

#include <vector>
#include <queue>
#include <glm/glm.hpp>
#include <OGRE/OgreSceneNode.h>

struct Pos{
    int x,y;
};

struct Vertex{
    float x,y,z;
    int material;
    float nx,ny,nz;//normals
};

struct Triangle{//indices
    int v1,v2,v3;
    float nx,ny,nz;//normals
};

class Map {
public:
    static std::string getPos(int x,int y);
    static std::string getMaterialById(int material);
    
    Map(int x,int y,float height);
    void init(int x,int y,float height);
    virtual ~Map();
    Ogre::SceneNode* getChunk(int x,int y, int z);
    float ** map;
    int ** map_materials;//TODO
    bool ** fresh[5];
    
    void brush(int material);
    void brush(float height, bool relative);//void brush(int x, int y, float height, float radius, float blend_border, bool relative);
    void select(int x,int y, float radius, float radius_blend);
    void deselect();
    
    int save(const char* filename);
    int load(const char* filename);
    
    unsigned int x,y;
    static std::vector<Triangle> tris[5];
private:
    int smallestX,smallestY,largestX,largestY;//select
    bool **selected;
    float **selectedB;
    static bool initialised;
};

#endif	/* MAP_H */

