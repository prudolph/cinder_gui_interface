#include "cinder/app/AppNative.h"
#include "NavigationBarObject.h"

using namespace ci;
using namespace ci::app;
using namespace std;


class cinder_gui_interfaceApp : public AppBasic {
public:
    void prepareSettings( Settings *settings );
	void setup();
	void update();
	void draw();
    
    void buttonCallback(GuiObject *o );
    GuiObject       guiObject,guiObject1;
    NavigationBarObject navigationBarObject;
    
    tuio::Client mTuio;
    
    Rectf contentArea;
    ColorA contentAreaColor;
};


void cinder_gui_interfaceApp::prepareSettings( Settings *settings )
{
	settings->enableMultiTouch();
    settings->setWindowSize( 800, 800 );
}

void cinder_gui_interfaceApp::setup()
{
    mTuio.connect();
    
    
    guiObject.setup(getWindow(),&mTuio);
    guiObject.setPositon(Vec2f(400,400));
    guiObject.setSize(Vec2f(100,100));
    guiObject.setContainerColor(ColorA(1.0f,0.0f,0.0f,1.0f));
    guiObject.setText("BTN 0");
    
    guiObject.addCallBack(bind(&cinder_gui_interfaceApp::buttonCallback,this,std::__1::placeholders::_1));
    
    
    guiObject1.setup(getWindow(),&mTuio);
    guiObject1.setPositon(Vec2f(600,600));
    guiObject1.setSize(Vec2f(100,100));
    
    guiObject1.addCallBack(bind(&cinder_gui_interfaceApp::buttonCallback,this,std::__1::placeholders::_1));
    guiObject1.setContainerColor(ColorA(0.0f,1.0f,0.0f,1.0f));
    guiObject1.setText("BTN 1");
    
    
    navigationBarObject.setup(getWindow(),&mTuio,3,Rectf(10,0,getWindowWidth()-10,50) ,bind(&cinder_gui_interfaceApp::buttonCallback,this,std::__1::placeholders::_1));
    
    contentArea = Rectf(100,100,getWindowWidth()-50,getWindowHeight()-100);
    contentAreaColor = ColorA(1.0f, 1.0f, 1.0f, 0.5f);
}


void cinder_gui_interfaceApp::update()
{
}

void cinder_gui_interfaceApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    //draw the content area
    gl::color( contentAreaColor );
    gl::drawSolidRect( contentArea);
    
    
    GuiObject::draw();
    
    //draw touches
    for(int i=0;i<mTuio.getActiveTouches().size();i++){
        gl::color(0.25f,0.25f,0.25f);
        
        gl::drawSolidCircle(mTuio.getActiveTouches().at(i).getPos(), 25.0f);
        
    }
    
    
    for(int i=0;i<GuiObject::getMouseTouches().size();i++){
        gl::color(0.25f,0.25f,0.25f);
        gl::drawSolidCircle(GuiObject::getMouseTouches().at(i).getPos(), 25.0f);
    }
    
    
}

void cinder_gui_interfaceApp::buttonCallback(GuiObject *o){
    console()<<"BUTTON  "<< o->getText()<<"Has been selected"<<endl;
    contentAreaColor =o->getContainerColor()/2;
}



CINDER_APP_NATIVE( cinder_gui_interfaceApp, RendererGl )
