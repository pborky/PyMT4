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

enum CmdIdType
{
    CmdId_CheckShutdownCondition = 100,

    CmdId_AccountBalance = 1000,
    CmdId_AccountCredit,
    CmdId_AccountCompany,
    CmdId_AccountCurrency,
    CmdId_AccountEquity,
    CmdId_AccountFreeMargin,
    CmdId_AccountFreeMarginCheck,
    CmdId_AccountFreeMarginMode,
    CmdId_AccountLeverage,
    CmdId_AccountMargin,
    CmdId_AccountName,
    CmdId_AccountNumber,
    CmdId_AccountProfit,
    CmdId_AccountServer,
    CmdId_AccountStopoutLevel,
    CmdId_AccountStopoutMode,

    CmdId_iBars = 1100,
    CmdId_iBarShift,
    CmdId_iClose,
    CmdId_iHigh,
    CmdId_iHighest,
    CmdId_iLow,
    CmdId_iLowest,
    CmdId_iOpen,
    CmdId_iTime,
    CmdId_iVolume,

    CmdId_OrderClose = 1200,
    CmdId_OrderCloseBy,
    CmdId_OrderClosePrice,
    CmdId_OrderCloseTime,
    CmdId_OrderComment,
    CmdId_OrderCommission,
    CmdId_OrderDelete,
    CmdId_OrderExpiration,
    CmdId_OrderLots,
    CmdId_OrderMagicNumber,
    CmdId_OrderModify,
    CmdId_OrderOpenPrice,
    CmdId_OrderOpenTime,
    CmdId_OrderPrint,
    CmdId_OrderProfit,
    CmdId_OrderSelect,
    CmdId_OrderSend,
    CmdId_OrdersHistoryTotal,
    CmdId_OrderStopLoss,
    CmdId_OrdersTotal,
    CmdId_OrderSwap,
    CmdId_OrderSymbol,
    CmdId_OrderTakeProfit,
    CmdId_OrderTicket,
    CmdId_OrderType,

    CmdId_IsConnected = 1300,
    CmdId_IsDemo,
    CmdId_IsDllsAllowed,
    CmdId_IsExpertEnabled,
    CmdId_IsLibrariesAllowed,
    CmdId_IsOptimization,
    CmdId_IsStopped,
    CmdId_IsTesting,
    CmdId_IsTradeAllowed,
    CmdId_IsTradeContextBusy,
    CmdId_IsVisualMode,
    CmdId_UninitializeReason,

    CmdId_TerminalCompany = 1400,
    CmdId_TerminalName,
    CmdId_TerminalPath,

    CmdId_Alert = 1500,
    CmdId_Comment,
    CmdId_GetTickCount,
    CmdId_MarketInfo,
    CmdId_PlaySound,
    CmdId_Print,
    CmdId_SendFTP,
    CmdId_SendMail,
    CmdId_SendNotification,
    CmdId_Symbol,
    CmdId_Period,
	CmdId_RefreshRates,

    CmdId_TimeCurrent = 1600,
    CmdId_TimeLocal,
    CmdId_TimeGMT,
    CmdId_TimeGMTOffset,

    CmdId_iAC = 1700,
	CmdId_iAD,
	CmdId_iADX,
	CmdId_iAlligator,
	CmdId_iAO,
	CmdId_iATR,
	CmdId_iBearsPower,
	CmdId_iBands,
	CmdId_iCCI,
	CmdId_iDeMarker,
	CmdId_iEnvelopes,
	CmdId_iForce,
	CmdId_iFractals,
	CmdId_iGator,
	CmdId_iIchimoku,
	CmdId_iBWMFI,
	CmdId_iMomentum,
	CmdId_iMFI,
	CmdId_iMA,
	CmdId_iOsMA,
	CmdId_iMACD,
	CmdId_iOBV,
	CmdId_iRSI,
	CmdId_iRVI,
	CmdId_iSAR,
	CmdId_iStdDev,
	CmdId_iStochastic,
	CmdId_iWPR,

	CmdId_Set_Flow_Return_Value = 3000,
};


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

    int    pymt4_getStringArgument(uchar &[DLL_READ_BUFFER_SIZE]);
    double pymt4_getDoubleArgument();
    int    pymt4_getIntArgument();
    bool   pymt4_getBoolArgument();

    bool pymt4_setDoubleResult(double result, int error);
    bool pymt4_setStringResult(uchar &result[DLL_READ_BUFFER_SIZE], int error);
    bool pymt4_setIntResult(int result, int error);
    bool pymt4_setBoolResult(int result, int error);
#import


void checkSelectedTicket(int ticketId)
{
    //---
    static int lastTicketId = 0;

    //---
    if (ticketId != lastTicketId)
    {
        bool selected = OrderSelect(ticketId, SELECT_BY_TICKET);
    }
    lastTicketId = ticketId;
}


bool evaluateCommand(int pendingCommandId)
{
    //---
    bool return_value = true;
    uchar ucBuffer1[DLL_READ_BUFFER_SIZE];
    uchar ucBuffer2[DLL_READ_BUFFER_SIZE];
    uchar ucResult[DLL_READ_BUFFER_SIZE];
    ArrayInitialize(ucBuffer1, NULL);
    ArrayInitialize(ucBuffer2, NULL);
    ArrayInitialize(ucResult, NULL);

    //---
    switch (pendingCommandId)
    {
        /* Account Functions */
        case CmdId_AccountBalance:
        {
            pymt4_setDoubleResult(AccountBalance(), GetLastError());
            break;
        }

        case CmdId_AccountCredit:
        {
            pymt4_setDoubleResult(AccountCredit(), GetLastError());
            break;
        }

        case CmdId_AccountCompany:
        {
            //--- Convert the strings to uchar[] arrays
            string account_company = AccountCompany();
            StringToCharArray(account_company, ucResult);
            pymt4_setStringResult(ucResult, GetLastError());
            break;
        }

        case CmdId_AccountCurrency:
        {
            //--- Convert the strings to uchar[] arrays
            StringToCharArray(AccountCurrency(), ucResult);
            pymt4_setStringResult(ucResult, GetLastError());
            break;
        }

        case CmdId_AccountEquity:
        {
            pymt4_setDoubleResult(AccountEquity(), GetLastError());
            break;
        }

        case CmdId_AccountFreeMargin:
        {
            pymt4_setDoubleResult(AccountFreeMargin(), GetLastError());
            break;
        }

        case CmdId_AccountFreeMarginCheck:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int cmd = pymt4_getIntArgument();
            double volume = pymt4_getDoubleArgument();
            pymt4_setDoubleResult(AccountFreeMarginCheck(symbol_string, cmd, volume), GetLastError());
            break;
        }

        case CmdId_AccountFreeMarginMode:
        {
            pymt4_setDoubleResult(AccountFreeMarginMode(), GetLastError());
            break;
        }

        case CmdId_AccountLeverage:
        {
            pymt4_setIntResult(AccountLeverage(), GetLastError());
            break;
        }

        case CmdId_AccountMargin:
        {
            pymt4_setDoubleResult(AccountMargin(), GetLastError());
            break;
        }

        case CmdId_AccountName:
        {
            //--- Convert the strings to uchar[] arrays
            StringToCharArray(AccountName(), ucResult);
            pymt4_setStringResult(ucResult, GetLastError());
            break;
        }

        case CmdId_AccountNumber:
        {
            pymt4_setIntResult(AccountNumber(), GetLastError());
            break;
        }

        case CmdId_AccountProfit:
        {
            pymt4_setDoubleResult(AccountProfit(), GetLastError());
            break;
        }

        case CmdId_AccountServer:
        {
            //--- Convert the strings to uchar[] arrays
            StringToCharArray(AccountServer(), ucResult);
            pymt4_setStringResult(ucResult, GetLastError());
            break;
        }

        case CmdId_AccountStopoutLevel:
        {
            pymt4_setIntResult(AccountStopoutLevel(), GetLastError());
            break;
        }

        case CmdId_AccountStopoutMode:
        {
            pymt4_setIntResult(AccountStopoutMode(), GetLastError());
            break;
        }

        /* Trading Functions */
        case CmdId_iBars:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            pymt4_setIntResult(iBars(symbol_string, pymt4_getIntArgument()), GetLastError());
            break;
        }

        case CmdId_iBarShift:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            pymt4_setIntResult(iBarShift(symbol_string, pymt4_getIntArgument(), pymt4_getBoolArgument()), GetLastError());
            break;
        }

        case CmdId_iClose:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            pymt4_setDoubleResult(iClose(symbol_string, pymt4_getIntArgument(), pymt4_getIntArgument()), GetLastError());
            break;
        }

        case CmdId_iHigh:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            pymt4_setDoubleResult(iHigh(symbol_string, pymt4_getIntArgument(), pymt4_getIntArgument()), GetLastError());
            break;
        }

        case CmdId_iHighest:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            pymt4_setDoubleResult(iHighest(symbol_string, pymt4_getIntArgument(), pymt4_getIntArgument(), pymt4_getIntArgument(), pymt4_getIntArgument()), GetLastError());
            break;
        }

        case CmdId_iLow:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            pymt4_setDoubleResult(iLow(symbol_string, pymt4_getIntArgument(), pymt4_getIntArgument()), GetLastError());
            break;
        }

        case CmdId_iLowest:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            pymt4_setDoubleResult(iLowest(symbol_string, pymt4_getIntArgument(), pymt4_getIntArgument()), GetLastError());
            break;
        }

        case CmdId_iOpen:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            pymt4_setDoubleResult(iOpen(symbol_string, pymt4_getIntArgument(), pymt4_getIntArgument()), GetLastError());
            break;
        }

        case CmdId_iTime:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            pymt4_setDoubleResult(iTime(symbol_string, pymt4_getIntArgument(), pymt4_getIntArgument()), GetLastError());
            break;
        }

        case CmdId_iVolume:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            pymt4_setDoubleResult(iVolume(symbol_string, pymt4_getIntArgument(), pymt4_getIntArgument()), GetLastError());
            break;
        }

        /* Order Functions */
        case CmdId_OrderClose:
        {
            pymt4_setBoolResult(OrderClose(pymt4_getIntArgument(),
                                             pymt4_getDoubleArgument(),
                                             pymt4_getDoubleArgument(),
                                             pymt4_getIntArgument(),
                                             pymt4_getIntArgument()), GetLastError());
            break;
        }

        case CmdId_OrderCloseBy:
        {
            pymt4_setBoolResult(OrderCloseBy(pymt4_getIntArgument(), pymt4_getIntArgument(), pymt4_getIntArgument()), GetLastError());
            break;
        }

        case CmdId_OrderClosePrice:
        {
            checkSelectedTicket(pymt4_getIntArgument());
            pymt4_setDoubleResult(OrderClosePrice(), GetLastError());
            break;
        }

        case CmdId_OrderCloseTime:
        {
            checkSelectedTicket(pymt4_getIntArgument());
            pymt4_setDoubleResult(OrderCloseTime(), GetLastError());
            break;
        }

        case CmdId_OrderComment:
        {
            checkSelectedTicket(pymt4_getIntArgument());
            //--- Convert the strings to uchar[] arrays
            StringToCharArray(OrderComment(), ucResult);
            pymt4_setStringResult(ucResult, GetLastError());
            break;
        }

        case CmdId_OrderCommission:
        {
            checkSelectedTicket(pymt4_getIntArgument());
            pymt4_setDoubleResult(OrderCommission(), GetLastError());
            break;
        }

        case CmdId_OrderDelete:
        {
            pymt4_setBoolResult(OrderDelete(pymt4_getIntArgument(), pymt4_getIntArgument()), GetLastError());
            break;
        }

        case CmdId_OrderExpiration:
        {
            checkSelectedTicket(pymt4_getIntArgument());
            pymt4_setDoubleResult(OrderExpiration(), GetLastError());
            break;
        }

        case CmdId_OrderLots:
        {
            checkSelectedTicket(pymt4_getIntArgument());
            pymt4_setDoubleResult(OrderLots(), GetLastError());
            break;
        }

        case CmdId_OrderMagicNumber:
        {
            checkSelectedTicket(pymt4_getIntArgument());
            pymt4_setIntResult(OrderMagicNumber(), GetLastError());
            break;
        }

        case CmdId_OrderModify:
        {
            pymt4_setBoolResult(
                                 OrderModify(
                                    pymt4_getIntArgument(),
                                    pymt4_getDoubleArgument(),
                                    pymt4_getDoubleArgument(),
                                    pymt4_getDoubleArgument(),
                                    pymt4_getIntArgument(),
                                    pymt4_getIntArgument()
                                 ), GetLastError()
                              );
            break;
        }

        case CmdId_OrderOpenPrice:
        {
            checkSelectedTicket(pymt4_getIntArgument());
            pymt4_setDoubleResult(OrderOpenPrice(), GetLastError());
            break;
        }

        case CmdId_OrderOpenTime:
        {
            checkSelectedTicket(pymt4_getIntArgument());
            pymt4_setDoubleResult(OrderOpenTime(), GetLastError());
            break;
        }

        case CmdId_OrderPrint:
        {
            checkSelectedTicket(pymt4_getIntArgument());
            OrderPrint();
            pymt4_setBoolResult(True, GetLastError());
            break;
        }

        case CmdId_OrderProfit:
        {
            checkSelectedTicket(pymt4_getIntArgument());
            pymt4_setDoubleResult(OrderProfit(), GetLastError());
            break;
        }

        case CmdId_OrderSelect:
        {
            bool result = OrderSelect(pymt4_getIntArgument(), pymt4_getIntArgument(), pymt4_getIntArgument());
            if (result)
            {
                pymt4_setIntResult(OrderTicket(), GetLastError());
            }
            else
            {
                pymt4_setIntResult(-1, GetLastError());
            }
            break;
        }

        case CmdId_OrderSend:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string   symbol = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int      cmd = pymt4_getIntArgument();
            double   volume = pymt4_getDoubleArgument();
            double   price = pymt4_getDoubleArgument();
            int      slippage = pymt4_getIntArgument();
            double   stoploss = pymt4_getDoubleArgument();
            double   takeprofit = pymt4_getDoubleArgument();

            int ucBuffer2_size = pymt4_getStringArgument(ucBuffer2);
            string   comment = CharArrayToString(ucBuffer2, 0, ucBuffer2_size);
            int      magic = pymt4_getIntArgument();
            datetime expiration = pymt4_getIntArgument();
            int      arrow_color = pymt4_getIntArgument();

            pymt4_setIntResult(
                             OrderSend(
                                symbol,
                                cmd,
                                volume,
                                price,
                                slippage,
                                stoploss,
                                takeprofit,
                                comment,
                                magic,
                                expiration,
                                arrow_color
                             ), GetLastError()
                          );
            break;
        }

        case CmdId_OrdersHistoryTotal:
        {
            pymt4_setIntResult(OrdersHistoryTotal(), GetLastError());
            break;
        }

        case CmdId_OrderStopLoss:
        {
            checkSelectedTicket(pymt4_getIntArgument());
            pymt4_setDoubleResult(OrderStopLoss(), GetLastError());
            break;
        }

        case CmdId_OrdersTotal:
        {
            pymt4_setIntResult(OrdersTotal(), GetLastError());
            break;
        }

        case CmdId_OrderSwap:
        {
            checkSelectedTicket(pymt4_getIntArgument());
            pymt4_setDoubleResult(OrderStopLoss(), GetLastError());
            break;
        }

        case CmdId_OrderSymbol:
        {
            checkSelectedTicket(pymt4_getIntArgument());
            //--- Convert the strings to uchar[] arrays
            StringToCharArray(OrderSymbol(), ucResult);
            pymt4_setStringResult(ucResult, GetLastError());
            break;
        }

        case CmdId_OrderTakeProfit:
        {
            checkSelectedTicket(pymt4_getIntArgument());
            pymt4_setDoubleResult(OrderTakeProfit(), GetLastError());
            break;
        }

        case CmdId_OrderTicket:
        {
            checkSelectedTicket(pymt4_getIntArgument());
            pymt4_setIntResult(OrderTicket(), GetLastError());
            break;
        }

        case CmdId_OrderType:
        {
            checkSelectedTicket(pymt4_getIntArgument());
            pymt4_setIntResult(OrderType(), GetLastError());
            break;
        }


        /* Checkup */
        case CmdId_IsConnected:
        {
            bool res = IsConnected();
            Print(res);
            pymt4_setBoolResult(IsConnected(), GetLastError());
            break;
        }

        case CmdId_IsDemo:
        {
            pymt4_setBoolResult(IsDemo(), GetLastError());
            break;
        }

        case CmdId_IsDllsAllowed:
        {
            pymt4_setBoolResult(IsDllsAllowed(), GetLastError());
            break;
        }

        case CmdId_IsExpertEnabled:
        {
            pymt4_setBoolResult(IsExpertEnabled(), GetLastError());
            break;
        }

        case CmdId_IsLibrariesAllowed:
        {
            pymt4_setBoolResult(IsLibrariesAllowed(), GetLastError());
            break;
        }

        case CmdId_IsOptimization:
        {
            pymt4_setBoolResult(IsOptimization(), GetLastError());
            break;
        }

        case CmdId_IsStopped:
        {
            pymt4_setBoolResult(IsStopped(), GetLastError());
            break;
        }

        case CmdId_IsTesting:
        {
            pymt4_setBoolResult(IsTesting(), GetLastError());
            break;
        }

        case CmdId_IsTradeAllowed:
        {
            pymt4_setBoolResult(IsTradeAllowed(), GetLastError());
            break;
        }

        case CmdId_IsTradeContextBusy:
        {
            pymt4_setBoolResult(IsTradeContextBusy(), GetLastError());
            break;
        }

        case CmdId_IsVisualMode:
        {
            pymt4_setBoolResult(IsVisualMode(), GetLastError());
            break;
        }

        case CmdId_UninitializeReason:
        {
            pymt4_setIntResult(UninitializeReason(), GetLastError());
            break;
        }

        /* Client Terminal */
        case CmdId_TerminalCompany:
        {
            //--- Convert the strings to uchar[] arrays
            StringToCharArray(TerminalCompany(), ucResult);
            pymt4_setStringResult(ucResult, GetLastError());
            break;
        }

        case CmdId_TerminalName:
        {
            //--- Convert the strings to uchar[] arrays
            StringToCharArray(TerminalName(), ucResult);
            pymt4_setStringResult(ucResult, GetLastError());
            break;
        }

        case CmdId_TerminalPath:
        {
            //--- Convert the strings to uchar[] arrays
            StringToCharArray(TerminalPath(), ucResult);
            pymt4_setStringResult(ucResult, GetLastError());
            break;
        }

        /* Common Functions */
        case CmdId_Alert:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            Alert(symbol_string);
            pymt4_setBoolResult(True, GetLastError());
            break;
        }

        case CmdId_Comment:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            Comment(symbol_string);
            pymt4_setBoolResult(True, GetLastError());
            break;
        }

        case CmdId_GetTickCount:
        {
            pymt4_setIntResult(GetTickCount(), GetLastError());
            break;
        }

        case CmdId_MarketInfo:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int mode = pymt4_getIntArgument();
            pymt4_setDoubleResult(MarketInfo(symbol_string, mode), GetLastError());
            break;
        }

        case CmdId_PlaySound:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            PlaySound(symbol_string);
            pymt4_setBoolResult(True, GetLastError());
            break;
        }

        case CmdId_Print:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            Print(symbol_string);
            pymt4_setBoolResult(True, GetLastError());
            break;
        }

        case CmdId_SendFTP:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            pymt4_setBoolResult(SendFTP(symbol_string,symbol_string), GetLastError());
            break;
        }

        case CmdId_SendMail:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string dll_arg_string1 = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int ucBuffer2_size = pymt4_getStringArgument(ucBuffer2);
            string dll_arg_string2 = CharArrayToString(ucBuffer2, 0, ucBuffer2_size);
            SendMail(dll_arg_string1, dll_arg_string2);
            pymt4_setBoolResult(True, GetLastError());
            break;
        }

        case CmdId_SendNotification:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            pymt4_setBoolResult(SendNotification(symbol_string), GetLastError());
            break;
        }

        case CmdId_Symbol:
        {
            //--- Convert the strings to uchar[] arrays
            StringToCharArray(Symbol(), ucResult);
            pymt4_setStringResult(ucResult, GetLastError());
            break;
        }

        case CmdId_Period:
        {
            pymt4_setIntResult(Period(), GetLastError());
            break;
        }

        case CmdId_RefreshRates:
        {
            pymt4_setBoolResult(RefreshRates(), GetLastError());
            break;
        }

        case CmdId_TimeCurrent:
        {
            pymt4_setDoubleResult(TimeCurrent(), GetLastError());
            break;
        }

        case CmdId_TimeLocal:
        {
            pymt4_setDoubleResult(TimeLocal(), GetLastError());
            break;
        }

        case CmdId_TimeGMT:
        {
            pymt4_setDoubleResult(TimeGMT(), GetLastError());
            break;
        }

        case CmdId_TimeGMTOffset:
        {
            pymt4_setIntResult(TimeGMTOffset(), GetLastError());
            break;
        }

        case CmdId_iAC:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iAC(symbol_string, timeframe, shift), GetLastError());
            break;
        }

        case CmdId_iAD:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iAD(symbol_string, timeframe, shift), GetLastError());
            break;
        }

        case CmdId_iADX:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int period = pymt4_getIntArgument();
            int applied_price = pymt4_getIntArgument();
            int mode = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iADX(symbol_string, timeframe, period, applied_price, mode, shift), GetLastError());
            break;
        }

        case CmdId_iAlligator:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int jaw_period = pymt4_getIntArgument();
            int jaw_shift = pymt4_getIntArgument();
            int teeth_period = pymt4_getIntArgument();
            int teeth_shift = pymt4_getIntArgument();
            int lips_period = pymt4_getIntArgument();
            int lips_shift = pymt4_getIntArgument();
            int ma_method = pymt4_getIntArgument();
            int applied_price = pymt4_getIntArgument();
            int mode = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iAlligator(symbol_string, timeframe, jaw_period, jaw_shift,
                                    teeth_period, teeth_shift, lips_period, lips_shift, ma_method,
                                    applied_price, mode, shift), GetLastError());
            break;
        }

        case CmdId_iAO:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iAO(symbol_string, timeframe, shift), GetLastError());
            break;
        }

        case CmdId_iATR:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int period = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iATR(symbol_string, timeframe, period, shift), GetLastError());
            break;
        }

        case CmdId_iBearsPower:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int period = pymt4_getIntArgument();
            int applied_price = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iBearsPower(symbol_string, timeframe, period, applied_price, shift), GetLastError());
            break;
        }

        case CmdId_iBands:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int period = pymt4_getIntArgument();
            double deviation = pymt4_getDoubleArgument();
            int bands_shift = pymt4_getIntArgument();
            int applied_price = pymt4_getIntArgument();
            int mode = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iBands(symbol_string, timeframe, period, deviation, bands_shift, applied_price, mode, shift), GetLastError());
            break;
        }

        case CmdId_iCCI:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int period = pymt4_getIntArgument();
            int applied_price = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iCCI(symbol_string, timeframe, period, applied_price, shift), GetLastError());
            break;
        }

        case CmdId_iDeMarker:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int period = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iDeMarker(symbol_string, timeframe, period, shift), GetLastError());
            break;
        }

        case CmdId_iEnvelopes:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int ma_period = pymt4_getIntArgument();
            int ma_method = pymt4_getIntArgument();
            int ma_shift = pymt4_getIntArgument();
            int applied_price = pymt4_getIntArgument();
            double deviation = pymt4_getDoubleArgument();
            int mode = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iEnvelopes(symbol_string, timeframe, ma_period, ma_method,
                                            ma_shift, applied_price, deviation, mode, shift), GetLastError());
            break;
        }

        case CmdId_iForce:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int period = pymt4_getIntArgument();
            int ma_method = pymt4_getIntArgument();
            int applied_price = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iForce(symbol_string, timeframe, period, ma_method,
                                            applied_price, shift), GetLastError());
            break;
        }

        case CmdId_iFractals:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int mode = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iFractals(symbol_string, timeframe, mode, shift), GetLastError());
            break;
        }

        case CmdId_iGator:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int jaw_period = pymt4_getIntArgument();
            int jaw_shift = pymt4_getIntArgument();
            int teeth_period = pymt4_getIntArgument();
            int teeth_shift = pymt4_getIntArgument();
            int lips_period = pymt4_getIntArgument();
            int lips_shift = pymt4_getIntArgument();
            int ma_method = pymt4_getIntArgument();
            int applied_price = pymt4_getIntArgument();
            int mode = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iGator(symbol_string, timeframe, jaw_period, jaw_shift,
                                    teeth_period, teeth_shift, lips_period, lips_shift, ma_method,
                                    applied_price, mode, shift), GetLastError());
            break;
        }

        case CmdId_iIchimoku:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int tenkan_sen = pymt4_getIntArgument();
            int kijun_sen = pymt4_getIntArgument();
            int senkou_span_b = pymt4_getIntArgument();
            int mode = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iIchimoku(symbol_string, timeframe, tenkan_sen, kijun_sen,
                                            senkou_span_b, mode, shift), GetLastError());
            break;
        }

        case CmdId_iBWMFI:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iBWMFI(symbol_string, timeframe, shift), GetLastError());
            break;
        }

        case CmdId_iMomentum:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int period = pymt4_getIntArgument();
            int applied_price = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iMomentum(symbol_string, timeframe, period, applied_price, shift), GetLastError());
            break;
        }

        case CmdId_iMFI:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int period = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iMFI(symbol_string, timeframe, period, shift), GetLastError());
            break;
        }

        case CmdId_iMA:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int ma_period = pymt4_getIntArgument();
            int ma_method = pymt4_getIntArgument();
            int ma_shift = pymt4_getIntArgument();
            int applied_price = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iMA(symbol_string, timeframe, ma_period, ma_method,
                                            ma_shift, applied_price, shift), GetLastError());
            break;
        }

        case CmdId_iOsMA:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int fast_ema_period = pymt4_getIntArgument();
            int slow_ema_period = pymt4_getIntArgument();
            int signal_period = pymt4_getIntArgument();
            int applied_price = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iOsMA(symbol_string, timeframe, fast_ema_period, slow_ema_period,
                                            signal_period, applied_price, shift), GetLastError());
            break;
        }

        case CmdId_iMACD:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int fast_ema_period = pymt4_getIntArgument();
            int slow_ema_period = pymt4_getIntArgument();
            int signal_period = pymt4_getIntArgument();
            int applied_price = pymt4_getIntArgument();
            int mode = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iMACD(symbol_string, timeframe, fast_ema_period, slow_ema_period,
                                            signal_period, applied_price, mode, shift), GetLastError());
            break;
        }

        case CmdId_iOBV:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int applied_price = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iOBV(symbol_string, timeframe, applied_price, shift), GetLastError());
            break;
        }

        case CmdId_iRSI:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int period = pymt4_getIntArgument();
            int applied_price = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iRSI(symbol_string, timeframe, period, applied_price, shift), GetLastError());
            break;
        }

        case CmdId_iRVI:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int period = pymt4_getIntArgument();
            int mode = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iRVI(symbol_string, timeframe, period, mode, shift), GetLastError());
            break;
        }

        case CmdId_iSAR:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            double sar_step = pymt4_getDoubleArgument();
            double maximum = pymt4_getDoubleArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iSAR(symbol_string, timeframe, sar_step, maximum, shift), GetLastError());
            break;
        }

        case CmdId_iStdDev:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int ma_period = pymt4_getIntArgument();
            int ma_shift = pymt4_getIntArgument();
            int ma_method = pymt4_getIntArgument();
            int applied_price = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iStdDev(symbol_string, timeframe, ma_period, ma_shift,
                                            ma_method, applied_price, shift), GetLastError());
            break;
        }

        case CmdId_iStochastic:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int Kperiod = pymt4_getIntArgument();
            int Dperiod = pymt4_getIntArgument();
            int slowing = pymt4_getIntArgument();
            int method = pymt4_getIntArgument();
            int price_field = pymt4_getIntArgument();
            int mode = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iStochastic(symbol_string, timeframe, Kperiod, Dperiod,
                                    slowing, method, price_field, mode, shift), GetLastError());
            break;
        }

        case CmdId_iWPR:
        {
            //--- Load string into uchar Buffer
            int ucBuffer1_size = pymt4_getStringArgument(ucBuffer1);
            string symbol_string = CharArrayToString(ucBuffer1, 0, ucBuffer1_size);
            int timeframe = pymt4_getIntArgument();
            int period = pymt4_getIntArgument();
            int shift = pymt4_getIntArgument();
            pymt4_setDoubleResult(iWPR(symbol_string, timeframe, period, shift), GetLastError());
            break;
        }

        case CmdId_Set_Flow_Return_Value:
        {
            return_value = pymt4_getBoolArgument();
            pymt4_setBoolResult(true, GetLastError());
            break;
        }

        default:
        {
            PrintFormat("Received unknown (%d) command!", pendingCommandId);
            ExpertRemove();
        }
    }

    return(return_value);
}


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
    bool is_continue = True;
    while (is_continue)
    {
        //---
        int pendingCommandId = pymt4_requestPendingCommand();
        if (pendingCommandId != CmdId_CheckShutdownCondition)
        {
            is_continue = evaluateCommand(pendingCommandId);
        }

        if (IsStopped())
        {
            break;
        }

        //--- Delay to allow user to press cancel.
        //--- Uncomment this delay if you are debugging or else it will affect performance especially
        //--- during benchmarking run.
        Sleep(50);
    }

    pymt4_uninitialize(ucResult, WindowHandle(Symbol(), Period()));
    Print ("PyMT4 Host Uninitialized ...");
    return (False);
}


int deinit()
{
    Print("Host Has Stopped");
    return(0);
}

int start()
{
    Print("Host Has Started");
    return(0);
}