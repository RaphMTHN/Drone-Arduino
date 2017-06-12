import processing.serial.*;
import processing.opengl.*;
import toxi.geom.*;
import toxi.processing.*;
import controlP5.*;

// NOTE: requires ToxicLibs to be installed in order to run properly.
// 1. Download from http://toxiclibs.org/downloads
// 2. Extract into [userdir]/Processing/libraries
//    (location may be different on Mac/Linux)
// 3. Run and bask in awesomeness

// TODO : 
// 1. Add a "gyro calibrated" switch : needed to turn on the program

// GRAPHICS

ToxiclibsSupport gfx;
ControlP5 cp5;
Knob leftKnob;
Knob rightKnob;
Icon onSwitch;
Toggle calibSwitch, logSwitch;
Button emergency;
Textfield throttleConstField, throttleAmpField;
Textfield KpField, KiField, KdField;
Textfield consEchelonField;
PImage[] ledImages, emgImages;
float x;
// SERIAL

Serial port;

// processing packet (read)
int processingPacketLength = 13;

//                     { start,state,actual_time,thLeft,thRight,roll,end }
byte[] processingPacket = new byte[processingPacketLength];
int serialCount = 0;                 // current packet byte position
int synced = 0;
boolean packetToTreat = false;

// arduino packet (write)
int arduinoPacketLength = 15;

//                     {start,state,thConst,thAmp ,  Kp  ,  Ki  ,  Kd  ,  cons  , end }
byte[] arduinoPacket = { '$' ,  0  ,  0,0  , 0,0  , 0,0  , 0,0  , 0,0  , 0,0  , '&' };
boolean packetToWrite = false;

// log file
PrintWriter output;
boolean writeLog = false;
boolean packetToLog = false;

// GENERAL

byte state = 0;
float throttleConst;
float throttleAmp;
//
float cons;
//
float Kp=1, Ki=0, Kd=0.1;

//
float throttleLeft;
float throttleRight;
float roll;
int measure_time;
int init_measure_time;

void setup() {
  
  // GRAPHICS
  
  // setup window + toxiclib for graphics
  size(1200, 600, OPENGL);
  gfx = new ToxiclibsSupport(this);

  // setup lights and antialiasing
  lights();
  smooth();
      
  // setup gui controller
  cp5 = new ControlP5(this);
  
  // setup leftKnob
  leftKnob = cp5.addKnob("left thrust",0,100)
   .setSize(150,150)
   .setPosition(0,0)
   .setLock(true)
   .setViewStyle(leftKnob.ARC)
   .setAngleRange(2*PI-PI/6)
   .setStartAngle(PI/2 +PI/12)
  ;
  
  // setup rightKnob
  rightKnob = cp5.addKnob("right thrust",0,100)
   .setSize(150,150)
   .setPosition(0,0)
   .setLock(true)
   .setViewStyle(leftKnob.ARC)
   .setAngleRange(2*PI-PI/6)
   .setStartAngle(PI/2 +PI/12)
  ;
  
  // setup On/Off switch
  onSwitch = cp5.addIcon("onSwitch",10)
   .setPosition(0,0)
   .setSize(45,30)
   .setFont(createFont("fontawesome-webfont.ttf", 40))
   .setFontIcons(#00f205,#00f204)
   .setSwitch(true)
   .setColorBackground(color(255,100))
   .activateBy(1)
   ;
 
  // setup Calibration switch
  calibSwitch = cp5.addToggle("calibSwitch")
    .setPosition(0,0)
    .setSize(50,20)
    .setValue(true)
    .setMode(ControlP5.SWITCH)
   ;
  
  // setup log switch
  logSwitch = cp5.addToggle("logSwitch")
    .setPosition(0,0)
    .setSize(50,20)
    .setValue(true)
    .setMode(ControlP5.SWITCH)
  ;
 
  // setup Led Image  
  ledImages = new PImage[] {loadImage("led_01.png"),loadImage("led_11.png"),loadImage("led_13.png"),loadImage("led_12.png")};
  for(int i=0;i<4;i++) ledImages[i].resize(0,50);
  
  // setup Emergency button 
  emgImages = new PImage[] {loadImage("emergency_a.png"),loadImage("emergency_b.png"),loadImage("emergency_c.png")};
  for(int i=0;i<3;i++) emgImages[i].resize(0,100);
  
  emergency = cp5.addButton("emergency")
    .setValue(0)
    .setPosition(0,0)
    .setImages(emgImages)
    .activateBy(1)
    .updateSize()
  ;
   
  // map key for emergency : "E"
  cp5.mapKeyFor(new ControlKey() {public void keyEvent() {onSwitch.setOff(); state = 0;}}, 'E');
   
  // create 2 textfields for Throttle Command
  throttleAmpField = cp5.addTextfield("throttleAmpField", 0, 0, 50, 25);
  throttleConstField = cp5.addTextfield("throttleConstField", 0, 0, 50, 25);
  
  // create 3 textfields for PID
  KpField = cp5.addTextfield("KpField", 0, 0, 50, 25);
  KiField = cp5.addTextfield("KiField", 0, 0, 50, 25);
  KdField = cp5.addTextfield("KdField", 0, 0, 50, 25);
  
  // create 1 textfield for consEchelon
  consEchelonField = cp5.addTextfield("consEchelonField",0,0,50,25);
  
  // SERIAL
  
  // open the serial port
  port = new Serial(this, "COM3", 115200);
}

void draw() {
  
  // SERIAL
  
  // data from processing are sent when a button is pressed
  
  if(packetToWrite){
    // prepare the packet
    println("packet preparing ...");
    arduinoPacket[1] = state;
    arduinoPacket[2] = floatToByte1(throttleConst);
    arduinoPacket[3] = floatToByte2(throttleConst);
    arduinoPacket[4] = floatToByte1(throttleAmp);
    arduinoPacket[5] = floatToByte2(throttleAmp);
    arduinoPacket[6] = floatToByte1(Kp);
    arduinoPacket[7] = floatToByte2(Kp);
    arduinoPacket[8] = floatToByte1(Ki);
    arduinoPacket[9] = floatToByte2(Ki);
    arduinoPacket[10] = floatToByte1(Kd);
    arduinoPacket[11] = floatToByte2(Kd);
    arduinoPacket[12] = floatToByte1(cons+180);
    arduinoPacket[13] = floatToByte2(cons+180);
    
    // send the packet
    port.write(arduinoPacket);
    println("state = " + (int)state + " | TC = " + throttleConst + " | TA = " + throttleAmp);
    println("packet sent !");
    packetToWrite = false;
  }
  
  /* ------------- */

  readData();

  
  // GRAPHICS
  
  //draw background
  
  background(color(240));
  drawBackground();
  
  /* ------------- */
  
  // translate everything to the middle of the viewport
  pushMatrix();
  translate(width / 2, height / 2);

  /* ------------- */
  
  // rotate the referential
  
  // 3-step rotation from yaw/pitch/roll angles (gimbal lock!)
  // ...and other weirdness I haven't figured out yet
  //rotateY(-ypr[0]);
  //rotateZ(-ypr[1]);
  //rotateX(-ypr[2]);
    
  rotateZ(-roll/180*PI);
  
  /* ------------- */
  
  // draw the plane
  
  // draw main body in red
  fill(255, 0, 0, 200);
  box(10, 10, 200);
    
  // draw front-facing tip in blue
  fill(0, 0, 255, 200);
  pushMatrix();
  translate(0, 0, -120);
  rotateX(PI/2);
  drawCylinder(0, 20, 20, 8);
  popMatrix();
    
  // draw wings and tail fin in green
  fill(0, 255, 0, 200);
  beginShape(TRIANGLES);
  vertex(-100,  2, 30); vertex(0,  2, -80); vertex(100,  2, 30);  // wing top layer
  vertex(-100, -2, 30); vertex(0, -2, -80); vertex(100, -2, 30);  // wing bottom layer
  vertex(-2, 0, 98); vertex(-2, -30, 98); vertex(-2, 0, 70);  // tail left layer
  vertex( 2, 0, 98); vertex( 2, -30, 98); vertex( 2, 0, 70);  // tail right layer
  endShape();
  beginShape(QUADS);
  vertex(-100, 2, 30); vertex(-100, -2, 30); vertex(  0, -2, -80); vertex(  0, 2, -80);
  vertex( 100, 2, 30); vertex( 100, -2, 30); vertex(  0, -2, -80); vertex(  0, 2, -80);
  vertex(-100, 2, 30); vertex(-100, -2, 30); vertex(100, -2,  30); vertex(100, 2,  30);
  vertex(-2,   0, 98); vertex(2,   0, 98); vertex(2, -30, 98); vertex(-2, -30, 98);
  vertex(-2,   0, 98); vertex(2,   0, 98); vertex(2,   0, 70); vertex(-2,   0, 70);
  vertex(-2, -30, 98); vertex(2, -30, 98); vertex(2,   0, 70); vertex(-2,   0, 70);
  endShape();
    
  popMatrix();
  
  /* ------------- */

  // draw the GUI
  // (ex : if the window is resized)

  // Title :
  
  //write the title of the window
  textSize(40);
  textAlign(CENTER);
  fill(0, 102, 153);
  text("Quadcoptère - GUI",width/2, height/2 - 225);

  // On/Off switch :
  
  // set the position of the switch
  onSwitch.setPosition(width/2 - 450, height/2 - 300 + 60);
  
  // update the image of the led
  if(state==0) image(ledImages[0],width/2 -450 + 100,height/2 - 300 +50);
  if(state==1) image(ledImages[3],width/2 -450 + 100,height/2 - 300 +50);
  if(state==2) image(ledImages[2],width/2 -450 + 100,height/2 - 300 +50);
  if(state==3) image(ledImages[1],width/2 -450 + 100,height/2 - 300 +50);
  
  // manage lock
  if(state == 0 || state == 3){
    onSwitch.setLock(true);
  } else {
    onSwitch.setLock(false);
  }
  
  //write the name of the switch
  textSize(20);
  textAlign(CENTER);
  fill(0, 102, 153);
  text("ON/OFF",width/2 - 500, height/2 - 218);  
  
  // Calibration switch :
  
  // set the position of the switch
  calibSwitch.setPosition(width/2 - 450, height/2 - 200 + 10);

  // Log switch :
  
  // set the position of the switch
  logSwitch.setPosition(width/2 - 450, height/2 - 150 + 10);  
  
  // Emergency button : 
  
  // set the position of the button
  emergency.setPosition(width/2 + 375 - 50, height/2 - 300 + 25);
  
  // manage lock
  if(state == 0){
    emergency.setLock(true);
  } else {
    emergency.setLock(false);
  }
  
  // Thrust indicators :
  
  // set the position of the knobs
  leftKnob.setPosition(width/2 - 450, height/2 -75);
  rightKnob.setPosition(width/2 + 300, height/2 -75);

  // update the values of the knobs
  leftKnob.setValue(throttleLeft);
  rightKnob.setValue(throttleRight);  
  
  // Thrust constants input fields : 
  
  // draw throttle const input field
  textSize(13);
  textAlign(LEFT,CENTER);
  fill(0,0,0);
  text("throttle constant",width/2 - 150 + 0,height/2 +150 + 50);
  throttleConstField.setPosition(width/2 - 150 + 0, height/2 + 150 + 75);
  textSize(15);
  textAlign(CENTER,CENTER);
  fill(0, 102, 153);
  text(throttleConst,width/2 - 150 + 100,height/2 + 150 + 85);
    
  // draw throttle amp input field
  textSize(13);
  textAlign(LEFT,CENTER);
  fill(0,0,0);
  text("throttle amplitude",width/2 + 0,height/2 +150 + 50);
  throttleAmpField.setPosition(width/2 + 0, height/2 + 150 + 75);
  textSize(15);
  textAlign(CENTER,CENTER);
  fill(0, 102, 153);
  text("± " + throttleAmp,width/2 + 100,height/2 + 150 + 85);
    
  // manage lock
  if(state == 1){
    throttleConstField.setLock(false);
    throttleAmpField.setLock(false);
  } else {
    throttleConstField.setLock(true);
    throttleAmpField.setLock(true);
  }
  
 
  // PID constants input fields : 
  
  // draw Kp input field
  textSize(13);
  textAlign(LEFT,CENTER);
  fill(0,0,0);
  text("Kp constant",width/2 - 450 - 100,height/2 + 50 + 85);
  KpField.setPosition(width/2 - 450 + 0, height/2 + 50 + 75);
  textSize(15);
  textAlign(CENTER,CENTER);
  fill(0, 102, 153);
  text("" + Kp,width/2 - 450 + 100,height/2 + 50 + 85);
    
  // draw I const input field
  textSize(13);
  textAlign(LEFT,CENTER);
  fill(0,0,0);
  text("Ki constant",width/2 - 450 - 100,height/2 + 100 + 85);
  KiField.setPosition(width/2 - 450 + 0, height/2 + 100 + 75);
  textSize(15);
  textAlign(CENTER,CENTER);
  fill(0, 102, 153);
  text("" + Ki,width/2 - 450 + 100,height/2 + 100 + 85);
  
  // draw I const input field
  textSize(13);
  textAlign(LEFT,CENTER);
  fill(0,0,0);
  text("Kd constant",width/2 - 450 - 100,height/2 + 150 + 85);
  KdField.setPosition(width/2 - 450 + 0, height/2 + 150 + 75);
  textSize(15);
  textAlign(CENTER,CENTER);
  fill(0, 102, 153);
  text("" + Kd,width/2 - 450 + 100,height/2 + 150 + 85);
  
    
  // manage lock
  
  if(state == 1){
    KpField.setLock(false);
    KiField.setLock(false);
    KdField.setLock(false);
  } else {
    KpField.setLock(true);
    KiField.setLock(true);
    KdField.setLock(true);
  }
  
  
  // consigne input Field : 
  
  // draw consEchelon input field
  textSize(13);
  textAlign(LEFT,CENTER);
  fill(0,0,0);
  text("Consigne Echelon (°)",width/2 + 450 - 150,height/2 + 50 + 85);
  consEchelonField.setPosition(width/2 + 450 + 0, height/2 + 50 + 75);
  textSize(15);
  textAlign(CENTER,CENTER);
  fill(0, 102, 153);
  text("" + cons,width/2 + 450 + 100,height/2 + 50 + 85);
   
  // manage lock
  
    if(state == 1 || state == 2){
    consEchelonField.setLock(false);
    consEchelonField.setLock(false);
    consEchelonField.setLock(false);
  } else {
    consEchelonField.setLock(true);
    consEchelonField.setLock(true);
    consEchelonField.setLock(true);
  }
  
  /* ------------- */
  
}

// MISC FUNCTIONS

// GRAPHICS

// draw the background of the GUI
void drawBackground(){
  noStroke();
  fill(color(#E4E7F7));
  rect(0,0,width,height/2 - 150);
  rect(0,height/2 - 150,width/2 - 150,height/2 + 150);
  rect(width/2 + 150,height/2 - 150,width,height/2 + 150);
  rect(0,height/2 + 150, width, height);
  stroke(color(0));
}

// draw a cylinder for the plane
void drawCylinder(float topRadius, float bottomRadius, float tall, int sides) {
    float angle = 0;
    float angleIncrement = TWO_PI / sides;
    beginShape(QUAD_STRIP);
    for (int i = 0; i < sides + 1; ++i) {
        vertex(topRadius*cos(angle), 0, topRadius*sin(angle));
        vertex(bottomRadius*cos(angle), tall, bottomRadius*sin(angle));
        angle += angleIncrement;
    }
    endShape();
    
    // If it is not a cone, draw the circular top cap
    if (topRadius != 0) {
        angle = 0;
        beginShape(TRIANGLE_FAN);
        
        // Center point
        vertex(0, 0, 0);
        for (int i = 0; i < sides + 1; i++) {
            vertex(topRadius * cos(angle), 0, topRadius * sin(angle));
            angle += angleIncrement;
        }
        endShape();
    }
  
    // If it is not a cone, draw the circular bottom cap
    if (bottomRadius != 0) {
        angle = 0;
        beginShape(TRIANGLE_FAN);
    
        // Center point
        vertex(0, tall, 0);
        for (int i = 0; i < sides + 1; i++) {
            vertex(bottomRadius * cos(angle), tall, bottomRadius * sin(angle));
            angle += angleIncrement;
        }
        endShape();
    }
}

// manage the On/Off switch
void onSwitch(boolean isOn) {
  if(isOn) state = 2;
  else     state = 1;
  // send arduinoPacket
  packetToWrite = true;
}

// manage the Calib switch
void calibSwitch(boolean isOff) {
  if(!isOff){
    state = 1;
    calibSwitch.hide();
  }
  else     state = 3;
  // send arduinoPacket
  packetToWrite = true;
}

// manage the Log switch
void logSwitch(boolean isOff) {
  if(isOff) {
    writeLog = false;
  } else {
    writeLog = true;
    String fileName = "log/" + day() + "-" + month() + "-" + "-" + hour() +"-" + minute() + "[" + Kp + "," + Ki + "," + Kd + "].csv";
    output = createWriter(fileName);
    output.println("t (s);roll (deg);cons (deg);TL (%);TR (%)");
    init_measure_time = measure_time; 
  }
}

// manage the emergency button
void emergency(){
  onSwitch.setOff();
  state = 0;
  // send arduinoPacket
  packetToWrite = true;
}

// manage the throttleConst inputField
public void throttleConstField(String theValue) {
  float fValue = float(theValue);
  if(Float.isNaN(fValue)) fValue = 0;
  if(fValue > 100) fValue = 100;
  if(fValue < 0) fValue = 0;
  if(throttleAmp > min(100 - fValue,fValue)) throttleAmp = min(100 - fValue,fValue);
  throttleConst = fValue;
}

// manage the throttleAmp inputField
public void throttleAmpField(String theValue) {
  float fValue = float(theValue);
  if(Float.isNaN(fValue)) fValue = 0;
  if(fValue > 50) fValue = 50;
  if(fValue < 0) fValue = 0;
  if(fValue > min(100 - throttleConst,throttleConst)) fValue = min(100 - throttleConst,throttleConst);
  throttleAmp = fValue;
}

// manage the Kp inputField
public void KpField(String theValue) {
  float fValue = float(theValue);
  if(Float.isNaN(fValue)) fValue = 0;
  if(fValue > 10) fValue = 10;
  if(fValue < 0) fValue = 0;
  Kp = fValue;
}

// manage the Ki inputField
public void KiField(String theValue) {
  float fValue = float(theValue);
  if(Float.isNaN(fValue)) fValue = 0;
  if(fValue > 10) fValue = 10;
  if(fValue < 0) fValue = 0;
  Ki = fValue;
}

// manage the Kd inputField
public void KdField(String theValue) {
  float fValue = float(theValue);
  if(Float.isNaN(fValue)) fValue = 0;
  if(fValue > 10) fValue = 10;
  if(fValue < 0) fValue = 0;
  Kd = fValue;
}

// manage the consEchelon inputField
public void consEchelonField(String theValue) {
  float fValue = float(theValue);
  if(Float.isNaN(fValue)) fValue = 0;
  if(fValue > 25) fValue = 25;
  if(fValue < -25) fValue = -25;
  cons = fValue;
  // envoi du paquet contenant la nouvelle consigne
  if(state==2) packetToWrite = true;
}


// SERIAL

// READ DATA FROM ARDUINO
/*
Le fonctionnement du port serial côté processing reste mystérieux : afin de prendre en compte l'intégralité
des données reçues, on est obligé de traiter les données à l'intérieur de la boucle while(port.available()>0 car 
les paquets arrivent à plusieurs. Sinon, seul le premier paquet est lu par le programme...
*/

// read incoming data

void readData(){
  while(port.available() > 0) {
    byte ch = (byte)port.read();
    if (synced == 0 && ch != '$') {
      // initial synchronization - also used to resync/realign if needed
    } else {
      synced = 1;
     
      if (serialCount == processingPacketLength-1 && ch != '&')  {
        serialCount = 0;
        synced = 0;
      }
      if (serialCount > 0 || ch == '$') {
        if(serialCount>0 && serialCount<processingPacketLength-1){
          processingPacket[serialCount] = ch;
        }
        serialCount++;
        if (serialCount == processingPacketLength) {
          packetToTreat = true;              
          serialCount = 0; // restart packet byte position
        }
      }
    }
    treatData();
  }  
}

// TREAT DATA READ IN PREVIOUS FUNCTION
void treatData(){
  
  // treat the data
  
  if(packetToTreat){
    state = processingPacket[1];
    measure_time = bytesToInt(processingPacket[2],processingPacket[3],processingPacket[4],processingPacket[5]);
    throttleLeft = bytesToFloat(processingPacket[6],processingPacket[7]);
    throttleRight = bytesToFloat(processingPacket[8],processingPacket[9]);
    roll = bytesToFloat(processingPacket[10],processingPacket[11])-180;
    println("state = " + (int)state + " | time = " + measure_time + " | TL = " + throttleLeft + " | TR = " + throttleRight + " | roll = " + roll + " | cons = " + cons);
    if(writeLog && state==2) packetToLog = true;
    packetToTreat = false;
  }
  
  // write in log file
  
  if(packetToLog){
    output.println( (measure_time - init_measure_time)/1e6 + ";" + roll  + ";" + cons + ";" + throttleLeft + ";" + throttleRight);
    packetToLog = false;
  }

}

// BINARY OPERATIONS

// convert a float to the upper byte
byte floatToByte1(float fval){
  return (byte)(floor(fval*100)>>8);
}

// convert a float to the lower byte
byte floatToByte2(float fval){
  return (byte)(floor(fval*100));
}

// convert the upper byte and the lower byte to the float
float bytesToFloat(byte byte1, byte byte2){
  return ((float)(char(byte1) << 8 | char(byte2)))/100;
}

// convert the upper byte and the lower byte to the int
int bytesToInt(byte byte1, byte byte2, byte byte3, byte byte4){
  return ( (int)(char(byte1) << 24 | char(byte2) << 16 | char(byte3) << 8 | char(byte4)) );
}