#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "pipeapp.h"

#define	PIPE_BUFFER_SIZE	1024

void callback(int size, void* buffer) {
        g_print("buffer size: %i\n", size);
}

gint
main(gint argc, gchar *argv[]) {
        /* create a new pipeline */
        gchar *descr = NULL;
        char buff[PIPE_BUFFER_SIZE];
        if (argc > 1) {
                /* read pipeline desc from file */
                char *file_name = argv[1];
                FILE *fp = fopen(file_name, "r");

                if (fp == NULL) {
                        g_print("Error while opening the file: %s.\n", file_name);
                        exit(EXIT_FAILURE);
                }

                fgets(buff, PIPE_BUFFER_SIZE, fp);
                fclose(fp);
                descr = (gchar *) &buff;
                g_print("Pipeline from %s\n", file_name);
        }
        int initialized = pipeapp_init(descr);
        if (!initialized) {
                exit(EXIT_FAILURE);
        }

        pipeapp_set_callback(&callback);

        /* play */
        pipeapp_start();
        GMainLoop *loop = g_main_loop_new(NULL, FALSE);
        g_main_loop_run(loop);
        g_main_loop_unref(loop);

        pipeapp_stop();
        return EXIT_SUCCESS;
}