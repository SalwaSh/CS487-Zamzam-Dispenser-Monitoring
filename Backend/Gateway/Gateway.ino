// Include the ESP8266WiFi library
#include <ESP8266WiFi.h>
// Include the ESP-NOW library
#include <espnow.h> 
// Include the Firebase ESP8266 library
#include <Firebase_ESP_Client.h>

// Define the Wi-Fi credentials
// #define WIFI_SSID "AndroidAP40AA"
// #define WIFI_PASSWORD "cukj2015"
#define WIFI_SSID "ManazelMarez 202"
#define WIFI_PASSWORD "12344321"
// Define the Project Database URL and Firebase API Key
#define DATABASE_URL 
#define API_KEY 

// Define the structure (like, a class) for the message
typedef struct struct_message {
    int id;
    float distance;
} struct_message;

// Declare a variable to save received data
struct_message myData;
// Declare two elements that store messages received from two different boards
struct_message board1;
struct_message board2;
// Declare an array of boards with a size of 2
struct_message boardsStruct[2] = {board1, board2};

// Declare a FirebaseData object to handle Firebase operations, like: reading data, writing data, establish Firebase connection
FirebaseData fbdo;
// Declare a FirebaseAuth object for user authentication (email, and password)
// two email and password  are empty, going with an anonymous user
FirebaseAuth auth;
// Declare a FirebaseConfig object to store Firebase configuration (Project Database URL & Firebase API Key)
FirebaseConfig config;
// Declare a boolean variable to track the success of signup operation
bool signupOK = false;
// Declare a boolean variable to track if new data has been received
bool newDataReceived = false;

// Callback function that will be executed when data is received
// It takes MAC address (uint8_t: allows each byte of the MAC address to be stored individually), 
// incoming data, length of the incoming data
void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) {
  // Declare a variable to store the MAC address as a string
  char macStr[18];
  Serial.print("Packet received from: ");
  //Convert the MAC address to a string representation
  //Snprintf is used to format the MAC address into the macStr array
  //The "02" specifies that each byte should be represented by two characters (digits).
  //The "x" indicates that the byte should be formatted as a hexadecimal number.
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  // Copy the received data into the myData structure
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  // Update the corresponding board's ID and percentage in the boardsStruct array
  boardsStruct[myData.id-1].id = myData.id;
  boardsStruct[myData.id-1].distance = myData.distance;
  Serial.printf("x value: %d \n", boardsStruct[myData.id-1].distance);
  Serial.println();
  // Set the flag to indicate that new data has been received
  newDataReceived = true;
}
 
void setup() {
  // Initialize the serial communication
  Serial.begin(115200);
  // Set the Wi-Fi mode to station mode
  //In station mode, the device acts as a client and connects to an existing Wi-Fi network rather than creating its own network. 
  //This allows the device to connect to the internet.
  WiFi.mode(WIFI_STA);
  // Disconnect from any previous Wi-Fi connection.
  WiFi.disconnect();
  // Init ESP-NOW
  // Initialize ESP-NOW and check for errors
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;  }
 // Connect to Wi-Fi using the provided credentials
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
 // Wait for the Wi-Fi connection to be established
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    // Delay for 300 milliseconds before checking the connection status again
    delay(300);
}
  Serial.println();
  // Print a message to indicate that the device is successfully connected to Wi-Fi
  Serial.print("Connected with IP: ");
  // Print the local IP address assigned to the device
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;
  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  // Sign up to Firebase using the provided credentials and with an anonymous user
  if (Firebase.signUp(&config, &auth, "", "")){
    // If the sign-up is successful
    Serial.println("ok");
    signupOK = true;
  }
  else{
    // If the sign-up fails, print the error message
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  // Initialize Firebase connection with the provided configuration
  Firebase.begin(&config, &auth);
  // Enable automatic reconnection to Wi-Fi
  Firebase.reconnectWiFi(true);
  
  // Set the ESPNow role as a receiver (client)
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  // Once ESPNow is successfully initialized, register the callback function to receive data
  esp_now_register_recv_cb(OnDataRecv);
}

void loop(){
  // Check if Firebase is ready and sign up was successful
  if (Firebase.ready() && signupOK) {
    // Check if new data is received
    if (newDataReceived) {
      // Send data to Firebase Realtime Database
      String DBpath = "Containers/Container" + String(boardsStruct[myData.id-1].id) + "/waterLevel";
      if (Firebase.RTDB.setFloat(&fbdo, DBpath ,  boardsStruct[myData.id-1].distance )) {
        Serial.println("Data send successful");
        Serial.println("PATH: " + fbdo.dataPath());
        Serial.println("TYPE: " + fbdo.dataType());
      }
      else {
        Serial.println("Data send failed");
        Serial.println("REASON: " + fbdo.errorReason());
      }
      // Reset the flag for "new data is received"
      newDataReceived = false; 
    }
  }
}