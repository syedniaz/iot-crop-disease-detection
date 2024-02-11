#include <SoftwareSerial.h>
SoftwareSerial BT_Serial(2, 3); // RX, TX

// #include <IRremote.h>
// const int RECV_PIN = A5;
// IRrecv irrecv(RECV_PIN);
// decode_results results;

#define enA 10 // Enable1 L298 Pin enA
#define in1 9  // Motor1  L298 Pin in1
#define in2 8  // Motor1  L298 Pin in1
#define in3 7  // Motor2  L298 Pin in1
#define in4 6  // Motor2  L298 Pin in1
#define enB 5  // Enable2 L298 Pin enB

int bt_ir_data; // variable to receive data from the serial port and IRremote
int Speed = 250;
int mode = 0;
// int IR_data;

void setup()
{
  pinMode(enA, OUTPUT); // declare as output for L298 Pin enA
  pinMode(in1, OUTPUT); // declare as output for L298 Pin in1
  pinMode(in2, OUTPUT); // declare as output for L298 Pin in2
  pinMode(in3, OUTPUT); // declare as output for L298 Pin in3
  pinMode(in4, OUTPUT); // declare as output for L298 Pin in4
  pinMode(enB, OUTPUT); // declare as output for L298 Pin enB

  // irrecv.enableIRIn(); // Start the receiver
  // irrecv.blink13(true);

  Serial.begin(9600);    // start serial communication at 9600bps
  BT_Serial.begin(9600); // start Bluetooth serial communication

  delay(500);
}

void loop()
{
  if (BT_Serial.available() > 0)
  {                            //if some data is sent, reads it and saves in state
    bt_ir_data = BT_Serial.read();
    Serial.println(bt_ir_data);
    if (bt_ir_data > 20)
    {
      Speed = bt_ir_data;
    }
  }

  // if (irrecv.decode(&results))
  // {
  //   Serial.println(results.value, HEX);
  //   bt_ir_data = IRremote_data();
  //   Serial.println(bt_ir_data);
  //   irrecv.resume(); // Receive the next value
  //   delay(100);
  // }

  if (bt_ir_data == 8)
  {
    mode = 0; // Manual Android Application and IR Remote Control Command
    stop();
  }
  else if (bt_ir_data == 9)
  {
  }
  else if (bt_ir_data == 10)
  {
  }

  analogWrite(enA, Speed); // Write The Duty Cycle 0 to 255 Enable Pin A for Motor1 Speed
  analogWrite(enB, Speed); // Write The Duty Cycle 0 to 255 Enable Pin B for Motor2 Speed

  if(mode==0){     
//===============================================================================
//                          Key Control Command
//=============================================================================== 
    if(bt_ir_data == 1){
       forword(); 
    }  // if the bt_data is '1' the DC motor will go forward
    else if(bt_ir_data == 2){
      backword();
    }  // if the bt_data is '2' the motor will Reverse
    else if(bt_ir_data == 3){
      turnLeft();
      }  // if the bt_data is '3' the motor will turn left
    else if(bt_ir_data == 4){
      turnRight();
      } // if the bt_data is '4' the motor will turn right
    else if(bt_ir_data == 5){
      stop();
      }
  }

  delay(10);
}

// long IRremote_data()
// {
//   // IR remote data mapping (if needed)
//   // ...

//   return IR_data;
// }

void forword(){  //forword
digitalWrite(in1, HIGH); //Right Motor forword Pin 
digitalWrite(in2, LOW);  //Right Motor backword Pin 
digitalWrite(in3, LOW);  //Left Motor backword Pin 
digitalWrite(in4, HIGH); //Left Motor forword Pin 
}

void backword(){ //backword
digitalWrite(in1, LOW);  //Right Motor forword Pin 
digitalWrite(in2, HIGH); //Right Motor backword Pin 
digitalWrite(in3, HIGH); //Left Motor backword Pin 
digitalWrite(in4, LOW);  //Left Motor forword Pin 
}

void turnRight(){ //turnRight
digitalWrite(in1, LOW);  //Right Motor forword Pin 
digitalWrite(in2, HIGH); //Right Motor backword Pin  
digitalWrite(in3, LOW);  //Left Motor backword Pin 
digitalWrite(in4, HIGH); //Left Motor forword Pin 
}

void turnLeft(){ //turnLeft
digitalWrite(in1, HIGH); //Right Motor forword Pin 
digitalWrite(in2, LOW);  //Right Motor backword Pin 
digitalWrite(in3, HIGH); //Left Motor backword Pin 
digitalWrite(in4, LOW);  //Left Motor forword Pin 
}

void stop(){ //stop
digitalWrite(in1, LOW); //Right Motor forword Pin 
digitalWrite(in2, LOW); //Right Motor backword Pin 
digitalWrite(in3, LOW); //Left Motor backword Pin 
digitalWrite(in4, LOW); //Left Motor forword Pin 
}
