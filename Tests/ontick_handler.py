"""
 (C) Copyright 2013 Rob Watson rmawatson [at] hotmail.com  and others.

 All rights reserved. This program and the accompanying materials
 are made available under the terms of the GNU Lesser General Public License
 (LGPL) version 2.1 which accompanies this distribution, and is available at
 http://www.gnu.org/licenses/lgpl-2.1.html

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.

 Contributors:
     Rob Watson ( rmawatson [at] hotmail )
"""

from __future__ import (absolute_import, division, print_function, unicode_literals)
from PyMT4 import *
from common import *

def OnTickHandler(symbol, bid, ask, counter):
    print ('%09d:  Symbol: \'%s\'  Bid: %.5f  Ask: %.5f' % (counter, symbol, bid, ask))

if __name__ == "__main__":
    is_connected = Connect()
    if is_connected == True:
        print ('Connected')
        current_symbol = unicode2ascii('*')
        RegisterOnTickHandler(current_symbol, OnTickHandler)

        try:
            while (True):
                time.sleep(0.5)
        except (KeyboardInterrupt, SystemExit):
            pass

        Disconnect()
        print ('Disconnected')
    else:
        print ('Not connected')
