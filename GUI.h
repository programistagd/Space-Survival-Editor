/* 
 * File:   GUI.h
 * Author: radek
 *
 * Created on 24 grudzień 2012, 22:51
 */

#ifndef GUI_H
#define	GUI_H

#include <CEGUI/CEGUI.h>

class GUI {
public:
    static GUI* getGUI();
    virtual ~GUI();
private:
    GUI();//TODO implement GUI
    static GUI* ptr;
    //CEGUI::Renderer mRenderer;
};

#endif	/* GUI_H */

