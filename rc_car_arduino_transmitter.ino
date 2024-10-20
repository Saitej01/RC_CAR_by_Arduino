/*
This code is for arduino at remote. 
In our project we have nodemcu which having one analog pin,
so for extending the analog pins 
we connected arduino to nodemcu.(Serial communication)
Here arduino analog pin is used to read the values of joystick left and right turn values.
These values are mapped to turn angles and send to the nodemcu transmitter by serial communication.
*/

const int turn=A0;
int mapped;
void setup()
{
  Serial.begin(115200);
  pinMode(turn,INPUT);
}
void loop()
{
  int turnvalue=analogRead(turn);
  if(turnvalue<501){
    mapped = map(turnvalue, 501, 1023, 0, 67);
  }else{
    mapped = map(turnvalue, 501, 1023, 0, 70);
  }
  Serial.print(mapped);
  Serial.println();
}