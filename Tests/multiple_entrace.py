from __future__ import (absolute_import, division, print_function, unicode_literals)

REPEATITIVE_LOOP = 200

from PyMT4 import *
from common import *
import time
from unidecode import unidecode

time_start = printStartDateTime()

for i in range(REPEATITIVE_LOOP):
    is_connected = Connect()
    if is_connected == True:
        print ('Connected')

        unicode_message = 'Pulse Check %s' % str(i + 1)
        print ('Iteration: %d, Printing \"%s\"' % (i, unicode_message))
        ping_message = unidecode(unicode_message)
        print_result = Print(ping_message)
        assert print_result, 'Iteration: %d, Unable to perform Printing' % i
        time.sleep(0.05)
        Disconnect()
        print ('Disconnected')
    else:
        print ('Not connected')

time_delta = printEndDateTime(time_start)
if time_delta > 0:
    print ('Performance Index = %.1f commands per second' % (REPEATITIVE_LOOP/time_delta))
