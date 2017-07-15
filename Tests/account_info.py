from __future__ import (absolute_import, division, print_function,
                        unicode_literals)

from PyMT4 import *
from common import *
import time

is_connected = Connect()
if is_connected == True:
    print ('Connected')
    time_start = printStartDateTime()

    current_symbol = Symbol()
    print ('AccountBalance: ', AccountBalance())
    print ('AccountCredit: ', AccountCredit())
    print ('AccountCompany: ', AccountCompany())
    print ('AccountCurrency: ', AccountCurrency())
    print ('AccountEquity: ', AccountEquity())
    print ('AccountFreeMargin: ', AccountFreeMargin())
    print ('AccountFreeMarginCheck: ', AccountFreeMarginCheck(current_symbol, 0, 1.0))
    print ('AccountFreeMarginMode: ', AccountFreeMarginMode())
    print ('AccountLeverage: ', AccountLeverage())
    print ('AccountMargin: ', AccountMargin())
    print ('AccountName: ', AccountName())
    print ('AccountNumber: ', AccountNumber())
    print ('AccountProfit: ', AccountProfit())
    print ('AccountServer: ', AccountServer())
    print ('AccountStopoutLevel: ', AccountStopoutLevel())
    print ('AccountStopoutMode: ', AccountStopoutMode())

    printEndDateTime(time_start)
    Disconnect()
    print ('Disconnected')
else:
    print ('Not connected')