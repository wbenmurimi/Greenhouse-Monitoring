  #include <dht.h>
  
  dht DHT;
  
  #define DHT11_PIN 7
  #define LED1 13
  #define MOISTURE 8
  int moisture_val;
  
  int thresholdUp = 400;
  int thresholdDown = 250;
  
  int soilPin = 8; //Soil humidity
  int soilPower = 7;//Variable for Soil moisture Power
  
  const int analogInPin = A0; //pH
  
  int sensorValue = 0; 
  unsigned long int avgValue; 
  float b;
  int buf[10],temp;
  
  double q =0.125; //process noise covariance
  double r =20; //measurement noise covariance
  double finalTemp =26; //value
  double p =0.5; //estimation error covariance
  double k;  //kalman gain
  double initialTemp=22; //Initial room temp
  
  void setup(){
    Serial.begin(9600);
    // initialize the digital pin as an output.
    // Pin 13 has an LED connected on most Arduino boards:
    pinMode(7, OUTPUT);     
    //Pin 2 is connected to the output of proximity sensor
    pinMode(LED1,OUTPUT);
  
  }
  
  void loop()
  {
    //Temparature and humidity
    int chk = DHT.read11(DHT11_PIN);
    //Serial.print("Temp: ");
   initialTemp=DHT.temperature;
     //prediction update
    p = p + q;
    
    //measurement update
    k = p / (p + r);
    finalTemp = finalTemp + k * (initialTemp - finalTemp);
    p = (1 - k) * p;
   
    Serial.print(finalTemp);
    Serial.print(",");
    Serial.print(DHT.humidity);
     Serial.print(",");
  
    //Moisture
   moisture_val= analogRead(MOISTURE);
   moisture_val= map(moisture_val,300, 1023, 100, 0);
   
    Serial.print(moisture_val);
    Serial.print(",");
  
  
    if(moisture_val <80 )      //Check the sensor output
    {
      digitalWrite(LED1, HIGH);   // set the LED on-- start irrigating
    }
    else
    {
      digitalWrite(LED1, LOW);    // set the LED off-- stop irrigating
    }
     
  
  //PH
   for(int i=0;i<10;i++) 
   { 
    buf[i]=analogRead(analogInPin);
    delay(10);
   }
   for(int i=0;i<9;i++)
   {
    for(int j=i+1;j<10;j++)
    {
     if(buf[i]>buf[j])
     {
      temp=buf[i];
      buf[i]=buf[j];
      buf[j]=temp;
     }
    }
   }
   avgValue=0;
   for(int i=2;i<8;i++)
   avgValue+=buf[i];
   float pHVol=(float)avgValue*5.0/1024/6;
   float phValue = -5.70 * pHVol + 21.34;
  
   Serial.println(phValue);
    
    delay(5000);
     
  }
  

