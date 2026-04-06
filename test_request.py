import requests

url = 'http://localhost:5000/upload_image'
files = {'file': open('prediction_test.jpg', 'rb')}

try:
    response = requests.post(url, files=files)
    print("Status Code:", response.status_code)
    try:
        print("JSON Response:", response.json())
    except:
        print("Text Response:", response.text)
except Exception as e:
    print("Error:", e)
