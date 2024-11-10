#include <WiFi.h>
#include <WebServer.h>  // Simple HTTP server

#include <CryptoAES_CBC.h>
#include <AES.h>
#include <string.h>

// 128-bit AES Key (16 bytes)
byte key[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
                 0x08, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };

// The plaintext to encrypt (longer than 16 bytes)
const char* text = "Hello World, Nice to meet you!";
byte plaintext[64];  // Make sure this array is large enough to hold the entire plaintext
byte ciphertext[64]; // Buffer to hold the encrypted text
byte decryptedtext[64]; // Buffer for decrypted text
byte iv[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
                 0x08, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
String ciphertextStr = "";

AES128 aes128;

const char* ssid = "Toon_2.4G";
const char* password = "11111111";

WebServer server(80);

void handleRoot() {
    server.send(200, "text/plain", ciphertextStr);
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

     // Convert the plaintext string into a byte array
    int length = strlen(text);
    for (int i = 0; i < length; i++) {
      plaintext[i] = text[i];
    }

    // Pad the plaintext if necessary to make its length a multiple of 16
    int padding = 16 - (length % 16);
    for (int i = 0; i < padding; i++) {
      plaintext[length + i] = padding;  // Pad with the padding value
    }
    length += padding;  // Update length to include padding

    aes128.setKey(key, 16);  // Set the AES key

    Serial.println("Before Encryption:");

    // Print the plaintext as hex values
    for (int i = 0; i < length; i++) {
      Serial.print(plaintext[i], HEX);
      Serial.print(" ");
    }

    Serial.println();
    
    // Encrypt the plaintext in blocks (CBC mode)
    byte block[16];  // Temporary block for each 16-byte chunk
    byte iv_copy[16]; // Copy of IV for CBC mode to keep the IV intact between blocks
    memcpy(iv_copy, iv, 16);  // Copy the original IV for each encryption round

    int blocks = length / 16;
    int index = 0;

    for (int i = 0; i < blocks; i++) {
      // Copy the current block from the plaintext
      memcpy(block, &plaintext[index], 16);

      // Encrypt the block
      aes128.encryptBlock(ciphertext + (i * 16), block);  // Store encrypted data in ciphertext
      
      // Update index to the next block
      index += 16;
    }

    Serial.println("After Encryption:");

    // Print the encrypted ciphertext as hex values
    for (int i = 0; i < length; i++) {
      // Append each byte in hex format to the string
      ciphertextStr += String(ciphertext[i], HEX) + " ";
    }
    Serial.println(ciphertextStr);
    server.on("/", handleRoot);
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}
