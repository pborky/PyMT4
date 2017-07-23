/* (C) Copyright 2013 Rob Watson rmawatson [at] hotmail.com  and others.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * Contributors:
 *     Rob Watson ( rmawatson [at] hotmail )
 *
 */

#property strict

//+------------------------------------------------------------------+
//| script program start function                                    |
//+------------------------------------------------------------------+
// Size of temporary buffer used to communicate with DLL
#define DLL_READ_BUFFER_SIZE  1024

#import "pymt4.dll"
    //--- MT4 B600 Unicode fixes
    //--- https://www.mql5.com/en/forum/149321
    //--- https://www.mql5.com/en/forum/149412
    bool pymt4_initialize(uchar &windowName[DLL_READ_BUFFER_SIZE], int windowHandle);
    bool pymt4_uninitialize(uchar &windowName[DLL_READ_BUFFER_SIZE], int windowHandle);
    bool pymt4_isinitialized();
    bool pymt4_notify(uchar &windowName[DLL_READ_BUFFER_SIZE], int windowHandle);
    bool pymt4_notifyOnTick(uchar &symbol[DLL_READ_BUFFER_SIZE], double bid, double ask);
    int  pymt4_requestPendingCommand();
#import

int init()
{
    //--- Convert the strings to uchar[] arrays
    uchar ucResult[DLL_READ_BUFFER_SIZE];
    ArrayInitialize(ucResult, 0);
    int symbol_size = StringToCharArray(Symbol(), ucResult);
    if (!pymt4_initialize(ucResult, WindowHandle(Symbol(), Period())))
    {
        Print ("PyMT4 Already Initialized ...");
        return (7);
    }

    Print ("PyMT4 Host Initialized ...");
    return(0);
}
    
int deinit()
{
    //--- Convert the strings to uchar[] arrays
    uchar ucResult[DLL_READ_BUFFER_SIZE];
    ArrayInitialize(ucResult, 0);
    int symbol_size = StringToCharArray(Symbol(), ucResult);
    pymt4_uninitialize(ucResult, WindowHandle(Symbol(), Period()));
    Print ("PyMT4 Host Uninitialized ...");
    return(0);
}
    
int start()
{
    if (pymt4_isinitialized())
    {
        //--- Convert the strings to uchar[] arrays
        uchar ucResult[DLL_READ_BUFFER_SIZE];
        ArrayInitialize(ucResult, 0);
        int symbol_size = StringToCharArray(Symbol(), ucResult);
        pymt4_notifyOnTick(ucResult, Bid, Ask);
    }
    return(0);
}