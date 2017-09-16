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
        void drawTrees();
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
        void regenerateGround();
        void saveScreenshot(ofxDatGuiButtonEvent e);
        void regenerateSeed(ofxDatGuiButtonEvent e);
    
        ofxOscReceiver receiver;
    
        ofPlanePrimitive base;
        ofPlanePrimitive groundPrimitive;
        ofMesh ground;
        ofPlanePrimitive water;
        ofShader groundShader, waterShader;
        ofImage groundTex, waterTex, snowTex;
        //ofFbo treeFbo;
        //ofLight light;
        ofxCvGrayscaleImage grayImage;
    
        ofxDatGui* sliders;
        ofxDatGui* toggles;
        ofxDatGui* actions;
    
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
        float lastHeight = 0;
        float lastDensity = 0;
        float lastErosion = 0;
        float lastDilation = 0;
    
        ofColor groundColor1 = ofColor::fromHex(0x502b00);
        ofColor groundColor2 = ofColor::fromHex(0x297d00);
    
        ofColor waterColor1 = ofColor::fromHex(0x77b35a);
        ofColor waterColor2 = ofColor::fromHex(0x5a9bb3);
    
        ofColor treeColor1 = ofColor::fromHex(0x502b00);
        ofColor treeColor2 = ofColor::fromHex(0x297d00);
    
        float noiseSeed;
    
        vector<ofVec2f> trees;
        int maxTreeCount = 1000;
};
