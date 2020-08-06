#!/bin/sh -e

#! videoscale add-borders=1 ! video/x-raw,width=1280,height=720
#x264enc speed-preset=veryfast tune=zerolatency bitrate=500 \
#! rtspclientsink location=rtsp://localhost:8554/stream
#videotestsrc ! video/x-raw,width=320,height=320
#! rtspclientsink location=rtsp://localhost:8554/stream
#! videoscaleratio height=720 \

#videotestsrc ! video/x-raw,width=1280,height=720
#! capsfilter caps=video/x-raw,width=400,height=400
#
#rtph264depay \
#! h264parse \

gst-launch-1.0 \
rtspsrc location=rtsp://localhost:8554/stream \
! "application/x-rtp,media=(string)video" \
! decodebin \
! videoscaleratio height=320 \
! x264enc speed-preset=veryfast tune=zerolatency \
! decodebin \
! xvimagesink
