#include <DHT.h> //incluímos libbrería para sensor de temperatura y  humedad
#include <DHT_U.h>

// pins de RGB
int pinR = 3; //rojo
int pinG = 5; //verde
int pinB = 6; //azul

//pins de joystick
int pinx = A1; //eje x
int piny = A0; //eje y
int joyb = 4; //botón

//pin y variables de botón
int button = 2;
int last1 = LOW;
int last2 = LOW;
int counter1 = 0;
int counter2 = 0;

//pins de display
int A = 7; //segmento superior derecho
int B = 8; //segmento superior
int C = 9; //segmento superior izquierdo
int D = 10; //segmento central
int E = 11; //segmento inferior derecho
int F = 12; //segmento inferior
int G = 13; //segmento inferior izquierdo

//pins de sensores
int mic = A2;
int weather = A3;
#define DHTTYPE DHT11
DHT dht(weather, DHTTYPE);
#define IR A5

void setup() {

  Serial.begin(9600);
  dht.begin();

  pinMode(pinR,OUTPUT);
  pinMode(pinG,OUTPUT);
  pinMode(pinB,OUTPUT);

  pinMode(joyb,INPUT_PULLUP);

  pinMode(button,INPUT_PULLUP);

  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(C,OUTPUT);
  pinMode(D,OUTPUT);
  pinMode(E,OUTPUT);
  pinMode(F,OUTPUT);
  pinMode(G,OUTPUT);

  //establecemos el RGB a 0 todos los valores
  analogWrite(pinR,0);
  analogWrite(pinG,0);
  analogWrite(pinB,0);

  //hacemos que el dipsplay empiece mostrando 0 y empiece modo default
  display(0);
  modes(0);
}


void loop() {
  //definimos variable para el contador
  int state = digitalRead(joyb);
 
  if (state == LOW && last1 == HIGH) { //state = el botón se presiona AHORA || last = el botón no se presionó ANTES
    counter1 = counter1 + 1; //se suma 1 al contador
    display(counter1);
    delay(300);
    display(10);
    analogWrite(pinR,0);
    analogWrite(pinG,0);
    analogWrite(pinB,0);
    delay(300);
    if (counter1 > 2) counter1 = 0; //el contador se reincia si llega al 2
    display(0);
    counter2 = 0;
    }
  last1 = state; //se actualiza el último estado luego de hacer click
  modes(counter1); //cambia el modo

}

void modes(int number) {
  switch (number) {
    case 0: {
      //definimos ejes del joystick
      int x = analogRead(pinx);
      int y = abs(1023 - analogRead(piny));

      Serial.println(counter1);

      //definimos colores del RGB
      int red = 0;
      int green = 0;
      int blue = 0; //futura idea: crear una variable amarillo para que se comporte como el azul pero en el eje x derecho

      //definimos valores y rangos de los colores
      if (x > 512) {
        //eje x izquierdo
        red = constrain(map(y,512,1023,0,255),0,255);
        green = constrain(map(y,512,0,0,255),0,255);
        blue = constrain(map(x,505,1023,0,255),0,255);
      } else {
        //eje x derecho
        red = constrain(map(y,0,1023,20,255),0,255);
        green = constrain(map(y,1000,0,0,255),0,255);
      }

      //prendemos luces RGB
      analogWrite(pinR,red);
      analogWrite(pinG,green);
      analogWrite(pinB,blue);

      //definimos contador para el display
      int state = digitalRead(button);
      if (state == LOW && last2 == HIGH) { //state = el botón se presiona AHORA || last = el botón no se presionó ANTES
        counter2 = counter2 + 1; //se suma 1 al contador
        if (counter2 > 9) counter2 = 0; //el contador se reincia si llega al 10
        display(counter2); //se ejecuta el display
      }
      last2 = state; //se actualiza el último estado luego de hacer click
      break;
    }

    case 1: {
      //definimos variables
      int levelmic = constrain(analogRead(mic),30,300); //a base de prueba y error, ajustables según entorno
      int color1 = constrain(map(levelmic,30,300,0,255),0,255); //rangos de calibración, ajustables
      int num = constrain(map(levelmic,30,300,0,9),0,9);

      //0 azul y display a escala
      analogWrite(pinB,0);
      display(num);
      
      if (levelmic > 55) { //si el nivel de sonido supera x número
        analogWrite(pinR,color1);
        analogWrite(pinG,255-color1); //verde proporcional al rojo
        delay(500); //para evitar cambios de luz rápidos
      } else {
        analogWrite(pinR,0);
        analogWrite(pinG,color1); //verde cuando no toma un valor mayor a x número
      }
      break;
    }

    case 2: {
      //definimos variables
      int temp = dht.readTemperature();
      int hum = dht.readHumidity();
      int color2 = constrain(map(hum,20,90,0,255),0,255);
      int dec = temp/10; //primer dígito de temperatura
      int un = temp%10; //segundo dígito de temperatura

      //definimos rgb para humedad
      if (hum > 30) {
        analogWrite(pinR,255-color2);
        analogWrite(pinG,color2);
      } else {
        analogWrite(pinR,200);
        analogWrite(pinG,0);
      }

      //mostramos display de temperatura
      delay(1000);
      display(dec);
      delay(500);
      display(un);
      delay(500);
      display(10);
      Serial.print("temperatura: ");
      Serial.println(temp);
      Serial.print("humedad: ");
      Serial.println(hum);
      break;
    }
  }
}


void display(int number) { //código del display

  //apagamos todos los segmentos primero
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);

  //segmento para cada número
  switch (number) {
   case 0:
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(E, HIGH);
      digitalWrite(F, HIGH);
      digitalWrite(G, HIGH);
      break;
     
    case 1:
      digitalWrite(A, HIGH);
      digitalWrite(E, HIGH);
      break;
     
    case 2:
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(D, HIGH);
      digitalWrite(F, HIGH);
      digitalWrite(G, HIGH);
      break;
     
    case 3:
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(D, HIGH);
      digitalWrite(E, HIGH);
      digitalWrite(F, HIGH);
      break;


    case 4:
      digitalWrite(A, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, HIGH);
      digitalWrite(E, HIGH);
      break;
     
    case 5:
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, HIGH);
      digitalWrite(E, HIGH);
      digitalWrite(F, HIGH);
      break;
     
    case 6:
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, HIGH);
      digitalWrite(E, HIGH);
      digitalWrite(F, HIGH);
      digitalWrite(G, HIGH);
      break;
     
    case 7:
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(E, HIGH);
      break;
     
    case 8:
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, HIGH);
      digitalWrite(E, HIGH);
      digitalWrite(F, HIGH);
      digitalWrite(G, HIGH);
      break;
     
    case 9:
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, HIGH);
      digitalWrite(E, HIGH);
      break;
    
    case 10:
      break;
  }
}
