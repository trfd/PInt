﻿//
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

namespace CK
{
	namespace Unity
	{
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
			Dictionary<string,int> mHashes;

			#endregion

			#region Constructor



			#endregion

		}
	}
}
