  /** AxeDoor
 * Codigo para hacer sonar alarma en una hora especifica
 * Microcontrolador: Esp32
 * Escrito por: Útil Mente Geek
 * PaginaWeb: www.utilmentegeek.com
 * YouTube: https://www.youtube.com/channel/UCBqfcl_tue6Zsk4Hc3BErWg
 * Instagram: https://www.instagram.com/utilmentegeek/
 * 
 * Recuerda que tambien pudes contribuir a la mejora de este codigo en GitHub
 * Y apoyar el desarrollo de estos proyectos así como resolver dudas sobre el codigo
 * en nuestras redes socilaes @Util Mente Geek.
 */
 //Notas para cancion de mario
  #define NOTE_REST 0
  #define NOTE_C4 262   // Do (C4)
  #define NOTE_CS4 277  // Do sostenido (C#4)
  #define NOTE_D4 294   // Re (D4)
  #define NOTE_DS4 311  // Re sostenido (D#4)
  #define NOTE_E4 330   // Mi (E4)
  #define NOTE_F4 349   // Fa (F4)
  #define NOTE_FS4 370  // Fa sostenido (F#4)
  #define NOTE_G4 392   // Sol (G4)
  #define NOTE_GS4 415  // Sol sostenido (G#4)
  #define NOTE_A4 440   // La (A4)
  #define NOTE_AS4 466  // La sostenido (A#4)
  #define NOTE_B4 494   // Si (B4)
  #define NOTE_C5 523   // Do (C5)
  #define NOTE_CS5 554  // Do sostenido (C#5)
  #define NOTE_D5 587   // Re (D5)
  #define NOTE_DS5 622  // Re sostenido (D#5)
  #define NOTE_E5 659   // Mi (E5)
  #define NOTE_F5 698   // Fa (F5)
  #define NOTE_FS5 740  // Fa sostenido (F#5)
  #define NOTE_G5 784   // Sol (G5)
  #define NOTE_GS5 831  // Sol sostenido (G#5)
  #define NOTE_A5 880   // La (A5)
  #define NOTE_AS5 932  // La sostenido (A#5)
  #define NOTE_B5 988   // Si (B5)
  #define NOTE_C6 1047  // Do (C6)
  #define NOTE_CS6 1109 // Do sostenido (C#6)
  #define NOTE_D6 1175  // Re (D6)
  #define NOTE_DS6 1245 // Re sostenido (D#6)
  #define NOTE_E6 1319  // Mi (E6)
  #define NOTE_F6 1397  // Fa (F6)
  #define NOTE_FS6 1480 // Fa sostenido (F#6)
  #define NOTE_G6 1568  // Sol (G6)
  #define NOTE_GS6 1661 // Sol sostenido (G#6)
  #define NOTE_A6 1760  // La (A6)
  #define NOTE_AS6 1865 // La sostenido (A#6)
  #define NOTE_B6 1976  // Si (B6)
  #define NOTE_C7 2093  // Do (C7)
  #define NOTE_CS7 2217 // Do sostenido (C#7)
  #define NOTE_D7 2349  // Re (D7)
  #define NOTE_DS7 2489 // Re sostenido (D#7)
  #define NOTE_E7 2637  // Mi (E7)
  #define NOTE_F7 2794  // Fa (F7)
  #define NOTE_FS7 2960 // Fa sostenido (F#7)
  #define NOTE_G7 3136  // Sol (G7)
  #define NOTE_GS7 3322 // Sol sostenido (G#7)
  #define NOTE_A7 3520  // La (A7)
  #define NOTE_AS7 3729 // La sostenido (A#7)
  #define NOTE_B7 3951  // Si (B7)

//Tiempo de duracion para ls notas
  #define WHOLE 1600
  #define HALF 800
  #define QUARTER 400
  #define EIGHTH 200
  #define SIXTEENTH 100

  #include <Wire.h> // libreria que permite reconocer conexiones i2c
  #include "RTClib.h" // libreria de adafruit para rtc
  #include <Adafruit_GFX.h> //Libreria de graficos
  #include <Adafruit_SH110X.h> // libreria para usar la pantalla oled con ese controlador especifico

  #define SCREEN_WIDTH 128 // ancho de display oled en pixeles
  #define SCREEN_HEIGHT 64 // altura de display oled en pixeles
  #define OLED_RESET     -1 // pin de reset compartido con arduino, si tiene pin de reset la pantalla ponerlo con el de arduino
  #define SCREEN_ADDRESS 0x3C //direccion i2C de pantalla oled; si se tiene dudas poner programa de scanner i2c 
 Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  
  RTC_DS3231 rtc; //Se crea un objeto "rtc" de la clase rtc_ds3231 que se usa para interactuar con el modulo
  
  char daysOfTheWeek[7][12] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};//Se crea array de dos dimensiones de 7x12; dias de la semana x caracteres maximos.
  int Hora = 0; //Se declara variable entera para guardar la hora
  int Minuto = 0; //Se declara variable entera para guardar los minutos
  int Segundo = 0; //Se declara variable entera para guardar los segundos
  int anho = 0; //Se declara variable entera para guardar el año
  int dia = 0; //Se declara variable entera para guardar el dia
  int mes = 0; //Se declara variable entera para guardar el mes
  
  int BUZZER_PIN = 2; //Se declara pin que se usara para el buzzer
  int BUZZER_CHANNEL = 0; //Se declara un canal para usar con el buzzer, el cual es por si se llega a usar pwm.
  void setup () {
    Serial.begin(9600); // Se inicia comunicacion serial
    display.begin(SCREEN_ADDRESS, true);
    
    pinMode(BUZZER_PIN, OUTPUT); // se declara un pin establecido como salida
   
    delay(3000); // tiempo de espera
  
    if (! rtc.begin()) {                    //Se intenta iniciar el módulo RTC DS3231. Si no se puede encontrar el módulo RTC,
      Serial.println("Couldn't find RTC"); //se imprime un mensaje de error, se vacía el búfer de la comunicación serial
      Serial.flush();                      //y el programa entra en un bucle infinito.
      while (1);                        // serial.flush hace justo eso vacias el bufer donde se guardan los mensajes en serial y se asegura de enviarlos antes de continuar.
      delay(10);
    }
  
    if (rtc.lostPower()) { // si el rtc pierde el voltaje se actualiza con la fecha y hora dependiendo de la linea puesta.
      Serial.println("RTC lost power, lets set the time!");
      // las lineas siguientes ponen al rtc con fecha y hora de la compilacion de este programa
         rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      // Esta linea de abajo pone en el rtc una fecha y hora especificos para poner
      // Enero 21, 2014 a las 3am puedes llamar a la funcion:
      // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
     
    }
    // Cuando es necesario reestablecer la hora en un dispositivo previamente configurado
    // la siguiente linea pone la fecha y la hora de la compilacion del programa 
       rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // Esta linea pone una fecha y hora especifica por ejemplo
    // Enero 21, 2014 a las 3am puedes llamar a la funcion:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  
  }
  
  void loop () {
      DateTime now = rtc.now(); // se obtiene la hora y fecha del rtc
      
    //Se imprime fecha, día y hora en monitor serial
      Serial.print(now.year()); // imprime el año
      Serial.print('/');
      Serial.print(now.month()); // imprime el mes
      Serial.print('/');
      Serial.print(now.day()); // imprime el dia
      Serial.print(" (");
      Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
      Serial.print(") ");
      Serial.print(now.hour());
      Serial.print(':');
      Serial.print(now.minute());
      Serial.print(':');
      Serial.print(now.second());
      Serial.println();

      //SE guardan la fecha y hora en sus variables correspondientes
      Hora = now.hour();
      Minuto = now.minute();
      Segundo = now.second();
      anho = now.year();
      mes = now.month();
      dia = now.day();
      

      //comienza impresion en oled
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(SH110X_WHITE);
      display.drawRect(0, 0, 128, 64, SH110X_WHITE);
      display.drawLine(0, 44, 128, 44, SH110X_WHITE);
      display.drawLine(0, 23, 128, 23, SH110X_WHITE);// donde inicia en xy(0,23), distancia que tendra (128) y a que altura acaba (23)
      display.setCursor(15,5);
      display.print(daysOfTheWeek[now.dayOfTheWeek()]);
      display.setCursor(20,47);
      display.print(Hora);
      display.print(':');
      display.print(Minuto);
      display.print(':');
      display.print(Segundo);
      display.setCursor(15,27);
      display.print(dia);
      display.print('/');
      display.print(mes);
      display.print('/');
      display.print(anho);
      display.display();
      
      delay(100);
//S e pregunta por la hora y minuto espesificado para hacer sonar el buzzer por 30 segundos
if (Hora == 14 && Minuto == 58 && Segundo <= 30) {
    tone(BUZZER_PIN, NOTE_E7, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_E7, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_REST, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_E7, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_REST, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_C7, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_E7, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_REST, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_G7, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_REST, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_REST, QUARTER);
    delay(QUARTER);
    tone(BUZZER_PIN, NOTE_G6, QUARTER);
    delay(QUARTER);
    tone(BUZZER_PIN, NOTE_C7, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_G6, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_REST, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_E6, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_REST, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_REST, QUARTER);
    delay(QUARTER);
    tone(BUZZER_PIN, NOTE_E6, QUARTER);
    delay(QUARTER);
    tone(BUZZER_PIN, NOTE_REST, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_E7, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_REST, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_C7, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_E7, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_REST, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_G7, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_REST, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_REST, QUARTER);
    delay(QUARTER);
    tone(BUZZER_PIN, NOTE_G6, QUARTER);
    delay(QUARTER);
    tone(BUZZER_PIN, NOTE_C7, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_G6, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_REST, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_E6, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_REST, EIGHTH);
    delay(EIGHTH);
    tone(BUZZER_PIN, NOTE_REST, QUARTER);
    delay(QUARTER);
    tone(BUZZER_PIN, NOTE_E6, QUARTER);
    delay(QUARTER);
    tone(BUZZER_PIN, NOTE_REST, EIGHTH);
    delay(EIGHTH);
   } else {
     noTone(BUZZER_PIN);
     delay(10);
       }
  }
