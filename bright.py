import streamlit as st
import cv2
import numpy as np

@st.cache_resource
def get_cap():
    return cv2.VideoCapture(0)

def adjust_hue(frame, hue_shift):

    if frame is None or frame.size == 0:
        return frame  # Return original if invalid

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    hsv[..., 0] = (hsv[..., 0].astype(int) + hue_shift) % 180
    return cv2.cvtColor(hsv, cv2.COLOR_HSV2BGR)


st.title("Camera with Hue Control")
hue_factor = st.slider("Hue", min_value=-180, max_value=180, step=10, value=0)

FRAME_WINDOW = st.image([])

cap = get_cap()
while (cap.isOpened()):
    ret, frame = cap.read()
    if not ret:
        continue
                                                                                                                                         
    frame = adjust_hue(frame, hue_factor)
    FRAME_WINDOW.image(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
