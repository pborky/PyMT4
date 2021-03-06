from __future__ import (absolute_import, division, print_function, unicode_literals)

REPEATITIVE_LOOP = 10000
                        
from PyMT4 import *
from common import *

is_connected = Connect()
if is_connected == True:
    print ('Connected')
    time_start = printStartDateTime()

    for i in range(REPEATITIVE_LOOP):
        if i % 1000 == 0:
            print ('Printed i = %s' % str(i))
        print_result = Print (str(i))
        assert print_result, 'Unable to perform Printing'

    time_delta = printEndDateTime(time_start)
    if time_delta > 0:
        print ('Performance Index = %.1f commands per second' % (REPEATITIVE_LOOP/time_delta))
    Disconnect()
    print ('Disconnected')
else:
    print ('Not connected')