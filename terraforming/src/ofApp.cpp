#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofEnableSmoothing();
    ofSetVerticalSync(true);
    ofEnableAntiAliasing();
    ofEnableAlphaBlending();
    
    //ofSetLogLevel(OF_LOG_ERROR);
    
    receiver.setup(PORT);
    base.set(size, size, subdivisions, subdivisions);
    
    groundShader.load("ground");
    waterShader.load("water");
    //light.setAmbientColor(ofColor(0, 0, 0));
    //light.setPosition(0,0, 300);
    
    waterTex.allocate(subdivisions, subdivisions, OF_IMAGE_GRAYSCALE);
    groundTex.allocate(subdivisions, subdivisions, OF_IMAGE_GRAYSCALE);
    snowTex.allocate(size, size, OF_IMAGE_GRAYSCALE);
    //treeTex.allocate(size,size, OF_IMAGE_COLOR);
    
    
    ofFbo::Settings settings;
    settings.height = size;
    settings.width = size;
    settings.internalformat = GL_RGBA;
    settings.numSamples = 8;
    
    //treeFbo.allocate(settings);
    
    //treeFbo.allocate(size,size, GL_RGBA);
    
    groundPrimitive.set(size, size, subdivisions, subdivisions);
    ground = groundPrimitive.getMesh();
    water.set(size, size, subdivisions, subdivisions);
    //ground.mapTexCoordsFromTexture(groundTex.getTextureReference()); //?
    water.mapTexCoordsFromTexture(waterTex.getTextureReference());

    sliders = new ofxDatGui();

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
    climate = sliders->addSlider("Climate", 0.0, 1.0, 0.8);
    treeAmount = sliders->addSlider("Tree Amount", 0.0, 1.0, 0.5); //!
    treeType = sliders->addSlider("Tree Type", 0.0, 1.0, 0.5); //!
    treeColor = sliders->addSlider("Tree Color", 0.0, 1.0, 0.5); //!
    sliders->addBreak();
    rotation = sliders->addSlider("Plane Rotation", 0.0, 1.0, 0.0);
    cameraAngle = sliders->addSlider("Camera Angle", 0.0, 1.0, 0.6);
    cameraDistance = sliders->addSlider("Camera Distance", 0.0, 1.0, 0.2);
    cameraFov = sliders->addSlider("Camera FOV", 0.0, 1.0, 0.5);
    
    toggles = new ofxDatGui();
    toggles->setPosition(271, 0);
    heightToggle = toggles->addToggle("Height");
    densityToggle = toggles->addToggle("Density");
    erosionToggle = toggles->addToggle("Erosion");
    dilationToggle = toggles->addToggle("Dilation");
    toggles->addBreak();
    groundColorToggle = toggles->addToggle("Ground Color");
    waterLevelToggle = toggles->addToggle("Water Level");
    waterColorToggle = toggles->addToggle("Water Color");
    waterOpacityToggle = toggles->addToggle("Water Opacity");
    toggles->addBreak();
    climateToggle = toggles->addToggle("Climate");
    treeAmountToggle = toggles->addToggle("Tree Amount");
    treeTypeToggle = toggles->addToggle("Tree Type");
    treeColorToggle = toggles->addToggle("Tree Color");
    toggles->addBreak();
    rotationToggle = toggles->addToggle("Rotation");
    cameraAngleToggle = toggles->addToggle("Camera Angle");
    cameraDistanceToggle = toggles->addToggle("Camera Distance");
    cameraFovToggle = toggles->addToggle("Camera FOV");
    
    actions = new ofxDatGui();
    actions->setPosition(542, 0);
    actions->addButton("Save screenshot (S)")->onButtonEvent(this, &ofApp::saveScreenshot);
    actions->addButton("Regenerate seed (R)")->onButtonEvent(this, &ofApp::regenerateSeed);
    actions->addFRM();
    ofxDatGuiLog::quiet();
    
    sliders->setVisible(false);
    toggles->setVisible(false);
    actions->setVisible(false);
    
    camera.disableMouseInput();
    
    noiseSeed = ofRandom(100.0);
    
    for(int i=0;i<maxTreeCount;i++){
        trees.push_back(ofVec2f(ofRandom(1,size-1),ofRandom(1,size-1)));
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //GROUND
    float noiseScale =  ofMap(density->getValue(), 0.0, 1.0, 0.01, 0.1);
    
    ofPixels &groundPixels = groundTex.getPixels();
    int w = groundTex.getWidth();
    int h = groundTex.getHeight();
    for(int y=0; y<h; y++) {
        for(int x=0; x<w; x++) {
            int i = y * w + x;
            float noiseValue1 = ofNoise(x * noiseScale, y * noiseScale, noiseSeed);
            float noiseValue2 = ofNoise(x/200.0, y/200.0, 200+noiseSeed);
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
    
    if( lastHeight != (float)height->getValue() || lastDensity != (float)density->getValue() || lastErosion != (float)erosion->getValue() || lastDilation != (float)dilation->getValue() ){
        regenerateGround();
        lastHeight = height->getValue();
        lastDensity = density->getValue();
        lastErosion = erosion->getValue();
        lastDilation = dilation->getValue();
    }
    //WATER
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
    
    
    //SNOW
    grayImage.resize(size, size);
    grayImage.blurHeavily();
    grayImage.threshold(climate->getValue()*255.0);
    grayImage.blurHeavily();
    grayImage.dilate();
    
    snowTex.setFromPixels(grayImage.getPixels());
    
    //CAMERA
    float angle = ofMap(rotation->getValue(), 0.0, 1.0, 0.0, 360.0)* PI / 180.0;
    float dist = ofMap(cameraDistance->getValue(), 0.0, 1.0, 400.0, 1200.0);
    float ca = ofMap(cameraAngle->getValue(), 0.0, 1.0, 0.0, 1200.0);
    
    camera.setPosition(dist * cos(angle), dist*sin(angle), ca);
    camera.setFov(ofMap(cameraFov->getValue(), 0.0, 1.0, 60.0, 120.0));
    camera.lookAt(ofVec3f(0.0, 0.0, 0.0), ofVec3f(0.0,0.0,1.0));
    
    //OSC
    while(receiver.hasWaitingMessages()){
        ofxOscMessage m;
        receiver.getNextMessage(m);
        if(m.getAddress() == "/height" && !heightToggle->getChecked()){
            height->setValue(m.getArgAsFloat(0));
        }else if(m.getAddress() == "/density" && !densityToggle->getChecked()){
            density->setValue(m.getArgAsFloat(0));
        }else if(m.getAddress() == "/erosion" && !erosionToggle->getChecked()){
            erosion->setValue(m.getArgAsFloat(0));
        }else if(m.getAddress() == "/dilation" && !dilationToggle->getChecked()){
            dilation->setValue(m.getArgAsFloat(0));
        }else if(m.getAddress() == "/groundColor" && !groundColorToggle->getChecked()){
            groundColor->setValue(m.getArgAsFloat(0));
        }else if(m.getAddress() == "/waterLevel" && !waterLevelToggle->getChecked()){
            waterLevel->setValue(m.getArgAsFloat(0));
        }else if(m.getAddress() == "/waterColor" && !waterColorToggle->getChecked()){
            waterColor->setValue(m.getArgAsFloat(0));
        }else if(m.getAddress() == "/waterOpacity" && !waterOpacityToggle->getChecked()){
            waterOpacity->setValue(m.getArgAsFloat(0));
        }else if(m.getAddress() == "/climate" && !climateToggle->getChecked()){
            climate->setValue(m.getArgAsFloat(0));
        }else if(m.getAddress() == "/treeAmount" && !treeAmountToggle->getChecked()){
            treeAmount->setValue(m.getArgAsFloat(0));
        }else if(m.getAddress() == "/treeType" && !treeTypeToggle->getChecked()){
            treeType->setValue(m.getArgAsFloat(0));
        }else if(m.getAddress() == "/treeColor" && !treeColorToggle->getChecked()){
            treeColor->setValue(m.getArgAsFloat(0));
        }else if(m.getAddress() == "/rotation" && !rotationToggle->getChecked()){
            rotation->setValue(m.getArgAsFloat(0));
        }else if(m.getAddress() == "/cameraAngle" && !cameraAngleToggle->getChecked()){
            cameraAngle->setValue(m.getArgAsFloat(0));
        }else if(m.getAddress() == "/cameraDistance" && !cameraDistanceToggle->getChecked()){
            cameraDistance->setValue(m.getArgAsFloat(0));
        }else if(m.getAddress() == "/cameraFov" && !cameraFovToggle->getChecked()){
            cameraFov->setValue(m.getArgAsFloat(0));
        }else{
            ofLog() << m.getAddress();
        }
    }
}
//--------------------------------------------------------------
void ofApp::drawScene(){
    
    ofEnableDepthTest();
    
    ofSetColor(255);
    
    groundTex.getTextureReference().bind();
    groundShader.begin();
    groundShader.setUniformTexture("tex1", snowTex, 2);
    //groundShader.setUniformTexture("tex2", treeFbo.getTextureReference(0), 1);
    //groundShader.setUniform1f("height", ofMap(height->getValue(), 0.0, 1.0, 5.0, 600.0));
    ofColor outGround = groundColor1;
    outGround.lerp(groundColor2, groundColor->getValue());
    groundShader.setUniform3f("color", outGround.r/255.0, outGround.g/255.0, outGround.b/255.0);
    
    ground.draw();
    groundShader.end();
    groundTex.getTextureReference().unbind();
    
    waterTex.getTextureReference().bind();
    waterShader.begin();
    waterShader.setUniform1f("height", 5.0);
    
    ofColor outWater = waterColor1;
    outWater.lerp(waterColor2, waterColor->getValue());
    
    waterShader.setUniform4f("color", outWater.r/255.0, outWater.g/255.0, outWater.b/255.0,
                             ofMap(waterOpacity->getValue(), 0.0, 1.0, 0.1, 0.85));
    
    ofPushMatrix();
    ofTranslate(0, 0, ofMap(waterLevel->getValue(), 0.0, 1.0, -300.0, 300.0));
    water.draw();
    ofPopMatrix();
    waterShader.end();
    waterTex.getTextureReference().unbind();
    
    
    //light.enable();
    drawTrees();
    //light.disable();
    ofDisableDepthTest();
    
}
//--------------------------------------------------------------
void ofApp::drawTrees(){
    ofColor outTree = treeColor1;
    outTree.lerp(treeColor2, treeColor->getValue());
    ofSetSphereResolution(3+8.0*treeType->getValue());
    float treeHeight = 10*treeType->getValue();
    float treeTrunkWeight = 5;
    for(int i=0;i<ofMap(treeAmount->getValue(),0.0,1.0,0, maxTreeCount); i++){
        ofNode node;
        float maxHeight = ofMap(height->getValue(), 0.0, 1.0, 5.0, 600.0);
        float x = trees[i].x-400.0;
        float y =trees[i].y-400.0;
        float z = (groundTex.getColor(trees[i].x/4.0, trees[i].y/4.0).r/255.0)*maxHeight;
        
        ofSetColor(20);
        ofSetLineWidth(treeTrunkWeight);
        ofDrawLine(x, y, z, x, y, z + treeHeight);
        
        ofSetColor(outTree);
        //ofNoFill();
        ofSetLineWidth(1);
        ofDrawSphere(x, y, z + treeHeight, 5.0);
        //ofSeedRandom(i);
        
        //ofDrawSphere(x+ofRandom(-3,3), y+ofRandom(-3,3), z + treeHeight + 1 + ofRandom(-1,1), 5.0);
        //ofDrawSphere(x+ofRandom(-3,3), y+ofRandom(-3,3), z + treeHeight - 1 + ofRandom(-1,1), 5.0);
        //node.setPosition(x, y, z);
        //node.draw();
    }
    ofSeedRandom(ofRandom(1000.0));
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetBackgroundColor(ofColor(210));
    camera.begin();
    drawScene();
    camera.end();
}
//--------------------------------------------------------------
void ofApp::regenerateGround(){
    //ofLog() << "regenerating ground";
    vector<ofVec3f> vertices = ground.getVertices();
    float maxHeight = ofMap(height->getValue(), 0.0, 1.0, 5.0, 600.0);
    //float ma
    for(int i=0;i<vertices.size();i++){
        ofVec3f vertex = ofVec3f(vertices[i].x, vertices[i].y, 0.0);
        float x = vertex.x/4.0+100.0;
        float y = vertex.y/4.0 + 100.0;
        
        float h = (groundTex.getColor(x, y).r/255.0)*maxHeight;
        if(x == 0 || y == 0 || x==200.0 || y==200.0){ //boczki
            h = -300.0;
        }
        vertex.z=h;
        ground.setVertex(i, vertex);
        ground.setTexCoord(i, ofVec2f(x, y));
    }
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
    }else if(key == 'R' || key == 'r'){
        noiseSeed = ofRandom(100.0);
        height->setValue(ofRandom(1));
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

//--------------------------------------------------------------
void ofApp::regenerateSeed(ofxDatGuiButtonEvent e)
{
    cout << "onButtonEvent: " << e.target->getLabel() << endl;
    noiseSeed = ofRandom(100.0);
    height->setValue(ofRandom(1));
    //regenerateGround();
}
