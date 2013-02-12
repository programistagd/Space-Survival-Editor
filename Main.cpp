/* 
 * File:   Main.cpp
 * Author: radek
 *
 * Created on 3 październik 2012, 21:17
 */
#include "OgreFramework.h"
#include <OGRE/OgreSceneNode.h>
#include <iostream>
#include <vector>
#include "Loader.h"
#include "Map.h"

#include <OGRE/OgreCompositorManager.h>

#include <boost/thread.hpp>
#include <OGRE/OgreMaterialManager.h>
//

class App : public OIS::KeyListener
{
public:
	App(){};
	~App(){
            boost::unique_lock<boost::mutex> lock(OgreFramework::getSingletonPtr()->mutex);
                delete OgreFramework::getSingletonPtr();
                lock.unlock();
                }
 
	void start(){
            std::cout<<"starting\n\n\n";
            new OgreFramework();
	if(!OgreFramework::getSingletonPtr()->initOgre("SpaceSurvival Engine Pre-Alpha v0.1", this, 0))
		return;
 std::cout<<"init\n\n\n";
 
        OgreFramework::getSingletonPtr()->m_pLog->logMessage("Initialized!");
        //OgreFramework::getSingletonPtr()->m_pRoot->loadPlugin("Plugin_CgProgramManager");//enable shaders
	setupScene();
	run();
        }
 
        bool keyPressed(const OIS::KeyEvent &keyEventRef) //TODO events interface or what?
        {
                OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

                if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_P))
                {
                    std::cout<<"Loading\n";
                         //do something
                  /*  Network::connectNetwork("localhost",([] (short x){
                if(x==0){
                    std::cout<<"Cannot connect!\n";
                }
                else{
                    std::cout<<"Connected!\n";
                    OgreFramework::getSingletonPtr()->m_pSceneMgr->clearScene();
                    //show login screen
                    //Effects::getManager()->addText(400,300,"Connected");
                }
                    
            }));*/
            //add loading screen
            OgreFramework::getSingletonPtr()->m_pSceneMgr->clearScene();
                }

                return true;
        }

        bool keyReleased(const OIS::KeyEvent &keyEventRef)
        {
                OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);

                return true;
        }

 
private:
        void setupScene(){
            /*Effects::getManager()->addLight(Loader::getLoader()->getSunPos(),"SunLight");
            Effects::getManager()->initSky();
            //Effects::getManager()->initLightShafts();
            int ax=4,ay=4;
            for(int x=0;x<25;x++){
                for(int y=0;y<25;y++){
                    for(int z=0;z<4;z++){
                        Loader::getLoader()->addChunkGen(ax+x,ay+y,z+1,456);
                    }
                }
            }*/
            OgreFramework::getSingletonPtr()->map = new Map(64,64,20.0f);
        }
	void run(){
            boost::unique_lock<boost::mutex> lock(OgreFramework::getSingletonPtr()->mutex);
            	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Start main loop...");
                //float fps_limit = 70.0;
                double timeSinceLastFrame = 0;
                double startTime = 0;
                
              OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics();
              lock.unlock();
                while(!m_bShutdown && !OgreFramework::getSingletonPtr()->isOgreToBeShutDown()) 
                {
                    boost::unique_lock<boost::mutex> lock(OgreFramework::getSingletonPtr()->mutex);
                        if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed())m_bShutdown = true;

                        Ogre::WindowEventUtilities::messagePump();

                        if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
                        {
                                startTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();

                                OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
                                OgreFramework::getSingletonPtr()->m_pMouse->capture();
                                
                                
                                OgreFramework::getSingletonPtr()->updateCaption();
                                //update mouse selection to shaders
                                Ogre::ResourcePtr r = Ogre::MaterialManager::getSingletonPtr()->getByName("MultiTextureEditor");
                                if(!r.isNull()){
                                    float mx=OgreFramework::getSingletonPtr()->cursor_x+OgreFramework::getSingletonPtr()->m_pCamera->getPosition().x;
                                    float my=OgreFramework::getSingletonPtr()->cursor_y+OgreFramework::getSingletonPtr()->m_pCamera->getPosition().z;
                                    //float my=OgreFramework::getSingletonPtr()->cursor_y+sqrt(OgreFramework::getSingletonPtr()->m_pCamera->getPosition().z);
                                    /*//std::cout<<((Ogre::Material*)r.get())->getTechnique(0)->getPass(0)->getVertexProgramParameters()->getSharedParameters().size()<<"\n";//->setNamedConstant("pointer", Ogre::Vector3(OgreFramework::getSingletonPtr()->cursor_x,OgreFramework::getSingletonPtr()->cursor_y,OgreFramework::getSingletonPtr()->cursor_r));
                                    float mx=(OgreFramework::getSingletonPtr()->cursor_x-OgreFramework::getSingletonPtr()->cursror_e_w/2.0)/OgreFramework::getSingletonPtr()->cursror_e_w;
                                    float my=(-OgreFramework::getSingletonPtr()->cursor_y+OgreFramework::getSingletonPtr()->cursror_e_h/2.0)/OgreFramework::getSingletonPtr()->cursror_e_h;
                                    Ogre::Ray ray=OgreFramework::getSingletonPtr()->m_pCamera->getCameraToViewportRay(mx,my);
                                    Ogre::Vector3 cam = OgreFramework::getSingletonPtr()->m_pCamera->getDirection();
                                    float factor = ray.getDirection().dotProduct(cam) / (cam.length() * ray.getDirection().length());
                                    Ogre::Vector3 point = ray.getPoint(OgreFramework::getSingletonPtr()->m_pCamera->getPosition().y*factor);
                                    */
                                    switch(OgreFramework::getSingletonPtr()->mode){
                                        case 0:
                                                ((Ogre::Material*)r.get())->getTechnique(0)->getPass(0)->getVertexProgramParameters()->setNamedConstant("pointer", Ogre::Vector4(mx,my,OgreFramework::getSingletonPtr()->cursor_r,OgreFramework::getSingletonPtr()->cursor_rb));
                                                break;
                                        case 1:
                                            ((Ogre::Material*)r.get())->getTechnique(0)->getPass(0)->getVertexProgramParameters()->setNamedConstant("pointer", Ogre::Vector4(mx,my,OgreFramework::getSingletonPtr()->cursor_r,0));
                                            break;
                                        case 2:
                                            ((Ogre::Material*)r.get())->getTechnique(0)->getPass(0)->getVertexProgramParameters()->setNamedConstant("pointer", Ogre::Vector4(mx,my,2,0));
                                            break;
                                        default:
                                            ((Ogre::Material*)r.get())->getTechnique(0)->getPass(0)->getVertexProgramParameters()->setNamedConstant("pointer", Ogre::Vector4(-20,-20,0,0));
                                    }
                                        
                                }
                                else{
                                    //TODO error!
                                }
                                
                                OgreFramework::getSingletonPtr()->updateOgre(timeSinceLastFrame);
                                OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
                                
                                   /* while(OgreFramework::getSingletonPtr()->chunksToAdd.size()>0){//adding chunks
                                        ChunkAdding* ch = OgreFramework::getSingletonPtr()->chunksToAdd.front();
                                        OgreFramework::getSingletonPtr()->chunksToAdd.pop();
                                        Ogre::ManualObject* ogreMesh =  OgreFramework::getSingletonPtr()->m_pSceneMgr->createManualObject(std::string("Chunk")+(char)ch->x+(char)ch->y+(char)ch->z);
                                        ogreMesh->setDynamic(true);
                                        ogreMesh->setCastShadows(true);
                                        ogreMesh->begin("MultiTexture", Ogre::RenderOperation::OT_TRIANGLE_LIST);
                                        {
                                            for(int i=0;i<ch->size;++i){
                                                ogreMesh->position(ch->pos[i]);
                                                ogreMesh->normal(ch->normal[i]);
                                                ogreMesh->colour(ch->color[i]);
                                            }
                                        }
                                        ogreMesh->end();

                                        Ogre::SceneNode* ogreNode =  OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode(std::string("NChunk")+(char)ch->x+(char)ch->y+(char)ch->z, Ogre::Vector3(0, 0, 0));
                                        ogreNode->setVisible(true);
                                        ogreNode->attachObject(ogreMesh);
                                        ogreNode->setPosition(ch->x*CHUNK_SIZE,ch->z*(CHUNK_SIZE),ch->y*CHUNK_SIZE);
                                        delete ch;
                                    }*/
                                //update chunks
                                /*Pos* pos=NULL;
                                unsigned int chunksUpdated=0;
                                float cx=OgreFramework::getSingletonPtr()->m_pCamera->getPosition().x,cy=OgreFramework::getSingletonPtr()->m_pCamera->getPosition().z;
                                while(chunksUpdated<8 and (pos=OgreFramework::getSingletonPtr()->map->getNextUpdatedChunk())!=NULL){
                                    int xx=pos->x*128-cx;
                                    int yy=pos->y*128-cy;
                                    if(xx*xx+yy*yy<1280){
                                    std::cout<<"Updating chunk\n";
                                    chunksUpdated++;
                                    try{
                                        OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode(std::string("NChunk")+(char)pos->x+(char)pos->y)->removeAndDestroyAllChildren();
                                    }
                                    catch(...){}
                                    OgreFramework::getSingletonPtr()->map->getChunk(pos->x,pos->y);
                                    delete pos;
                                    }
                                    else{
                                        OgreFramework::getSingletonPtr()->map->chunks_q.push(pos);
                                    }
                                }*/
                                float cx=OgreFramework::getSingletonPtr()->m_pCamera->getPosition().x,cy=OgreFramework::getSingletonPtr()->m_pCamera->getPosition().z;
                                int lod = OgreFramework::getSingletonPtr()->m_pCamera->getPosition().y/OgreFramework::getSingletonPtr()->lodFactor;
                                if(lod>4)
                                    lod=4;
                                else if(lod<0)
                                    lod=0;
                                int showY = tan(OgreFramework::getSingletonPtr()->m_pCamera->getFOVy().valueRadians()) * OgreFramework::getSingletonPtr()->m_pCamera->getPosition().y/256.0+1;
                                if(showY>7)
                                    showY=7;
                                int showX=showY*4.0/3.0;
                                try{
                                for(int xx=cx/128-showX;xx<=cx/128+showX;xx++){
                                    for(int yy=cy/128-showY;yy<=cy/128+showY;yy++){
                                        if(xx>=0 and yy>=0 and xx<OgreFramework::getSingletonPtr()->map->x and yy<OgreFramework::getSingletonPtr()->map->y and !OgreFramework::getSingletonPtr()->map->fresh[lod][xx][yy]){
                                            /*try{
                                                OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode(std::string("NChunk")+Map::getPos(xx,yy))->removeAndDestroyAllChildren();
                                            }
                                            catch(...){
                                            //    std::cout<<"Chunk didnt exist\n";
                                            }*/
                                            OgreFramework::getSingletonPtr()->map->getChunk(xx,yy,lod);
                                            //std::cout<<"Updating chunk\n";
                                            throw (int)0;
                                            }
                                    }
                                }
                                }
                                catch(int){}//exit from loop
                                lock.unlock();
                                boost::this_thread::sleep(boost::posix_time::milliseconds(1));
                                timeSinceLastFrame = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - startTime; 
                        }
                        else
                        {
        #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
                    Sleep(1000);
        #else
                    sleep(1);
        #endif
                        }
                }

                OgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit");
                OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");
        }
 
	Ogre::SceneNode*			m_pOgreHeadNode;
	Ogre::Entity*				m_pOgreHeadEntity;
 
	bool					m_bShutdown;
};

/*
 * 
 */
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
 {
    try
      {
		App app;
		app.start();
      }
	catch(std::exception& e)
      {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBoxA(NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occurred: %s\n", e.what());
#endif
      }
    return 0;
}

