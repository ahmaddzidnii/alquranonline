#include <LiquidCrystal_I2C.h> // include library LCD
#include <DHT.h>               // include library DHT

// Inisialisasi LCD dengan alamat 0x27 dan ukuran 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Inisialisasi sensor DHT11 pada pin 2
DHT dht(2, DHT11);

// Definisikan pin untuk relay kipas
int kipas = 3;

void setup() {
  // Memulai komunikasi serial dengan baud rate 9600
  Serial.begin(9600);
  
  // Memulai sensor DHT
  dht.begin();
  
  // Memulai LCD
  lcd.init();
  lcd.backlight();
  
  // Mengatur pin relay kipas sebagai output
  pinMode(kipas, OUTPUT);
}

void loop() {
  // Baca data dari sensor DHT
  int kelembaban = dht.readHumidity();
  int suhu = dht.readTemperature();
  
  // Tampilkan data suhu di LCD
  lcd.setCursor(0,0);
  lcd.print("Suhu:");
  lcd.setCursor(6,0);
  lcd.print(suhu);
  lcd.setCursor(8,0);
  lcd.print(".C");

  // Tampilkan status kipas di LCD
  lcd.setCursor(0, 1);
  lcd.print("Kipas :");
  lcd.setCursor(8, 1);
  
  // Buffer untuk menyimpan string
  char buffer[100];

  if (suhu > 32) {
    // Nyalakan kipas jika suhu di atas 32°C
    digitalWrite(kipas, LOW);
    lcd.print("ON ");
    
    // Format JSON dan cetak ke Serial Monitor
    sprintf(buffer, "{\"suhu\":%d,\"kelembaban\":%d,\"kipas\":%s}", suhu, kelembaban, "true");
    Serial.println(buffer);
  } else {
    // Matikan kipas jika suhu 32°C atau lebih rendah
    digitalWrite(kipas, HIGH);
    lcd.print("OFF");
    
    // Format JSON dan cetak ke Serial Monitor
    sprintf(buffer, "{\"suhu\":%d,\"kelembaban\":%d,\"kipas\":%s}", suhu, kelembaban, "false");
    Serial.println(buffer);
  }
  
  // Tunggu sebentar sebelum loop berikutnya
  delay(1000);
}
