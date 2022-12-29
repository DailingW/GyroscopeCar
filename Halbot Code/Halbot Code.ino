//L293D
//Motor A
#define ENABLE 5
#define motorPin1 4  // Pin 15 of L293
#define motorPin2 3  // Pin 10 of L293
//Motor B
#define ENABLE_2 10
#define motorPin3 9  // Pin  7 of L293
#define motorPin4 8  // Pin  2 of L293

int direction = 0;
// float x = 0;
// float y = 0;

void setup() {
  //Set pins as outputs
  pinMode(ENABLE,OUTPUT);
  pinMode(ENABLE_2,OUTPUT);


  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  Serial.println("Data Recieved:");

  String one = Serial.readString();
  Serial.println(one);
  //String y = Serial.readString();
    
  //String z = Serial.readString();
  String first = "";
  String second = "";
  for(int i = 0; i < one.length(); i++) {
    if(one.charAt(i) == ' ') {
      first = one.substring(0, i);
      second = one.substring(i + 1);
    }
  }

  // Values obtained when phone is held in neutral position
  float xRotInitial = 0.0;
  float yRotInitial = -1.5;
  //float zRotInitial = 0.0;

  // Values obtained when phone is adjusted
  float xRotFinal = first.toFloat();
  float yRotFinal = second.toFloat();
  //float zRotFinal = z.toFloat();

  Serial.println(xRotFinal);
  Serial.println(yRotFinal);
  

  // Change between final and initial positions
  float xRotChange = xRotFinal - xRotInitial;
  float yRotChange = yRotFinal - yRotInitial;
  //float zRotChange = 0;

  if(yRotFinal == 0.0 && xRotFinal == 0.0) {
    direction = 0;
    Serial.println("Nothing");
  }

  // Order of If statements: Front, Backward, Right, Left
  if(yRotFinal == 0.0 && xRotFinal == 0.0) { 
    direction = 0;
    Serial.println("Nothing");
    } else if (yRotFinal > -0.7) {
      direction = 1;
      Serial.println("Front");
    } else if (yRotFinal < -2.2) {
      direction = -1;
      Serial.println("Back");
    } else if (xRotFinal > 0.7) {
      direction = 2;
      Serial.println("Right");
    } else if (xRotFinal < -0.7) {
      direction = -2;
      Serial.println("Left");
    } else {
      direction = 0;
      Serial.println("Nothing");
  }  
  
  delay(10);

  

    //Motor Control - Motor A: motorPin1,motorpin2 & Motor B: motorpin3,motorpin4
  if (direction == 1) {
    //This code  will turn Motor A and Motor B clockwise (backwards)
    
    analogWrite(ENABLE, 255);
    analogWrite(ENABLE_2, 255);
    
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, HIGH);
  } else if (direction == -1) {
    //This code will turn Motor A and motor B counter-clockwise (forwards)

    analogWrite(ENABLE, 255);
    analogWrite(ENABLE_2, 255);

    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);
  } else if (direction == 2) {
    // Turn Right
    analogWrite(ENABLE, 255);
    analogWrite(ENABLE_2, 255);

    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, HIGH);
  } else if (direction == -2) {
    // Turn Left
    analogWrite(ENABLE, 255);
    analogWrite(ENABLE_2, 255);

    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);
  } else {
    //And this code will stop motors

    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, LOW);
  }
  //delay(200);
}

// Directions
// x - pitch
// y - roll
// z - yaw