import netP5.*;
import oscP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;

float[] values = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
String[] labels = {"height","density","erosion","dilation", "groundColor","waterLevel","waterColor","waterOpacity","climate","treeAmount","treeType","treeColor", "rotation","cameraAngle","cameraDistance","cameraFov"}; 

void setup(){
  size(800, 200);
  frameRate(25);
  oscP5 = new OscP5(this,12000);
  myRemoteLocation = new NetAddress("127.0.0.1",12345);
}
void draw(){
  background(255);
  noStroke();
  fill(127);
  setValues();
  sendMessages();
  
  for(int i=0;i<values.length;i++){
    rect(i*width/values.length,height-values[i]*height, width/values.length,values[i]*height);
  }
}
void setValues(){
  for(int i=0;i<values.length;i++){
    values[i] = noise(i,frameCount/100.0);
  }
  //printArray(values);
}
void sendMessages(){
  for(int i=0;i<values.length;i++){
    OscMessage myMessage = new OscMessage("/" + labels[i]);
    myMessage.add(values[i]);
    oscP5.send(myMessage, myRemoteLocation);
  }
}