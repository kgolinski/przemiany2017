#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxOsc.h"
#include "ofxOpenCv.h"

#define PORT 12345

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
        void drawScene();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void saveScreenshot(ofxDatGuiButtonEvent e);
    
        ofxOscReceiver receiver;
    
        ofPlanePrimitive base;
        ofPlanePrimitive ground;
        ofPlanePrimitive water;
        ofShader groundShader, waterShader;
        ofImage groundTex, waterTex, snowTex;
    
        ofxCvGrayscaleImage grayImage;
    
        ofxDatGui* sliders;
        ofxDatGui* toggles;
        ofxDatGui* actions;
    
    
    // PARAMETRY:
    // wysokosc gor +
    // gestosc gor +
    // erozja +
    
    // rodzaj lasow +
    // ilosc lasow +
    // color lasow +
    
    // klimat/temperatura (snieg) +
    
    // poziom wody +
    // kolor wody +
    // typ gleby +
    // kat kamery +
    // rotacja -
    // skala +
    
    
    // ilosc 'zwierzatek' (?)
    // wielkosc 'zwierzatek' (?)
    
    //mozliwosc zamrozenia jakiegos parametru
    
        ofxDatGuiSlider* height;
        ofxDatGuiSlider* density;
        ofxDatGuiSlider* erosion;
        ofxDatGuiSlider* dilation;
    
        ofxDatGuiSlider* groundColor;
        ofxDatGuiSlider* waterLevel;
        ofxDatGuiSlider* waterColor;
        ofxDatGuiSlider* waterOpacity;
    
        ofxDatGuiSlider* climate;
        ofxDatGuiSlider* treeAmount;
        ofxDatGuiSlider* treeType;
        ofxDatGuiSlider* treeColor;

        ofxDatGuiSlider* rotation;
        ofxDatGuiSlider* cameraAngle;
        ofxDatGuiSlider* cameraDistance;
        ofxDatGuiSlider* cameraFov;
    
        ofxDatGuiToggle* heightToggle;
        ofxDatGuiToggle* densityToggle;
        ofxDatGuiToggle* erosionToggle;
        ofxDatGuiToggle* dilationToggle;
        ofxDatGuiToggle* groundColorToggle;
        ofxDatGuiToggle* waterLevelToggle;
        ofxDatGuiToggle* waterColorToggle;
        ofxDatGuiToggle* waterOpacityToggle;
        ofxDatGuiToggle* climateToggle;
        ofxDatGuiToggle* treeAmountToggle;
        ofxDatGuiToggle* treeTypeToggle;
        ofxDatGuiToggle* treeColorToggle;
        ofxDatGuiToggle* rotationToggle;
        ofxDatGuiToggle* cameraAngleToggle;
        ofxDatGuiToggle* cameraDistanceToggle;
        ofxDatGuiToggle* cameraFovToggle;
    
        ofEasyCam camera;
        float size = 800;
        float subdivisions = 200;
    
        ofColor groundColor1 = ofColor::fromHex(0x502b00);
        ofColor groundColor2 = ofColor::fromHex(0x297d00);
    
        ofColor waterColor1 = ofColor::fromHex(0x77b35a);
        ofColor waterColor2 = ofColor::fromHex(0x5a9bb3);
    
};
