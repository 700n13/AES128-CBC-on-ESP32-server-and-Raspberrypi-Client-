import requests

# IP address of the ESP32 (replace with the actual IP address)
esp32_ip = "http://192.168.1.38"

try:
    response = requests.get(esp32_ip)  # Sending GET request to ESP32
    if response.status_code == 200:
        print("Response from ESP32:\n", response.text)
        print("data byte:", len(bytes(response.text.encode("utf-8"))))
    else:
        print("Failed to get response. Status code:", response.status_code)
except requests.exceptions.RequestException as e:
    print("Error:", e)
