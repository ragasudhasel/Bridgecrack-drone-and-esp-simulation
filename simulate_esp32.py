import requests
import time
import os
import random

# Configuration
URL = 'http://localhost:5000/upload_image'
IMAGE_PATH = 'prediction_test.jpg' 

def simulate_esp32():
    if not os.path.exists(IMAGE_PATH):
        print(f"Error: Test image '{IMAGE_PATH}' not found.")
        return

    print(f"Starting ESP32 Simulation... Uploading '{IMAGE_PATH}' every 5 seconds.")
    
    count = 1
    while True:
        try:
            # Re-open file each time to simulate a new capture
            with open(IMAGE_PATH, 'rb') as img:
                # We send it with a unique filename so the server treats it as new
                files = {'file': (f'esp32_cam_{count}.jpg', img, 'image/jpeg')}
                
                print(f"[{count}] Uploading image...", end=" ")
                response = requests.post(URL, files=files)
                
                if response.status_code == 200:
                    data = response.json()
                    if data.get('success'):
                        print("Success! ID:", data.get('detections', [{}])[0].get('id'))
                    else:
                        print("Failed:", data.get('error'))
                else:
                    print("HTTP Error:", response.status_code)
            
            count += 1
            time.sleep(5) 
            
        except Exception as e:
            print(f"\nConnection Error: {e}")
            print("Is the server running?")
            time.sleep(5)

if __name__ == "__main__":
    simulate_esp32()
