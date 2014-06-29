//
// CKUtilsTypeWrapper.cs
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

namespace CK
{
	namespace Utils
	{
		#region TypeWrapper
		/// <summary>
		/// This class is a simple wrapping adaptator
		/// of .NET's System.Type to string.
		/// In this way the type will be serialized
		/// as a basic string. 
		/// In order to allow custom drawer for Unity and
		/// restriction on the possible type value it can
		/// get, the wrapper can not be a simple extension 
		/// of System.string
		/// </summary>
		[System.Serializable]
		public class TypeWrapper 
		{
			#region Private Members
			/// <summary>
			/// String name of the type.
			/// </summary>
			private string mName;

			/// <summary>
			/// Actual Type
			/// </summary>
			private System.Type mType;

			#endregion

			#region Properties

			/// <summary>
			/// Gets or sets the name of the type.
			/// When the setter is used, TypeWrapper will immediatly
			/// match the new string value to an existing type. 
			/// If it fails, the value will be set to System.Object
			/// and an Error will be logged with the handled Exception message.
			/// </summary>
			/// <value>The name of the type.</value>
			public string typeName
			{ 
				get
				{ 
					return mName; 
				}

				set
				{ 
					try
					{
						mType = System.Type.GetType(value);
						mName = value;
					}
					catch(System.Exception excpt)
					{
						CK.Log.Error("TypeWrapper string name "+value+" doesn't correspond to any type. " +
						             "Values will be set to default ones.Exception: "+excpt.Message,
						             null,LogContextInfo.ENGINE);

						// Reset value to default
						mName = "System.Object";
						mType = typeof(System.Object);
					}
				}
			}

			/// <summary>
			/// Gets or sets the actual type of TypeWrapper instance.
			/// This is the safest way of setting both name and type values,
			/// since setting through TypeWrapper.typeName can fail.
			/// </summary>
			/// <value>Type value</value>
			public System.Type type
			{
				get
				{
					return mType;
				}

				set
				{
					if(value != null)
						mType = value;
					else
						mType = typeof(System.Object);
					
					mName = mType.Name;
				}
			}

			#endregion

			#region Constructors

			public TypeWrapper()
			{
				// Setting to default values
				mType = typeof(System.Object);
				mName = mType.Name;
			}

			public TypeWrapper(System.Type pType)
			{
				// Using property
				type = pType;
			}

			public TypeWrapper(string pTypeName)
			{
				// Using property
				typeName = pTypeName;
			}
		
			#endregion
		}

		#endregion

		#region TypeWrapperRestrictionAttribute



		[AttributeUsage(AttributeTargets.Field)]
		public class TypeWrapperRestrictionAttribute : Attribute
		{

		}

		#endregion
	}
}
