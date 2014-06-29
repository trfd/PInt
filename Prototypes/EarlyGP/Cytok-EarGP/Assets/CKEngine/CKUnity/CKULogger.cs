//
// CKULogger.cs
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
	namespace Unity
	{
		/// <summary>
		/// Unity Debug Logger redirects CK.Log to UnityEngine.Debug.Log.
		/// </summary>
		public class UnityDebugLogger : ILogger
		{
			/// <summary>
			/// Write the message to Unity's console.
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
				// Create a string of the form : "[ContextInfo] Message".
				// Example: "[GAMEPLAY] Player died"
				string ctxInfoStr = "";
				
				if(ctxInfo != LogContextInfo.NONE)
					ctxInfoStr = "["+ctxInfo.ToString()+"]";


				//Check if context object is a unity object
				UnityEngine.Object objCtx = null;

				string actualMsg = msg;
				if(ctx != null)
				{
					objCtx = ctx as UnityEngine.Object;

					// Context Object ctx is not a UnityEngine.Object
					// Then add its description to the message.
					if(objCtx == null)
						actualMsg += "{ "+ctx.ToString()+" }";
				}
			
				switch(lType)
				{
				case LogType.INFO:
					Debug.Log(ctxInfoStr+actualMsg,objCtx);
					break;
				
				case LogType.WARNING:
					Debug.LogWarning(ctxInfoStr+actualMsg,objCtx);
					break;
				
				case LogType.ASSERT:
				case LogType.ERROR:
					Debug.LogError(ctxInfoStr+actualMsg,objCtx);
					break;

				// If log type not handled display warning message of the form:
				// [LOGTYPE][CONTEXT_INFO] Message
				default:
					Debug.LogWarning("["+lType+"]"+ctxInfoStr+actualMsg,objCtx);
					break;
				}


			}
			
			/// <summary>
			/// Write the exception to Unity's console
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
				Debug.LogException(excpt);
			}
		}
	}
}