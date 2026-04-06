from html2image import Html2Image
import time

hti = Html2Image(size=(1920, 1080))
hti.output_path = '.'

# Force some delay or layout
time.sleep(2)

print("Taking screenshot of Dashboard (/)...")
hti.screenshot(url='http://127.0.0.1:5000/', save_as='dashboard_screenshot.png')

print("Taking screenshot of Inspection page (/inspection)...")
hti.screenshot(url='http://127.0.0.1:5000/inspection', save_as='inspection_screenshot.png')

print("Screenshots saved successfully!")
