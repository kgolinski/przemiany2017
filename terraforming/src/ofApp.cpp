#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // PARAMETRY:
    // wysokosc gor +
    // gestosc gor
    // erozja
    
    // rodzaj lasow
    // ilosc lasow
    
    // klimat/temperatura (snieg)
    // poziom wody
    // kolor wody
    // typ gleby
    // kat kamery +
    // skala +
    
    // predkosc rotacji(?)
    // ilosc 'zwierzatek' (?)
    // wielkosc 'zwierzatek' (?)
    
    //guzik do zapisu screenshota
    //mozliwosc zamrozenia jakiegos parametru
    
    receiver.setup(PORT);
    base.set(size, size, subdivisions, subdivisions);
    
    groundShader.load("ground");
    waterShader.load("water");
    
    waterTex.allocate(subdivisions, subdivisions, OF_IMAGE_GRAYSCALE);
    groundTex.allocate(subdivisions, subdivisions, OF_IMAGE_GRAYSCALE);
    snowTex.allocate(size, size, OF_IMAGE_GRAYSCALE);
    
    ground.set(size, size, subdivisions, subdivisions);
    water.set(size, size, subdivisions, subdivisions);
    ground.mapTexCoordsFromTexture(groundTex.getTextureReference());
    water.mapTexCoordsFromTexture(waterTex.getTextureReference());

    sliders = new ofxDatGui();
    //sliders->addHeader("Parameters");
    
    height = sliders->addSlider("Height", 0.0, 1.0, 0.2);
    density = sliders->addSlider("Density", 0.0, 1.0, 0.0);
    erosion = sliders->addSlider("Erosion", 0.0, 1.0, 0.0);
    dilation = sliders->addSlider("Dilation", 0.0, 1.0, 0.0);
    sliders->addBreak();
    groundColor = sliders->addSlider("Ground Color", 0.0, 1.0, 0.0);
    waterLevel = sliders->addSlider("Water Level", 0.0, 1.0, 0.5);
    waterColor = sliders->addSlider("Water Color", 0.0, 1.0, 0.5);
    waterOpacity = sliders->addSlider("Water Opacity", 0.0, 1.0, 0.4);
    sliders->addBreak();
    climate = sliders->addSlider("Climate", 0.0, 1.0, 0.5);
    foliageAmount = sliders->addSlider("Tree Amount", 0.0, 1.0, 0.5); //!
    foliageType = sliders->addSlider("Tree Type", 0.0, 1.0, 0.5); //!
    foliageColor = sliders->addSlider("Tree Color", 0.0, 1.0, 0.5); //!
    sliders->addBreak();
    rotation = sliders->addSlider("Plane Rotation", 0.0, 1.0, 0.6);
    cameraAngle = sliders->addSlider("Camera Angle", 0.0, 1.0, 0.6);
    scale = sliders->addSlider("Scale", 0.0, 1.0, 0.2);
    baseLevel = sliders->addSlider("Base Level", 0.0, 1.0, 0.5);
    
    toggles = new ofxDatGui();
    toggles->setPosition(271, 0);
    heightToggle = toggles->addToggle("Height");
    
    actions = new ofxDatGui();
    actions->setPosition(542, 0);
    //actions->addHeader("Actions");
    actions->addButton("Save screenshot (S)")->onButtonEvent(this, &ofApp::saveScreenshot);
    ofxDatGuiLog::quiet();
    
    camera.setDistance(300.0f);
    camera.setNearClip(0.01f);
    camera.setFarClip(500.0f);
    //camera.setPosition(0.4f, 0.2f, 0.8f);
    camera.lookAt(ofVec3f(0.0f, 0.0f, 0.0f));
    //camera.disableMouseInput();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    float noiseScale =  ofMap(density->getValue(), 0.0, 1.0, 0.01, 0.1);
    //float noiseVel = 1;//ofGetElapsedTimef();
    
    ofPixels &groundPixels = groundTex.getPixels();
    int w = groundTex.getWidth();
    int h = groundTex.getHeight();
    for(int y=0; y<h; y++) {
        for(int x=0; x<w; x++) {
            int i = y * w + x;
            float noiseValue1 = ofNoise(x * noiseScale, y * noiseScale, 1);
            float noiseValue2 = ofNoise(x/200.0, y/200.0, 200);
            groundPixels[i] = 255 * (noiseValue1*noiseValue2);
        }
    }
    groundTex.update();
    grayImage.setFromPixels(groundTex.getPixels());
    
    for(int i=0;i <ofMap(erosion->getValue(),0.0, 1.0, 0, 20);i++){
        grayImage.erode();
    }
    
    for(int i=0;i <ofMap(dilation->getValue(),0.0, 1.0, 0, 20);i++){
        grayImage.dilate();
    }
    
    groundTex.setFromPixels(grayImage.getPixels());
    noiseScale = 0.05;
    float noiseVel = ofGetElapsedTimef()/1.0;
    
    ofPixels &waterPixels = waterTex.getPixels();
    w = waterTex.getWidth();
    h = waterTex.getHeight();
    for(int y=0; y<h; y++) {
        for(int x=0; x<w; x++) {
            int i = y * w + x;
            float noiseValue = ofNoise(x * noiseScale, y * noiseScale, noiseVel);
            waterPixels[i] = 255 * noiseValue;
        }
    }
    waterTex.update();

    
    grayImage.resize(size, size);
    grayImage.blurHeavily();
    grayImage.threshold(climate->getValue()*255);
    grayImage.blurHeavily();
    grayImage.dilate();
    
    snowTex.setFromPixels(grayImage.getPixels());
    
    //OSC
    while(receiver.hasWaitingMessages()){
        ofxOscMessage m;
        receiver.getNextMessage(m);
        if(m.getAddress() == "/height"){
            height->setValue(m.getArgAsInt32(0));
        }else if(m.getAddress() == "/density"){
            density->setValue(m.getArgAsInt32(0));
        }else{
            ofLog() << m.getAddress();
        }
    }
}


void ofApp::drawScene(){
    ofPushMatrix();
    
    ofEnableDepthTest();
    ofSetColor(255);
    
    ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);
    float s = ofMap(scale->getValue(), 0.0, 1.0, 0.1, 2.0);
    ofScale(s,s,s);
    ofTranslate(0, 0, ofMap(baseLevel->getValue(), 0.0, 1.0, -200.0*s, 200.0*s));
    ofRotateX(ofMap(cameraAngle->getValue(), 0.0, 1.0, 15.0, 90.0));
    ofRotateZ(ofMap(rotation->getValue(), 0.0, 1.0, 0.0, 360.0));
    //ofRotateZ(rotation->getValue());
    
    groundTex.getTextureReference().bind();
    groundShader.begin();
    groundShader.setUniformTexture("tex1", snowTex, 2);
    groundShader.setUniformTexture("tex1", snowTex, 2);
    groundShader.setUniform1f("height", ofMap(height->getValue(), 0.0, 1.0, 5.0, 600.0)*s);
    ofColor outGround = groundColor1;
    outGround.lerp(groundColor2, groundColor->getValue());
    groundShader.setUniform3f("color", outGround.r/255.0, outGround.g/255.0, outGround.b/255.0);
    
    ground.draw();
    //ground.drawWireframe();
    groundShader.end();
    groundTex.getTextureReference().unbind();
    
    waterTex.getTextureReference().bind();
    waterShader.begin();
    waterShader.setUniform1f("height", 5.0);
    
    ofColor outWater = waterColor1;
    outWater.lerp(waterColor2, waterColor->getValue());
    
    waterShader.setUniform4f("color", outWater.r/255.0, outWater.g/255.0, outWater.b/255.0, ofMap(waterOpacity->getValue(), 0.0, 1.0, 0.1, 0.85));
    ofPushMatrix();
    ofTranslate(0, 0, ofMap(waterLevel->getValue(), 0.0, 1.0, -300.0, 300.0)*s);
    water.draw();
    
    //ground.drawWireframe();
    waterShader.end();
    waterTex.getTextureReference().unbind();
    ofPopMatrix();
    
    //ofSetColor(255,0,0);
    //base.drawWireframe();
    
    // boczki
    
    /*ofSetColor(128);
     float depth = height->getValue()/2.0;
     ofPixels &pixels = img.getPixels();
     
     
     ofBeginShape();
     ofVertex(-size/2.0, size/2.0, -depth);
     ofVertex(-size/2.0, -size/2.0, -depth);
     
     for(int i=0;i<subdivisions;i++){
     int p = (int)pixels[i*subdivisions];
     float h = -1.0 + (p/255.0) * 2.0;
     ofVertex(-size/2.0, -size/2.0+i*(size/subdivisions), h*height->getValue());
     }
     ofEndShape();
     
     
     ofBeginShape();
     
     ofVertex(size/2.0, -size/2.0, -depth);
     ofVertex(-size/2.0, -size/2.0, -depth);
     
     for(int i=0;i<subdivisions;i++){
     int p = (int)pixels[i];
     float h = -1.0 + ((((float)p)/255.0));
     ofVertex(-size/2.0+i*(size/(subdivisions-1)), -size/2.0, depth+h*height->getValue());
     }
     ofEndShape(true);
     */
    ofDisableDepthTest();
    ofPopMatrix();
    //snowTex.draw(ofGetWidth()-200.0,ofGetHeight()-200.0, 200, 200);

}
//--------------------------------------------------------------
void ofApp::draw(){
    //.camera.begin();
    drawScene();
    //camera.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key==OF_KEY_TAB){
        sliders->setVisible(!sliders->getVisible());
        toggles->setVisible(!toggles->getVisible());
        actions->setVisible(!actions->getVisible());
    }
    else if(key == 'S' || key == 's'){
        ofSaveFrame();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::saveScreenshot(ofxDatGuiButtonEvent e)
{
    cout << "onButtonEvent: " << e.target->getLabel() << endl;
    ofSaveFrame();
}
