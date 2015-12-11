#include <gst/gst.h>
#include <gst/app/gstappsink.h>

/*
 * первый standalone вариант для проверки работоспособности
 * был разделен на lib и main
 */

GstFlowReturn app_sink_new_buffer(GstAppSink *sink, gpointer user_data) {
        // prog_data *pd = (prog_data *) user_data;
        //GstBuffer *buffer1 = gst_app_sink_pull_buffer(sink);

        GstBuffer *buffer = gst_app_sink_pull_buffer(sink);
        if (buffer == NULL) {
                return GST_FLOW_ERROR;
        }

        g_print("buffer size: %i\n", buffer->size);
        gst_buffer_unref(buffer);
        return GST_FLOW_OK;
}

gint
main1(gint argc, gchar *argv[]) {
        GstElement *pipeline, *fakesrc, *flt, *conv, *videosink;

        /* init GStreamer */
        gst_init(&argc, &argv);
        g_print("%s\n", gst_version_string());

        /* create a new pipeline */
        GError *error = NULL;
        gchar *descr = NULL;
        if (argc > 1) {
                /* read pipeline desc from file */
                char *file_name = argv[1];
                FILE *fp = fopen(file_name, "r");

                if (fp == NULL) {
                        g_print("Error while opening the file: %s.\n", file_name);
                        exit(EXIT_FAILURE);
                }

                char buff[1024];
                fgets(buff, 1024, fp);
                fclose(fp);
                descr = g_strdup((gchar *) &buff);
                g_print("Pipeline from %s: %s\n", file_name, descr);
        } else {

                descr = g_strdup("videotestsrc ! "
                                        "video/x-raw-rgb,width=640,height=480,framerate=10/1 ! "
                                        "pngenc snapshot=false ! "
                                        "appsink name=sink drop=true");
                g_print("Pipeline: %s\n", descr);
        }
        pipeline = gst_parse_launch(descr, &error);
        g_free(descr);

        if (error != NULL) {
                g_print("could not construct pipeline: %s\n", error->message);
                g_error_free(error);
                exit(EXIT_FAILURE);
        }

        /* get sink */
        GstElement *appsink = gst_bin_get_by_name(GST_BIN (pipeline), "sink");

        /* Register the callback */
        GstAppSinkCallbacks *appsink_callbacks = (GstAppSinkCallbacks *) malloc(sizeof(GstAppSinkCallbacks));
        appsink_callbacks->eos = NULL;
        appsink_callbacks->new_preroll = NULL;
        appsink_callbacks->new_buffer = app_sink_new_buffer;
        appsink_callbacks->new_buffer_list = NULL;

        gst_app_sink_set_callbacks(GST_APP_SINK(appsink), appsink_callbacks, NULL, free);

        /* play */
        gst_element_set_state(pipeline, GST_STATE_PLAYING);
        GMainLoop *loop = g_main_loop_new(NULL, FALSE);
        g_main_loop_run(loop);
        g_main_loop_unref(loop);

        /* clean up */
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(GST_OBJECT (pipeline));
        return EXIT_SUCCESS;
}
