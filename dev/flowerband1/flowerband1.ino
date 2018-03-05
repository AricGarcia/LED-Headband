/*
   Flower Band Lighting code 2.0

   Aric Garcia
   10/14/2014

   This code utilizes an attiny85 controller programmed over a USB Programmer set up with arduino bootloader
   to control a string of RBGW LEDs. The LEDs are controlled by 4 mosfets (1 per string) and are supplied by
   a 12V LiPo source. each mosfet has a 100ohm to stop signal reverberation and increase the switching
   frequency. A push button defaulted LOW can provide signal for control logic.

   This version of code simply will check all parts individually and increase bus loading to verify all
   components are functional given any possible commanding pattern. Each increment in loading will be
   provided by the push button which will also verify the push button functionality.

   pin0: Red Control (Output) Pull HIGH to activate
   pin1: Blue Control (Output) Pull HIGH to activate
   pin2: Green Control (Output) Pull HIGH to activate
   pin3: Push Button (Input) If HIGH button is pressed
   pin4: Warm White Control (Output) Pull HIGH to activate
*/
#define OFF HIGH
#define ON  LOW

//Pin Assignments
int SWITCH = 0;
int RED_PIN = 1;
int BLUE_PIN = 2;
int GREEN_PIN = 3;
int WWHITE_PIN = 4;

//Supporting Variables
int led_mode = 0; // Initial status of the function loops
int NUM_STATES = 10; // this should match the number of switch cases in loop
int color_pin_array[ 4 ] = {RED_PIN, BLUE_PIN, GREEN_PIN, WWHITE_PIN};

//Supporting Functions
void power_off(){
    digitalWrite(RED_PIN, HIGH); // Inverting for OD test
    digitalWrite(GREEN_PIN, HIGH); // Inverting for OD test
    digitalWrite(BLUE_PIN, HIGH); // Inverting for OD test
    digitalWrite(WWHITE_PIN, HIGH); // Inverting for OD test
}


//void power_on_static(byte RBGW_MASK=0, float R_PWM=0, float B_PWM=0, float G_PWM=0, float WW_PWM=0){
//  byte red = RBGW_MASK & (1 << 0);
//  byte blue = (RBGW_MASK & (1 << 1))/2;
//  byte green = (RBGW_MASK & (1 << 2))/4;
//  byte white = (RBGW_MASK & (1 << 3))/8;
//  byte total_duty_cycle = (red*R_PWM)+(blue*B_PWM)+(green*G_PWM)+(white*WW_PWM);
//  R_PWM = (R_PWM/total_duty_cycle)*20;
//  B_PWM = (B_PWM/total_duty_cycle)*20;
//  G_PWM = (G_PWM/total_duty_cycle)*20;
//  WW_PWM = (WW_PWM/total_duty_cycle)*20;
//  int color_array[ 4 ] = {red, blue, green, white};
//  int pwm_array[ 4 ] = { R_PWM, B_PWM, G_PWM, WW_PWM};
//  for(int i = 0; i < 4; i++){
//    if(color_array[i] > 0){
//      digitalWrite(color_pin_array[i], HIGH);
//      delay(pwm_array[i]);
//      digitalWrite(color_pin_array[i], LOW);
//    }
//  }
//}


void pwm_bit_bang(int pin, int pwm, int ms_delay){
  if(pwm >= 1000){
    pwm = 999;
  }
  else if(pwm <= 0){
    pwm = 1;
  }
  unsigned long curr_t = millis();
  long prev_t = curr_t;
  while((curr_t - prev_t) <= ms_delay){
    digitalWrite(pin, LOW); // Inverting for OD test
    delayMicroseconds(pwm);
    digitalWrite(pin, HIGH); // Inverting for OD test
    delayMicroseconds(1000-pwm);
    curr_t = millis();
  }
}


void power_on_static(int R_PWM=0, int B_PWM=0, int G_PWM=0, int WW_PWM=0){
  int red = 0;
  int blue = 0;
  int green = 0;
  int white = 0;
  if(R_PWM > 0 ){
    red = 1;
  }
  if(B_PWM > 0 ){
    blue = 1;
  }
  if(G_PWM > 0 ){
    green = 1;
  }
  if(WW_PWM > 0 ){
    white = 1;
  }
  int ms_delay = 12/(red+blue+green+white);
  int color_array[ 4 ] = { red, blue, green, white };
  int pwm_array[ 4 ] = { R_PWM, B_PWM, G_PWM, WW_PWM };
  for(int i = 0; i < 4; i++){
    if(color_array[i] > 0){
      pwm_bit_bang(color_pin_array[i], pwm_array[i]*10, ms_delay);
    }
  }
}


//void power_on_sin(int amplitude_percentage, int period){
//   
//}


int checkForButtonEvent(int Button_Status, int Led_Mode)
{
  if (Button_Status == OFF) {  // If the button has been pushed we will get in this loop

    //If the button is pushed change the LED Mode (STATE)
    if ( Led_Mode < NUM_STATES) {
      Led_Mode++;// move to the next STATE
      
    }
    else if (Led_Mode >= NUM_STATES) {
      Led_Mode = 0; //reset to the intitial state
    }

    //TURN ALL LIGHTS OFF BEFORE SWITCHING STATES
    power_off();
    delay(500);        // delay in between reads for "DEBOUNCING"
    return Led_Mode;
  }
  else{ return led_mode;}
}


void setup() {
  for (int i = 0; i <= 3; i++) {
    pinMode(color_pin_array[i], OUTPUT);
  }
  power_off();
  pinMode(SWITCH, INPUT);
}


void loop() {
  int button_status = digitalRead(SWITCH);
  led_mode = checkForButtonEvent(button_status, led_mode);
//  Serial.print("switch_status: ");
//  Serial.println(switch_status);
  switch(led_mode){
    case 1:
      // blue
      power_on_static(0, 100, 0, 0);
      break;
    case 0:
      // green
      power_on_static(0, 0, 100, 0);
      break;
    case 2:
      // red
      power_on_static(100, 0, 0, 0);
      break;
    case 3:
      // warm white
      power_on_static(0, 0, 0, 100);
      break;
    case 4:
      // turqoise
      power_on_static(0, 100, 100, 0);
      break;
    case 5:
      // yellow
      power_on_static(100, 0, 100, 0);
      break;
    case 6:
      // red/warm white
      power_on_static(100, 0, 0, 100);
      break;
    case 7:
      // cool white
      power_on_static(100, 100, 100, 0);
      break;
    case 8:
      // warm white/cool white
      power_on_static(100, 100, 100, 100);
      break;
    case 9:
      // purple
      power_on_static(10, 100, 0, 0);
      break;
//    case 10:
//      power_on_sin();
    case 10:
      power_off();
      break;
      }
}

