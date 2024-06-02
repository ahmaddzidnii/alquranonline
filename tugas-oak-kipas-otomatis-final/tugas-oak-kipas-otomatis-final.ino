#include <LiquidCrystal_I2C.h> // include library LCD
#include <DHT.h>               // include library DHT

#define dht11 2 // mendefinisikan pin sensor DHT11 pada pin digital 2
#define relay_kipas 3  // mendefinisikan pin relay kipas pada pin digital 3

// Inisialisasi LCD dengan alamat 0x27 dan ukuran 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Inisialisasi sensor DHT11 pada pin digital 2
DHT dht(dht11, DHT11);

// Definisikan pin untuk relay kipas

void setup() {
  // Memulai komunikasi serial dengan baud rate 9600
  Serial.begin(9600);
  
  // Memulai sensor DHT
  dht.begin();
  
  // Memulai LCD
  lcd.init();
  lcd.backlight(); // Menyalakan backlight LCD
  
  // Mengatur pin relay kipas sebagai output
  pinMode(relay_kipas, OUTPUT);
}

void loop() {
  // Baca data dari sensor DHT
  int kelembaban = dht.readHumidity();
  int suhu = dht.readTemperature();
  
  // Tampilkan data suhu di LCD
  lcd.setCursor(0,0); // Set kursor di kolom 0, baris 0
  lcd.print("Suhu:");
  lcd.setCursor(6,0); // Set kursor di kolom 6, baris 0
  lcd.print(suhu);
  lcd.setCursor(8,0); // Set kursor di kolom 8, baris 0
  lcd.print(".C");

  // Tampilkan status kipas di LCD
  lcd.setCursor(0, 1); // Set kursor di kolom 0, baris 1
  lcd.print("Kipas :");
  lcd.setCursor(8, 1); // Set kursor di kolom 8, baris 1
  
  char buffer[100]; // Buffer untuk menyimpan string

  if (suhu > 32) {
    digitalWrite(kipas, LOW); // karena relay yg saya pakai adalah relay LOW LEVEL TRIGGER maka relay diberi LOW untuk menyalakan relay
    lcd.print("ON "); 
    
    // Format JSON dan cetak ke Serial Monitor untuk keperluan komunikasi ke backend nodejs via serial port
    sprintf(buffer, "{\"suhu\":%d,\"kelembaban\":%d,\"kipas\":%s}", suhu, kelembaban, "true");
    Serial.println(buffer);
  } else {
    digitalWrite(kipas, HIGH); // karena relay yg saya pakai adalah relay LOW LEVEL TRIGGER maka relay diberi HIGH untuk mematikan relay
    lcd.print("OFF");
    
    // Format JSON dan cetak ke Serial Monitor untuk keperluan komunikasi ke backend nodejs via serial port
    sprintf(buffer, "{\"suhu\":%d,\"kelembaban\":%d,\"kipas\":%s}", suhu, kelembaban, "false");
    Serial.println(buffer);
  }
  delay(1000);
}
