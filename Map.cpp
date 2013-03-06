/* 
 * File:   Map.cpp
 * Author: radek
 * 
 * Created on 1 luty 2013, 21:42
 */

#include "Map.h"
#include "OgreFramework.h"
#include <set>
#include <fstream>

bool Map::initialised = false;
std::vector<Triangle> Map::tris[5];

void generateTris(int size,std::vector<Triangle> &tris){
    for(int xx=0;xx<size;xx++){
            for(int yy=0;yy<size;yy++){
                int n=xx+yy*(size+1);
                Triangle t1,t2;//todo clockwise or counter?
                t1.v1=n;
                t1.v2=n+1;
                t1.v3=n+size+1;
                t2.v1=n+size+2;
                t2.v2=n+size+1;
                t2.v3=n+1;
                tris.push_back(t1);
                tris.push_back(t2);
            }
        }
}

Map::Map(int x,int y,float height=1.0f) {
    init(x,y,height);
}

void Map::init(int x,int y, float height=0.0f){
    if(!initialised){
        generateTris(128,tris[0]);
        generateTris(64,tris[1]);
        generateTris(32,tris[2]);
        generateTris(16,tris[3]);
        generateTris(8,tris[4]);
        initialised=true;
    }
    map = new float*[x*128+1];
    for(int i=0;i<x*128+1;i++){
        map[i]=new float[y*128+1];
        for(int j=0;j<y*128+1;j++){
            map[i][j]=height;
        }
    }
    map_materials = new int*[x*128+1];
    for(int i=0;i<x*128+1;i++){
        map_materials[i]=new int[y*128+1];
        for(int j=0;j<y*128+1;j++){
            map_materials[i][j]=0;
        }
    }
    selected = new bool*[x*128+1];
    for(int i=0;i<x*128+1;i++){
        selected[i]=new bool[y*128+1];
        for(int j=0;j<y*128+1;j++){
            selected[i][j]=false;
        }
    }
    selectedB = new float*[x*128+1];
    for(int i=0;i<x*128+1;i++){
        selectedB[i]=new float[y*128+1];
        for(int j=0;j<y*128+1;j++){
            selectedB[i][j]=0.0;
        }
    }
    
    this->x=x;
    this->y=y;
    for(int i=0;i<5;i++){
        fresh[i] = new bool*[x];
        for(int xx=0;xx<x;xx++){
            fresh[i][xx]=new bool[y];
            for(int yy=0;yy<y;yy++){
                fresh[i][xx][yy]=false;
            }
        }
    }
    smallestX=this->x*128+10;
    smallestY=this->y*128+10;
    largestX=0;
    largestY=0;
}

std::string Map::getPos(int x, int y){
    std::stringstream ss;
    ss<<x<<"-"<<y;
    return ss.str();
}

std::string Map::getMaterialById(int material){
    switch(material){
        case -1:
            return "Nothing (for caves)";
        case 0:
            return "Stone";
        case 1:
            return "Dirt Snowy";
        case 2:
            return "Snow";
        case 3:
            return "Ice";
        case 4:
            return "Grass Wet";
        case 5:
            return "Grass Dry";
        case 6:
            return "Dirt";
        case 7:
            return "Sand";            
        case 8:
            return "SandStone";
        case 9:
            return "Clay";
        default:
            return "Unknown";
    }
}

Map::~Map() {
    for(int i=0;i<x*128+1;i++){
        delete [] map[i];
    }
    delete [] map;
    for(int i=0;i<x*128+1;i++){
        delete [] map_materials[i];
    }
    delete [] map_materials;
}

Ogre::SceneNode* Map::getChunk(int x,int y, int z){
    if(x<0 or y<0 or x>=(this->x) or y>=(this->y))
        return NULL;
    std::vector<Vertex> verts;
    std::vector<Triangle> tris2=tris[z];
    int n=pow(2,z);
    int x128=x*128,y128=y*128;
    /*for(int xx=x128;xx<x128+129;xx++){
        for(int yy=y128;yy<y128+129;yy++){
            Vertex v;
            v.x=xx-x128;
            v.z=yy-y128;
            v.y=map[xx][yy];
            v.material=map_materials[xx][yy];
            v.nx=0;
            v.ny=0;
            v.nz=0;
            verts.push_back(v);
        }
    }*/
    for(int xx=x128;xx<x128+129;xx+=n){
        for(int yy=y128;yy<y128+129;yy+=n){
            Vertex v;
            v.x=xx-x128;
            v.z=yy-y128;
            double h=0;
            for(int dx=xx;dx<xx+n;dx++){
                for(int dy=yy;dy<yy+n;dy++){
                    h+=map[dx][dy];
                }
            }
            v.y=h/(n*n);
            int mat=0;
            if(n>1){
                std::multiset<int> mats;
                for(int dx=xx;dx<xx+n;dx++){
                    for(int dy=yy;dy<yy+n;dy++){
                        mats.insert(map_materials[dx][dy]);
                    }
                }
                int max=0;
                for(std::multiset<int>::iterator i=mats.begin();i!=mats.end();i++){
                    if(mats.count(*i)>max){
                        mat=*i;
                    }
                }
            }
            else
                mat=map_materials[xx][yy];
            v.material=mat;
            v.nx=0;
            v.ny=0;
            v.nz=0;
            verts.push_back(v);
        }
    }
    for(int i=0;i<tris2.size();i++){//TODO when counting normals take care of nearby vertices +1 (seamless)
        glm::vec3 v1=glm::vec3(verts[tris2[i].v1].x,verts[tris2[i].v1].y,verts[tris2[i].v1].z);
        glm::vec3 v2=glm::vec3(verts[tris2[i].v2].x,verts[tris2[i].v2].y,verts[tris2[i].v2].z);
        glm::vec3 v3=glm::vec3(verts[tris2[i].v3].x,verts[tris2[i].v3].y,verts[tris2[i].v3].z);
        glm::vec3 a=v3-v2,b=v1-v2;
        glm::vec3 normal = glm::normalize(glm::cross(a,b));

        tris2[i].nx=normal.x;
        tris2[i].ny=normal.y;
        tris2[i].nz=normal.z;
        verts[tris2[i].v1].nx+=normal.x;
        verts[tris2[i].v1].ny+=normal.y;
        verts[tris2[i].v1].nz+=normal.z;
        verts[tris2[i].v2].nx+=normal.x;
        verts[tris2[i].v2].ny+=normal.y;
        verts[tris2[i].v2].nz+=normal.z;
        verts[tris2[i].v3].nx+=normal.x;
        verts[tris2[i].v3].ny+=normal.y;
        verts[tris2[i].v3].nz+=normal.z;
    }
    for(int i=0;i<verts.size();i++){
        glm::vec3 n = glm::normalize(glm::vec3(verts[i].nx,verts[i].ny,verts[i].nz));
        verts[i].nx=n.x;
        verts[i].ny=n.y;
        verts[i].nz=n.z;
    }
        //add vertexes, no VBO but in game its needed
    
        Ogre::SceneNode* ogreNode;

        try{
            ogreNode =  OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode(std::string("NChunk")+getPos(x,y));
            OgreFramework::getSingletonPtr()->m_pSceneMgr->destroyManualObject(std::string("Chunk")+getPos(x,y));
        }
        catch(...){
            ogreNode =  OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode(std::string("NChunk")+getPos(x,y), Ogre::Vector3(0, 0, 0));
        }
        Ogre::ManualObject* ogreMesh=OgreFramework::getSingletonPtr()->m_pSceneMgr->createManualObject(std::string("Chunk")+getPos(x,y));;
        ogreMesh->setDynamic(true);
        ogreMesh->begin("MultiTextureEditor", Ogre::RenderOperation::OT_TRIANGLE_LIST);//ogreMesh->begin("MultiTexture", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    {
        //std::cout<<"Triangles: "<<tris2.size()<<"\n";
        for(int i=0;i<verts.size();i++){
            ogreMesh->position(Ogre::Vector3(verts[i].x,verts[i].y,verts[i].z));
            ogreMesh->normal(Ogre::Vector3(verts[i].nx,verts[i].ny,verts[i].nz));
            if(verts[i].material<0)
                ogreMesh->colour(Ogre::ColourValue(0.0,0.0,0.0,0.0));
            else
                ogreMesh->colour(Ogre::ColourValue(((float)verts[i].material+1.0f)/100.0f,1.0,0.0,0.0));
        }
    for(int i=0;i<tris2.size();i++){
        ogreMesh->index(tris2[i].v1);
        ogreMesh->index(tris2[i].v2);
        ogreMesh->index(tris2[i].v3);
            /*ogreMesh->position(Ogre::Vector3(verts[tris2[i].v1].x,verts[tris2[i].v1].y,verts[tris2[i].v1].z));
            ogreMesh->normal(Ogre::Vector3(verts[tris2[i].v1].nx,verts[tris2[i].v1].ny,verts[tris2[i].v1].nz));
            ogreMesh->colour(Ogre::ColourValue(verts[tris2[i].v1].material/12.0,1.0,0.0,0.0));
            ogreMesh->position(Ogre::Vector3(verts[tris2[i].v2].x,verts[tris2[i].v2].y,verts[tris2[i].v2].z));
            ogreMesh->normal(Ogre::Vector3(verts[tris2[i].v2].nx,verts[tris2[i].v2].ny,verts[tris2[i].v2].nz));
            ogreMesh->colour(Ogre::ColourValue(verts[tris2[i].v2].material/12.0,1.0,0.0,0.0));
            ogreMesh->position(Ogre::Vector3(verts[tris2[i].v2].x,verts[tris2[i].v2].y,verts[tris2[i].v2].z));
            ogreMesh->normal(Ogre::Vector3(verts[tris2[i].v2].nx,verts[tris2[i].v2].ny,verts[tris2[i].v2].nz));
            ogreMesh->colour(Ogre::ColourValue(verts[tris2[i].v3].material/12.0,1.0,0.0,0.0));*/
        }
    }
        ogreMesh->end();
        ogreNode->setVisible(true);
        ogreNode->attachObject(ogreMesh);
        ogreNode->setPosition(x*128,0,y*128);
        //ogreNode->attachObject(OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("athene.mesh"));
        for(int i=0;i<5;i++){
            if(i==z)
                fresh[i][x][y]=true;
            else
                fresh[i][x][y]=false;
        }
    return ogreNode;
}

void Map::select(int x,int y, float radius,float radius_blend){
    if(x<0 or y<0 or x>this->x*128 or y>this->y*128)
        return;
    float b=radius_blend;
    radius_blend=radius+radius_blend;
    int x0=x-radius_blend,y0=y-radius_blend;
    if(x0<0) x0=0;
    if(y0<0) y0=0;
    float r2=radius*radius, rB2=radius_blend*radius_blend;
    for(int xx=x0;xx<x+radius_blend and xx<this->x*128;xx++){
        for(int yy=y0;yy<y+radius_blend and yy<this->y*128;yy++){
            float dx=xx-x;
            float dy=yy-y;
            float d=dx*dx+dy*dy;
            if(d<=r2){
                selected[xx][yy]=true;
            }
            else if(d<=rB2){
                float x=1.0-((sqrt(d)-sqrt(r2))/b);
                if(x>selectedB[xx][yy])
                    selectedB[xx][yy]=x;
            }
            
        }
    }
    if(x0<smallestX)
        smallestX=x0;
    if(y0<smallestY)
        smallestY=y0;
    if(x+radius_blend>largestX)
        largestX=x+radius_blend;
    if(y+radius_blend>largestY)
        largestY=y+radius_blend;
}

void Map::brush(float height, bool relative=true){
    if(smallestX>=largestX || smallestY>=largestY)
        return;
    for(int xx=smallestX;xx<largestX and xx<this->x*128;xx++){
        for(int yy=smallestY;yy<largestY and yy<this->y*128;yy++){
            if(selected[xx][yy]){
                if(relative){
                    map[xx][yy]+=height;
                }
                else{
                    map[xx][yy]=height;
                }
                selected[xx][yy]=false;
                selectedB[xx][yy]=0.0;
            }
            else if(selectedB[xx][yy]>0.0){
                if(relative){
                    map[xx][yy]+=height*selectedB[xx][yy];
                }
                else{
                    map[xx][yy]=map[xx][yy]*(1.0-selectedB[xx][yy])+height*selectedB[xx][yy];
                }
                selectedB[xx][yy]=0.0;
            }
        }
    }
    for(int xx=smallestX/128;xx<=largestX/128 and xx<this->x;xx++){
        for(int yy=smallestY/128;yy<=largestY/128 and yy<this->y;yy++){
            fresh[0][xx][yy]=false;
            fresh[1][xx][yy]=false;
            fresh[2][xx][yy]=false;
            fresh[3][xx][yy]=false;
            fresh[4][xx][yy]=false;
        }
    }
    smallestX=this->x*128+10;
    smallestY=this->y*128+10;
    largestX=0;
    largestY=0;
}

void Map::brush(int material){
    if(smallestX>=largestX || smallestY>=largestY)
        return;
    for(int xx=smallestX;xx<largestX and xx<this->x*128;xx++){
        for(int yy=smallestY;yy<largestY and yy<this->y*128;yy++){
            if(selected[xx][yy]){
                map_materials[xx][yy]=material;
                selected[xx][yy]=false;
                selectedB[xx][yy]=0.0;
            }
        }
    }
    for(int xx=smallestX/128;xx<=largestX/128 and xx<this->x;xx++){
        for(int yy=smallestY/128;yy<=largestY/128 and yy<this->y;yy++){
            fresh[0][xx][yy]=false;
            fresh[1][xx][yy]=false;
            fresh[2][xx][yy]=false;
            fresh[3][xx][yy]=false;
            fresh[4][xx][yy]=false;
        }
    }
    smallestX=this->x*128+10;
    smallestY=this->y*128+10;
    largestX=0;
    largestY=0;
}

void Map::deselect(){
    if(smallestX>=largestX || smallestY>=largestY)
        return;
    for(int xx=smallestX;xx<largestX and xx<this->x*128;xx++){
        for(int yy=smallestY;yy<largestY and yy<this->y*128;yy++){
                selected[xx][yy]=false;
                selectedB[xx][yy]=0.0;
        }
    }
}

/*Pos* Map::getNextUpdatedChunk(){
    if(chunks_q.empty())
        return NULL;
    Pos* pos = chunks_q.front();
    chunks_q.pop();
    return pos;
}*/

int Map::save(const char* filename){
    OgreFramework::getSingletonPtr()->caption->setCaption("Saving...");
    OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
    std::ofstream out;
    out.open(filename,std::ios::out|std::ios::trunc|std::ios::binary);
    if(!out.is_open())
        return -1;
    //w*h|h+mat
    
    out.write((char*)&x,sizeof(unsigned int));
    out.write((char*)&y,sizeof(unsigned int));
    for(int i=0;i<x*128;i++){
        for(int j=0;j<y*128;j++){
            out.write((char*)&map[i][j],sizeof(float));
            out.write((char*)&map_materials[i][j],sizeof(int));
        }
    }
    out.flush();
    out.close();
    return 1;
}

int Map::load(const char* filename){
    OgreFramework::getSingletonPtr()->caption->setCaption("Loading...");
    OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
    std::ifstream in;
    in.open(filename,std::ios::in|std::ios::binary);
    if(!in.is_open())
        return -1;
    //w*h|h+mat
    in.read((char*)&x,sizeof(unsigned int));
    in.read((char*)&y,sizeof(unsigned int));
    init(x,y);
    for(int i=0;i<x*128;i++){
        for(int j=0;j<y*128;j++){
            in.read((char*)&map[i][j],sizeof(float));
            in.read((char*)&map_materials[i][j],sizeof(int));
        }
    }
    in.close();
    return 1;
}