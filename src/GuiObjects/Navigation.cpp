//
//  Navigation.cpp
//  guiObjects
//
//  Created by Paul Rudolph on 7/10/13.


#include "Navigation.h"

Navigation::Navigation(){
    
    buttonSpacing=5.0f;
    nextObjectPosition=Vec2f(0.0,0.0f);
    gui_ContainerColor= ColorA(0.0f,1.0f,0.0f,0.0f);
    gui_CanMove=false;
}


void Navigation:: setup(app::WindowRef window,Vec2f pos, Vec2f size){
    registerForInput(window);
    setPosition(pos);
    setSize(size);
}


//This overrides the default draw function. This allows custom drawing rules to be set
void Navigation::drawObject(){
    
    gl::color(gui_ContainerColor);
    gl::drawSolidRect(getRect());

    for (int i =0;i< childObjects.size();i++)
        childObjects.at(i)->drawObject();
}


void Navigation::addChild(GuiObject* o){
    //position button with in the navigation bar

    o->setPosition(convertToObjectSpace(nextObjectPosition));
    nextObjectPosition.x+= o->getWidth()+buttonSpacing;

    childObjects.push_back(o);
}


void Navigation::touchesBeganHandler(){
    //Test the point if there is a child object that contains it        
    for (TouchEvent::Touch curTouch : GuiObject::gui_ObjectTouches ){
        //Test each Child if there are touch points in their container
        for(GuiObject *curObj: childObjects){
            bool shouldSelect = (curObj->hasTouchPoint(curTouch.getPos()) && !curObj->isSelected());
            if(shouldSelect){
                curObj->setSelected(true);
                onObjectSelect(curObj);
            }else curObj->setSelected(false);
        }
    }
}

//sample Callback function
void Navigation::onObjectSelect(GuiObject* object){
    //Add additional Select functionality here ex:button tweening, touch delays
    
    //Fire off an event to any outside objects that are listening for select event.
    getSelectedSignal() (object);
}


void Navigation::setPosition( float x, float y ){
    gui_Position = Vec2f(x,y);
    setChildObjectPositions();
}


void Navigation::setPosition( Vec2f pt ){
    gui_Position = pt;
    setChildObjectPositions();
}


void Navigation::setChildObjectPositions(){
        nextObjectPosition=Vec2f(0.0,0.0f);
    for(GuiObject *cObj: childObjects){
        cObj->setPosition(convertToObjectSpace(nextObjectPosition));
        nextObjectPosition.x+= cObj->getWidth()+buttonSpacing;
    }
}
