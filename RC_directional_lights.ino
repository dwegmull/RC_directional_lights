// Manages forward and reverse light on a locomotive based on the pulse duration
// going to the RC servo controlling the Johnson bar (reverser).
// This code is intnended to be used with an Adafruit Trinket 3.3V
// Other Arduino boards can be used but some pin definitions and timing constants will have to be adjusted.

#define TIME_UNIT 18.4 // Duration of one side of the pulse on output 3 in micro seconds.
#define FORWARD_IN (1290 / TIME_UNIT)
#define FORWARD_OUT (1400 / TIME_UNIT)
#define REVERSE_IN (1750 / TIME_UNIT)
#define REVERSE_OUT (1600 / TIME_UNIT)
#define PULSES_PER_SECOND 50
#define STOPPED_TIMEOUT PULSES_PER_SECOND * 60 * 2 // After two minutes stopped, both lights will go out

typedef enum
{
  stopped,
  forward,
  reverse
} modes_t;

int numberOfPulses;
modes_t mode;


void setup() 
{
  pinMode(0, INPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(1, HIGH);
  digitalWrite(2, HIGH);
  pinMode(3, OUTPUT); // Used for calibrating the clock by looking at the pulses on a scope.
  numberOfPulses = 0;
  mode = stopped;
}

void loop() 
{
  char pulseLength;
  pulseLength = 0;
  while(0 == digitalRead(0))  ; // Wait for the start of the servo pulse
  while(1 == digitalRead(0))
  {
    // Measure the pulse length
    pulseLength++;
    digitalWrite(3, pulseLength & 0x01); // Use a scope to find out how long this loop is
  }
  switch(mode)
  {
    default:
    case stopped:
      if(pulseLength < FORWARD_IN)
      {
        digitalWrite(1, HIGH);
        digitalWrite(2, LOW);
        mode = forward;
        break;
      }
      if(pulseLength > REVERSE_IN)
      {
        digitalWrite(2, HIGH);
        digitalWrite(1, LOW);
        mode = reverse;
        break;
      }
      if(numberOfPulses > STOPPED_TIMEOUT)
      {
        digitalWrite(2, LOW);
        digitalWrite(1, LOW);
      }
      else
      {
        numberOfPulses++;
      }
      break;
  case forward:
      if(pulseLength > FORWARD_OUT)
      {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        mode = stopped;
        numberOfPulses = 0;
      }
      break;
  case reverse:
      if(pulseLength < REVERSE_OUT)
      {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        mode = stopped;
        numberOfPulses = 0;
      }
      break;
  }
}
