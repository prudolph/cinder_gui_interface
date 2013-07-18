//
//  NavigationBarObject.cpp
//  guiObjects
//
//  Created by Paul Rudolph on 7/10/13.
//
//


#include "NavigationBarObject.h"

#include <boost/lexical_cast.hpp>

using namespace std;
using namespace ci;

NavigationBarObject::NavigationBarObject(){
    nbBufferSz=5.0f;
    nbContainerColor= ColorA(0.0f,0.0f,1.0f,1.0f);

}


void NavigationBarObject:: setup(ci::app::WindowRef window,
                                 tuio::Client *tuio,Rectf containerRect,
                                 boost::function<void(GuiObject*)> fn){
    nbContainer = containerRect;

    registerForInput(window,tuio);
    
}

void NavigationBarObject::draw(){
   /*
    
    gl::color(nbContainerColor);
    gl::drawSolidRect(nbContainer);
    for (int i =0;i< nbChildren.size();i++){
        nbChildren.at(i)->draw();
        
    }
    
    */
}

void NavigationBarObject::addChild(GuiObject* o){
        nbChildren.push_back(o);
        organizeChildren();
}

void NavigationBarObject::navButtonSelected(GuiObject * selectedObject){
    //only on button can be selected at a time
    console()<<"Nav Button Selected"<<endl;
    
    // go through each bar item and  unselect the other buttons
    for (int i =0;i< nbChildren.size();i++){
        if(nbChildren.at(i)->isSelected() && nbChildren.at(i)!=selectedObject){
            nbChildren.at(i)->setSelected(false,true);
        }
    
    }
}


void NavigationBarObject::organizeChildren(){
    for(int i =0;i<nbChildren.size();i++){
        
        float x=nbContainer.getUpperLeft().x+  ((nbChildren.at(i)->getSize().x) +nbBufferSz );
        float y =nbContainer.getUpperLeft().y;
        console()<<"x pos "<< x<<endl;
         nbChildren.at(i)->setPositon(Vec2f(x,y));
      
    }
}

