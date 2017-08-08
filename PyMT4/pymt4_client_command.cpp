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

#include "pymt4_client_command.h"

namespace PyMT4
{
	
	LastErrorMap _lastErrorMap;
	boost::mutex _lastErrorMapLock;
	boost::mutex _dispatchLock;
	namespace Command
	{
		/* Account Info https://docs.mql4.com/account */

		double AccountBalance()
		{return DispatchFunction<double>()(PyMT4::CmdId_AccountBalance);}

		double AccountCredit()
		{return DispatchFunction<double>()(PyMT4::CmdId_AccountCredit);}

		std::string AccountCompany()
		{return DispatchFunction<std::string>()(PyMT4::CmdId_AccountCompany);}

		std::string AccountCurrency()
		{return DispatchFunction<std::string>()(PyMT4::CmdId_AccountCurrency);}

		double AccountEquity()
		{return DispatchFunction<double>()(PyMT4::CmdId_AccountEquity);}

		double AccountFreeMargin()
		{return DispatchFunction<double>()(PyMT4::CmdId_AccountFreeMargin);}

		double AccountFreeMarginCheck(const char *symbol,const int32_t& cmd,const double& volume)
		{return DispatchFunction<double,const char *,const int32_t& ,const double&>()(PyMT4::CmdId_AccountFreeMarginCheck,symbol,cmd,volume); }

		double AccountFreeMarginMode()
		{return DispatchFunction<double>()(PyMT4::CmdId_AccountFreeMarginMode);}

		int32_t AccountLeverage()
		{return DispatchFunction<int32_t>()(PyMT4::CmdId_AccountLeverage);}

		double AccountMargin()
		{return DispatchFunction<double>()(PyMT4::CmdId_AccountMargin);}

		std::string AccountName()
		{return DispatchFunction<std::string>()(PyMT4::CmdId_AccountName);}

		int32_t AccountNumber()
		{return DispatchFunction<int32_t>()(PyMT4::CmdId_AccountNumber);}

		double AccountProfit()
		{return DispatchFunction<double>()(PyMT4::CmdId_AccountProfit);}

		std::string AccountServer()
		{return DispatchFunction<std::string>()(PyMT4::CmdId_AccountServer);}

		int32_t AccountStopoutLevel()
		{return DispatchFunction<int32_t>()(PyMT4::CmdId_AccountStopoutLevel);}

		int32_t AccountStopoutMode()
		{return DispatchFunction<int32_t>()(PyMT4::CmdId_AccountStopoutMode);}


		/*Timeseries http://docs.mql4.com/series */

		int32_t iBars(const char *symbol,const int32_t& timeframe)
		{return DispatchFunction<int32_t,const std::string&,const int32_t&>()(PyMT4::CmdId_iBars,symbol,timeframe);}

		int32_t		iBarShift(const char *symbol,const Datetime& datetime,const bool& exact)
		{return DispatchFunction<int32_t,const std::string,const Datetime,const bool>()(CmdId_iBarShift,symbol,datetime,(BOOL)exact);}

		double		iClose(const char *symbol,const int32_t& timeframe, const int32_t& shift)
		{return DispatchFunction<double,const std::string,const int32_t,const int32_t>()(CmdId_iClose,symbol,timeframe,shift);}

		double		iHigh(const char *symbol, const int32_t& timeframe, const int32_t& shift)
		{return DispatchFunction<double,const std::string,const int32_t,const int32_t>()(CmdId_iHigh,symbol,timeframe,shift);}

		double		iHighest(const char *symbol, const int32_t& timeframe,const int32_t& type,const int32_t& count,const int32_t& start)
		{return DispatchFunction<double,const std::string,const int32_t,const int32_t,const int32_t,const int32_t>()(CmdId_iHighest,symbol,timeframe,type,count,start);}

		double		iLow(const char *symbol, const int32_t& timeframe,const int32_t& shift)
		{return DispatchFunction<double,const std::string,const int32_t,const int32_t,const int32_t,const int32_t>()(CmdId_iLow,symbol,timeframe,shift);}

		double		iLowest(const char *symbol, const int32_t& timeframe,const int32_t& type,const int32_t& count,const int32_t& start)
		{return DispatchFunction<double,const std::string,const int32_t,const int32_t,const int32_t,const int32_t>()(CmdId_iLowest,symbol,timeframe,type,count,start);}

		double		iOpen(const char *symbol, const int32_t& timeframe,const int32_t& shift)
		{return DispatchFunction<double,const std::string,const int32_t,const int32_t,const int32_t,const int32_t>()(CmdId_iOpen,symbol,timeframe,shift);}

		Datetime    iTime(const char *symbol, const int32_t& timeframe, const int32_t& shift)
		{return DispatchFunction<Datetime,const std::string,const int32_t,const int32_t,const int32_t,const int32_t>()(CmdId_iTime,symbol,timeframe,shift);}

		double		iVolume(const char *symbol, const int32_t& timeframe, const int32_t& shift)
		{return DispatchFunction<double,const std::string,const int32_t,const int32_t,const int32_t,const int32_t>()(CmdId_iVolume,symbol,timeframe,shift);}


		///* Trading Functions http://docs.mql4.com/trading */

		bool		OrderClose(const int32_t& ticket, const double& lots,const double& price, const int32_t& slippage, const Color& color)
		{return DispatchFunction<bool,const int32_t,const double,const double,const int32_t,const int32_t,const Color>()(CmdId_OrderClose,ticket,lots,price,slippage,color);}


		bool		OrderCloseBy(const int32_t& ticket, const double& opposite,const Color& color)
		{return DispatchFunction<bool,const int32_t,const double,const Color>()(CmdId_OrderCloseBy,ticket,opposite,color);}

		double		OrderClosePrice(const int32_t& ticket)		/* OrderSelect() */
		{return DispatchFunction<double,const int32_t>()(CmdId_OrderClosePrice,ticket);}

		Datetime	OrderCloseTime(const int32_t& ticket)		/* OrderSelect() */
		{return DispatchFunction<Datetime,const int32_t>()(CmdId_OrderCloseTime,ticket);}

		std::string OrderComment(const int32_t& ticket) 		/* OrderSelect() */
		{return DispatchFunction<std::string,const int32_t>()(CmdId_OrderComment,ticket);}

		double		OrderCommission(const int32_t& ticket)		/* OrderSelect() */
		{return DispatchFunction<double,const int32_t>()(CmdId_OrderCommission,ticket);}

		bool		OrderDelete(const int32_t& ticket,const Color& color) 
		{return DispatchFunction<bool,const int32_t,Color>()(CmdId_OrderDelete,ticket,color);}

		Datetime	OrderExpiration(const int32_t& ticket)		/* OrderSelect() */
		{return DispatchFunction<Datetime,const int32_t>()(CmdId_OrderExpiration,ticket);}

		double		OrderLots(const int32_t& ticket)			/* OrderSelect() */
		{return DispatchFunction<double,const int32_t>()(CmdId_OrderLots,ticket);}

		int32_t		OrderMagicNumber(const int32_t& ticket) 	/* OrderSelect() */
		{return DispatchFunction<int32_t,const int32_t>()(CmdId_OrderMagicNumber,ticket);}

		bool		OrderModify(const int32_t& ticket,const double& price, const double& stoploss, const double& takeprofit, const Datetime& expiration, const Color& arrow_color)
		{return DispatchFunction<bool,const int32_t,const double,const double, const double, const Datetime,const Color>()(CmdId_OrderModify,ticket,price,stoploss,takeprofit,expiration,arrow_color);}

		double		OrderOpenPrice(const int32_t& ticket)		/* OrderSelect() */
		{return DispatchFunction<double,const int32_t>()(CmdId_OrderOpenPrice,ticket);}

		Datetime	OrderOpenTime(const int32_t& ticket)		/* OrderSelect() */
		{return DispatchFunction<Datetime,const int32_t>()(CmdId_OrderOpenTime,ticket);}

		bool		OrderPrint(const int32_t& ticket)			/* OrderSelect() */
		{return DispatchFunction<bool,const int32_t>()(CmdId_OrderPrint,ticket);}

		double		OrderProfit(const int32_t& ticket)			/* OrderSelect() */
		{return DispatchFunction<double,const int32_t>()(CmdId_OrderProfit,ticket);}

		int32_t		OrderSelect(const int32_t& index, const int32_t& select, const int32_t& pool)
		{return DispatchFunction<int32_t,const int32_t,const int32_t,const int32_t>()(CmdId_OrderSelect,index,select,pool);}

		int32_t		OrderSend(
								const char *symbol, 
								const int32_t& cmd, 
								const double& volume, 
								const double& price, 
								const int32_t& slippage, 
								const double& stoploss, 
								const double& takeprofit, 
								const char *comment,
								const int32_t& magic,
								const Datetime& expiration,
								const Color& arrow_color
							)
		{return DispatchFunction<int32_t,const std::string, const int32_t,const double,const double,const int32_t,const double,const double,const std::string,const int32_t,const Datetime, const Color>()
			(CmdId_OrderSend,
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
			arrow_color);}


		int32_t		OrdersHistoryTotal()
		{return DispatchFunction<int32_t>()(CmdId_OrdersHistoryTotal);}

		double		OrderStopLoss(const int32_t& ticket)	 /* OrderSelect() */
		{return DispatchFunction<double,const int32_t>()(CmdId_OrderStopLoss,ticket);}

		int32_t		OrdersTotal()
		{return DispatchFunction<int32_t>()(CmdId_OrdersTotal);}

		double		OrderSwap(const int32_t& ticket)		 /* OrderSelect() */
		{return DispatchFunction<int32_t,const int32_t>()(CmdId_OrderSwap,ticket);}

		std::string OrderSymbol(const int32_t& ticket)		 /* OrderSelect() */
		{return DispatchFunction<std::string,const int32_t>()(CmdId_OrderSymbol,ticket);}

		double		OrderTakeProfit(const int32_t& ticket)  /* OrderSelect() */
		{return DispatchFunction<double,const int32_t>()(CmdId_OrderTakeProfit,ticket);}

		int32_t		OrderTicket(const int32_t& ticket)		 /* OrderSelect() */
		{return DispatchFunction<int32_t,const int32_t>()(CmdId_OrderTicket,ticket);}

		int32_t		OrderType(const int32_t& ticket)		 /* OrderSelect() */
		{return DispatchFunction<int32_t,const int32_t>()(CmdId_OrderType,ticket);}


		/* Checkup - http://docs.mql4.com/check/ */

		bool IsConnected()
		{return DispatchFunction<bool>()(CmdId_IsConnected);}

		bool IsDemo()
		{return DispatchFunction<bool>()(CmdId_IsDemo);}

		bool IsDllsAllowed()
		{return DispatchFunction<bool>()(CmdId_IsDllsAllowed);}

		bool IsExpertEnabled()
		{return DispatchFunction<bool>()(CmdId_IsExpertEnabled);}

		bool IsLibrariesAllowed()
		{return DispatchFunction<bool>()(CmdId_IsLibrariesAllowed);}

		bool IsOptimization()
		{return DispatchFunction<bool>()(CmdId_IsOptimization);}

		bool IsStopped()
		{return DispatchFunction<bool>()(CmdId_IsStopped);}

		bool IsTesting()
		{return DispatchFunction<bool>()(CmdId_IsTesting);}

		bool IsTradeAllowed()
		{return DispatchFunction<bool>()(CmdId_IsTradeAllowed);}

		bool IsTradeContextBusy()
		{return DispatchFunction<bool>()(CmdId_IsTradeContextBusy);}

		bool IsVisualMode()
		{return DispatchFunction<bool>()(CmdId_IsVisualMode);}

		int32_t  UninitializeReason()
		{return DispatchFunction<int32_t>()(CmdId_UninitializeReason);}

		/* DateTime functions */
		Datetime TimeCurrent()
		{return DispatchFunction<Datetime>()(CmdId_TimeCurrent);}

		Datetime TimeLocal()
		{return DispatchFunction<Datetime>()(CmdId_TimeLocal);}

		Datetime TimeGMT()
		{return DispatchFunction<Datetime>()(CmdId_TimeGMT);}

		int32_t TimeGMTOffset()
		{return DispatchFunction<int32_t>()(CmdId_TimeGMTOffset);}


		/* Client Terminal http://docs.mql4.com/terminal */

		std::string TerminalCompany()
		{return DispatchFunction<std::string>()(CmdId_TerminalCompany);};

		std::string TerminalName()
		{return DispatchFunction<std::string>()(CmdId_TerminalName);};

		std::string TerminalPath()
		{return DispatchFunction<std::string>()(CmdId_TerminalPath);};

		/* Common Functions */
		bool RefreshRates()
		{return DispatchFunction<bool>()(CmdId_RefreshRates);}
		
		bool ExpertRemove()
		{
			return DispatchFunction<bool>()(CmdId_ExpertRemove);
		}

		bool TerminalClose(const int & ret_code)
		{
			return DispatchFunction<bool, const int>()(CmdId_TerminalClose, ret_code);
		}

		bool Alert(const char *alert)
		{
			std::string resultString = alert;
			return DispatchFunction<bool, const std::string>()(CmdId_Alert, resultString.c_str());
		}

		bool Print(const char *print)
		{
			std::string resultString = print;
//#ifdef _DEBUG
//			std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK -> CmdId_Print, print:\'" << print << "\'" << ", resultString:\'" << resultString.c_str() << "\'" << std::endl;
//#endif
			return DispatchFunction<bool, const std::string>()(CmdId_Print, resultString.c_str());
		}

		bool Comment(const char *comment)
		{
			std::string resultString = comment;
			return DispatchFunction<bool, const std::string>()(CmdId_Comment, resultString.c_str());
		}

		int32_t GetTickCount()
		{return DispatchFunction<int32_t>()(CmdId_GetTickCount);}

		/* Market Info https://docs.mql4.com/marketinformation */
		double MarketInfo(const char *symbol, const int& type)
		{
			std::string resultString = symbol;
			return DispatchFunction<double,const std::string, const int>()(CmdId_MarketInfo, resultString.c_str(), type);
		}

		bool PlaySound(const char *filename)
		{
			std::string resultString = filename;
			return DispatchFunction<bool,const std::string>()(CmdId_PlaySound, resultString.c_str());
		}

		bool SendFTP(const char *filename, const char *ftp_path)
		{
			std::string resultString1 = filename; 
			std::string resultString2 = ftp_path;
			return DispatchFunction<bool, const std::string, const std::string>()(CmdId_SendFTP, resultString1.c_str(), resultString2.c_str());
		}

		bool SendMail(const char *subject, const char *text)
		{
			std::string resultString1 = subject;
			std::string resultString2 = text;
			return DispatchFunction<bool, const std::string, const std::string>()(CmdId_SendMail, resultString1.c_str(), resultString2.c_str());
		}

		bool SendNotification(const char *message)
		{
			std::string resultString = message;
			return DispatchFunction<bool, const std::string>()(CmdId_SendNotification, resultString.c_str());
		}

		std::string Symbol(void)
		{
			return DispatchFunction<std::string>()(CmdId_Symbol);
		};

		int32_t	Period(void)
		{
			return DispatchFunction<int32_t>()(CmdId_Period);
		}

		/* Technical Indicators https://docs.mql4.com/indicators */
		double iAC(const char *symbol, const int& timeframe, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int>()(CmdId_iAC, resultString.c_str(), timeframe, shift);
		}

		double iAD(const char *symbol, const int& timeframe, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int>()(CmdId_iAD, resultString.c_str(), timeframe, shift);
		}

		double iADX(const char *symbol, const int& timeframe, const int& period, const int& applied_price, const int& mode, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int, const int, const int>()(CmdId_iADX, resultString.c_str(), timeframe, period, applied_price, mode, shift);
		}

		double iAlligator
		(
			const char *symbol,
			const int& timeframe,
			const int& jaw_period,
			const int& jaw_shift,
			const int& teeth_period,
			const int& teeth_shift,
			const int& lips_period,
			const int& lips_shift,
			const int& ma_method,
			const int& applied_price,
			const int& mode,
			const int& shift
		)
		{
			std::string resultString = symbol;
			return DispatchFunction<double,
									const std::string,
									const int,
									const int,
									const int,
									const int,
									const int,
									const int,
									const int,
									const int,
									const int,
									const int,
									const int>()
				(CmdId_iAlligator,
					resultString.c_str(),
					timeframe,
					jaw_period,
					jaw_shift,
					teeth_period,
					teeth_shift,
					lips_period,
					lips_shift,
					ma_method,
					applied_price,
					mode,
					shift);
		}

		double iAO(const char *symbol, const int& timeframe, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int>()(CmdId_iAO, resultString.c_str(), timeframe, shift);
		}

		double iATR(const char *symbol, const int& timeframe, const int& period, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int>()(CmdId_iATR, resultString.c_str(), timeframe, period, shift);
		}

		double iBearsPower(const char *symbol, const int& timeframe, const int& period, const int& applied_price, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int, const int>()(CmdId_iBearsPower, resultString.c_str(), timeframe, period, applied_price, shift);
		}

		double iBands(const char *symbol, const int& timeframe, const int& period, const double& deviation, const int& bands_shift, const int& applied_price, const int& mode, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const double, const int, const int, const int, const int>()(CmdId_iBands, resultString.c_str(), timeframe, period, deviation, bands_shift, applied_price, mode, shift);
		}

		double iCCI(const char *symbol, const int& timeframe, const int& period, const int& applied_price, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int, const int>()(CmdId_iCCI, resultString.c_str(), timeframe, period, applied_price, shift);
		}

		double iDeMarker(const char *symbol, const int& timeframe, const int& period, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int>()(CmdId_iDeMarker, resultString.c_str(), timeframe, period, shift);
		}

		double iEnvelopes(const char *symbol, const int & timeframe, const int & ma_period, const int & ma_method, const int & ma_shift, const int & applied_price, const double & deviation, const int & mode, const int & shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int, const int, const int, const double, const int, const int>()(CmdId_iEnvelopes, resultString.c_str(), timeframe, ma_period, ma_method, ma_shift, applied_price, deviation, mode, shift);
		}

		double iForce(const char *symbol, const int& timeframe, const int& period, const int& ma_method, const int& applied_price, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int, const int, const int>()(CmdId_iForce, resultString.c_str(), timeframe, period, ma_method, applied_price, shift);
		}

		double iFractals(const char *symbol, const int& timeframe, const int& mode, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int>()(CmdId_iFractals, resultString.c_str(), timeframe, mode, shift);
		}

		double iGator
		(
			const char *symbol,
			const int& timeframe,
			const int& jaw_period,
			const int& jaw_shift,
			const int& teeth_period,
			const int& teeth_shift,
			const int& lips_period,
			const int& lips_shift,
			const int& ma_method,
			const int& applied_price,
			const int& mode,
			const int& shift
		)
		{
			std::string resultString = symbol;
			return DispatchFunction<double,
				const std::string,
				const int,
				const int,
				const int,
				const int,
				const int,
				const int,
				const int,
				const int,
				const int,
				const int,
				const int>()
				(CmdId_iGator,
					resultString.c_str(),
					timeframe,
					jaw_period,
					jaw_shift,
					teeth_period,
					teeth_shift,
					lips_period,
					lips_shift,
					ma_method,
					applied_price,
					mode,
					shift);
		}

		double iIchimoku(const char *symbol, const int & timeframe, const int & tenkan_sen, const int & kijun_sen, const int & senkou_span_b, const int & mode, const int & shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int, const int, const int, const int>()(CmdId_iIchimoku, resultString.c_str(), timeframe, tenkan_sen, kijun_sen, senkou_span_b, mode, shift);
		}

		double iBWMFI(const char *symbol, const int& timeframe, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int>()(CmdId_iBWMFI, resultString.c_str(), timeframe, shift);
		}

		double iMomentum(const char *symbol, const int& timeframe, const int& period, const int& applied_price, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int, const int>()(CmdId_iMomentum, resultString.c_str(), timeframe, period, applied_price, shift);
		}

		double iMFI(const char *symbol, const int& timeframe, const int& period, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int>()(CmdId_iMFI, resultString.c_str(), timeframe, period, shift);
		}

		double iMA(const char *symbol, const int& timeframe, const int& ma_period, const int& ma_shift, const int& ma_method, const int& applied_price, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int, const int, const int, const int>()(CmdId_iMA, resultString.c_str(), timeframe, ma_period, ma_shift, ma_method, applied_price, shift);
		}

		double iOsMA(const char *symbol, const int& timeframe, const int& fast_ema_period, const int& slow_ema_period, const int& signal_period, const int& applied_price, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int, const int, const int, const int>()(CmdId_iOsMA, resultString.c_str(), timeframe, fast_ema_period, slow_ema_period, signal_period, applied_price, shift);
		}

		double iMACD(const char *symbol, const int& timeframe, const int& fast_ema_period, const int& slow_ema_period, const int& signal_period, const int& applied_price, const int& mode, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int, const int, const int, const int, const int>()(CmdId_iMACD, resultString.c_str(), timeframe, fast_ema_period, slow_ema_period, signal_period, applied_price, mode, shift);
		}

		double iOBV(const char *symbol, const int& timeframe, const int& applied_price, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int>()(CmdId_iOBV, resultString.c_str(), timeframe, applied_price, shift);
		}

		double iRSI(const char *symbol, const int& timeframe, const int& period, const int& applied_price, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int, const int>()(CmdId_iRSI, resultString.c_str(), timeframe, period, applied_price, shift);
		}

		double iRVI(const char *symbol, const int& timeframe, const int& mode, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int>()(CmdId_iRVI, resultString.c_str(), timeframe, mode, shift);
		}

		double iSAR(const char *symbol, const int & timeframe, const double & sar_step, const double & maximum, const int & shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const double, const double, const int>()(CmdId_iSAR, resultString.c_str(), timeframe, sar_step, maximum, shift);
		}

		double iStdDev(const char *symbol, const int& timeframe, const int& ma_period, const int& ma_shift, const int& ma_method, const int& applied_price, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int, const int, const int, const int>()(CmdId_iStdDev, resultString.c_str(), timeframe, ma_period, ma_shift, ma_method, applied_price, shift);
		}

		double iStochastic(const char *symbol, const int & timeframe, const int & Kperiod, const int & Dperiod, const int & slowing, const int & method, const int & price_field, const int & mode, const int & shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int, const int, const int, const int, const int, const int>()(CmdId_iStochastic, resultString.c_str(), timeframe, Kperiod, Dperiod, slowing, method, price_field, mode, shift);
		}

		double iWPR(const char *symbol, const int& timeframe, const int& period, const int& shift)
		{
			std::string resultString = symbol;
			return DispatchFunction<double, const std::string, const int, const int, const int>()(CmdId_iWPR, resultString.c_str(), timeframe, period, shift);
		}

		bool Set_Flow_Return_Value(const bool & flow_return_value)
		{
			return DispatchFunction<bool, const bool>()(CmdId_Set_Flow_Return_Value, flow_return_value);
		}

		PyMT4::Error GetLastError()
		{
			boost::mutex::scoped_lock errorMapLock(_lastErrorMapLock);
			LastErrorMap::const_iterator errorMapItem = _lastErrorMap.find(boost::this_thread::get_id());
			
			if (errorMapItem == _lastErrorMap.end())
				return ERR_NO_ERROR;
			
			return (PyMT4::Error)errorMapItem->second;
		}

	}
}