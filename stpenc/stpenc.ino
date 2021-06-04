#define EN        8
#define Y_DIR     6
#define Y_STP     3
#define encoder0PinA  2
#define encoder0PinB  4
struct StepperEncoder
{
  volatile signed int value;
  bool dataOK;
} Enc;
struct StepperParser
{
  String dataIn;
  String dt[10];
  bool dataValid;
  bool start;
} Parse;
struct StepperMotor
{
  bool dataOK;
  int vel;
  int steps;
  bool dir;
  bool runs;
} Motor;


void EncoderInit() {
  Enc.dataOK = 0;
  Enc.value = 0;
  pinMode(encoder0PinA, INPUT);
  digitalWrite(encoder0PinA, HIGH);       // turn on pull-up resistor
  pinMode(encoder0PinB, INPUT);
  digitalWrite(encoder0PinB, HIGH);       // turn on pull-up resistor
  attachInterrupt(0, doEncoder, CHANGE);
}

void ParseInit() {
  Parse.dataIn = "";
  Parse.start = 0;
  Parse.dataValid = 0;
}

void StepperInit() {
  pinMode(Y_DIR, OUTPUT); pinMode(Y_STP, OUTPUT);
  pinMode(EN, OUTPUT);
  Motor.vel = 11000;
  Motor.steps = 0;
  Motor.dir = 0;
  Motor.runs = 0;
}

void setup() {
  // put your setup code here, to run once:
  EncoderInit();
  StepperInit();
  Serial.begin(9600);
  Serial.println("BOOTING");
}
void doEncoder() {
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
    Enc.value++;
  } else {
    Enc.value--;
  }
  Enc.dataOK = 1;
}

void step(boolean dir, byte dirPin, byte stepperPin, int steps)
{
  digitalWrite(dirPin, dir);
  delay(10);
  for (int i = 0; i < steps; i++) {

    digitalWrite(stepperPin, HIGH);
    delayMicroseconds(Motor.vel);
    digitalWrite(stepperPin, LOW);
    delayMicroseconds(Motor.vel);
//    Serial.print("Encoder :");
//    Serial.print(Enc.value / 2, DEC);
//    Serial.print(", Step :");
//    Serial.println(i);
  }
}
void loop() {
  //    put your main code here, to run repeatedly:
  while (Serial.available() > 0)
  {
    char inChar = (char)Serial.read();
    Parse.dataIn += inChar;
    if (inChar == '\n')
      Parse.start = true;
  }
  if (Parse.start)
  {
    parsingData();
    Parse.start = false;
  }
  if(Motor.runs){
  Serial.println("Motor Running..");
  step(Motor.dir, Y_DIR, Y_STP, Motor.steps);
  Motor.runs=0;
  }
}
void parsingData()
{
  int j = 0;
  //kirim data yang telah diterima sebelumnya
  Serial.print("data masuk : ");
  Serial.print(Parse.dataIn);
  Serial.print("\n");
  //inisialisasi variabel, (reset isi variabel)
  Parse.dt[j] = "";
  //proses parsing data
  for (int i = 1; i < Parse.dataIn.length(); i++)
  {
    //pengecekan tiap karakter dengan karakter (#) dan (,)
    if ((Parse.dataIn[i] == '#') || (Parse.dataIn[i] == ','))
    {
      //increment variabel j, digunakan untuk merubah index array penampung
      j++;
      Parse.dt[j] = "";
    }
    else
    {
      //proses tampung data saat pengecekan karakter selesai.
      Parse.dt[j] = Parse.dt[j] + Parse.dataIn[i];
    }
  }
  //auth
  if (Parse.dataIn[0] == '*' && Parse.dataIn[Parse.dataIn.length() - 2] == '#') {
    Motor.steps=Parse.dt[1].toInt();
    Motor.vel=11000-(Parse.dt[2].toInt()*100);
    Parse.dataValid = 1;
    Serial.print(Parse.dt[0]);
    Serial.print(",");
    Serial.print(Parse.dt[1]);
    Serial.print(",");
    Serial.print(Motor.vel);
    Serial.println("%");
  }
  switch(Parse.dt[0].toInt()){
    case 1:
    Motor.dir = 1;
    Motor.runs = 1;
    break;
    case 2:
    Motor.dir = 0;
    Motor.runs = 1;
    break;
    case 0:
    Motor.runs = 0;
    break;
    default:
    Motor.runs = 0;
  }
  Parse.dataIn="";
}
