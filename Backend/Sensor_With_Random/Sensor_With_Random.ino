#include <ESP8266WiFi.h>
#include <espnow.h>

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xE8, 0xDB, 0x84, 0xE7, 0x98, 0xE3};

// Set your Board ID (ESP32 Sender #1 = BOARD_ID 1, ESP32 Sender #2 = BOARD_ID 2, etc)
#define BOARD_ID 1

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int id;
  int distance;
} struct_message;

// Create a struct_message called myData to store variables to be sent
struct_message myData;

double duration; // Variable to store the duration of the pulse
float distance; // Variable to store the calculated distance
float maxDistance = 9; // Maximum distance when the container is empty
float minDistance = 1;  // Minimum distance when the container is full
#define TRIG_PIN D7  // Trig pin of ultrasonic sensor
#define ECHO_PIN D2  // Echo pin of ultrasonic sensor
#define LED_PIN D6  // Low water level LED pin

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("\r\nLast Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  } else {
    Serial.println("Delivery fail");
  }
}

void setup() {
  // Init Serial Monitor
  Serial.begin(9600);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Set ESP-NOW role
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  // Once ESPNow is successfully init, we will register for Send CB to
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

    // Generate a random distance within the range
    float randomDistance = random(minDistance , maxDistance ) ;

    // Calculate the percentage of water level
    // float percentage = (1 - (randomDistance - minDistance) / (maxDistance - minDistance)) * 100;
    float percentage = randomDistance;

    // Set values to send
    myData.id = BOARD_ID;
    myData.distance = percentage;

    // Send message via ESP-NOW
    esp_now_send(0, (uint8_t *)&myData, sizeof(myData));

    // Control the LED based on the water level
    if (percentage < 3) {
      digitalWrite(LED_PIN, HIGH); // Turn on the LED connected to pin D6
    } else {
      digitalWrite(LED_PIN, LOW); // Otherwise, turn off the LED
    }

    Serial.print("percentage: "); // Print the label "percentage" to the serial monitor
    Serial.println(percentage);   // Print the calculated percentage to the serial monitor
    lastTime = millis();
  }
}
