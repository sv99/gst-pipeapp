//
// Created by svolkov on 10.12.15.
//

#include <stdlib.h>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>


#include "pipeapp.h"

#if defined(__arm__) || defined(__TARGET_ARCH_ARM)
/* v2r davinchi arm platform */
#define DEFAULT_PIPE_DESCR "v4l2src always-copy=false chain-ipipe=true ! " \
                "capsfilter caps=video/x-raw-yuv,format=\\(fourcc\\)NV12,width=640,height=480,framerate=5/1 ! " \
                "dmaiaccel ! " \
                "dmaienc_jpeg ! " \
                "appsink name=sink drop=true"
#else
/* osx test platform */
#define DEFAULT_PIPE_DESCR "videotestsrc ! " \
                 "video/x-raw-rgb,width=640,height=480,framerate=5/1 ! " \
                 "jpegenc ! " \
                 "appsink name=sink drop=true"

#endif

/* Global vars */
GstElement *pipeline = NULL;
// GstElement *appsink = NULL;
int initialized = 0;
PipeappCallback pipeapp_callback = NULL;

int pipeapp_init(gchar *descr) {
        /* init GStreamer */
        g_print("pipeapp_init\n");
        gst_init(NULL, NULL);
        g_print("%s\n", gst_version_string());

        if (descr == NULL) {
                descr = DEFAULT_PIPE_DESCR;
        }
        g_print("Pipeline: %s\n", descr);

        /* create a new pipeline */
        GError *error = NULL;
        pipeline = gst_parse_launch(descr, &error);

        initialized = error == NULL;
        if (!initialized) {
                g_print("could not construct pipeline: %s\n", error->message);
                g_error_free(error);
        }
        return initialized;
}

void pipeapp_start() {
        g_print("pipeapp_start\n");
        if (initialized) {
                /* play */
                gst_element_set_state(pipeline, GST_STATE_PLAYING);
        }
}

void pipeapp_stop() {
        g_print("pipeapp_stop\n");
        if (initialized) {
                /* stop */
                gst_element_set_state(pipeline, GST_STATE_PAUSED);
        }
}

GstFlowReturn app_sink_new_buffer(GstAppSink *sink, gpointer user_data) {
        //g_print("app_sink_new_buffer\n");
        GstBuffer *buffer = gst_app_sink_pull_buffer(sink);
        if (buffer == NULL) {
                return GST_FLOW_ERROR;
        }

        if (pipeapp_callback != NULL) {
                //g_print("app_sink_new_buffer: fire event\n");
//                g_print("buffer: 0x");
//                for (int i = 0; i < 16; i++) {
//                        printf("%02X", buffer->data[i]);
//                }
//                g_print("\n");
                pipeapp_callback(buffer->size, buffer->data, user_data);
                //g_print("app_sink_new_buffer: end event\n");
        }
        gst_buffer_unref(buffer);
        return GST_FLOW_OK;
}

void pipeapp_set_callback(PipeappCallback pipeappCallback, gpointer user_data) {
        g_print("pipeapp_set_callback\n");
        if (initialized) {
                //g_print("pipeapp_set_callback: %i 0x%x <- 0x%x\n", (int) pipeapp_callback, (int) &pipeapp_callback, (int)pipeappCallback);
                pipeapp_callback = pipeappCallback;

                /* get sink */
                GstElement *appsink = gst_bin_get_by_name(GST_BIN (pipeline), "sink");

                /* Register the callback */
                GstAppSinkCallbacks *appsink_callbacks = (GstAppSinkCallbacks *) malloc(sizeof(GstAppSinkCallbacks));
                appsink_callbacks->eos = NULL;
                appsink_callbacks->new_preroll = NULL;
                appsink_callbacks->new_buffer = app_sink_new_buffer;
                appsink_callbacks->new_buffer_list = NULL;

                gst_app_sink_set_callbacks(GST_APP_SINK(appsink), appsink_callbacks, user_data, free);
        }

}

void __attribute__ ((destructor)) fini(void) {
        g_print("lib fini\n");
        if (pipeline != NULL) {
                gst_element_set_state(pipeline, GST_STATE_NULL);
                gst_object_unref(GST_OBJECT (pipeline));
        }
}
