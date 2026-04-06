from ultralytics import YOLO

def train():
    # Pre-trained Nano model (CPU-la fast-ah irukkum)
    model = YOLO('yolov8n.pt') 

    # Start training
    model.train(
        data='data.yaml',    # Unga folder-la irukura file name
        epochs=50,           # CPU-kaaga 50 epochs pothum
        imgsz=640,           
        device='cpu',        # GPU illaadha-dhaala 'cpu' nu kudukkirom
        batch=4,             # CPU load-ai kuraikka 4 veiyunga
        project='bridge_inspection'
        name='run1'
    )

if __name__ == '__main__':
    train()