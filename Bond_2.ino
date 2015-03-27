#include <SPI.h>
#include <boards.h>
#include <ble_shield.h>
#include <services.h>
#include <IRremote.h>

const int IR_LED_Pin = 3;
const int Button_Pin = 4;
const int Recv_Pin = 2;
const int LED_Pin = 10; // red light
const int LED_Pin_yel = 5; //yellow light

IRrecv irrecv(Recv_Pin);
decode_results results;

//byte readVal = 0x00;
//double x2, y2, z2;


void setup() {
  // initialize all pins
  
  ble_begin(); // set up bluetooth

  pinMode(LED_Pin_yel, OUTPUT); 
  pinMode(IR_LED_Pin, OUTPUT);
  pinMode(Button_Pin, INPUT);
  pinMode(LED_Pin, OUTPUT);
  digitalWrite(LED_Pin, LOW); // fixes bug
  
  Serial.begin(9600); // initialize serial port
   
  irrecv.enableIRIn(); // start the IR reciever
}


void loop() {
  // put your main code here, to run repeatedly:
  
  
   // Send IR code if button is pressed
  int Button_State = digitalRead(Button_Pin);
  
  if(ble_connected()) { // visual indication that bluetooth has connected
  
  digitalWrite(LED_Pin_yel, HIGH); // turn on yellow light if connected
  
        if (Button_State == LOW)  {
        Serial.println("Code sent!");
        
          for (int i = 1; i < 4; i++) {
          Serial.println("Code sent!");  
          Send_Code();
          delay(500);
          }
        
        }
    
    

    }
  
  // If unique code is recieved (12D52043) -> light up red LED
  if (irrecv.decode(&results))  {
    if(results.value == 0x12D52043)  {
      Serial.println("Unique code recieved!");
      digitalWrite(LED_Pin, HIGH); 
      delay(500);
      digitalWrite(LED_Pin, LOW);
     }
    Serial.println(results.value, HEX);
    
  //Bluetooth stuff...
 byte readVal = 0x00;
  
  //something in the buffer
  if(ble_available()){
    //read a byte from the buffer
    readVal = ble_read();
  }
  
  if(readVal == 0x01){
    //write a byte back to the master
    ble_write_bytes((unsigned char*)(&(results.value)),sizeof(results.value));
  }


  if (!ble_connected()){
    readVal = 0x00;
    //visual indication that it is not connected
    digitalWrite(LED_Pin_yel, LOW);
    
  }
  
    irrecv.resume(); // recieve the next value
   }
  delay(100);
 
  // Allow BLE Shield to send/receive data
  ble_do_events();
  //digitalWrite(RED, LOW);
  
  
  

} // close void loop



/* This procedure sends a 38KHz pulse to the IR_LED_Pin for a 
certain # of microseconds.  We will use this whenever we need to send codes.
*/
void pulseIR(long microsecs)  {
  // We will count down from the number of microseconds we are told to wait
  
  cli();  // this turns off any background interrupts
  
  while (microsecs > 0)  {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
    digitalWrite(IR_LED_Pin, HIGH); //this takes about 3 um seconds to happen
    delayMicroseconds(10); //stay on for 10 more um seconds
    digitalWrite(IR_LED_Pin, LOW); // this also take about 3 um
    delayMicroseconds(10); // stay off for 10 more um seconds
    
    // so 26 microseconds altogether
    microsecs = microsecs - 26;
  }
  
  sei(); //this turns interrupts back on
}



// This procedure is the code for sending a unique 38kHz signal
// Sends: HEX 14B766FF or DEC 347563775
void Send_Code()  {
  
  
  delayMicroseconds(1600);
  pulseIR(640);
  delayMicroseconds(1560);
  pulseIR(640);
  delayMicroseconds(460);
  pulseIR(600);
  delayMicroseconds(1600);
  pulseIR(640);
  delayMicroseconds(1580);
  pulseIR(640);
  delayMicroseconds(1540);
  pulseIR(640);
  delayMicroseconds(1580);
  pulseIR(640);
  delayMicroseconds(1540);
  pulseIR(640);
  delayMicroseconds(460);
  pulseIR(580);
  delayMicroseconds(500);
  pulseIR(600);
  delayMicroseconds(520);
  pulseIR(580);
  delayMicroseconds(1620);
  pulseIR(580);
  delayMicroseconds(500);
  pulseIR(580);
  delayMicroseconds(540);
  pulseIR(600);
  delayMicroseconds(500);
  pulseIR(580);
  
}

