import cv2


cap = cv2.VideoCapture(0)  # open the camera

if cap.isOpened():
    # get vcap property
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))  # float
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))  # float
else:
    width = 1280
    height = 720

fourcc = cv2.VideoWriter_fourcc(*'H264')
out = cv2.VideoWriter('appsrc ! videoconvert ! x264enc tune=zerolatency noise-reduction=0 speed-preset=ultrafast ! rtph264pay config-interval=1 pt=96 ! udpsink host=127.0.0.1 port=5000', fourcc, cv2.CAP_PROP_FPS, (1280, 720), True) #ouput GStreamer pipeline

if not out.isOpened():
    print('VideoWriter not opened')
    exit(0)

while cap.isOpened():
    ret, frame = cap.read()

    if ret:

        # Write to pipeline
        frame = cv2.resize(frame, (1280, 720))
        out.write(frame)

        cv2.imshow("test", frame)

        if cv2.waitKey(1)&0xFF == ord('q'):
            break

cap.release()
out.release()
