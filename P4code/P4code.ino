//This code is bypassing the Arduino API to change the register bits. 
//This is done to change the prescaler for the analog to digital converters sample rate to increase the effective sample rate of the analog to digital converter.  
#define FASTADC 1

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
//Defining values that are static. This is done to save memory in the program
#define thresh 5 
#define SAMPLES 128
//Values used in the reording of samples

//variable for managing the sample arrays 
 
double x[SAMPLES];
double y[SAMPLES];
double z[SAMPLES];
//variables to store analog in data
int a, b, c;
//Arrays storing the predefined values for the 6 knock zones
double zone1[] = {0.40, 0.10, 0.10, 0.80, 0.40, 0.35};
double zone2[] = {0.25,0.25, 0.10, 0.60,0.60, 0.35};
double zone3[] = {0.10,0.40, 0.10, 0.40,0.80, 0.35};
double zone4[] = {0.25, 0.10, 0.30, 0.70, 0.30, 0.60};
double zone5[] = {0.20, 0.20, 0.30, 0.50, 0.50, 0.70};
double zone6[] = {0.10, 0.25, 0.30, 0.30, 0.70, 0.60};
//Variables for ratio calculation
double ratio;
double ratio1;
double ratio2;
double ratio3;
//Variables for storing the peak (largest value) in the sample array
int peak1;
int peak2;
int peak3;

//Code run only on startup
void setup() {
  //start serial communication 
  Serial.begin(115200);
  //This code will set the prescale for the Arduino analog to digital converter to 16 if FASTADC is 1 giving an effective sample rate of XXXX
#if FASTADC
  // set prescale to 16
  sbi(ADCSRA, ADPS2) ;
  cbi(ADCSRA, ADPS1) ;
  cbi(ADCSRA, ADPS0) ;
#endif
}
//Code that is run continuously 
void loop() {
  //Read input from Arduino analog pins connected to the piezo sensors
  a = analogRead(A0);
  b = analogRead(A5);
  c = analogRead(A2);
  //Function that records sensor inputs into an array also triggers rest of the process
  record();
  

}
//Calculates the ratio between 3 variables
void getratio(int x, int y,int z) {
  ratio =(x+y+z);
  ratio1 = x/ratio;
  ratio2 = y/ratio;
  ratio3 = z/ratio;
}
//Records inputs from analog in, saves to an array and runs the rest of the process
void record() {
  //Filter out signals than 5 lower than anything lower than 5 is considered likely to be noise as knocks generally output higer values
  if (analogRead(A0) >= thresh || analogRead(A5) >= thresh) {
    //Fill the arrays
    for(int i=0; i<SAMPLES; i++)
    {
        
        x[i] = analogRead(A0);
        y[i] = analogRead(A5);
        z[i] = analogRead(A2);
      
    }
  //Findpeaks  
  findpeaks();
  //Get ratio using peaks as input
  getratio(peak1,peak2,peak3);
  //Determine the zone
  detectratiozone();
  
  
  }
}
//Findpeaks scans through the 3 arrays containing the sampled signals finding the higest value in the array
void findpeaks(){
  peak1 = x[0];
  peak2 = y[0];
  peak3 = z[0];
  for(int i=0; i<SAMPLES; i++){
   if (x[i] > peak1) {
      peak1 = x[i];
    }
   if (y[i] > peak2) {
      peak2 = y[i];
    } 
    if (z[i] > peak3) {
      peak3 = z[i];
    }     
  }  
}
//Use an if else to determine the zone fitting with the ratio from the knock code
void detectratiozone() {
  
  if(ratio1 > zone1[0] && ratio2 > zone1[1] && ratio3 > zone1[2] && ratio1 < zone1[3] && ratio2 < zone1[4] && ratio3 < zone1[5]){
    //zone1
    Serial.println("1");
  }
  else if(ratio1 > zone2[0] && ratio2 > zone2[1] && ratio3 > zone2[2] && ratio1 < zone2[3] && ratio2 < zone2[4] && ratio3 < zone2[5]){
    //zone2
    Serial.println("2");
  }
  else if(ratio1 > zone3[0] && ratio2 > zone3[1] && ratio3 > zone3[2] && ratio1 < zone3[3] && ratio2 < zone3[4] && ratio3 < zone3[5]){
    //zone1 due to unrelaible reading on sensor 1 it is safer to assume this will trigger in zone 1
    Serial.println("3");
  }
  else if(ratio1 > zone4[0] && ratio2 > zone4[1] && ratio3 > zone4[2] && ratio1 < zone4[3] && ratio2 < zone4[4] && ratio3 < zone4[5]){
    //zone1 due to unrelaible reading on sensor 1 it is safer to assume this will trigger in zone 1
    Serial.println("4");
  }
  else if(ratio1 > zone5[0] && ratio2 > zone5[1] && ratio3 > zone5[2] && ratio1 < zone5[3] && ratio2 < zone5[4] && ratio3 < zone5[5]){
    //zone1 due to unrelaible reading on sensor 1 it is safer to assume this will trigger in zone 1
    Serial.println("5");
  }
  else if(ratio1 > zone6[0] && ratio2 > zone6[1] && ratio3 > zone6[2] && ratio1 < zone6[3] && ratio2 < zone6[4] && ratio3 < zone6[5]){
    //zone1 due to unrelaible reading on sensor 1 it is safer to assume this will trigger in zone 1
    Serial.println("6");
  }
  else{
    //unreadable
    Serial.println("Unreadable");
  }
  //Serial print is sent from the Arduino to a connected PC where a python program picks up the printed message and plays a sound accordingly
}
