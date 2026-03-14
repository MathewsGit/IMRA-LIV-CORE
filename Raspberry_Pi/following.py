import cv2
import time

# Initialize the face detector
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# Initialize CSRT tracker
tracker = cv2.TrackerMIL_create()
tracking = False
tracked_object = None

def log_direction_change(distance_from_center, frame_center):
    print(distance_from_center)
    if distance_from_center > 50:
        print("Turn right")
    elif distance_from_center < -50:
        print("Turn left")
    else:
        print("Face centered")

# Use the default webcam (0) for video capture
cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()

    # If not tracking, start tracking the entire frame
    if not tracking:
        faces = face_cascade.detectMultiScale(frame, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))
        if len(faces) > 0:
            (x, y, w, h) = faces[0]
            tracked_object = (x, y, w, h)
            tracker.init(frame, tracked_object)
            tracking = True
    else:
        # If tracking, update the tracker and get the new position
        success, box = tracker.update(frame)
        if success:
            (x, y, w, h) = [int(a) for a in box]
            tracked_object = (x, y, w, h)

    # Draw rectangles around the tracked object
    if tracked_object is not None:
        (x, y, w, h) = tracked_object
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

        # Calculate the center of the detected face and frame center
        face_center = x + w // 2
        frame_center = frame.shape[1] // 2  # Assuming the frame is in landscape orientation

        # Calculate the distance from face center to frame center
        distance_from_center = face_center - frame_center

        # Log the direction change instead of moving the servo
        log_direction_change(distance_from_center, frame_center)

    cv2.imshow("CSRT Tracking", frame)

    # Check for the 'q' key press to exit
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break

# Release resources and close windows
cap.release()
cv2.destroyAllWindows()
