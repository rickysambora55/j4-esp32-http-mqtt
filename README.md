## 🚀 About Me

Nama : Ricky Sambora<br>
NIM : 4.31.20.1.21<br>
Kelas : TE-3B

# Jobsheet 4 Komunikasi HTTP dan MQTT

Pada jobsheet 4 terdapat 4 project yaitu komunikasi sensor dan feedback menggunakan plaform cayenne, adafruit.io, thingspeak, serta komunikasi terpusat menggunakan ESP-NOW dan cayenne.

### Alat dan Bahan

-   ESP32
-   Arduino IDE (Terinstal ESP32)
-   Library DHT dan Adafruit unified sensor
-   Breadboard
-   Kabel Jumper
-   Resistor < 220Ω
-   LED
-   Sensor DHT
-   Sensor Water Level

### Instalasi ESP-32

1. Buka Arduino IDE
    - Masuk ke **Preferences**
    - Isikan board url dengan link : https://dl.espressif.com/dl/package_esp32_index.json dan simpan
    - Buka **Tools** > **Board** > **Boards Manager**
    - Cari ESP32, by Espressif. Kemudian instal
    - Pilih flash mode DIO/QIU menyesuaikan
2. Jalankan program .ino
3. Jika terdapat error saat uploading, tekan dan tahan tombol _Boot_ pada ESP32 saat upload, hingga _Connecting_ selesai

Nb. Proses instalasi dapat dilewati jika Arduino IDE sudah terintegrasi dengan ESP32.
Atau download `libraries` dan upload pada direktori projek.

### Instalasi DHT & Adafruit Libraries

1. Buka Arduino IDE
2. Buka **Sketch** > **Include Library** > **Library Manager**
3. Cari **DHT sensor library** by Adafruit. Kemudian instal.
    - Atau dapat melalui link [DHT Library](https://github.com/adafruit/DHT-sensor-library) dan upload pada libraries di direktori projek. Rename direktori menjadi **DHT_sensor_library**.
4. Instal juga library **Adafruit unified sensor** by Adafruit
    - Atau dapat melalui link [Adafruit Sensor](https://github.com/adafruit/Adafruit_Sensor) dan upload pada libraries di direktori projek. Rename direktori menjadi **Adafruit_Unified_Sensor**.

Nb. Proses instalasi dapat dilewati jika libraries telah diinstal.
Atau download `libraries` dan upload pada direktori projek.

## Project A - Komunikasi ESP-Sensor-LED dengan Cayenne

### Rangkaian & Instalasi

1. Siapkan ESP32 dan hubungkan ke Arduino IDE. Siapkan sensor water level (atau dht).
2. Buat rangkaian berikut.

<img src="https://user-images.githubusercontent.com/49542850/210127141-16c8ccac-03bc-4d1c-b08f-6b6acf302fbf.png" width="600px">

3. Download kode dari source code sesuai project.
4. Kunjungi website [Cayenne](https://cayenne.mydevices.com/) kemudian register atau login.
5. Tekan **Add New** > **Device/Widget** dan pilih **Bring Your Own Thing** pada bagian kiri.

<img src="https://user-images.githubusercontent.com/49542850/210127143-f94a8192-7e57-48d2-85f1-ac51fb300468.png" width="600px">

6. Install library:
    - Buka Arduino IDE
    - Buka **Sketch** > **Include Library** > **Manage Libraries**.
    - Cari library **CayenneMQTT** by myDevices kemudian install.
    - Atau dapat melalui link [CayenneMQTT](https://github.com/adafruit/Adafruit_Sensor) dan upload pada libraries di direktori projek. Rename direktori menjadi **CayenneMQTT**.
7. Pilih perangkat yang sudah ditambahkan, kemudian rename board untuk mempermudah. Copy kredensial yang diberikan dan isikan dalam kode pada bagian yang sudah disediakan. Kemudian upload kode.

<img src="https://user-images.githubusercontent.com/49542850/210127144-d112f437-7a3e-4969-af98-be98eb8568cf.png" width="600px">

8. Setelah ESP terdeteksi, kemudian tekan **Add New** > **Device/Widget** dan pilih custom widget tank.

<img src="https://user-images.githubusercontent.com/49542850/210127145-976c8dda-257f-4d08-b3e8-d7bcd965f6e5.png" width="600px">

9. Masukkan Nama, Data: Tank level, Unit: Analog/Menyesuaikan, Channel: 1 (pada contoh), Min: 0, Max: 100, dan Display. Kemudian simpan.

<img src="https://user-images.githubusercontent.com/49542850/210127136-a67a45d4-7c5a-4846-b810-5e2fe61bcdd6.png" width="600px">

10. Lakukan hal yang sama untuk LED. Pilih widget tipe button, isikan Nama, Data: Digital Acuator, Unit: Digital, Channel: 0 (pada contoh), dan icon. Kemudian simpan.

<img src="https://user-images.githubusercontent.com/49542850/210127139-b8c872b4-6f11-4bb1-80a1-afb11564d474.png" width="600px">

11. Jalankan kode.
12. LED dapat dikontrol dengan menekan icon widget LED.

<img src="https://user-images.githubusercontent.com/49542850/210127140-b82c9394-7435-4974-85ea-009bb44f0c0f.png" width="600px">

Nb. Untuk versi sensor DHT dapat mendownload kode untuk DHT dan konfigurasi Cayenne serupa. Widget type value, data temperature dan humidity.

### Penjelasan

ESP membaca data analog dari sensor water level. Pada contoh, nilai yang didapat adalah 0 hingga 1900. Nilai ini dapat bervariasi tergantung jenis air, sensor, dan keakuratan (kualitas). Nilai tersebut diubah menjadi bentuk persentase agar lebih mudah dalam pembacaan dan kemudian diupload pada Cayenne melalui protokol MQTT. Konfigurasi channel dan value antara kode dan Cayenne harus sinkron agar data dapat dibaca. Respon dari web Cayenne memiliki keterlambatan sekitar 3-5 detik, sehingga data tidak real-time. Proses upload/publish dilakukan pada fungsi `CAYENNE_OUT(channel){}` dengan perintah `Cayenne.virtualWrite(channel, data, [type], [unit])`.

Sedangkan untuk LED membutuhkan download data menggunakan fungsi `CAYENNE_IN(channel){}`. Pada fungsi tersebut digunakan perintah menyalakan LED dengan value berupa placeholder cayenne, yaitu `digitalWrite(ledPin, !getValue.asInt())`. Value tersebut didapatkan berdasarkan konfigurasi widget yaitu data digital 0/1.

### Keluaran

https://user-images.githubusercontent.com/49542850/210127142-1d9c5821-0879-481f-92ba-81bd1a397621.mp4

## Project B - Komunikasi ESP-Sensor-LED dan Google Assistant dengan Adafruit.io

### Rangkaian & Instalasi

1. Siapkan ESP32 dan hubungkan ke Arduino IDE. Siapkan sensor DHT (atau lainnya).
2. Buat rangkaian berikut.

<img src="" width="600px">

3. Download kode dari source code sesuai project.
4. Kunjungi website [Adafruit](https://io.adafruit.com/) kemudian register atau login.
5. Pada navbar paling atas, pilih **IO**.
    - Pada bagian ini kita akan menggunakan **Dashboard** dan **Feeds**.
    - **Dashboard** adalah papan kerja atau ruang kontrol terhadap Feeds (perangkat) IoT.
    - **Feeds** adalah suatu grup sebagai identitas suatu atau kumpulan perangkat.
    - Feeds dapat berisi lebih dari satu perangkat. Jika kita mengubah nilai dari suatu feeds, maka seluruh perangkat yang ada pada feeds akan terpengaruh.

<img src="" width="600px">

6. Tekan **Dashboard** dan pilih **New Dashboard**.
7. Klik gerigi opsi dan pilih **Create new block**

### Penjelasan

Sensor DHT (yang digunakan adalah DHT11) akan membaca data berupa suhu ruangan, kelembapan, dan index suhu.

### Keluaran

<img src="https://cdn.discordapp.com/attachments/1043462519336996894/1051441808548302888/B._Sensor_DHT.png" width="600px">

## Project C - Sensor RFID

### Rangkaian & Instalasi

.

### Penjelasan

.

### Keluaran

<img src="https://cdn.discordapp.com/attachments/1043462519336996894/1051504836014784522/C._RFID.png" width="600px">

https://user-images.githubusercontent.com/49542850/206910208-4912f5ef-f3ea-4ebc-94e4-2878b3abd4db.mp4

## Kesimpulan

-   ESP32 kompatibel dengan beberapa sensor seperti DHT dan RFID serta penggerak seperti servo. Akan tetapi, beberapa alat memerlukan library atau tool yang berbeda dengan arduino dan tidak kompatibel dengan ESP. Sehingga perlu melakukan import eksternal.
-   Beberapa pin ESP32 dapat mengukur nilai kapasitif dari pin tersebut. Sehingga dapat diperlakukan seperti touch sensor. Contoh pin yang dimaksud untuk ESP32 v1 adalah pin 21, 22, 24, 12, 13, 16, 17, 18, dan 20.
-   Sensor DHT mampu mengukur suhu dan kelembaban ruangan dan dapat divariasi menjadi satuan celcius maupun farenheit untuk suhunya. Serta terdapat heat index.
-   Sensor RFID mampu membaca chip atau lempengan yang terdapat pada kartu RFID atau pin dan lain sebagainya untuk diterjemahkan menjadi address/alamat. Address tiap kartu bersifat unik dan jarang ada kesamaan, address difungsikan seperti Mac pada perangkat jaringan.
-   Address pada kartu RFID dapat digunakan sebagai identifier kartu sehingga dapat memilah kartu yang akan diberikan akses dan ditolak.
-   Sistem RFID juga digunakan pada e-KTP dan e-Toll namun dengan frekuensi yang berbeda.
-   Perangkat servo dapat bergerak porosnya sejauh 180°. Poros harus digerakan sudut demi sudut.
