int val1;
int val2;

void setup() {
  // initialize the serial communication:
  Serial.begin(9600);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
}

void loop() {
  // send the value of analog input 0:
  Serial.println(analogRead(A0));
  Serial.println(analogRead(A1));
  if(analogRead(A0)>= 10){
    val1 = analogRead(A0);}
  if(analogRead(A1)>= 10){
    val2 = analogRead(A1);}
  analogWrite(10,val1);
  analogWrite(11,val2);
  // wait a bit for the analog-to-digital converter to stabilize after the last
  // reading:
  delay(2);
}
