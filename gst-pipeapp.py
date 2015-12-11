# coding: utf-8
from __future__ import unicode_literals, print_function, division, absolute_import

__author__ = 'svolkov'

#
# 10.12.2015
#

import os
import time
from ctypes import *

def callback(size, buffer):
    print("Size", size)
    return 0

print(os.getcwd())
gstpipeapp = CDLL("lib/libgstpipeapp.dylib")

# init
gstpipeapp.pipeapp_init(None)

# callback
CALLBACKFUNC = CFUNCTYPE(c_int, c_int, c_void_p)
my_callback = CALLBACKFUNC(callback)
gstpipeapp.pipeapp_set_callback(my_callback)

# start
gstpipeapp.pipeapp_start()
print("started")
time.sleep(3)
print("stop")
gstpipeapp.pipeapp_stop()

gstpipeapp = None