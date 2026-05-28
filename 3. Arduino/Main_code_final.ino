/*
==============================================
=============== Projet Capteur ===============
==============================================
Auteurs: Romain PODGORSKI, Yanis ROBINSON
Last update: 29/05/2025
*/

//====================================================
//==================== Libraries =====================
//====================================================

#include <SoftwareSerial.h>
#define rxPin 7 //Broche 7 en tant que RX, à raccorder sur TX du HC-05
#define txPin 8 //Broche 8 en tant que TX, à raccorder sur RX du HC-05
#define baudrate 9600
SoftwareSerial mySerial(rxPin ,txPin); //Définition du software serial
#include <SPI.h>
#include <Adafruit_SSD1306.h>


//====================================================
//============= Declarations de variables ============
//====================================================

//Partie potentiomètre digital
const byte csPin           = 10;      
const int  maxPositions    = 256;     
const long rAB             = 92500;   
const byte rWiper          = 125;
const byte pot0            = 0x11;    
const byte pot0Shutdown    = 0x21;    
int V_dig;
double V_anal;
int potValue;

//Partie encodeur rotatoire
#define encoderPin_SW 5
#define encoderPin_CLK 2
#define encoderPin_DT 4
int buttonState; 
int lastButtonState = HIGH; 
int encodIncr = 0;
long lastDebounceTime = 0; 
long debounceDelay = 50; 
volatile int encoderPos = 0;
volatile int lastButtonValue =0;
long lastPosTime = 0; 
long posDelay = 100; 

//Partie ecran OLED
#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)
byte tailleDeCaractereIN = 2;
bool measActive = false;
int lastMenuState = -1;

Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

//Variables ampli
const float R1 = 100000.0;
const float R3 = 100000.0;
float resistanceWB = 10000.0 ;
const float R5 = 10000.0;
const float Vcc = 5.0;
const float R_DIV = 47000.0;

//Variables de mesure 
float graphiteMe = 0.0;
float flexMe = 0.0;

//====================================================
//====================== Setup =======================
//====================================================


void setup() {
  
  //Partie bluetooth
  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
    
  mySerial.begin(baudrate);
  Serial.begin(baudrate);

  //Partie potentiomètre digital
  digitalWrite(csPin, HIGH);        
  pinMode(csPin, OUTPUT);           

  //Partie encodeur rotatoire
  pinMode(encoderPin_SW, INPUT);
  SPI.begin();                
  
  pinMode(encoderPin_CLK, INPUT); 
  digitalWrite(encoderPin_CLK, HIGH);       

  pinMode(encoderPin_DT, INPUT); 
  digitalWrite(encoderPin_DT, HIGH);       

  attachInterrupt(0, get_encodPos, RISING); //interruption sur la broche 2 (CLK) pour la gestion de la position de l'encodeur rotatoire



	//Partie ecran
	set_OLED_screen(tailleDeCaractereIN); // Initialisation de l'écran OLED et affichage du menu
  ecranOLED.display();                            // Transfert le buffer à l'écran
  delay(2000);
}


//====================================================
//==================== Main Loop =====================
//====================================================


void loop(){

  get_encodButton(); // Récupération de l'état du bouton de l'encodeur rotatoire
  OLED_manage_menu(); // Gestion de l'affichage du menu sur l'écran OLED en fonction de la position de l'encodeur rotatoire et des appuis sur son bouton
  if (abs(encoderPos % 3) == 1 && measActive) { //On fait les mesures que si on est dans le menu "Meas on" et que les mesures sont actives
    delay(200);
    // Récupération des mesures de résistance du graphite et du flex, affichage dans le port série
    Serial.print("Graph meas : ");
    graphiteMe = graphiteMeas();
    Serial.println(graphiteMe);
    Serial.print("Flex meas : ");
    flexMe = flexMeas();
    Serial.println(flexMe);
    // Envoi des mesures de résistance du graphite et du flex à l'appli via bluetooth, au format "graphite;flex\n"
    mySerial.print(graphiteMe, 2);
    mySerial.print(";");
    mySerial.print(flexMe, 2);
    mySerial.print("\n");
    
  }
}


//====================================================
//==================== Functions =====================
//====================================================

//================== Fonction potentiomètre digital ====================

void setPotWiper(int addr, int pos) {  // Règlage du potentiomètre digital et récupération de la résistance correspondante
  pos = constrain(pos, 0, 255);            
  digitalWrite(csPin, LOW);                
  SPI.transfer(addr);                     
  SPI.transfer(pos);
  digitalWrite(csPin, HIGH);              

  resistanceWB = ((rAB * pos) / maxPositions ) + rWiper;
  Serial.print("Wiper position: ");
  Serial.print(pos);
  Serial.print(" Resistance: ");
  Serial.print(resistanceWB);
  Serial.println(" ohms");
}

//=============== Fonctions de mesures des résistances =================
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

//==================== Fonctions encodeur rotatoire ======================

void get_encodButton(){   // Récupération de l'état du bouton de l'encodeur rotatoire, avec un debouncing logiciel
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

void get_encodPos(){ // Récupération de la position de l'encodeur rotatoire, avec un debouncing logiciel
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

//===================== Fonction de calibration =======================
void calibration(){   // Recherche de la position du potentiomètre digital pour obtenir une tension de 2.5V à l'entrée de l'amplificateur, et envoi de la résistance correspondante à l'appli via bluetooth
  Serial.println("Calibration en cours");
  setPotWiper(pot0, potValue); 
  delay(50);
  V_dig = analogRead(A0);
  V_anal = V_dig*5.0/1023.0;
  Serial.print("V_anal initial : ");
  Serial.println(V_anal);
  while (V_anal>2.5 && potValue < 256){ // S'arrete si la tension est inférieure à 2.5V ou si on a atteint la position maximale du potenti
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


//================== Fonctions gestion ecran OLED ====================
void set_OLED_screen(byte tailleDeCaractere){ // Initialisation de l'écran OLED et affichage du menu
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

void OLED_InverseColor(bool Inverse) { // Permet d'inverser les couleurs du texte et du fond pour faire ressortir le menu sélectionné
  if (Inverse == true) {
    ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);       // (Couleur du texte en noir et couleur du fond en blanc)
  }
  else {
    ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  }
}

void OLED_manage_menu(){ // Fonciton principale de gestion du menu sur l'écran OLED, qui affiche le menu en fonction de la position de l'encodeur rotatoire et des appuis sur son bouton, et qui envoie les commandes correspondantes à l'appli via bluetooth
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

