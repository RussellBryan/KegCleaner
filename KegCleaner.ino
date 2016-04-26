// Pin Asignments
int hose_in_open = 0;
int hose_in_close = 1;
int cleaner_in_open = 2;
int cleaner_in_close = 3;
int sanit_in_open = 4;
int sanit_in_close = 5;
int hose_re_open = 6;
int hose_re_close = 7;
int cleaner_re_open = 8;
int cleaner_re_close = 9;
int sanit_re_open = 10;
int sanit_re_close = 11;

int pump = 12;

int start = A0;
int STOP = A1;
int RESET = A3; //Or TO Reset Pin

int start_time = -1;
int sanatize = -1;

void setup() {
  Serial.begin(9600);
  for (int i=0; i<13; i++) 
    pinMode(i, INPUT);

}

void loop() {
  if (start_time == -1)
  {
    if (digitalRead(start) == HIGH)
    {
      start_time = millis();
      Serial.println(digitalRead(A0));
    }
  }
  else 
  {
    int time = millis() - start_time;
    //Cleaner Input Open
    //Cleaner Return Open
    
    //Pump On
    
    //Pump Off
    
    //Cleaner input Close
    //Cleaner Return Close
    
    //Hose Input Open
    //Hose Return Open
    
    //Hose Input Close
    //Hose Return Close
    
    //Iterate Above Code as nescesary
    if (cycle<num_cycles) {
      start_time = millis();
      cycle++
    }
    else {
      start_time = millis();
      sanatize = 1;
      
    //Sanatizer Cycle
    if (sanitize) {
      //Sanatizer Input Open
      //Sanitizer Return Open
      
      //Pump On
      
      //Pump Off
      
      //Sanatizer Input Close
      //Sanatizer Return Close
    }
  }

}
