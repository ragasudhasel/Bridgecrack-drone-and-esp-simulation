from ultralytics import YOLO
import cv2

# Load the model
model_path = r'd:/bridge1 -app/bridge_inspection/run1/weights/best.pt'
model = YOLO(model_path)

# Test image
img_path = r'd:/bridge1 -app/112150905.jpg'

# Run inference
results = model(img_path)

# Show results
for result in results:
    boxes = result.boxes
    print(f"Detected {len(boxes)} objects")
    result.save('prediction_test.jpg')
