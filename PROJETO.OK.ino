#include <Wire.h>
#include <SFE_MMA8452Q.h> // Biblioteca Acelerometro

int pin_IN1 = 2 ;
int pin_IN2 = 4 ;
int pin_velocidade = 3;

String leitura = "";
String juntar = "";
String recebe[2];
int aux = 0;
int vel_ultimo = 0;


MMA8452Q acelerometro1(0x1C);
//Cria uma instancia do MMA8452 chamada acelerometro1
//com endereco I2C 0x1C (pino SA0 em LOW/Desligado)
MMA8452Q acelerometro2(0x1D);
//Cria uma instancia do MMA8452 chamada acelerometro2
//com endereco I2C 0x1D (pino SA0 em HIGH/Ligado) 3.3V


void setup() {
  //Inicializa Pinos
  pinMode(pin_IN1, OUTPUT);
  pinMode(pin_IN2, OUTPUT);
  pinMode(pin_velocidade, OUTPUT);

  Serial.begin(9600);
  acelerometro1.init();
  acelerometro2.init();
}

void loop() {
  while (Serial.available() > 0) {

    leitura = (byte)Serial.read();

    if (leitura == '#') {
      recebe[aux] = juntar;
      juntar = "";
      aux++;
    } else if (leitura == ';') {
      recebe[aux] = juntar;
      juntar = "";
      leitura = "";
      aux = 0;
      break;
    } else {
      juntar = leitura;
    }
    delay(10);
  }

  ligar_motor(recebe[0]);

  velocidade(recebe[1]);

  acelerometro();

  delay(100);
}

void ligar_motor(String status_motor) {

  if (status_motor == "L") {
    digitalWrite(pin_IN1, HIGH);
    digitalWrite(pin_IN2, LOW);
  } else if (status_motor == "D") {
    frear_motor();
    digitalWrite(pin_IN1, LOW);
    digitalWrite(pin_IN2, LOW);
  }
}

void velocidade(String vel) {
  if (vel == "H") {
    analogWrite(pin_velocidade, 240); // Pino PWM vai de 0 à 255
    vel_ultimo = 240;
  } else if (vel == "L") {
    analogWrite(pin_velocidade, 90); // Pino PWM vai de 0 à 255
    vel_ultimo = 90;
  } else if (vel == "M") {
    analogWrite(pin_velocidade, 170); // Pino PWM vai de 0 à 255
    vel_ultimo = 170;
  } else if (vel == "U") {
    analogWrite(pin_velocidade, vel_ultimo); // Último valor lido
  }
}

void frear_motor() {
  digitalWrite(pin_IN1, HIGH);
  digitalWrite(pin_IN2, HIGH);
  delay(2000);
}

void acelerometro() {
  if (acelerometro1.available() && acelerometro2.available()) {
    acelerometro1.read();
    acelerometro2.read();

    Serial.print(acelerometro1.x, 3);
    Serial.print(";");
    Serial.print(acelerometro1.y, 3);
    Serial.print("#");
    Serial.print(acelerometro2.x, 3);
    Serial.print(";");
    Serial.print(acelerometro2.y, 3);
    Serial.println();
  }
}
