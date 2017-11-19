double R = 100;           // this is a current limitting resistance
double K = 30;            // this is the springs constant
double f = 50;            // this is the used frequency
double S = 10;            // this is a resitor at which the voltage will be measured
double r = 0.2;           // this is the resistance of the coil
double u = 1;   // this is the magnetic permeability
double N = 5;             // this is the number of windings
double A = 0.0001;        // this is the area of the windings
double Rt = R + S + r;
double c = 2*3.14*f*u*sq(N)*A;

int pin = A0;
double v0;
double dom0;
double nom0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pin, INPUT);
  v0 = map(analogRead(pin), 0, 1024, 0, 5);
  dom0 = sqrt(25*sq(S) + sq(v0)*sq(Rt));
  nom0 = sq(v0);

}


void loop() {
  // put your main code here, to run repeatedly:
  double v = map(analogRead(pin), 0, 1024, 0, 5);
  double dom = sqrt(25*S*S + sq(v)*sq(Rt));
  double nom = sq(v);
  double force = c*(nom/dom - nom0/dom0);
  Serial.println(force);
  Serial.println(v0);
  Serial.println(v);
  Serial.println(nom);
  Serial.println(nom0);
  Serial.println(dom);
  Serial.println(dom0);
  Serial.println(c);
  Serial.println("----------------------");
  delay(1000);

}
