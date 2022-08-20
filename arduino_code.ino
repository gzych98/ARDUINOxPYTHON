#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>

char userInput;
int userDataPyTk;
Servo serwomechanizm; 
double currentTemp; 
int setTemp;

OneWire oneWire(A5);
DallasTemperature sensors(&oneWire);

void setup(){
  Serial.begin(9600);                        
  pinMode(LED_BUILTIN, OUTPUT);
  sensors.begin();                            // initialize temp sensor
  serwomechanizm.attach(9);                   // servo attached to pin 9
}

void getUserData(){
  String userDataUpdate;  
  delay(500); // Need to wait for pyserial send time 
  userDataUpdate = Serial.readString();  
  userDataPyTk = userDataUpdate.toInt();  
}

void loop(){

sensors.requestTemperatures();                // read current temp
Serial.print("Aktualna temperatura: ");
currentTemp = sensors.getTempCByIndex(0);
Serial.println(currentTemp); 

double var = currentTemp/setTemp;
        if(var > 2) {  
          serwomechanizm.write(90);
          digitalWrite(LED_BUILTIN, LOW);
        } // close valve
        else if(var < 1) {
          serwomechanizm.write(0); 
          digitalWrite(LED_BUILTIN, HIGH);          
        } // open valve
        else {
          int range = 90 * (var-1);
          serwomechanizm.write(range);
        } 

if(Serial.available()> 0){     
    userInput = Serial.read();                // read user input
      if(userInput == 'o'){                   // open valve             
        digitalWrite(LED_BUILTIN, HIGH); 
        serwomechanizm.write(0);
        setTemp = currentTemp*3;
      }
      if(userInput == 'x'){                   // close valve
       digitalWrite(LED_BUILTIN, LOW);
       serwomechanizm.write(90);  
       setTemp = currentTemp*0.5;       
      }
      if(userInput == 'b'){                   // set temperature
        getUserData();
        setTemp = userDataPyTk;

        double var = currentTemp/setTemp;
        if(var > 2) {    
          serwomechanizm.write(90);
        } // close valve
        else if(var < 1) {
          serwomechanizm.write(0);           
        } // open valve
        else {
          int range = 90 * (var-1);
          serwomechanizm.write(range);
        } 
      }
  }
}
