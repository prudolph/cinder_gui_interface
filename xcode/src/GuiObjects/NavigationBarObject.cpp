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
    oContainerColor= ColorA(0.0f,0.0f,0.0f,0.00f);
    oCanMove=false;
    

}


void NavigationBarObject:: setup(Rectf containerRect,
                                 boost::function<void(GuiObject*)> fn){
    oContainer = containerRect;
 
    
}

void NavigationBarObject::objectDraw(){
  
    
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
        
        float x=oContainer.getUpperLeft().x+  ((nbChildren.at(i)->getSize().x) +nbBufferSz );
        float y =oContainer.getUpperLeft().y;
        console()<<"x pos "<< x<<endl;
         nbChildren.at(i)->setPositon(Vec2f(x,y));
      
    }
}


void NavigationBarObject::touchesBeganHandler(){
//    console()<<"NavBar touch BEGIN Handler"<<endl;
//    console()<<"NavBar has TOuches "<< GuiObject::currentObjTouches.size() <<endl;
//    
    
    GuiObject *selectedObj=NULL;
    
    //Test the point if there is a child object that contains it
    for(int t =0;t<GuiObject::currentObjTouches.size();++t){
        TouchEvent::Touch curTouch = GuiObject::currentObjTouches.at(t);
        
        //Test each Child if there are touch points in their container
        for(int i=0;i< nbChildren.size();i++){
            GuiObject *curObj = nbChildren.at(i);
            if(curObj->containerHasPoint(curTouch.getPos()) && !curObj->isSelected()){
                selectedObj=curObj;
                touchCountMap[curObj]++;

                //get out of these loops
                break;break;
            }
        }
    }
    
    if(selectedObj){
        //go through the other children and unselect them
        for(int i=0;i< nbChildren.size();i++){
            // if this is not the selected object unselect it
            if(nbChildren.at(i)!=selectedObj) nbChildren.at(i)->setSelected(false,true);
        }
        
        //set the new object to selected
        selectedObj->setSelected(true);
    }
}
void NavigationBarObject::touchesMovedHandler(){
  // console()<<"NavBar touch MOVED Handler"<<endl;
}
void NavigationBarObject::touchesEndedHandler(){
//    console()<<"NavBar touch END Handler"<<endl;
//        console()<<"NavBar has TOuches "<< GuiObject::currentObjTouches.size() <<endl;
    for(int i=0;i< nbChildren.size();i++){
        nbChildren.at(i)->currentObjTouches.clear();
    }

  
  /*
                    //Test each Child if there are touch points in their container that are not in the parent's touches
                    for(int i=0;i< nbChildren.size();i++){

                        GuiObject *curObj = nbChildren.at(i);
                        
                          for(int t =0;t< curObj->currentObjTouches.size();++t){
                              TouchEvent::Touch curTouch = curObj->currentObjTouches.at(t);
                              //look for the childs touch in the parent
                              auto found=std::find_if (currentObjTouches.begin(),currentObjTouches.end(),GuiObject::FindTouch(curTouch));
                                    if(found==currentObjTouches.end()){
                                        curObj->currentObjTouches.erase(found);
                                    }
                          }
                    
                    }
        */
}
