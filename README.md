# gst-pipeapp

Application construct gstreamer pipe with appsink and run callback when each frame converted. 

Divided on the app and lib.
Allow connect script lang, tested on the python in the gst-pipeapp.py.

Pipeline may by readed from file, othewith default test pipeline

    gst_jpg_frm [pipeline_txt_file_name]

Sample python callback in the gst-pipeapp.py.

## osx appsink test

gst-launch-1.0 -v videotestsrc ! \
    video/x-raw,width=640,height=480,framerate=10/1 ! \
    autovideosink

gst-launch-1.0 -v videotestsrc ! \
    video/x-raw,width=640,height=480,framerate=10/1 ! \
    jpegenc ! \
    appsink

gst-launch -v -e videotestsrc ! \
    video/x-raw-rgb,framerate=10/1,width=1024,height=768 ! \
    pngenc snapshot=false ! 
    appsink name=sink drop=true

## v2r appsink

gst-launch -v -e v4l2src always-copy=false num-buffers=1 chain-ipipe=true ! \
    capsfilter caps=video/x-raw-yuv,format=\(fourcc\)NV12,width=640,height=480 ! \
    dmaiaccel ! \
    dmaienc_jpeg ! \
    appsink name=sink drop=true

