//These lines include the necessary libraries for the ESP8266 WiFi module 
//and the ESP-NOW communication protocol.
#include <ESP8266WiFi.h>
#include <espnow.h>

//RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xE8, 0xDB, 0x84, 0xE7, 0x98, 0xE3};

// my Board ID (Container2)
#define BOARD_ID 2

// This defines a structure (struct_message) that represents the data to be sent.
//It includes an ID and a distance value.
// Must match the receiver structure
typedef struct struct_message {
    int id;
    float distance;
} struct_message;

// Create a struct_message called myData to store variables to be sent
struct_message myData;

//These lines declare variables for storing pulse duration, distance, and percentage. 
//It also defines pin assignments for the ultrasonic sensor trigger (TRIG_PIN), echo (ECHO_PIN), and the LED pin (LED_PIN).
double duration; // Variable to store the duration of the pulse
float distance;// Variable to store the calculated distance
#define TRIG_PIN D7  // Trig pin of ultrasonic sensor
#define ECHO_PIN D2  // Echo pin of ultrasonic sensor
#define LED_PIN D6  // Low water level LED pin

//These lines declare variables for tracking the last time the measurements were taken 
//and set a delay of 10 seconds between measurements.
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

// Callback function (OnDataSent) whenthat will be called when data is sent using ESP-NOW.
// It prints the delivery status (success or fail) 
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("\r\nLast Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
//This is the setup() function. It initializes the serial monitor,
// sets up the ESP-NOW communication, registers the callback function,
// adds the receiver device as a peer, and sets up the pin modes for the ultrasonic sensor and LED.
void setup() {
  // initializes the serial communication with a baud rate of 9600.
  Serial.begin(9600);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);//sets the device as a Wi-Fi station, connect the ESP32 module to a WiFi access point
  WiFi.disconnect();//disconnects from any existing Wi-Fi network

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } 
  // Set ESP-NOW role, sets the device role as a controller to initiating communication and sending data.
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  // Once ESPNow is successfully init, we will register for Send callback(CB) to
  // get the status of Transmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  pinMode(TRIG_PIN, OUTPUT); // Set pin D7 as an output for the trigger signal
  pinMode(ECHO_PIN, INPUT); // Set pin D2 as an input for the echo signal
  pinMode(LED_PIN, OUTPUT); // Set pin D6 as an output for the LED
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration * 0.034 / 2;

    // Set values to send
    myData.id = BOARD_ID;
    myData.distance = distance;

    // Send message via ESP-NOW
    esp_now_send(0, (uint8_t *) &myData, sizeof(myData));

    // Control the LED based on the water level
    if (distance > 5) {
      digitalWrite(LED_PIN, HIGH); // Turn on the LED connected to pin D6
    } else {
      digitalWrite(LED_PIN, LOW); // Otherwise, turn off the LED
    }

    Serial.print("distance: "); // Print the label "percentage" to the serial monitor
    Serial.println(distance); // Print the calculated percentage to the serial monitor
    lastTime = millis();
  }

}