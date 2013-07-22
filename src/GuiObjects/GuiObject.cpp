//
//  GuiObject.cpp
//  guiObjects
//
//  Created by Paul Rudolph on 7/2/13.
//
//



#include "GuiObject.h"
#include "cinder/gl/gl.h"
#include "boost/lexical_cast.hpp"

using namespace std;
using namespace ci::app;


//Static Variables, all classes have access to these shared objects.(Only one instance is created)
vector<GuiObject*>GuiObject::gui_Objects = vector<GuiObject*>();
vector <TouchEvent::Touch> GuiObject::gui_MouseTouches = vector<TouchEvent::Touch>();
Client GuiObject::gui_Tuio;

GuiObject::GuiObject(){

    //Set size and Position
    gui_Position = Vec2f(0,0);
    gui_Width=gui_Height=0.0f;

    //Set background color
    gui_ContainerColor = ColorA(1.0f,0.0f,0.0f,1.0f);
    
    //Set Base Attributes
    gui_CanMove     = true;
    gui_CanRotate   = true;
    gui_CanResize   = true;
    gui_AcceptTouch = true;
    
    gui_IsEnabled   = true;
    gui_IsVisible   = true;

    gui_DefaultDraw = true;
    gui_Selected    = false;
    gui_Hit         = false;
    
    gui_TextFont="Arial";

    //Add this object to the static list of objects
    gui_Objects.push_back(this);
}



void GuiObject::draw(){
    //Draw each gui object back to front
    for (std::vector<GuiObject*>::reverse_iterator it = gui_Objects.rbegin() ; it != gui_Objects.rend(); ++it){
        GuiObject *object = (GuiObject*)*it;
        if(object->gui_IsVisible && object->gui_DefaultDraw)  object->drawObject();
    }
    
#ifdef INPUT_DEBUG
    //DRAW TOUCHES
    gl::enableAlphaBlending();
        //DRAW TOUCH POINTS
        for(int i=0;i< gui_Tuio.getActiveTouches().size();i++){
            gl::color(ColorA(1.25f,0.25f,0.25f,0.25f));
            gl::drawSolidCircle( gui_Tuio.getActiveTouches().at(i).getPos(), 25.0f);
        }
        //DRAW MOUSE POINTS
        for(int i=0;i<gui_MouseTouches.size();i++){
            gl::color(ColorA(1.25f,0.25f,0.25f,0.25f));
            gl::drawSolidCircle(gui_MouseTouches.at(i).getPos(), 25.0f);
        }
    gl::disableAlphaBlending();
#endif
}


//Non-static default draw function
//THis can be overridden for each subclass object
void GuiObject::drawObject(){
   
    if(isSelected()){
        gl::color(1.0f,0.0f,0.0f);
        gl::lineWidth(8.0f);
        gl::drawStrokedRect(getRect());
    }
    
    if(isHit()){
        gl::color(0.0f,0.0f,1.0f);
        gl::lineWidth(4.0f);
        gl::drawStrokedRect(getRect());
    }
    
    gl::enableAlphaBlending();
        gl::color(gui_ContainerColor);
        gl::drawSolidRect(getRect());
    gl::disableAlphaBlending();
    
    
    //draw gui object text

    gl::enableAlphaBlending();
    gl::color(0,1,0);
  
    Vec2f textboxPos= Vec2f((getRect().getCenter().x-gui_TextTexture.getWidth()/2),
                            (getRect().getCenter().y-gui_TextTexture.getHeight()/2));
   
    gl::draw(gui_TextTexture,textboxPos);
    gl::disableAlphaBlending();

    
}

void GuiObject::registerForInput(App* app){
    app::WindowRef window = app->getWindow();
    
    if(!gui_Tuio.isConnected())  gui_Tuio.connect();

    //Mouse Events
    gui_CbMouseDown = window->getSignalMouseDown().connect( std::bind( &GuiObject::onMouseBegan, this, std::_1 ) );
	gui_CbMouseDrag = window->getSignalMouseDrag().connect( std::bind( &GuiObject::onMouseMoved, this, std::_1 ) );
    gui_CbMouseEnd  = window->getSignalMouseUp()  .connect( std::bind( &GuiObject::onMouseEnded, this, std::_1 ) );
    
    //Touch Events
    gui_CbTouchBegan =  gui_Tuio.registerTouchesBegan(this,  &GuiObject::touchesBegan);
    gui_CbTouchMoved =  gui_Tuio.registerTouchesMoved(this,  &GuiObject::touchesMoved);
    gui_CbTouchEnd   =  gui_Tuio.registerTouchesEnded(this,  &GuiObject::touchesEnded);
    
    
#ifdef INPUT_DEBUG
    console()<<"Registered Input callbacks"<<endl;
#endif

}

#pragma mark Input Functions -
#pragma mark Mouse Functions

//Converts mouse events into Touch Events
void GuiObject::onMouseBegan(MouseEvent &e){

    TouchEvent te= mouse2Touch(e);
    touchesBegan(te);
    gui_MouseTouches.insert(gui_MouseTouches.end(), te.getTouches().begin(),   te.getTouches().end());
    
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
    gui_MouseTouches.clear();
    gui_MouseTouches.insert(gui_MouseTouches.end(), te.getTouches().begin(),   te.getTouches().end());
  
    
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
    gui_MouseTouches.clear();
    
#ifdef INPUT_DEBUG
    console()<<"Mouse Ended "<<endl;
#endif

}

vector<TouchEvent::Touch> GuiObject::getMouseTouches(){
    return gui_MouseTouches;
}
TouchEvent GuiObject::mouse2Touch(MouseEvent e){
    app::TouchEvent::Touch t = app::TouchEvent::Touch( e.getPos(), Vec2f(), -1, app::getElapsedSeconds() , nullptr );
    vector<app::TouchEvent::Touch> tVector;
    tVector.push_back(t);
    app::TouchEvent te= TouchEvent(e.getWindow(),tVector );
    
    return te;
}




#pragma mark Touch Functions
void GuiObject::touchesBegan(app::TouchEvent event){

    //Go Through each touch object and check if this object contains the point
    for(TouchEvent::Touch touch : event.getTouches() ){
        
        //if this object contains the point and this is the object is not sitting below any other object;
        if(hasPoint( touch.getPos()) &&  gui_Objects.at(getTopMostObject(touch.getPos())) == this && gui_AcceptTouch){

            // if this object is accepting touches add the touch to the obj's touch list
            currentObjTouches.push_back(touch);
            event.setHandled( true );
        }
    }

    if(currentObjTouches.size()>0) touchesBeganHandler();

}
void GuiObject::touchesMoved(app::TouchEvent event){
    
    //Vec2f centroid;
    //int pntCnt=0;
    
        //Go Through all the touches in the event if the current touch is in the objects touch list then call the objects touches moved handler
        for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) {
            //If the touch object has current touches, check if they are in this event. if so move/reposition object based on the updated touches.
            auto found=std::find_if (currentObjTouches.begin(),currentObjTouches.end(), FindTouch(*touchIt));
            if(found!=currentObjTouches.end() ){//this touch id is touching this object
                touchesMovedHandler();
                event.setHandled();

                // centroid += touchIt->getPos();
               // pntCnt++;
            }
          
        }
    /*
    if(this->oCanMove){
        centroid/=pntCnt;//Get the center/average point of the touches
        if(currentObjTouches.size()>0 && !isNaN(centroid) ){
            oContainer.offsetCenterTo(centroid);
        }
    }
    if(currentObjTouches.size()>0 && !oSelected)  setSelected(true);
*/


}
void GuiObject::touchesEnded(app::TouchEvent event){
    
    
        for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) {
            
            //if current touches vector contains the iterators touch id, remove it from the list, since it is no longer touching the object. If there are no more touches in the current object touches set then the object is no longer selected

            auto found=std::find_if (currentObjTouches.begin(),currentObjTouches.end(), FindTouch(*touchIt));
            
            //this touch id is touching this object
            if(found!=currentObjTouches.end()){
                currentObjTouches.erase(found);
                touchesEndedHandler();
            }
        }
}

void GuiObject::touchesBeganHandler(){
    console()<<"Touch BEGAN Handler :: ABSTRACT FUNCTION - Must be OVERRIDDEN in subclass"<<endl;

}
void GuiObject::touchesMovedHandler(){
    console()<<"Touch MOVED Handler :: ABSTRACT FUNCTION - Must be OVERRIDDEN in subclass"<<endl;
}
void GuiObject::touchesEndedHandler(){
    console()<<"Touch ENDED Handler :: ABSTRACT FUNCTION - Must be OVERRIDDEN in subclass"<<endl;
}

void GuiObject::setSelected(bool state){
    gui_Selected  =state;
    if(gui_Selected) gui_OnSelectSignal(this);
}

void GuiObject::setHit(bool state){
    gui_Hit = state;
    if(gui_Hit) gui_OnHitSignal(this);
}


int GuiObject::getTopMostObject(Vec2f pos){
    //check if other buttons also get this touch
    for(int i =0;i<gui_Objects.size();i++){
        if( gui_Objects.at(i)->hasPoint(pos)){
            return i;
        }
    }
    return -1;
}


// Passing in 0,0 would result in i.e. 500,500
Vec2f GuiObject::convertToObjectSpace(Vec2f pos){
    return Vec2f(getPosition().x+pos.x,getPosition().y+pos.y);
}

Vec2f GuiObject::convertToWindowSpace(Vec2f pos){
    return Vec2f(-1,-1);
}



#pragma mark - Text functions
void GuiObject::setText(string s){

#ifdef INPUT_DEBUG
    if(getWidth()==0||getHeight()==0)console()<<"WARNING - Object has no area:Text will not be displayed"<<endl;
#endif

    gui_Text = s;
    gui_TextBox = TextBox()
                            .alignment( TextBox::CENTER )
                            .font( Font(gui_TextFont,18) )
                            .size( Vec2f(getWidth(), TextBox::GROW) )
                            .text( gui_Text );
    
    gui_TextBox.setColor( ColorA( 0.0f, 0.0f, 0.0f, 1.0f ) );
	gui_TextBox.setBackgroundColor( ColorA( 0, 0, 0, 0.0f ) );
    
	gui_TextTexture = gl::Texture( gui_TextBox.render() );
}

void GuiObject::updateTextBox(){
    
    gui_TextBox.setText(gui_Text );
    gui_TextTexture = gl::Texture( gui_TextBox.render() );
}
