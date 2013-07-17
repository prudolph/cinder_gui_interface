//
//  GuiObject.cpp
//  guiObjects
//
//  Created by Paul Rudolph on 7/2/13.
//
//



#include "GuiObject.h"
#include "cinder/gl/gl.h"


using namespace std;
using namespace ci::app;




struct FindTouch {
    uint32_t id;
    FindTouch( TouchEvent::Touch t1 ) : id(t1.getId()){}
    bool operator() (const TouchEvent::Touch &t2){
        return(t2.getId() == id );
    }
};



vector<GuiObject*>GuiObject::objectOrderList = vector<GuiObject*>();
vector <TouchEvent::Touch> GuiObject::mouseTouches = vector<TouchEvent::Touch>();
GuiObject::GuiObject(){
    oCenter=Vec2f(0,0);
    oContainerColor = ColorA(1.0f,0.0f,0.0f,1.0f);
    oContainer= Rectf( Vec2f( 0, 0 ), Vec2f( 10, 10 ) );

    oCanMove=oCanRotate=oCanResize=oIsEnabled=oIsVisible=true;
     oSelected=false;
    oTextFont="Arial";
    oStyle = NORMAL;
    
    objectOrderList.push_back(this);
}

void GuiObject:: setup(ci::app::WindowRef window,tuio::Client *tuio){
    oWindow = window;
    oTuioClient = tuio;
    registerInputCallBacks();
    

   }


void GuiObject::draw(){
    //draw each object back to front
    for (std::vector<GuiObject*>::reverse_iterator it = objectOrderList.rbegin() ; it != objectOrderList.rend(); ++it){



        GuiObject *object = (GuiObject*)*it;
   
        if(object->oIsVisible){
            if(object->oSelected){
                gl::color(0.0f,1.0f,1.0f);
                gl::lineWidth(5.0f);
                gl::drawStrokedRect(object->oContainer);
            }
                gl::color(object->oContainerColor);
                gl::drawSolidRect(object->oContainer);
        }
        
        //draw gui object text
        gl::enableAlphaBlending();
        Vec2f textboxPos= Vec2f(object->oContainer.getCenter().x-object->oTextTexture.getWidth()/2,
                                object->oContainer.getCenter().y-object->oTextTexture.getHeight()/2);
        gl::draw(object->oTextTexture,textboxPos);
        gl::disableAlphaBlending();
    
    }
}


void GuiObject::registerInputCallBacks(){
      
    oCbMouseDown = oWindow->getSignalMouseDown().connect(std::bind( &GuiObject::onMouseBegan, this, std::_1 ) );
	oCbMouseDrag = oWindow->getSignalMouseDrag().connect( std::bind( &GuiObject::onMouseMoved, this, std::_1 ) );
    oCBMouseEnd  = oWindow->getSignalMouseUp()  .connect( std::bind( &GuiObject::onMouseEnded, this, std::_1 ) );
  
      if(oTuioClient!=NULL){
          oTuioClient->registerTouchesBegan(this,  &GuiObject::touchesBegan);
          oTuioClient->registerTouchesMoved(this,  &GuiObject::touchesMoved);
          oTuioClient->registerTouchesEnded(this,  &GuiObject::touchesEnded);
    }
    
#ifdef INPUT_DEBUG
    console()<<"Registered Input callbacks"<<endl;
#endif

}

#pragma mark Input Functions -
#pragma mark Mouse Functions
void GuiObject::onMouseBegan(MouseEvent &e){
    

    TouchEvent te= mouse2Touch(e);
    touchesBegan(te);
    mouseTouches.insert(mouseTouches.end(), te.getTouches().begin(),   te.getTouches().end());
    
#ifdef INPUT_DEBUG
    /*
    if(oSelected)console()<<"Object : "<< this <<" Selected @ "<< e.getX()<<","<< e.getY() <<endl;
    else          console()<<"Mouse Began @ "<< e.getX()<<","<< e.getY() <<endl;
     */
#endif

}

void GuiObject::onMouseMoved(MouseEvent &e){

    TouchEvent te= mouse2Touch(e);
    touchesMoved(te);
    
    mouseTouches.clear();
    mouseTouches.insert(mouseTouches.end(), te.getTouches().begin(),   te.getTouches().end());
  
    
#ifdef INPUT_DEBUG
    /*
    if(oSelected) console()<<"Object : "<< this <<" Selected @ "<< e.getX()<<","<< e.getY() <<endl;
    else          console()<<"Mouse Moved to "<< e.getX()<<","<< e.getY() <<endl;
     */
#endif
}


void GuiObject::onMouseEnded(MouseEvent &e){
  
    TouchEvent te= mouse2Touch(e);
    touchesEnded(mouse2Touch(e));
    mouseTouches.clear();
    
#ifdef INPUT_DEBUG
          //  console()<<"Mouse Ended "<<endl;
#endif

}

vector<TouchEvent::Touch> GuiObject::getMouseTouches(){
    return mouseTouches;
}

#pragma mark Touch Functions
void GuiObject::touchesBegan(app::TouchEvent event){
 
#ifdef INPUT_DEBUG
    console()<<"Touches Began Event:: "<< event<<endl;
#endif
    
        for( vector<TouchEvent::Touch>::iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) {

            if( oContainer.contains( touchIt->getPos()) && objectOrderList.at(getTopMostObject(touchIt->getPos())) == this && !isSelected()){
                currentObjTouches.push_back(*touchIt);
                  event.setHandled( true );
                }
            }
    
    if(currentObjTouches.size()>0){
        setSelected(true);
        oSelectedSignal(this);//Fire off Signal
    }

}


void GuiObject::touchesMoved(app::TouchEvent event){
#ifdef INPUT_DEBUG
    console()<<"Touches Moved Event:: "<<event<<endl;
#endif
    
    
    Vec2f centroid;
    int pntCnt=0;
        //Go Through all the touches in the event
        for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) {
        
            //If the touch object has current touches, check if they are in this event. if so move/reposition object based on the updated touches.
            auto found=std::find_if (currentObjTouches.begin(),currentObjTouches.end(), FindTouch(*touchIt));
            if(found!=currentObjTouches.end() && isSelected()){//this touch id is touching this object
                centroid += touchIt->getPos();
                pntCnt++;
            }
          
        }
    if(this->oCanMove){
        centroid/=pntCnt;//Get the center/average point of the touches
        if(currentObjTouches.size()>0 && !isNaN(centroid) ){
            oContainer.offsetCenterTo(centroid);
        }
    }
    if(currentObjTouches.size()>0 && !oSelected)  setSelected(true);



}
void GuiObject::touchesEnded(app::TouchEvent event){
    
#ifdef INPUT_DEBUG
    console()<<"Touches Ended Event:: "<<event<<endl;
#endif
    
    
        for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) {
        
            //if current touches vector contains the iterators touch id, remove it from the list, since it is no longer touching the object. If there are no more touches in the current object touches set then the object is no longer selected

            auto found=std::find_if (currentObjTouches.begin(),currentObjTouches.end(), FindTouch(*touchIt));
            
            //this touch id is touching this object
            if(found!=currentObjTouches.end()) currentObjTouches.erase(found);
        }
        
        if(currentObjTouches.size()==0)  setSelected(false);

}

TouchEvent GuiObject::mouse2Touch(MouseEvent e){
    app::TouchEvent::Touch t = app::TouchEvent::Touch( e.getPos(), Vec2f(), -1, app::getElapsedSeconds() , nullptr );
    
    
    vector<app::TouchEvent::Touch> tVector;
    tVector.push_back(t);
    app::TouchEvent te= TouchEvent(e.getWindow(),tVector );

    return te;
    
}


int GuiObject::getTopMostObject(Vec2f pos){
    //check if other buttons also get this touch

    for(int i =0;i<objectOrderList.size();i++){
        if( objectOrderList.at(i)->oContainer.contains(pos)){
            return i;
        }
    }
    
    return -1;
}
 void GuiObject::setSelected(bool t,bool force){
        //update object order - put the clicked button at the top of the list
     if(t){

         objectOrderList.erase( find(objectOrderList.begin(),objectOrderList.end(),this));
         objectOrderList.insert(objectOrderList.begin(),(this));
     }

     if(oStyle == NORMAL){
         //console()<<"Style is NORMAL"<<endl;
         oSelected=t;
     }
     if(oStyle == TOGGLE){
         //console()<<"Style is TOGGLE"<<endl;
         if(t)oSelected=!oSelected;
     }
     
     if(oStyle == DROPDOWN){
         console()<<"Style is DROPDOWN"<<endl;
         oSelected=t;
     }
     
     if(force)oSelected = t;
     
  
     
     
    }



#pragma mark - Object Positioning functions
void GuiObject::setPositon(Vec2f pos){
    oContainer.set(oContainer.x1+pos.x , oContainer.y1+pos.y, oContainer.x2+pos.x, oContainer.y2+pos.y);
}
void GuiObject::rePosition(Vec2f pos){
    if(oCanMove)oContainer.offsetCenterTo(pos);

}


void GuiObject::setSize(Vec2f size){
    oContainer.set(oContainer.x1 , oContainer.y1, oContainer.x1+size.x, oContainer.y1+size.y);
}

#pragma mark - Text functions


void GuiObject::setText(string s){
    oText = s;
    console()<<"SET Text "<< oText<<endl;
    oTextBox = TextBox().alignment( TextBox::CENTER ).font( Font(oTextFont,18) ).size( Vec2f(oContainer.getWidth(), TextBox::GROW) ).text( oText );
    oTextBox.setColor( ColorA( 0.0f, 0.0f, 0.0f, 1.0f ) );
	oTextBox.setBackgroundColor( ColorA( 0, 0, 0, 0.0f ) );
	oTextTexture = gl::Texture( oTextBox.render() );
}



