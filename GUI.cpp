/* 
 * File:   GUI.cpp
 * Author: radek
 * 
 * Created on 24 grudzień 2012, 22:51
 */

#include "GUI.h"
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>

GUI::GUI() {
    //mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
}

GUI* GUI::ptr=NULL;

GUI* GUI::getGUI(){
    if(ptr==NULL)
        ptr=new GUI();
    return ptr;
}

GUI::~GUI() {
}

