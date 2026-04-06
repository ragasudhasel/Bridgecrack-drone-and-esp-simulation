from ultralytics import YOLO
import cv2

# Trained model-ai load pannunga
model = YOLO('bridge_inspection/run1/weights/best.pt')

# Oru test image-ai predict pannunga (path-ai check pannikonga)
results = model.predict(source=r"D:\bridge1\test\images\katrina-corrosion-112_png.rf.9588850c1554baecdd293bf31d84cc27.jpg", save=True, conf=0.25)
print("Prediction mudinjiduchu! Check the 'runs' folder for results.")
