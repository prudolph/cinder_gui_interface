//
//  NavigationBarObject.cpp
//  guiObjects
//
//  Created by Paul Rudolph on 7/10/13.
//
//


#include "NavigationBarObject.h"
#include "cinder/Rand.h"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace ci;

NavigationBarObject::NavigationBarObject(){
    cinder::app::console()<<"CREATED AN Navigation BAR OBJECT"<<endl;
    nbBufferSz=5.0f;
    nbContainerColor= ColorA(0.0f,0.0f,1.0f);
}


void NavigationBarObject:: setup(ci::app::WindowRef window,tuio::Client *tuio,int numItems,Rectf containerRect,
                                 boost::function<void(GuiObject*)> fn){
    nbContainer = containerRect;
    nbNumObjects = numItems;

    for(int i =0;i<nbNumObjects;i++){
        NavigationObject *n = new NavigationObject();
        n->setup(window,tuio);
        float x=nbContainer.getUpperLeft().x+((nbContainer.getWidth()/nbNumObjects)*i)+nbBufferSz;
        float y =nbContainer.getUpperLeft().y;
        n->setPositon(Vec2f(x,y));
        n->setContainerColor(ColorA(randFloat(0.0f,1.0),1.0f,randFloat(0.0f,1.0),1.0f ));//choose random color for now
        n->setSize(Vec2f((nbContainer.getWidth()/nbNumObjects)-nbBufferSz,50));

        n->getSelectedSignal().connect(fn);
        n->getSelectedSignal().connect(bind(&NavigationBarObject::navButtonSelected,this,std::__1::placeholders::_1));
        
        n->setText("Navigation Button "+  boost::lexical_cast<string>( i ) + "  ");
        
        n->setInputStyle(NavigationObject::TOGGLE);
                     
        navigationBarObjects.push_back(n);
    }

}

void NavigationBarObject::draw(){
    gl::color(nbContainerColor);
    gl::drawSolidRect(nbContainer);
    for (int i =0;i< navigationBarObjects.size();i++){
        navigationBarObjects.at(i)->draw();
        
    }
}

void NavigationBarObject::navButtonSelected(GuiObject * selectedObject){
    
    for (int i =0;i< navigationBarObjects.size();i++){
        if(navigationBarObjects.at(i)->isSelected() && navigationBarObjects.at(i)!=selectedObject){
            navigationBarObjects.at(i)->setSelected(false,true);
        }
    
    }
}




