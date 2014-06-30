//
// CKUtilsTypeRestrictionAttribute.cs
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

using CK.TypeCondition;

namespace CK
{
	namespace Utils
	{
		/// <summary>
		/// Add a restriction on the value a parameter System.Type
		/// can have. 
		/// </summary>
		/// Note that the class doesn't apply or check the restriction.
		/// It only computes the list of possible types from the list of
		/// all available type in the assembly.
		[AttributeUsage(AttributeTargets.All)]
		public class TypeRestrictionAttribute : Attribute 
		{
			#region Private Members

			/// <summary>
			/// The restriction to apply
			/// </summary>
			private ITypeCondition myRestriction;

			#endregion

			#region Protected Members

			/// <summary>
			/// List of all possible types.
			/// </summary>
			protected Type[] mTypeList;

			
			/// <summary>
			/// List of all possible types.
			/// </summary>
			protected string[] mTypeNameList;

			#endregion

			#region Constructor

			/// <summary>
			/// Default constructor.
			/// </summary>
			/// <param name="restrict">Restriction.</param>
			public TypeRestrictionAttribute(ITypeCondition restrict)
			{
				myRestriction = restrict;

				CreateTypeList();
			}

			#endregion

			#region Protected Methods

			/// <summary>
			/// Create the list of possible type based upon 
			/// the restriction object.
			/// </summary>
			protected void CreateTypeList()
			{
				// Temp. list 
				List<Type> lTypes = new List<Type>();
				List<string> lNames = new List<string>();

				// Get all loaded assemblies
				Assembly[] asms = AppDomain.CurrentDomain.GetAssemblies();

				Type[] asmTypes;

				for(int i=0 ; i<asms.Length ; i++)
				{
					// Get all types in assembly
					asmTypes = asms[i].GetTypes();

					for(int j=0 ; j<asmTypes.Length ; i++)
					{
						//Change the restriction's type to check
						myRestriction.type = asmTypes[j];

						// Evaluate restriction
						if(myRestriction.value)
						{
							lTypes.Add(asmTypes[j]);
							lNames.Add(asmTypes[j].Name);
						}
					}
				}

				// All work done, save arrays
				mTypeList = lTypes.ToArray();
				mTypeNameList = lNames.ToArray();
			}

			#endregion

			#region Public Interface

			/// <summary>
			/// Returns the list of all possible types allowed by the restriction.
			/// </summary>
			/// <returns>The list of possible types.</returns>
			public Type[] GetTypes()
			{
				return mTypeList;
			}

			/// <summary>
			/// Returns the list of name of all possible types 
			/// allowed by the restriction.
			/// </summary>
			/// <returns>The list of possible type names.</returns>
			public string[] GetTypeNames()
			{
				return mTypeNameList;
			}

			/// <summary>
			/// Checks if a given type is conform to the restriction.
			/// </summary>
			/// <returns><c>true</c> if the type is conform to the restriction; otherwise, <c>false</c>.</returns>
			/// <param name="type">Type to check.</param>
			public bool IsConform(Type type)
			{
				// Check if confom to restriction

				myRestriction.type = type;

				return myRestriction.value;
			}

			#endregion
		}
	}
}
