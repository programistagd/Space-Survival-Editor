//|||||||||||||||||||||||||||||||||||||||||||||||

#include "OgreFramework.h"
#include <iostream>
#include <iomanip>
#include <OGRE/OgreVector3.h>
//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

template<>  OgreFramework* Ogre::Singleton<OgreFramework>::ms_Singleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreFramework::OgreFramework()
{
    m_MoveSpeed                 = 0.1f;
    m_RotateSpeed               = 0.3f;

    m_bShutDownOgre             = false;
    m_iNumScreenShots   = 0;

    m_pRoot                             = 0;
    m_pSceneMgr                 = 0;
    m_pRenderWnd                = 0;
    m_pCamera                   = 0;
    m_pViewport                 = 0;
    m_pLog                              = 0;
    m_pTimer                    = 0;
    
    
    m_pInputMgr                 = 0;
    m_pKeyboard                 = 0;
    m_pMouse                    = 0;

    m_pTrayMgr          = 0;
    m_FrameEvent        = Ogre::FrameEvent();
    
    cursor_x=0;
    cursor_y=0;
    cursor_r=3;
    cursor_rb=0;
    lodFactor = 580;
    cursor_h=10;
    cursor_relative=true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
{
    Ogre::LogManager* logMgr = new Ogre::LogManager();

    m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
    m_pLog->setDebugOutputEnabled(true);

    m_pRoot = new Ogre::Root();

    
    /*if(!m_pRoot->showConfigDialog())
        return false;*/
    
    
    //MY EDIT - auto renderer select
    //std::cout<<"myedit\n\n";
    
    #if defined(_DEBUG)
    ogre->loadPlugin("RenderSystem_GL_d");
#else
    m_pRoot->loadPlugin("RenderSystem_GL");
    //m_pRoot->loadPlugin("Plugin_CgProgramManager");
#endif
    
    RenderSystem *renderSystem;
    bool ok = false;
    RenderSystemList renderers =
        m_pRoot->getAvailableRenderers();
    //std::cout<<"myedit2\n\n";

    // See if the list is empty (no renderers available)
    if(renderers.empty())
        std::cout<<"Houston we've got a problem!\n\n";//TODO do something please...

    for(RenderSystemList::iterator it = renderers.begin();
        it != renderers.end(); it++)
    {
        renderSystem = (*it);
        if(strstr((renderSystem->getName().c_str()), "OpenGL"))
        {
            ok = true;
            break;
        }
    }

    if(!ok) {
        // We still don't have a renderer; pick
        // up the first one from the list
        renderSystem = (*renderers.begin());
    }

    m_pRoot->setRenderSystem(renderSystem);

    // Manually set some configuration options (optional)
    //renderSystem->setConfigOption("Video Mode", "800 x 600");
    //std::cout<<"myedit - end\n\n";
    //END of My edit
    
    
    m_pRenderWnd = m_pRoot->initialise(true, wndTitle);

    m_pSceneMgr = m_pRoot->createSceneManager(ST_GENERIC, "SceneManager");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

    m_pCamera = m_pSceneMgr->createCamera("Camera");
    //m_pCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
    m_pCamera->setPosition(Vector3(0.1, 100, 0.1));
    //m_pCamera->lookAt(Vector3(0.1, 0.1, 0.1));
    //m_pCamera->setDirection(0,-1,0);
    m_pCamera->setNearClipDistance(1);
    //m_pCamera->setFixedYawAxis(false);
    m_pCamera->setOrientation(Ogre::Quaternion(0.7071067811865475244,-0.7071067811865475244,0.0,0.0));// Square root of 1/2
    m_pViewport = m_pRenderWnd->addViewport(m_pCamera);
    
    m_pViewport->setBackgroundColour(ColourValue(0.8f, 0.7f, 0.6f, 1.0f));

    m_pCamera->setAspectRatio(Real(m_pViewport->getActualWidth()) / Real(m_pViewport->getActualHeight()));

    m_pViewport->setCamera(m_pCamera);

    size_t hWnd = 0;
    OIS::ParamList paramList;
    m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);

    paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

    m_pInputMgr = OIS::InputManager::createInputSystem(paramList);

    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
    m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));

    m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
    m_pMouse->getMouseState().width      = m_pRenderWnd->getWidth();

    if(pKeyListener == 0)
        m_pKeyboard->setEventCallback(this);
    else
        m_pKeyboard->setEventCallback(pKeyListener);

    if(pMouseListener == 0)
        m_pMouse->setEventCallback(this);
    else
        m_pMouse->setEventCallback(pMouseListener);

    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    m_pTimer = new Ogre::Timer();
    m_pTimer->reset();

    m_pTrayMgr = new OgreBites::SdkTrayManager("TrayMgr", m_pRenderWnd, m_pMouse, this);
    m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    Ogre::FontManager::getSingleton().getByName("SdkTrays/Caption")->load();
    //m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);//a na co mi to?
    m_pTrayMgr->hideCursor();
    caption = m_pTrayMgr->createLabel(OgreBites::TL_TOPLEFT,"caption","Loading...",1280);
    m_pRenderWnd->setActive(true);

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreFramework::~OgreFramework()
{
    if(m_pInputMgr) OIS::InputManager::destroyInputSystem(m_pInputMgr);
    if(m_pTrayMgr)  delete m_pTrayMgr;
    if(m_pRoot)     delete m_pRoot;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        m_bShutDownOgre = true;//chango to menu etc.
        return true;
    }

    if(m_pKeyboard->isKeyDown(OIS::KC_SYSRQ))
    {
        m_pRenderWnd->writeContentsToTimestampedFile("BOF_Screenshot_", ".jpg");
        return true;
    }

    if(m_pKeyboard->isKeyDown(OIS::KC_M))
    {
        static int mode = 0;

        if(mode == 2)
        {
            m_pCamera->setPolygonMode(PM_SOLID);
            mode = 0;
        }
        else if(mode == 0)
        {
            m_pCamera->setPolygonMode(PM_WIREFRAME);
            mode = 1;
        }
        else if(mode == 1)
        {
            m_pCamera->setPolygonMode(PM_POINTS);
            mode = 2;
        }
    }

    if(m_pKeyboard->isKeyDown(OIS::KC_O))
    {
        if(m_pTrayMgr->areFrameStatsVisible())
        {
            //m_pTrayMgr->hideLogo();
            m_pTrayMgr->hideFrameStats();
        }
        else
        {
            //m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
            m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
        }
    }
    if(m_pKeyboard->isKeyDown(OIS::KC_X)){
        cursor_x=0;
        cursor_y=0;
    }
    if(m_pKeyboard->isKeyDown(OIS::KC_R)){
        cursor_relative=!cursor_relative;
    }
    if(m_pKeyboard->isKeyDown(OIS::KC_TAB)){
        if(mode==3){
            m_pCamera->setOrientation(Ogre::Quaternion(0.7071067811865475244,-0.7071067811865475244,0.0,0.0));// Square root of 1/2
        }
        if(m_pKeyboard->isKeyDown(OIS::KC_LSHIFT)){
            if(mode<=0)
                mode=3;
            else
                mode--;
        }else{
            mode++;
            if(mode>3){
                mode=0;
            }
        }
    }
    if(m_pKeyboard->isKeyDown(OIS::KC_CAPITAL)){
        cursor_select=!cursor_select;
    }
    if(m_pKeyboard->isKeyDown(OIS::KC_Q)){
        shader=!shader;
    }
    if(m_pKeyboard->isKeyDown(OIS::KC_H)){
        cursor_h=0;
    }
    if(m_pKeyboard->isKeyDown(OIS::KC_HOME)){
        map->load("map.map");
    }
    if(m_pKeyboard->isKeyDown(OIS::KC_INSERT)){
        map->load("autosave.map");
    }
    if(m_pKeyboard->isKeyDown(OIS::KC_END)){
        map->save("map.map");
    }
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::updateCaption(){
    std::stringstream s;
    switch(mode){
        case 0:
            s<<"Height Map | ";
            s<<"X:"<<(int)(m_pCamera->getPosition().x/128)<<" Y:"<<(int)(m_pCamera->getPosition().z/128)<<" | ";
            if(cursor_relative)
                s<<"Relative ";
            else
                s<<"Absolute ";
            s<<"Height:"<<std::setprecision(3)<<cursor_h<<" | ";
            {
                int x=(int)(m_pCamera->getPosition().x+cursor_x);
                int y=(int)(m_pCamera->getPosition().z+cursor_y);
                if(x<0 or y<0 or x>map->x*128 or y>map->y*128)
                    s<<"H: NaN";
                else
                    s<<"H: "<<std::setprecision(3)<<map->map[x][y];
            }
            s<<" | ";
            if(cursor_select)
                s<<"Select and release";
            else
                s<<"Instant brush";
        break;
        case 1:
            s<<"Material Map | ";
            s<<"X:"<<(int)(m_pCamera->getPosition().x/128)<<" Y:"<<(int)(m_pCamera->getPosition().z/128)<<" | ";
            s<<"M: "<<Map::getMaterialById(material)<<" | ";
            {
                int x=(int)(m_pCamera->getPosition().x+cursor_x);
                int y=(int)(m_pCamera->getPosition().z+cursor_y);
                if(x<0 or y<0 or x>map->x*128 or y>map->y*128)
                    s<<"H: NaN";
                else
                    s<<"H: "<<std::setprecision(3)<<map->map[x][y];
            }
        break;
        case 2:
            s<<"Buildings Placing TODO | ";
            s<<"X:"<<(int)(m_pCamera->getPosition().x/128)<<" Y:"<<(int)(m_pCamera->getPosition().z/128)<<" | ";
            {
                int x=(int)(m_pCamera->getPosition().x+cursor_x);
                int y=(int)(m_pCamera->getPosition().z+cursor_y);
                if(x<0 or y<0 or x>map->x*128 or y>map->y*128)
                    s<<"H: NaN";
                else
                    s<<"H: "<<std::setprecision(3)<<map->map[x][y];
            }
        break;
        default:
            s<<"Spectate | ";
            s<<"X:"<<(int)(m_pCamera->getPosition().x/128)<<" Y:"<<(int)(m_pCamera->getPosition().z/128)<<" | ";
            {
                int x=(int)(m_pCamera->getPosition().x);
                int y=(int)(m_pCamera->getPosition().z);
                if(x<0 or y<0 or x>map->x*128 or y>map->y*128)
                    s<<"mapH: NaN";
                else
                    s<<"mapH: "<<std::setprecision(3)<<map->map[x][y];
            }
            s<<" | camH:"<<m_pCamera->getPosition().y;
        break;
    }
    caption->setCaption(s.str());
}


//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mouseMoved(const OIS::MouseEvent &evt)
{
    if(mode==3){
        m_pCamera->yaw(Degree(evt.state.X.rel * -0.1f));
        m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
        return true;
    }
    else{
    
    cursor_x+=evt.state.X.rel/60.0*(sqrt(abs(m_pCamera->getPosition().y))+1);
    cursor_y+=evt.state.Y.rel/60.0*(sqrt(abs(m_pCamera->getPosition().y))+1);
    /*cursor_x=evt.state.X.abs;
    cursor_y=evt.state.Y.abs;
    cursror_e_w=evt.state.width;
    cursror_e_h=evt.state.height;*/
    //if (m_pTrayMgr->injectMouseMove(evt)) return true;
    if(isMousePressed){
        float mx=OgreFramework::getSingletonPtr()->cursor_x+OgreFramework::getSingletonPtr()->m_pCamera->getPosition().x;
        //float my=OgreFramework::getSingletonPtr()->cursor_y+sqrt(OgreFramework::getSingletonPtr()->m_pCamera->getPosition().z);
        float my=OgreFramework::getSingletonPtr()->cursor_y+OgreFramework::getSingletonPtr()->m_pCamera->getPosition().z;
        map->select(mx,my,cursor_r,cursor_rb);
        if(mode == 0){
            if(!cursor_select){
                if(isLeftMouse)
                    map->brush(cursor_h,cursor_relative);
                else
                    map->brush(-cursor_h,cursor_relative);
            }
        }
        else if(mode==1){
            map->brush(material);
        }
    }
    if(m_pKeyboard->isKeyDown(OIS::KC_LCONTROL)){
        if(mode==0){
            if(m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
                cursor_h+=evt.state.Z.rel* 0.09f;
            else
                cursor_h+=evt.state.Z.rel* 0.0077f;
        }
        else if(mode==1){
            if(evt.state.Z.rel>10){
                material++;
                if(material>12)
                    material=-1;
            }
            else if(evt.state.Z.rel<-10){
                material--;
                if(material<-1){
                    material=12;
                }
            }
        }
    }
    else if(m_pKeyboard->isKeyDown(OIS::KC_LSHIFT)){
        if(isMousePressed){
            m_pCamera->setPosition(m_pCamera->getPosition()-Vector3(evt.state.X.rel,0,evt.state.Y.rel));
        }
        m_pCamera->setPosition(m_pCamera->getPosition()+Vector3(0,evt.state.Z.rel* -0.1f,0));   
    }
    else{
        if(m_pKeyboard->isKeyDown(OIS::KC_RCONTROL)){
            if(m_pKeyboard->isKeyDown(OIS::KC_RSHIFT))
                cursor_rb+=evt.state.Z.rel*0.06;
            else
                cursor_rb+=evt.state.Z.rel* 0.006f;
            if(cursor_rb<0.0)
                cursor_rb=0.0;
        }else{
            if(m_pKeyboard->isKeyDown(OIS::KC_RSHIFT))
                cursor_r+=evt.state.Z.rel*0.07;
            else
                cursor_r+=evt.state.Z.rel* 0.0077f;
            if(cursor_r<0.9)
                cursor_r=0.9;
        }
    }
    
    return true;
    }
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    isMousePressed=true;
    if(id==OIS::MB_Left)
        isLeftMouse=true;
        float mx=OgreFramework::getSingletonPtr()->cursor_x+OgreFramework::getSingletonPtr()->m_pCamera->getPosition().x;
        //float my=OgreFramework::getSingletonPtr()->cursor_y+sqrt(OgreFramework::getSingletonPtr()->m_pCamera->getPosition().z);
        float my=OgreFramework::getSingletonPtr()->cursor_y+OgreFramework::getSingletonPtr()->m_pCamera->getPosition().z;
        map->select(mx,my,cursor_r,cursor_rb);
        if(mode==1){
                map->brush(material);
        }
        else if(!cursor_select and mode==0){
                if(isLeftMouse)
                    map->brush(cursor_h,cursor_relative);
                else
                    map->brush(-cursor_h,cursor_relative);
        }
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    isMousePressed=false;
    if(id==OIS::MB_Left)
        isLeftMouse=false;
    if(cursor_select and !m_pKeyboard->isKeyDown(OIS::KC_LSHIFT)){
        if(mode==0)
        {
            if(id == OIS::MB_Left)
                map->brush(cursor_h,cursor_relative);
            else if(id == OIS::MB_Right)
                map->brush(-cursor_h,cursor_relative);
            else
                map->deselect();
        }
    }else{
        map->deselect();
    }
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::updateOgre(double timeSinceLastFrame)
{
    m_MoveScale = m_MoveSpeed*(sqrt(abs(m_pCamera->getPosition().y))+1)   * (float)timeSinceLastFrame;
    m_RotScale  = m_RotateSpeed * (float)timeSinceLastFrame;

    m_TranslateVector = Vector3::ZERO;

    getInput();
    moveCamera();

    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::moveCamera()
{
    if(mode==3){
        if(m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
            m_pCamera->moveRelative(m_TranslateVector);
        else
            m_pCamera->moveRelative(m_TranslateVector / 10);
    }else{
        if(m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
            m_pCamera->move(m_TranslateVector);
        else
            m_pCamera->move(m_TranslateVector / 10);
    }
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::getInput()
{
    if(m_pKeyboard->isKeyDown(OIS::KC_A))
        m_TranslateVector.x = -m_MoveScale;

    if(m_pKeyboard->isKeyDown(OIS::KC_D))
        m_TranslateVector.x = m_MoveScale;

    if(m_pKeyboard->isKeyDown(OIS::KC_W))
        m_TranslateVector.z = -m_MoveScale;

    if(m_pKeyboard->isKeyDown(OIS::KC_S))
        m_TranslateVector.z = m_MoveScale;
}

//|||||||||||||||||||||||||||||||||||||||||||||||