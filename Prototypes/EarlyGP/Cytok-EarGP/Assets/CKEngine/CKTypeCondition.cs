//
// CKTypeCondition.cs
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

namespace CK
{
	namespace TypeCondition
	{

		/// <summary>
		/// Subclass of ICondition speciliazed 
		/// for System.Type related conditions.
		/// </summary>
		public abstract class ITypeCondition : ICondition
		{
			#region Property

			/// <summary>
			/// Type to check.
			/// </summary>
			public System.Type type;

			#endregion
		}

		/// <summary>
		/// Subclass of ITypeCondition that checks inheritance of types
		/// </summary>
		public class Inherits : ITypeCondition
		{
			#region Protected Members
			
			/// <summary>
			/// Parent Type to check
			/// </summary>
			protected System.Type mParentType;

			#endregion
			
			#region Constructor
			
			/// <summary>
			/// Constructor that takes the type upon which 
			/// the condition acts.
			/// </summary>
			/// <param name="type">Type.</param>
			public Inherits(System.Type ptype)
			{
				mParentType = ptype;
			}
			
			#endregion

			protected override bool Evaluate()
			{
				if(type == null)
					throw new System.NullReferenceException();

				return type.IsAssignableFrom(mParentType);
			}
		}

		/// <summary>
		/// Subclass of ITypeCondition that checks equality of types
		/// </summary>
		public class Equals : ITypeCondition
		{
			#region Protected Members
			
			/// <summary>
			/// Parent Type to check
			/// </summary>
			protected System.Type mOtherType;
			
			#endregion
			
			#region Constructor
			
			/// <summary>
			/// Constructor that takes the type upon which 
			/// the condition acts.
			/// </summary>
			/// <param name="type">Type.</param>
			public Equals(System.Type ptype)
			{
				mOtherType = ptype;
			}
			
			#endregion
			
			protected override bool Evaluate()
			{
				if(type == null)
					throw new System.NullReferenceException();

				return type.Equals(mOtherType);
			}
		}

		/// <summary>
		/// Subclass of ITypeCondition that checks if type is abstract
		/// </summary>
		public class IsAbstract : ITypeCondition
		{
			protected override bool Evaluate()
			{
				if(type == null)
					throw new System.NullReferenceException();
				
				return type.IsAbstract;
			}
		}

		/// <summary>
		/// Subclass of ITypeCondition that checks if type is an interface
		/// </summary>
		public class IsInterface : ITypeCondition
		{
			protected override bool Evaluate()
			{
				if(type == null)
					throw new System.NullReferenceException();
				
				return type.IsInterface;
			}
		}

		/// <summary>
		/// Subclass of ITypeCondition that checks if type is enum
		/// </summary>
		public class IsEnum : ITypeCondition
		{
			protected override bool Evaluate()
			{
				if(type == null)
					throw new System.NullReferenceException();

				return type.IsEnum;
			}
		}

		/// <summary>
		/// Subclass of ITypeCondition that checks if type is serializable
		/// </summary>
		public class IsSerializable : ITypeCondition
		{
			protected override bool Evaluate()
			{
				if(type == null)
					throw new System.NullReferenceException();
				
				return type.IsSerializable;
			}
		}

		/// <summary>
		/// Subclass of ITypeCondition that checks if type implement an interface
		/// </summary>
		public class ImplementsInterface : ITypeCondition
		{
			#region Protected Members
			
			/// <summary>
			/// Interface name to implement
			/// </summary>
			protected string mInterface;
			
			#endregion
			
			#region Constructor
			
			/// <summary>
			/// Constructor that takes the type upon which 
			/// the condition acts.
			/// </summary>
			/// <param name="type">Type.</param>
			public ImplementsInterface(string pInterface)
			{
				mInterface = pInterface;
			}
			
			#endregion
			
			protected override bool Evaluate()
			{
				if(type == null)
					throw new System.NullReferenceException();

				return (type.GetInterface(mInterface,true) != null);
			}
		}

		/// <summary>
		/// Subclass of ITypeCondition that checks if type implements a specific method.
		/// </summary>
		public class HasMethod : ITypeCondition
		{
			#region Protected Members
			
			/// <summary>
			/// Method name to check
			/// </summary>
			protected string mMethod;
			protected System.Type[] mArgs;
			protected System.Reflection.BindingFlags mFlags;
			
			#endregion
			
			#region Constructor
			
			/// <summary>
			/// Constructor for checking method via arguments signature.
			/// </summary>
			/// <param name="type">Type.</param>
			public HasMethod(string pMethod, params System.Type[] pArgs)
			{
				mMethod = pMethod;
				mArgs = pArgs;
				mFlags = System.Reflection.BindingFlags.Default;
			}

			/// <summary>
			/// Constructor for checking via binding flags.
			/// </summary>
			/// <param name="type">Type.</param>
			public HasMethod(string pMethod, System.Reflection.BindingFlags pFlags)
			{
				mMethod = pMethod;
				mArgs = null;
				mFlags = pFlags;
			}

			/// <summary>
			/// Constructor basic for default checking.
			/// </summary>
			/// <param name="type">Type.</param>
			public HasMethod(string pMethod)
			{
				mMethod = pMethod;
				mArgs = null;
				mFlags = System.Reflection.BindingFlags.Default;
			}
			
			#endregion
			
			protected override bool Evaluate()
			{
				if(type == null)
					throw new System.NullReferenceException();

				if(mArgs != null)
					return (type.GetMethod(mMethod,mArgs) != null);
				else
					return (type.GetMethod(mMethod,mFlags) != null);
			}
		}
	
	}

}