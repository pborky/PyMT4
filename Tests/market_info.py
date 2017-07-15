from __future__ import (absolute_import, division, print_function,
                        unicode_literals)

from PyMT4 import *
from common import *

is_connected = Connect()
if is_connected == True:
    print ('Connected')
    time_start = printStartDateTime()

    # Common parameters
    current_symbol = Symbol()
    timeframe = Period()
    shift = 0

    print ('Symbol: %s' % (current_symbol))
    print ('Period: %s' % (timeframe))

    Ask = MarketInfo(current_symbol, MODE_ASK)
    Bid = MarketInfo(current_symbol, MODE_BID)
    print ('Ask: %.5f' % (Ask))
    print ('Bid: %.5f' % (Bid))

    # OHLC data
    print ('iOpen[%d]: %.5f' % (shift, iOpen(current_symbol, timeframe, shift)))
    print ('iHigh[%d]: %.5f' % (shift, iHigh(current_symbol, timeframe, shift)))
    print ('iLow[%d]: %.5f' % (shift, iLow(current_symbol, timeframe, shift)))
    print ('iClose[%d]: %.5f' % (shift, iClose(current_symbol, timeframe, shift)))
    print ('iVolume[%d]: %d' % (shift, iVolume(current_symbol, timeframe, shift)))

    printEndDateTime(time_start)
    Disconnect()
    print ('Disconnected')
else:
    print ('Not connected')