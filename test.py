import cv2
from ultralytics import YOLO

# Model load pannunga
model = YOLO(r'bridge_inspection/run1/weights/best.pt')

# 0 kudutha Laptop WebCam-ai edukkum
cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret: break

    # Real-time detection
    results = model.predict(source=frame, conf=0.3)
    
    # Results-ai frame mela draw panna
    annotated_frame = results[0].plot()

    cv2.imshow("Webcam Live Test", annotated_frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()