//
// CKLog.cs
//
// Author:
//       Baptiste Dupy <baptiste.dupy@gmail.com>
//
// Copyright (c) 2014 
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

using UnityEngine;
using System.Collections;

namespace CK
{
	/// <summary>
	/// Type of log, indicates gravity of report.
	/// </summary>
	public enum LogType
	{
		INFO,
		WARNING,
		ERROR,
		ASSERT,
		EXCEPTION,
		STACK
	}

	/// <summary>
	/// Context information for logs.
	/// Allow to dispatch or select/ignore reports
	/// for loggers.
	/// </summary>
	public enum LogContextInfo
	{
		NONE,
		CORE,
		ENGINE,
		PLATFORM,
		DEVICE,
		GRAPHICS,
		PHYSICS,
		TERRAIN,
		ASSET,
		NETWORK,
		AUDIO,
		GAMEPLAY,
		INPUT,
		AI,
		UI,
		LAUNCHER,
		OTHER
	}

	/// <summary>
	/// Logger interface. Logger is the subsystem
	/// that actually write something somewhere when a CK.Log method is called.
	/// The most basic Logger is the one that writes into .NET's console.
	/// Enhanced debugger can be create such as HTML or ingame console logs.
	/// To change the logger simply set the value of CK.Log.logger to an 
	/// instance of your custom logger implementation.
	/// </summary>
	public interface ILogger
	{
		/// <summary>
		/// Generic call from CK.Log for every type of logging event,
		/// except for exception (See ILogger.Exception())
		/// </summary>
		/// <param name="lType">Type of the logging event</param>
		/// <param name="msg">Message to log</param>
		/// <param name="ctx">Related context object</param>
		/// <param name="ctxInfo">Context info</param>
		/// <param name="args">Additionnal arguments</param>
		void Message(CK.LogType lType, 
		             string msg, 
		             System.Object ctx = null , 
		             LogContextInfo ctxInfo = LogContextInfo.NONE , 
		             params System.Object[] args);

		/// <summary>
		/// Call from CK.Log when exception is logged.
		/// </summary>
		/// <param name="excpt">Excption.</param>
		/// <param name="ctx">Related context object</param>
		/// <param name="ctxInfo">Context info</param>
		/// <param name="args">Additionnal arguments</param>
		void Exception(System.Exception excpt,
		               System.Object ctx = null , 
		               LogContextInfo ctxInfo = LogContextInfo.NONE , 
		               params System.Object[] args);

	}

	/// <summary>
	/// Log system for the CKEngine.
	/// </summary>
	public class Log
	{
		#region Logger

		/// <summary>
		/// Logger implementation to use.
		/// </summary>
		public static ILogger logger;

		/// <summary>
		/// Initializes the <see cref="CK.Log"/> class.
		/// By default the logger is the SystemLogger that
		/// writes into the .NET console.
		/// </summary>
		static Log()
		{
			logger = new Unity.UnityDebugLogger();
		}

		#endregion

		#region Public Interface

		/// <summary>
		/// Log an info message
		/// </summary>
		/// <param name="info">Info.</param>
		public static void Info(string info , 
		                        System.Object ctx = null , 
		                        LogContextInfo ctxInfo = LogContextInfo.NONE , 
		                        params System.Object[] args)
		{
			logger.Message(LogType.INFO,info,ctx,ctxInfo,args);
		}

		/// <summary>
		/// Log a warning
		/// </summary>
		/// <param name="warn">Warn.</param>
		public static void Warning(string warn , 
		                           System.Object ctx = null , 
		                           LogContextInfo ctxInfo = LogContextInfo.NONE , 
		                           params System.Object[] args)
		{
			logger.Message(LogType.WARNING,warn,ctx,ctxInfo,args);
		}

		/// <summary>
		/// Log an Error
		/// </summary>
		/// <param name="err">Error.</param>
		public static void Error(string err , 
		                         System.Object ctx = null , 
		                         LogContextInfo ctxInfo = LogContextInfo.NONE , 
		                         params System.Object[] args)
		{
			logger.Message(LogType.ERROR,err,ctx,ctxInfo,args);
		}

		/// <summary>
		/// Log an exception
		/// </summary>
		/// <param name="excpt">Excpt.</param>
		public static void Exception(System.Exception excpt,
		                             System.Object ctx = null , 
		                             LogContextInfo ctxInfo = LogContextInfo.NONE , 
		                             params System.Object[] args)
		{
			logger.Exception(excpt,ctx,ctxInfo,args);
		}

		#endregion
	}

	/// <summary>
	/// Default and most basic implementation
	/// of Logger using .NET's console
	/// </summary>
	public class SystemLogger : ILogger
	{
		/// <summary>
		/// Write the message to the console.
		/// </summary>
		/// <param name="lType">Type of the logging event</param>
		/// <param name="msg">Message to log</param>
		/// <param name="ctx">Related context object</param>
		/// <param name="ctxInfo">Context info</param>
		/// <param name="args">Additionnal arguments</param>
		void ILogger.Message(CK.LogType lType, 
		             		 string msg, 
		             		 System.Object ctx = null , 
		             		 LogContextInfo ctxInfo = LogContextInfo.NONE , 
		             		 params System.Object[] args)
		{
			// Create a string of the form : "[ContextInfo : LogType] Message".
			// Example: "[GAMEPLAY : INFO] Player died"
			string ctxInfoStr = "";

			if(ctxInfo != LogContextInfo.NONE)
				ctxInfoStr = ctxInfo+" :";

			System.Console.WriteLine("["+ctxInfoStr+" "+lType+"] "+msg);
		}
		
		/// <summary>
		/// Write the exception to the console.
		/// </summary>
		/// <param name="excpt">Excption.</param>
		/// <param name="ctx">Related context object</param>
		/// <param name="ctxInfo">Context info</param>
		/// <param name="args">Additionnal arguments</param>
		void ILogger.Exception(System.Exception excpt,
		               		   System.Object ctx = null , 
		               		   LogContextInfo ctxInfo = LogContextInfo.NONE , 
		               		   params System.Object[] args)
		{
			// Create a string of the form : "[ContextInfo : EXCEPTION] Message".
			// Example: "[GAMEPLAY : INFO] Player died"
			string ctxInfoStr = "";
			
			if(ctxInfo != LogContextInfo.NONE)
				ctxInfoStr = ctxInfo+" :";
			
			System.Console.WriteLine("["+ctxInfoStr+" EXCEPTION] "+excpt.Message);
		}
	}
}