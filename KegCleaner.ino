// Pin Asignments
int hose_in_open = 0;
int hose_in_close = 1;
int cleaner_in_open = 2;
int cleaner_in_close = 3;
int sanit_in_open = 4;
int sanit_in_close = 5;
int hose_re_open = 6;
int hose_re_close = 7;
int cleaner_re_open = 9;
int cleaner_re_close = 8;
int sanit_re_open = 10;
int sanit_re_close = 11;

int pump = 12;

int start = A0;
int STOP = A1;
int RESET = A2;

int wash_led = A4;
int rinse_led = A5;
int sanit_led = 13;

int wash = 0;
int rinse = 1;
int sanitize = 0;
int washed = 0;

float start_time = -1;
float time;

//Pump Dutty Cycle For Drainage
int cycle = 0;
int num_cycles = 1;
int pump_on_time = 14;
int pump_off_time = 6;

int sanitize_time = 60;
int rinse_time = 10;



void setup() {
  Serial.begin(9600);
  for (int i=0; i<14; i++){
    pinMode(i, OUTPUT);
  }
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  
  reset_system();
  Serial.println("Ready");
  
}

void loop() {
  if (start_time == -1)
  {
    if (digitalRead(start) == HIGH)
    {
      start_time = millis()/1000;
      Serial.println("Started");
      int cycle = 0;
    }
  }
  else 
  {
    //Stop Button Protocall
    if (digitalRead(STOP) == HIGH) {
      digitalWrite(pump, LOW);
      int stopped = 1;
      int stop_time = millis()/1000;
      Serial.println("Paused");
      while (stopped) {
        if (digitalRead(start) == HIGH){
          stopped = 0;
          start_time += millis()/1000 - stop_time;
          }
         if (digitalRead(RESET) == HIGH) {
           reset_system();
        }
      }
      //Stop and Reset Button Protocall
      if (digitalRead(RESET) == HIGH) {
        reset_system();
      }
    }
      
    //Find Time in seconds
    time = (millis()/1000.0) - start_time;
    
    if (wash) {
      digitalWrite(wash_led, HIGH);
      if (time > 5 + pump_on_time + pump_off_time){
        //End of Cycle
        if (cycle < num_cycles){
          //Repeat Cycle
          start_time = (millis()/1000) - 5;
          cycle++;
          Serial.print("Washing: Cycle ");
          Serial.println(cycle);
          }
        else {
          //Finish Wash
          
          start_time = millis()/1000;
          digitalWrite(cleaner_in_open, LOW);
          digitalWrite(cleaner_re_open, LOW);
          Serial.println("Cleaner Valves Closing");
          digitalWrite(cleaner_in_close, HIGH);
          digitalWrite(cleaner_re_close, HIGH);
          wash = 0;
          rinse = 1;
          washed = 1;
          start_time = millis()/1000.0;
        }
      }
      else if (time > 5 + pump_on_time) {
        //Pump Off to Allow For Drainage
        Serial.println("Washing: Pump off");
        digitalWrite(pump, LOW);
      }
      else if (time > 5) {
        //Pump On
        Serial.println("Washing: Pump on");
        digitalWrite(pump, HIGH);
      }
      else if (time > 0) {
        Serial.println("Washing: Started Opening Valves");
        //Cleaner Input Open
        digitalWrite(cleaner_in_open, HIGH);
        //Cleaner Return Open
        digitalWrite(cleaner_re_close, LOW);
        digitalWrite(cleaner_re_open, HIGH);
      }
    }
    
    //Find Time in seconds
    time = (millis()/1000.0) - start_time;
    
    if (rinse) {
      digitalWrite(rinse_led, HIGH);
      if (time > 10 + rinse_time) {
        Serial.println(time);
        Serial.println("Rinsing: Finished - Closing Valves");
        digitalWrite(hose_in_open, LOW);
        digitalWrite(hose_in_close, HIGH);
        if (washed) {
          rinse = 0;
          sanitize = 1;
        }
        else {
          rinse = 0;
          wash = 1;
        }
        start_time = millis()/1000;
      }
      else if (time > 10) { //Wait For Return Valve to open
        Serial.println("Rinsing: Opening Hose input");
        digitalWrite(hose_in_open, HIGH);
      }
      else if (time > 5) { //Wait for Cleaner Valves to Close
        Serial.println("Rinsing: Opening Return Valve");
        digitalWrite(hose_re_open, HIGH);
      }
      else {Serial.println(time);}
    }
    
    //Find Time in seconds
    time = (millis()/1000.0) - start_time;
    
    //Sanatizer Cycle
    if (sanitize) {
      digitalWrite(sanit_led, HIGH);
      if (time > 40 + sanitize_time) {
        reset_system(); 
      }
      else if (time > 35) {
        Serial.println("Sanitizing: Pump on");
        //Wait for Valves to Open
        digitalWrite(pump, HIGH);
      }
      else if (time > 30) {
        digitalWrite(sanit_re_open, HIGH);
      }
      else if (time > 25) {
        //Wait for water to drain
        digitalWrite(hose_re_open, LOW);
        digitalWrite(hose_re_close, HIGH);
      }
      else if (time > 20) {
        //time to flush rinse water out
        digitalWrite(pump, LOW);
      }
      else if (time > 8) { 
        Serial.println("Sanatizing: Flushing Rinse Water");
        //Adjust Time to Control Sanitizer Flow
        digitalWrite(sanit_in_open, LOW);
        //Pump On
        digitalWrite(pump, HIGH);
      }
      else if (time > 5) { //wait for rinse valves to close
        Serial.println("Sanitizing: Started");
        //Sanitizer Input Open   
        digitalWrite(sanit_in_open, HIGH);
      } 
    }
  }
}


void reset_system() {
  //Write All Control Pins LOW: Pump off All Valves Paused
  for (int pin = 0; pin <13; pin++) {
    digitalWrite(pin, LOW);
  }
  //Close All input valves
  digitalWrite(hose_in_close, HIGH);
  digitalWrite(sanit_in_close, HIGH);
  digitalWrite(cleaner_in_close, HIGH);
  Serial.println("All inputs Closing...");
  //Wait for Any Fluid to drain
  delay(5000);
  //Close Return Valves
  digitalWrite(hose_re_close, HIGH);
  digitalWrite(sanit_re_close, HIGH);
  digitalWrite(cleaner_re_close, HIGH);
  Serial.println("All Returns Closing.");
  delay(5000);
  //Write All Pins Low Again
  for (int pin = 0; pin <13; pin++) {
    digitalWrite(pin, LOW);
  }
  Serial.println("All Pins LOW");
  //Reset Cycle Control Variables
  sanitize = 0;
  rinse = 1;
  wash = 0;
  washed = 0;
  cycle = 0;
  start_time = -1;
  //Debug
  start_time = millis()/1000.0;
}
