# Smart Lamp (Lampa Inteligenta) - IoT Project

Un sistem de iluminat complet automatizat, dezvoltat cu o arhitectura hibrida hardware-software.

## Descriere Minimala
Proiectul consta intr-o banda LED controlata de microcontrolerul ATmega328P. Lampa poate functiona complet autonom, pe baza luminii ambientale si a detectiei de miscare, sau poate fi controlata de la distanta de pe telefon, printr-o interfata web. Un server Python ruleaza local pentru a face puntea intre request-urile HTTP venite din retea (prin SSH Tunneling) si comunicatia seriala Bluetooth.

## Functionalitati Principale
* **Mod Automat (Smart):** Se aprinde doar cand este intuneric (senzor LDR) si detecteaza miscare in incapere (senzor PIR).
* **Mod Manual:** Aprindere / stingere fortata folosind un buton fizic (intreruperi hardware) sau butoanele din aplicatia web.
* **Control Remote Ocolind AP Isolation:** Utilizarea unui tunel SSH pentru a putea trimite comenzi din exterior (de pe date mobile) catre serverul local, sarind peste restrictiile de izolare din retelele publice.

## Arhitectura si Tehnologii Folosite
Proiectul este impartit in trei componente:

1. **Firmware (C / PlatformIO):**
   * Control hardware direct pe registrii ATmega328P.
   * Modulul UART pentru comunicatia Bluetooth (9600 bps).
   * Modulul ADC pentru citirea senzorului analogic de lumina.
   * Timere (Timer 0) pentru generarea semnalului PWM catre tranzistorul MOSFET.
   * Intreruperi Externe (INT0, INT1) pentru timpi de raspuns instantanei la senzori.

2. **Backend (Python / Flask):**
   * Un server REST API care preia comenzile de la interfata web.
   * Foloseste biblioteca `pyserial` pentru a scrie direct pe portul COM catre modulul Bluetooth.

3. **Frontend (JS):**
   * Interfata web, hostata direct pe GitHub Pages.
   * Foloseste `Fetch API` pentru a trimite request-uri asincrone (POST) catre server.

## Componente Hardware
* Microcontroler ATmega328P
* Modul Bluetooth HC-06 (cu divizor de tensiune pentru pinul RX)
* Senzor de miscare PIR (HC-SR501)
* Fotorezistor (LDR) in configuratie divizor de tensiune (10k ohm)
* Buton fizic (push button)
* Tranzistor MOSFET pentru controlul puterii benzii LED
* Banda LED 
* Buzzer activ