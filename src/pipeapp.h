//
// Created by svolkov on 10.12.15.
//

#ifndef GST_PIPEAPP_H
#define GST_PIPEAPP_H

#if defined(__cplusplus)
extern "C" {
#endif

int pipeapp_init(gchar *descr);

void pipeapp_start();

void pipeapp_stop();

typedef void ( * PipeappCallback)(int size, void* buffer);

void pipeapp_set_callback(PipeappCallback pipeappCallback);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif //GST_PIPEAPP_H
