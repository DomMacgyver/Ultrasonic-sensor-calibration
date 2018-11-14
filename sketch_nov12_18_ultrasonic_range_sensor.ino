
#include "Functions.h"
int row;
int x;

const unsigned int size = 5;// number of data points that will be collected
const unsigned int sample_size = 10; //then umber of samples to take for each average; larger numbers for more accuracy, smaller numbers for faster run times
const unsigned int TRIG_PIN = 7;
const unsigned int ECHO_PIN = 6;
char receivedChar;

// if you have already determined the offset, set calibration to true and set the offset to that value(man_offset: where it was measure, calibration_offset: what the difference in measurments was)
bool calibration = true;
double man_offset = 0.0;// how far away the senosr will be from the wall when measuring since ultrasonic senors cannot get a reading at point blank range, units in cm
double calibration_offset = 0.0;//in cm

const double interval = 2.0; // the distance between each measurement

//initilaize arrays
double darr[size];// array of pre-measured distances
double val[size];// array of average distance values from the sensor
unsigned long durations[size]; //array of average durations, in ms

//hard coded values for testing
//double darr[] = {5.0, 7.0, 9.0, 11.0, 13.0};
//double val[] = {4.2024, 5.6089, 7.9931, 9.9485, 11.8182};
//unsigned long durations[] = {3311, 11013, 13102, 21010, 22301};

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("Initialized");

  delay(500);
}

double offset_calibrate()
{
  double distance;
  while (Serial.available() == 0)//exit the loop when the user enters the distance into the serial monitor
  {
    //trigger the ultrasonic sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    //recive duration from the sensor
    const unsigned long duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration * 1.0  / 29.15 / 2.0; // calculate the distance in cm: formula: time / speed of sound in cm / 2 because sound travels there and back
    Serial.println(distance, 5);//print the distance
    delay(20);
  }
  double offset = Serial.parseFloat();
  calibration_offset = offset - distance; //set the value to what the user entered - distance computed
  Serial.println(calibration_offset, 5); // print the calibration offset to 5 decimal places
  calibration = false;// exits the loop
  return offset;
}

void array_initialize(double start, double step, int size)
{
  for (int i = 0; i < size; i++)
  {
    darr[i] = start + step * i;
  }
}


void loop() {

  //calibrate the front offset
  while (calibration)
  {
    man_offset = offset_calibrate();
  }
  array_initialize(man_offset, interval, size);// initialize the array of pre-measured distances



  for (int position = 0; position < size; position++)
  {
    bool newData = false;
    Serial.println("Waiting for input to advance");
    Serial.println("Current measurment distance is ");
    Serial.println(darr[position], 5);
    while (!newData)//wait for user to enter 'a' into the serial monitor before running for this distance; done for each distance measurment
    {
      if (Serial.available() > 0)
      {
        receivedChar = Serial.read();
        if (receivedChar == 'a')
        {
          newData = true;
          receivedChar = ' ';
        }
      }
    }

    unsigned long total = 0;

    Serial.println("Ready");//acnowledeges that input has been recieved
    delay(1000);
    Serial.println("Go");

    for (int num = 0; num < sample_size; num++)
    {
      digitalWrite(TRIG_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIG_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG_PIN, LOW);
      const unsigned long duration = pulseIn(ECHO_PIN, HIGH);
      //double distance = calibration_offset + duration * 1.0  / 29.15 / 2.0;

      // Serial.println(distance, 5);
      if (duration <= 1 || duration >= 200000)
      {
        num -= 1;//don't count this run if the value doesn't make sense
      }
      else
      {
        Serial.println(num, DEC);//print the current iterarion
        total += duration;// add the duration to the total
        delay(20);
      }
    }
    Serial.println("Done");

    double average = total / sample_size * 1.0;// compute the average
    durations[position] = average;// add this average durtion to the array
    double distance = average / 29.15 / 2.0;//calculate the predicted distance
    val[position] = distance; // stire the predicted distance to the array
  }

  //print the data (to 4 decimal places) as three(3) columns (measured on left, sensor in the middle, and durations on right) sperated by commas
  Serial.println();
  for (int count = 0; count < size; count++)
  {
    Serial.print(darr[count], 4);
    Serial.print(", ");
    Serial.print(val[count], 4);
    Serial.print(", ");
    Serial.print(durations[count], 4);
    Serial.println();
  }
  // Serial.println(calibration_offset, 5); // print the calibration offset to 5 decimal places
  //helpful site for viewing: http://www.alcula.com/calculators/statistics/scatter-plot/


  double line_vals[3];
  lsrl(darr, val, size, line_vals);
  Serial.println();//print a space

  //print the least squares regression line coefficents and correlation coefficent (R Squared), to 7 decimal places
  Serial.print("A0: ");
  Serial.print(line_vals[0], 7);
  Serial.print(", A1: ");
  Serial.print(line_vals[1], 7);
  Serial.print(", R Squared: ");
  Serial.print(line_vals[2], 7);
  Serial.println();
  delay(50000);
}
