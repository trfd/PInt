//
// CKUAnimatorBinding.cs
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
using System.Collections.Generic;
using System;

namespace CK
{
	namespace Unity
	{
		/// <summary>
		/// Coupe state's name - state type for
		/// </summary>
		[System.Serializable]
		public class AnimatorBindingPair
		{
			/// <summary>
			/// The complete name of the animator state
			/// including the layer name. 
			/// Usually looks like this: "Base Layer.MyState"
			/// </summary>
			public string stateName;

			/// <summary>
			/// The type of the state that derives from CK.State
			/// that need to be bind to.
			/// An instance of this type will be created at runtime.
			/// </summary>
			//[ExposeType(typeof(CK.State))]
			public Utils.TypeWrapper stateType;
		}

		/// <summary>
		/// Animator binding binds a state-context to an existing
		/// unity finite state-machine.
		/// This allow to bind easily code (from state's implementation) 
		/// to animation state.
		/// 
		/// Note that the AnimatorBinding system should not be use together
		/// with an AnimatorWrapper on the same Animator instance.
		/// </summary>
		public class AnimatorBinding : StateContext 
		{
			#region Private Members

			/// <summary>
			/// Holds the bindings between animator states' hashes
			/// and the CK.State instances
			/// </summary>
			Dictionary<int,State> mStateMap;

			/// <summary>
			/// Map between states' string names and hash names
			/// </summary>
			Dictionary<int,string> mHashName;

			#endregion

			#region Constructor

			public AnimatorBinding(AnimatorBindingPair[] bindings)
			{
				// Allocations

				mStateMap = new Dictionary<int, State>();
				mHashName   = new Dictionary<int, string>();

				// Fill maps from binding pairs

				int hash;

				for(int i=0 ; i<bindings.Length ; i++)
				{
					hash = Animator.StringToHash(bindings[i].stateName);

					mStateMap.Add(hash, (CK.State) System.Activator.CreateInstance(bindings[i].stateType.type));
					mHashName.Add(hash,bindings[i].stateName);
				}
			}

			#endregion

		}
	}
}
