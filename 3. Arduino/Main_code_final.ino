//partie bluetooth
#include <SoftwareSerial.h>
#define rxPin 7 //Broche 7 en tant que RX,   raccorder sur TX du HC-05
#define txPin 8 //Broche 8 en tant que TX,   raccorder sur RX du HC-05
#define baudrate 9600
SoftwareSerial mySerial(rxPin ,txPin); //D finition du software serial


//partie pot digital
#include <SPI.h>

const byte csPin           = 10;      // MCP42100 chip select pin
const int  maxPositions    = 256;     // wiper can move from 0 to 255 = 256 positions
const long rAB             = 92500;   // 100k pot resistance between terminals A and B, 
                                      // mais pour ajuster au multimètre, je mets 92500
const byte rWiper          = 125;     // 125 ohms pot wiper resistance
const byte pot0            = 0x11;    // pot0 addr // B 0001 0001
const byte pot0Shutdown    = 0x21;    // pot0 shutdown // B 0010 0001
int V_dig;
double V_anal;
int potValue;

//encodeur rotatoire
#define encoderPin_SW 5
#define encoderPin_CLK 2
#define encoderPin_DT 4
int buttonState; // the current reading from the input pin
int lastButtonState = HIGH; // the previous reading from the input pin
int encodIncr = 0;
long lastDebounceTime = 0; // the last time the output pin was toggled
long debounceDelay = 50; // the debounce time; increase if the output flickers
volatile int encoderPos = 0;
volatile int lastButtonValue =0;
long lastPosTime = 0; // the last time the output pin was toggled
long posDelay = 100; // the debounce time; increase if the output flickers

//partie ecran
#include <Adafruit_SSD1306.h>

#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)
byte tailleDeCaractereIN = 2;
bool measActive = false;
int lastMenuState = -1;

Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

//variables ampli
const float R1 = 100000.0;
const float R3 = 100000.0;
float resistanceWB = 10000.0 ;
const float R5 = 10000.0;
const float Vcc = 5.0;
const float R_DIV = 47000.0;

//variables de mesure 
float graphiteMe = 0.0;
float flexMe = 0.0;


void setup() {
  
  //partie bluetooth
  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
    
  mySerial.begin(baudrate);
  Serial.begin(baudrate);

  //partie pot digital
  digitalWrite(csPin, HIGH);        // chip select default to de-selected
  pinMode(csPin, OUTPUT);           // configure chip select as output

  //partie encodeur rotatoire
  pinMode(encoderPin_SW, INPUT);
  SPI.begin();                //à modifier pour pouvoir gérer le gain 
  
  pinMode(encoderPin_CLK, INPUT); 
  digitalWrite(encoderPin_CLK, HIGH);       // turn on pullup resistor

  pinMode(encoderPin_DT, INPUT); 
  digitalWrite(encoderPin_DT, HIGH);       // turn on pullup resistor

  attachInterrupt(0, get_encodPos, RISING);



	//partie ecran
	set_OLED_screen(tailleDeCaractereIN);
  ecranOLED.display();                            // Transfert le buffer à l'écran
  delay(2000);

  //calibration();
}


void loop(){

  get_encodButton();
  OLED_manage_menu();
  if (abs(encoderPos % 3) == 1 && measActive) {
    delay(200);
    Serial.print("Graph meas : ");
    graphiteMe = graphiteMeas();
    Serial.println(graphiteMe);
    Serial.print("Flex meas : ");
    flexMe = flexMeas();
    //Serial.println(flexMe);
    mySerial.print(graphiteMe, 2);
    mySerial.print(";");
    mySerial.print(flexMe, 2);
    mySerial.print("\n");
    //mySerial.println("1.23;4.56");
    
  }
/*
  //partie bluetooth
      int i = 0; 
	char someChar[32] ={0};
	//when characters arrive over the serial port...

	while (Serial.available()) {
	   do{
		someChar[i++] = Serial.read();
		delay(3);		
	   }while (Serial.available() > 0);
	   
	   //mySerial.println(someChar); 
	   Serial.println(someChar); 
	}
	while (mySerial.available()) {
		Serial.print((char)mySerial.read());
	}
*/
}



void setPotWiper(int addr, int pos) {
  pos = constrain(pos, 0, 255);            // limit wiper setting to range of 0 to 255
  digitalWrite(csPin, LOW);                // select chip
  SPI.transfer(addr);                      // configure target pot with wiper position
  SPI.transfer(pos);
  digitalWrite(csPin, HIGH);               // de-select chip

  // print pot resistance between wiper and B terminal
  resistanceWB = ((rAB * pos) / maxPositions ) + rWiper;
  Serial.print("Wiper position: ");
  Serial.print(pos);
  Serial.print(" Resistance: ");
  Serial.print(resistanceWB);
  Serial.println(" ohms");
}


float graphiteMeas(){
  float V_ADC_graph = analogRead(A0);
  float V_graph = V_ADC_graph*5.0/1023.0;
  float R_graph = (1.0 + R3/resistanceWB)*(R1*Vcc/V_graph)-R5-R1;
  return R_graph;
}

float flexMeas(){
  float V_ADC_flex = analogRead(A2);
  float V_flex = V_ADC_flex*5.0/1023.0;
  float R_flex = R_DIV * ( Vcc/V_flex - 1.0);  
  return R_flex;
}


void get_encodButton(){
  int reading = digitalRead(encoderPin_SW);
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // reset the debouncing timer
  }
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if ((reading == LOW) && (reading != buttonState)) { 
      encodIncr++; 
    }
    buttonState = reading;
  }
  lastButtonState = reading;
}

void get_encodPos(){
  static int prev_CLK = HIGH ;
  int new_CLK = digitalRead(encoderPin_CLK);
  if (new_CLK == HIGH && new_CLK != prev_CLK) {
    if (digitalRead(encoderPin_DT) != new_CLK){
      encoderPos-- ;
    }
    else{
      encoderPos++ ;

    }
  }
  prev_CLK = new_CLK;
}


void calibration(){
  potValue = 0;
  Serial.println("Calibration en cours");
  setPotWiper(pot0, potValue); 
  delay(50);
  V_dig = analogRead(A0);
  V_anal = V_dig*5.0/1023.0;
  Serial.print("V_anal initial : ");
  Serial.println(V_anal);
  while (V_anal>2.5 && potValue < 256){
    setPotWiper(pot0, potValue); 
    potValue = potValue + 1;
    V_dig = analogRead(A0);
    V_anal = V_dig*5.0/1023.0;
    Serial.print("V_anal : ");
    Serial.println(V_anal);
  }
  mySerial.println(resistanceWB);  // Envoi de la valeur du potentiomètre à l'appli
  Serial.println("Fin calibration");
}

void set_OLED_screen(byte tailleDeCaractere){
	if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    while(1);
	ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
  ecranOLED.setTextSize(tailleDeCaractere);
  ecranOLED.setCursor(0, 0);
  ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  ecranOLED.println("Main Menu:");
  OLED_InverseColor(true);
  ecranOLED.print(1);
  ecranOLED.println(" >Set pot");
  OLED_InverseColor(false);
  ecranOLED.print(2);
  ecranOLED.println(" >Meas on");
  ecranOLED.print(3);
  ecranOLED.println(" >Meas off");
  ecranOLED.display();
}

void OLED_InverseColor(bool Inverse) {
  if (Inverse == true) {
    ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);       // (Couleur du texte en noir et couleur du fond en blanc)
  }
  else {
    ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  }
}

void OLED_manage_menu(){
  int currentState = abs(encoderPos % 3);

  switch (currentState){
    case 0 :
      ecranOLED.clearDisplay();
      ecranOLED.setTextSize(2);
      ecranOLED.setCursor(0, 0);
      ecranOLED.println("Main Menu:");
      OLED_InverseColor(true);
      ecranOLED.print(1);
      ecranOLED.println(" >Set pot");
      OLED_InverseColor(false);
      ecranOLED.print(2);
      ecranOLED.println(" >Meas on");
      ecranOLED.print(3);
      ecranOLED.println(" >Meas off");
      ecranOLED.display();
      if (encodIncr != lastButtonValue){
        Serial.println("case1");
        mySerial.println(1);
        calibration();
        lastButtonValue = encodIncr;
      }
      break;
    case 1 :
      ecranOLED.clearDisplay();
      ecranOLED.setTextSize(2);
      ecranOLED.setCursor(0, 0);
      ecranOLED.println("Main Menu:");
      ecranOLED.print(1);
      ecranOLED.println(" >Set pot");
      OLED_InverseColor(true);
      ecranOLED.print(2);
      ecranOLED.println(" >Meas on");
      OLED_InverseColor(false);
      ecranOLED.print(3);
      ecranOLED.println(" >Meas off");
      ecranOLED.display();
      
      if (encodIncr != lastButtonValue){
        mySerial.println(2); 
        Serial.println("case2");
        measActive = true; 
        lastButtonValue = encodIncr;
      }
      break;
    case 2 :
      ecranOLED.clearDisplay();
      ecranOLED.setTextSize(2);
      ecranOLED.setCursor(0, 0);
      ecranOLED.println("Main Menu:");
      ecranOLED.print(1);
      ecranOLED.println(" >Set pot");
      ecranOLED.print(2);
      ecranOLED.println(" >Meas on");
      OLED_InverseColor(true);
      ecranOLED.print(3);
      ecranOLED.println(" >Meas off");
      OLED_InverseColor(false);
      ecranOLED.display();
      if (encodIncr != lastButtonValue){
        mySerial.println(3);
        measActive = false; 
        Serial.println("case3");
        lastButtonValue = encodIncr;
      }
      break;
  }
}

