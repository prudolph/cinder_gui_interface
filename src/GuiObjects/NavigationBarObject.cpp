//
//  NavigationBarObject.cpp
//  guiObjects
//
//  Created by Paul Rudolph on 7/10/13.


#include "NavigationBarObject.h"

NavigationBarObject::NavigationBarObject(){

    buttonSpacing=5.0f;
    nextObjectPosition=Vec2f(0.0,0.0f);
    gui_ContainerColor= ColorA(0.0f,1.0f,0.0f,0.0f);
    gui_CanMove=false;
}


void NavigationBarObject:: setup(App *app,Vec2f pos, float width, float height){
    
    registerForInput(app);
    setPosition(pos);
    setWidth(width);
    setHeight(height);
}


//This overrides the default draw function. This allows custom drawing rules to be set
void NavigationBarObject::drawObject(){
    
    gl::color(gui_ContainerColor);
    gl::drawSolidRect(getRect());

    for (int i =0;i< childObjects.size();i++)
        childObjects.at(i)->drawObject();
}


void NavigationBarObject::addChild(GuiObject* o){
    //position button with in the navigation bar
    o->setPosition(convertToObjectSpace(nextObjectPosition));
    nextObjectPosition.x+= o->getWidth()+buttonSpacing;

//    //Set callback for selected signal
//    o->getSelectedSignal().connect( bind(& NavigationBarObject::onObjectSelect,this,std::__1::placeholders::_1));
    childObjects.push_back(o);
}


void NavigationBarObject::touchesBeganHandler(){
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
void NavigationBarObject::onObjectSelect(GuiObject* object){
    //Add additional Select functionality here ex:button tweening, touch delays
    /*
     
     */
    
    //Fire off an event to any outside objects that are listening for select event.
    getSelectedSignal() (object);
    
}

