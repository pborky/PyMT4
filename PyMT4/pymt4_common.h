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


#pragma once

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>

#include <boost/foreach.hpp>

namespace PyMT4 {
    #define DEBUG(MSG) std::cout << __FUNCSIG__ << ": " << std::endl << ">> " << MSG << std::endl;
}

#define DEFAULT_PORT 5621

#include <typeinfo>
#include <boost/functional/hash.hpp>
#include <boost/unordered_map.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

#define EXPORT(RESULT_TYPE) __declspec(dllexport) RESULT_TYPE __stdcall

#define DECLAREPTR(CLASS,NAME) \
    typedef boost::shared_ptr<CLASS> NAME ## Ptr; \
    typedef boost::weak_ptr<CLASS> NAME ## WPtr;

#define DECLARE(CLASS) \
    class CLASS; \
    DECLAREPTR(CLASS,CLASS);


namespace PyMT4 { 
    class Exception : public std::exception
    {
    private:
        const std::string    m_message;
    public:
        Exception(const std::string& message) : m_message(message) {}
        virtual const char* what() const throw() { return m_message.c_str(); }
    }; 

    struct MT4String
    {
        int32_t               len;
        char              *string;
    };

    #pragma pack(push,1)
    struct MT4RateInfo
    {
        uint32_t      ctm;
        double            open;
        double            low;
        double            high;
        double            close;
        double            vol;
    };
    #pragma pack(pop)


    enum MessageTypeIdentifier
    {
        MessageCommandType = 0,
        MessageResultType,
        MessageEventType,
        MessageMax
    };

    
    enum EventIdentifier
    {
        EvtId_OnTick = 0,
        EventUnknown
    };


    enum CommandIdentifier
    {
        //PYMT4 Internal Commands
        CheckShutdownCondition  = 100,

        //PYMT4 System Commands
        CmdId_IsConnected = 200,
        CmdId_Is_Server_Online,
        CmdId_Is_Server_Asleep,

        //MT4 Commands
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

        CmdId_IsDemo = 1300,
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


        /* Common Functions http://docs.mql4.com/common */
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
        CmdId_ExpertRemove,
        CmdId_TerminalClose,

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

        /* Flow control */
        CmdId_Set_Flow_Return_Value = 3000,

        CommandUnknown = 10000
    };
    

    enum Constant
    {
        SELECT_BY_POS = 0,
        SELECT_BY_TICKET = 1,
        PERIOD_CURRENT = 0,
        PERIOD_M1  = 1,
        PERIOD_M5  = 5,
        PERIOD_M15 = 15,
        PERIOD_M30 = 30,
        PERIOD_H1  = 60,
        PERIOD_H4  = 240,
        PERIOD_D1  = 1440,
        PERIOD_W1  = 10080,
        PERIOD_MN1  = 43200,
        CHART_TIMEFRAME = 0,
        OP_BUY = 0,
        OP_SELL = 1,
        OP_BUYLIMIT = 2,
        OP_SELLLIMIT = 3,
        OP_BUYSTOP = 4,
        OP_SELLSTOP = 5,
        PRICE_CLOSE = 0,
        PRICE_OPEN = 1,
        PRICE_HIGH = 2,
        PRICE_LOW = 3,
        PRICE_MEDIAN = 4,
        PRICE_TYPICAL = 5,
        PRICE_WEIGHTED =6,
        MODE_TRADES = 0,
        MODE_HISTORY =1,
        MODE_OPEN = 0,
        MODE_LOW = 1,
        MODE_HIGH = 2,
        MODE_CLOSE = 3,
        MODE_VOLUME = 4,
        MODE_TIME = 5,
        MODE_BID = 9,
        MODE_ASK = 10,
        MODE_POINT = 11,
        MODE_DIGITS = 12,
        MODE_SPREAD = 13,
        MODE_STOPLEVEL = 14,
        MODE_LOTSIZE = 15,
        MODE_TICKVALUE = 16,
        MODE_TICKSIZE = 17,
        MODE_SWAPLONG = 18,
        MODE_SWAPSHORT = 19,
        MODE_STARTING = 20,
        MODE_EXPIRATION = 21,
        MODE_TRADEALLOWED = 22,
        MODE_MINLOT = 23,
        MODE_LOTSTEP = 24,
        MODE_MAXLOT = 25,
        MODE_SWAPTYPE = 26,
        MODE_PROFITCALCMODE = 27,
        MODE_MARGINCALCMODE = 28,
        MODE_MARGININIT = 29,
        MODE_MARGINMAINTENANCE = 30,
        MODE_MARGINHEDGED = 31,
        MODE_MARGINREQUIRED = 32,
        MODE_FREEZELEVEL = 33,
        DRAW_LINE = 0,
        DRAW_SECTION = 1,
        DRAW_HISTOGRAM = 2,
        DRAW_ARROW = 3,
        DRAW_ZIGZAG = 4,
        DRAW_NONE = 12,
        STYLE_SOLID = 0,
        STYLE_DASH = 1,
        STYLE_DOT = 2,
        STYLE_DASHDOT = 3,
        STYLE_DASHDOTDOT = 4,
        SYMBOL_THUMBSUP = 67,
        SYMBOL_THUMBSDOWN = 68,
        SYMBOL_ARROWUP = 241,
        SYMBOL_ARROWDOWN = 242,
        SYMBOL_STOPSIGN = 251,
        SYMBOL_CHECKSIGN = 252,
        SYMBOL_LEFTPRICE = 5,
        SYMBOL_RIGHTPRICE = 6,
        MODE_MAIN = 0,
        MODE_SIGNAL = 1,
        MODE_PLUSDI = 1,
        MODE_MINUSDI = 2,
        MODE_UPPER = 1,
        MODE_LOWER = 2,
        MODE_TENKANSEN = 1,
        MODE_KIJUNSEN = 2,
        MODE_SENKOUSPANA = 3,
        MODE_SENKOUSPANB = 4,
        MODE_CHINKOUSPAN = 5,
        MODE_SMA = 0,
        MODE_EMA = 1,
        MODE_SMMA = 2,
        MODE_LWMA = 3,
        MODE_GATORJAW = 1,
        MODE_GATORTEETH = 2,
        MODE_GATORLIPS = 3,
        _IDOK = 1,
        _IDCANCEL = 2,
        _IDABORT = 3,
        _IDRETRY = 4,
        _IDIGNORE = 5,
        _IDYES = 6,
        _IDNO = 7,
        _IDTRYAGAIN = 10,
        _IDCONTINUE = 11,
        _MB_OK = 0x00000000,
        _MB_OKCANCEL = 0x00000001,
        _MB_ABORTRETRYIGNORE = 0x00000002,
        _MB_YESNOCANCEL = 0x00000003,
        _MB_YESNO = 0x00000004,
        _MB_RETRYCANCEL = 0x00000005,
        _MB_CANCELTRYCONTINUE = 0x00000006,
        _MB_ICONSTOP = 0x0000001,
        _MB_ICONERROR = 0x0000001,
        _MB_ICONHAND = 0x0000001,
        _MB_ICONQUESTION = 0x0000002,
        _MB_ICONEXCLAMATION = 0x00000030,
        _MB_ICONWARNING = 0x0000003,
        _MB_ICONINFORMATION = 0x0000004,
        _MB_ICONASTERISK = 0x0000004,
        _MB_DEFBUTTON1 = 0x00000000,
        _MB_DEFBUTTON2 = 0x00000100,
        _MB_DEFBUTTON3 = 0x00000200,
        _MB_DEFBUTTON4 = 0x00000300,
        OBJ_VLINE = 0,
        OBJ_HLINE = 1,
        OBJ_TREND = 2,
        OBJ_TRENDBYANGLE = 3,
        OBJ_REGRESSION = 4,
        OBJ_CHANNEL = 5,
        OBJ_STDDEVCHANNEL = 6,
        OBJ_GANNLINE = 7,
        OBJ_GANNFAN = 8,
        OBJ_GANNGRID = 9,
        OBJ_FIBO = 10,
        OBJ_FIBOTIMES = 11,
        OBJ_FIBOFAN = 12,
        OBJ_FIBOARC = 13,
        OBJ_EXPANSION = 14,
        OBJ_FIBOCHANNEL = 15,
        OBJ_RECTANGLE = 16,
        OBJ_TRIANGLE = 17,
        OBJ_ELLIPSE = 18,
        OBJ_PITCHFORK = 19,
        OBJ_CYCLES = 20,
        OBJ_TEXT = 21,
        OBJ_ARROW = 22,
        OBJ_LABEL = 23,
        OBJPROP_TIME1 = 0,
        OBJPROP_PRICE1 = 1,
        OBJPROP_TIME2 = 2,
        OBJPROP_PRICE2 = 3,
        OBJPROP_TIME3 = 4,
        OBJPROP_PRICE3 = 5,
        OBJPROP_COLOR = 6,
        OBJPROP_STYLE = 7,
        OBJPROP_WIDTH = 8,
        OBJPROP_BACK = 9,
        OBJPROP_RAY = 10,
        OBJPROP_ELLIPSE = 11,
        OBJPROP_SCALE = 12,
        OBJPROP_ANGLE = 13,
        OBJPROP_ARROWCODE = 14,
        OBJPROP_TIMEFRAMES = 15,
        OBJPROP_DEVIATION = 16,
        OBJPROP_FONTSIZE = 100,
        OBJPROP_CORNER = 101,
        OBJPROP_XDISTANCE = 102,
        OBJPROP_YDISTANCE = 103,
        OBJPROP_FIBOLEVELS = 200,
        OBJPROP_LEVELCOLOR = 201,
        OBJPROP_LEVELSTYLE = 202,
        OBJPROP_LEVELWIDTH = 203,
        OBJPROP_FIRSTLEVEL = 210,
        OBJ_PERIOD_M1   = 0x0001,
        OBJ_PERIOD_M5   = 0x0002,
        OBJ_PERIOD_M15  = 0x0004,
        OBJ_PERIOD_M30  = 0x0008,
        OBJ_PERIOD_H1   = 0x0010,
        OBJ_PERIOD_H4   = 0x0020,
        OBJ_PERIOD_D1   = 0x0040,
        OBJ_PERIOD_W1   = 0x0080,
        OBJ_PERIOD_MN1  = 0x0100,
        OBJ_ALL_PERIODS = 0x01FF,
        OBJ_NULL        = 0,
        OBJ_EMPTY        = -1,
        REASON_REMOVE = 1,
        REASON_RECOMPILE = 2,
        REASON_CHARTCHANGE = 3,
        REASON_CHARTCLOSE = 4,
        REASON_PARAMETERS = 5,
        REASON_ACCOUNT = 6,
        _NULL = 0,
        EMPTY = -1,
        VALUE_EMPTY_VALUE = 0x7FFFFFFF,
        VALUE_CLR_NONE = 0xFFFFFFFF,
        VALUE_WHOLE_ARRAY = 0
    };

    enum Error {
        ERR_NO_ERROR = 0,
        ERR_NO_RESULT = 1,
        ERR_COMMON_ERROR = 2,
        ERR_INVALID_TRADE_PARAMETERS = 3,
        ERR_SERVER_BUSY = 4,
        ERR_OLD_VERSION = 5,
        ERR_NO_CONNECTION = 6,
        ERR_NOT_ENOUGH_RIGHTS = 7,
        ERR_TOO_FREQUENT_REQUESTS = 8,
        ERR_MALFUNCTIONAL_TRADE = 9,
        ERR_ACCOUNT_DISABLED = 64,
        ERR_INVALID_ACCOUNT = 65,
        ERR_TRADE_TIMEOUT = 128,
        ERR_INVALID_PRICE = 129,
        ERR_INVALID_STOPS = 130,
        ERR_INVALID_TRADE_VOLUME = 131,
        ERR_MARKET_CLOSED = 132,
        ERR_TRADE_DISABLED = 133,
        ERR_NOT_ENOUGH_MONEY = 134,
        ERR_PRICE_CHANGED = 135,
        ERR_OFF_QUOTES = 136,
        ERR_BROKER_BUSY = 137,
        ERR_REQUOTE = 138,
        ERR_ORDER_LOCKED = 139,
        ERR_LONG_POSITIONS_ONLY_ALLOWED = 140,
        ERR_TOO_MANY_REQUESTS = 141,
        ERR_TRADE_MODIFY_DENIED = 145,
        ERR_TRADE_CONTEXT_BUSY = 146,
        ERR_TRADE_EXPIRATION_DENIED = 147,
        ERR_TRADE_TOO_MANY_ORDERS = 148,
        ERR_TRADE_HEDGE_PROHIBITED = 149,
        ERR_TRADE_PROHIBITED_BY_FIFO = 150,
        ERR_NO_MQLERROR = 4000,
        ERR_WRONG_FUNCTION_POINTER = 4001,
        ERR_ARRAY_INDEX_OUT_OF_RANGE = 4002,
        ERR_NO_MEMORY_FOR_CALL_STACK = 4003,
        ERR_RECURSIVE_STACK_OVERFLOW = 4004,
        ERR_NOT_ENOUGH_STACK_FOR_PARAM = 4005,
        ERR_NO_MEMORY_FOR_PARAM_STRING = 4006,
        ERR_NO_MEMORY_FOR_TEMP_STRING = 4007,
        ERR_NOT_INITIALIZED_STRING = 4008,
        ERR_NOT_INITIALIZED_ARRAYSTRING = 4009,
        ERR_NO_MEMORY_FOR_ARRAYSTRING = 4010,
        ERR_TOO_LONG_STRING = 4011,
        ERR_REMAINDER_FROM_ZERO_DIVIDE = 4012,
        ERR_ZERO_DIVIDE = 4013,
        ERR_UNKNOWN_COMMAND = 4014,
        ERR_WRONG_JUMP = 4015,
        ERR_NOT_INITIALIZED_ARRAY = 4016,
        ERR_DLL_CALLS_NOT_ALLOWED = 4017,
        ERR_CANNOT_LOAD_LIBRARY = 4018,
        ERR_CANNOT_CALL_FUNCTION = 4019,
        ERR_EXTERNAL_CALLS_NOT_ALLOWED = 4020,
        ERR_NO_MEMORY_FOR_RETURNED_STR = 4021,
        ERR_SYSTEM_BUSY = 4022,
        ERR_DLLFUNC_CRITICALERROR = 4023,
        ERR_INTERNAL_ERROR = 4024,
        ERR_OUT_OF_MEMORY = 4025,
        ERR_INVALID_POINTER = 4026,
        ERR_FORMAT_TOO_MANY_FORMATTERS = 4027,
        ERR_FORMAT_TOO_MANY_PARAMETERS = 4028,
        ERR_ARRAY_INVALID = 4029,
        ERR_CHART_NOREPLY = 4030,
        ERR_INVALID_FUNCTION_PARAMSCNT = 4050,
        ERR_INVALID_FUNCTION_PARAMVALUE = 4051,
        ERR_STRING_FUNCTION_INTERNAL = 4052,
        ERR_SOME_ARRAY_ERROR = 4053,
        ERR_INCORRECT_SERIESARRAY_USING = 4054,
        ERR_CUSTOM_INDICATOR_ERROR = 4055,
        ERR_INCOMPATIBLE_ARRAYS = 4056,
        ERR_GLOBAL_VARIABLES_PROCESSING = 4057,
        ERR_GLOBAL_VARIABLE_NOT_FOUND = 4058,
        ERR_FUNC_NOT_ALLOWED_IN_TESTING = 4059,
        ERR_FUNCTION_NOT_CONFIRMED = 4060,
        ERR_SEND_MAIL_ERROR = 4061,
        ERR_STRING_PARAMETER_EXPECTED = 4062,
        ERR_INTEGER_PARAMETER_EXPECTED = 4063,
        ERR_DOUBLE_PARAMETER_EXPECTED = 4064,
        ERR_ARRAY_AS_PARAMETER_EXPECTED = 4065,
        ERR_HISTORY_WILL_UPDATED = 4066,
        ERR_TRADE_ERROR = 4067,
        ERR_RESOURCE_NOT_FOUND = 4068,
        ERR_RESOURCE_NOT_SUPPORTED = 4069,
        ERR_RESOURCE_DUPLICATED = 4070,
        ERR_INDICATOR_CANNOT_INIT = 4071,
        ERR_INDICATOR_CANNOT_LOAD = 4072,
        ERR_NO_HISTORY_DATA = 4073,
        ERR_NO_MEMORY_FOR_HISTORY = 4074,
        ERR_NO_MEMORY_FOR_INDICATOR = 4075,
        ERR_END_OF_FILE = 4099,
        ERR_SOME_FILE_ERROR = 4100,
        ERR_WRONG_FILE_NAME = 4101,
        ERR_TOO_MANY_OPENED_FILES = 4102,
        ERR_CANNOT_OPEN_FILE = 4103,
        ERR_INCOMPATIBLE_FILEACCESS = 4104,
        ERR_NO_ORDER_SELECTED = 4105,
        ERR_UNKNOWN_SYMBOL = 4106,
        ERR_INVALID_PRICE_PARAM = 4107,
        ERR_INVALID_TICKET = 4108,
        ERR_TRADE_NOT_ALLOWED = 4109,
        ERR_LONGS_NOT_ALLOWED = 4110,
        ERR_SHORTS_NOT_ALLOWED = 4111,
        ERR_TRADE_EXPERT_DISABLED_BY_SERVER = 4112,
        ERR_OBJECT_ALREADY_EXISTS = 4200,
        ERR_UNKNOWN_OBJECT_PROPERTY = 4201,
        ERR_OBJECT_DOES_NOT_EXIST = 4202,
        ERR_UNKNOWN_OBJECT_TYPE = 4203,
        ERR_NO_OBJECT_NAME = 4204,
        ERR_OBJECT_COORDINATES_ERROR = 4205,
        ERR_NO_SPECIFIED_SUBWINDOW = 4206,
        ERR_SOME_OBJECT_ERROR = 4207,
        ERR_CHART_PROP_INVALID = 4210,
        ERR_CHART_NOT_FOUND = 4211,
        ERR_CHARTWINDOW_NOT_FOUND = 4212,
        ERR_CHARTINDICATOR_NOT_FOUND = 4213,
        ERR_SYMBOL_SELECT = 4220,
        ERR_NOTIFICATION_ERROR = 4250,
        ERR_NOTIFICATION_PARAMETER = 4251,
        ERR_NOTIFICATION_SETTINGS = 4252,
        ERR_NOTIFICATION_TOO_FREQUENT = 4253,
        ERR_FTP_NOSERVER = 4260,
        ERR_FTP_NOLOGIN = 4261,
        ERR_FTP_CONNECT_FAILED = 4262,
        ERR_FTP_CLOSED = 4263,
        ERR_FTP_CHANGEDIR = 4264,
        ERR_FTP_FILE_ERROR = 4265,
        ERR_FTP_ERROR = 4266,
        ERR_FILE_TOO_MANY_OPENED = 5001,
        ERR_FILE_WRONG_FILENAME = 5002,
        ERR_FILE_TOO_LONG_FILENAME = 5003,
        ERR_FILE_CANNOT_OPEN = 5004,
        ERR_FILE_BUFFER_ALLOCATION_ERROR = 5005,
        ERR_FILE_CANNOT_DELETE = 5006,
        ERR_FILE_INVALID_HANDLE = 5007,
        ERR_FILE_WRONG_HANDLE = 5008,
        ERR_FILE_NOT_TOWRITE = 5009,
        ERR_FILE_NOT_TOREAD = 5010,
        ERR_FILE_NOT_BIN = 5011,
        ERR_FILE_NOT_TXT = 5012,
        ERR_FILE_NOT_TXTORCSV = 5013,
        ERR_FILE_NOT_CSV = 5014,
        ERR_FILE_READ_ERROR = 5015,
        ERR_FILE_WRITE_ERROR = 5016,
        ERR_FILE_BIN_STRINGSIZE = 5017,
        ERR_FILE_INCOMPATIBLE = 5018,
        ERR_FILE_IS_DIRECTORY = 5019,
        ERR_FILE_NOT_EXIST = 5020,
        ERR_FILE_CANNOT_REWRITE = 5021,
        ERR_FILE_WRONG_DIRECTORYNAME = 5022,
        ERR_FILE_DIRECTORY_NOT_EXIST = 5023,
        ERR_FILE_NOT_DIRECTORY = 5024,
        ERR_FILE_CANNOT_DELETE_DIRECTORY = 5025,
        ERR_FILE_CANNOT_CLEAN_DIRECTORY = 5026,
        ERR_FILE_ARRAYRESIZE_ERROR = 5027,
        ERR_FILE_STRINGRESIZE_ERROR = 5028,
        ERR_FILE_STRUCT_WITH_OBJECTS = 5029,
        ERR_WEBREQUEST_INVALID_ADDRESS = 5200,
        ERR_WEBREQUEST_CONNECT_FAILED = 5201,
        ERR_WEBREQUEST_TIMEOUT = 5202,
        ERR_WEBREQUEST_REQUEST_FAILED = 5203,
        ERR_PYMT4_TIMEOUT = 65536,
        ERR_PYMT4_NOT_CONNECTED = 65537
    };


    typedef boost::mpl::vector<std::string> SerializableTypeList;

    typedef int32_t Color;
    typedef double Datetime;

    #define DATETIME_DEFAULT 0
    #define CLR_NONE 0


};