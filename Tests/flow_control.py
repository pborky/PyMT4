from __future__ import (absolute_import, division, print_function,
                        unicode_literals)

from PyMT4 import *
from common import *

is_connected = Connect()
if is_connected == True:
    print ('Connected')
    time_start = printStartDateTime()

    # Force the script to exit from loop
    Set_Flow_Return_Value(0)

    printEndDateTime(time_start)
    Disconnect()
    print ('Disconnected')
else:
    print ('Not connected')