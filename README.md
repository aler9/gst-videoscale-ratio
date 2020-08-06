
# gst-videoscale-ratio

This is a GStreamer filter that allows to resize a video and keep its aspect ratio.

This pipeline will produce a `1920x1080` video:
```sh
gst-launch-1.0 \
videotestsrc ! video/x-raw,width=1280,height=720 \
! videoscaleratio height=1080 \
! xvimagesink
```

While a pipeline with the native `videoscale` needs to know in advantage the size of the output video, that is fixed and doesn't depend from the input video, resulting in a wrong aspect ratio:
```sh
gst-launch-1.0 \
videotestsrc ! video/x-raw,width=1280,height=720 \
! videoscale \
! "video/x-raw,width=1080,height=1080" \
! xvimagesink
```

## Installation

Install build dependencies:
```
apt install -y --no-install-recommends \
gcc \
meson \
libgstreamer-plugins-base1.0-dev
```

Compile and install:
```
meson --prefix=/usr build \
&& cd build \
&& ninja \
&& sudo ninja install
```
