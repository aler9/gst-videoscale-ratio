#!/bin/sh -e

gst-launch-1.0 \
videotestsrc ! video/x-raw,width=1280,height=720 \
! videoscaleratio height=320 \
! x264enc speed-preset=veryfast tune=zerolatency \
! decodebin \
! xvimagesink
