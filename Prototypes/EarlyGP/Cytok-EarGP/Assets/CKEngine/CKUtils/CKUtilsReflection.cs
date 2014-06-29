//
// CKUtilsReflection.cs
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

using System;
using System.Reflection;
using System.Collections.Generic;

namespace CK
{
	namespace Utils
	{
		public class Reflection  
		{
			/// <summary>
			/// Returns the list of all type inheriting from a base type
			/// </summary>
			/// <returns>The list children types.</returns>
			/// <param name="parentType">Parent type.</param>
			public static System.Type[] GetListChildrenTypes(System.Type parentType)
			{
				Assembly[] asms = AppDomain.CurrentDomain.GetAssemblies();
				
				Type[] alltypes;
				
				List<Type> types = new List<Type>();
				List<string> names = new List<string>();
				
				for(int i=0 ; i<asms.Length ; i++)
				{
					alltypes = asms[i].GetTypes();
					
					for(int j = 0 ; j<alltypes.Length ; j++)
					{
						if(alltypes[j].IsAssignableFrom(parentType))
						{
							types.Add(alltypes[j]);
							names.Add(alltypes[j].Name);
						}
					}
				}
				
				return types.ToArray();
			}
		}
	}
}
