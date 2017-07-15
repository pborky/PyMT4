from __future__ import (absolute_import, division, print_function,
                        unicode_literals)

from PyMT4 import *
from common import *
import time

is_connected = Connect()
if is_connected == True:
    print ('Connected')
    time_start = printStartDateTime()

    # Common parameters
    current_symbol = unicode2ascii('EURUSD')
    timeframe = PERIOD_CURRENT
    applied_price = PRICE_CLOSE
    ma_method = MODE_SMA
    shift = 0

    # RSI data
    rsi_period = 14
    print ('iRSI[%d]: %.1f' % (shift, iRSI(current_symbol, timeframe, rsi_period, applied_price, shift)))

    # STO data
    sto_k_period = 5
    sto_d_period = 3
    sto_slowing = 3
    sto_price_field = 0

    mode = MODE_MAIN
    print ('iStochastic[%d], Mode[%d]: %.1f' % (shift, mode, iStochastic(current_symbol, timeframe, sto_k_period, sto_d_period, sto_slowing, ma_method, sto_price_field, mode, shift)))

    mode = MODE_SIGNAL
    print ('iStochastic[%d], Mode[%d]: %.1f' % (shift, mode, iStochastic(current_symbol, timeframe, sto_k_period, sto_d_period, sto_slowing, ma_method, sto_price_field, mode, shift)))

    # Bands data
    bands_period = 20
    bands_deviation = 2.0
    bands_shift = 0
    mode = MODE_MAIN
    print ('iBands[%d], Mode[%d]: %.5f' % (shift, mode, iBands(current_symbol, timeframe, bands_period, bands_deviation, bands_shift, applied_price, mode, shift)))

    mode = MODE_UPPER
    print ('iBands[%d], Mode[%d]: %.5f' % (shift, mode, iBands(current_symbol, timeframe, bands_period, bands_deviation, bands_shift, applied_price, mode, shift)))

    mode = MODE_LOWER
    print ('iBands[%d], Mode[%d]: %.5f' % (shift, mode, iBands(current_symbol, timeframe, bands_period, bands_deviation, bands_shift, applied_price, mode, shift)))

    # Alligator data
    jaw_period = 13
    jaw_shift = 8
    teeth_period = 8
    teeth_shift = 5
    lips_period = 5
    lips_shift = 3

    alligator_ma_method = MODE_SMMA
    alligator_applied_price = PRICE_MEDIAN
    mode = MODE_GATORJAW
    print ('iAlligator[%d], Mode[%d]: %.5f' % (shift, mode, iAlligator(current_symbol, timeframe, jaw_period, jaw_shift, teeth_period,
                                                teeth_shift, lips_period, lips_shift, alligator_ma_method, alligator_applied_price, mode, shift)))

    mode = MODE_GATORTEETH
    print ('iAlligator[%d], Mode[%d]: %.5f' % (shift, mode, iAlligator(current_symbol, timeframe, jaw_period, jaw_shift, teeth_period,
                                                teeth_shift, lips_period, lips_shift, alligator_ma_method, alligator_applied_price, mode, shift)))

    mode = MODE_GATORLIPS
    print ('iAlligator[%d], Mode[%d]: %.5f' % (shift, mode, iAlligator(current_symbol, timeframe, jaw_period, jaw_shift, teeth_period,
                                                teeth_shift, lips_period, lips_shift, alligator_ma_method, alligator_applied_price, mode, shift)))

    printEndDateTime(time_start)
    Disconnect()
    print ('Disconnected')
else:
    print ('Not connected')