// x and y: integers used to store the duration of the pulse signals received from the flow sensor.
int x;// high value
int y; // low value 
float timeperiod = 0; // stores total time period of the pulse signals received from the flow sensor.
float frequency = 0; // stores frequency of the pulse signals received from the flow sensor.
float waterflowrate = 0; // stores calculated water flow rate based on the frequency of the pulse signals.
float total = 0; // stores cumulative amount of water flowed through the sensor.
float ls = 0; // stores amount of water flowed through the sensor per second.

void setup() {
  // put your setup code here, to run once:
pinMode(A4, INPUT);
Serial.begin(9600);
}

//pulseIn() function is used to measure the duration of the high and low pulses received from flow sensor
//sum of the durations is then used to calculate the time period, 
//and f = 1/T

// water flow rate calculated by dividing the frequency by a factor of 4.5. 
//The ls variable stores the water flow rate per second
//the total variable is updated with the cumulative water flow rate.

void loop() {
  // put your main code here, to run repeatedly:

  x = pulseIn(A4, HIGH);
  y = pulseIn(A4, LOW);
  timeperiod = x+y;
  frequency = 1000000/timeperiod;
  waterflowrate = frequency/4.5; 
  ls = waterflowrate/60; 

//checks if the frequency is greater than or equal to zero, 
//and if it is, it checks if the frequency is infinite. 
//If the frequency is infinite, the total amount of water flowed is printed to the serial monitor. 
//Otherwise, the total and waterflowrate variables are printed to the serial monitor.

  if (frequency >= 0)
  {

    if (isinf (frequency))
    {

      Serial.print("total");
      Serial.print(total);
    }
    else 
    {
      total = total + ls;
            Serial.print("total");
      Serial.println(total);
            Serial.print("waterflowrate"); 
        Serial.println(waterflowrate); 
      
    }
  }

}
