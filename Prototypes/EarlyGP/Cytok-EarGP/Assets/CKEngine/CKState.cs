//
// CKState.cs
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
	/// <summary>
	/// General purpose State class.
	/// States are managed by CK.StateContext
	/// </summary>
	public class State  
	{
		#region Delegates

		public delegate void StateDelegate(State state);

		/// <summary>
		/// Delegate called whenever the state is about to start
		/// </summary>
		public StateDelegate StateWillStart;

		/// <summary>
		/// Delegate called just after the state has started
		/// </summary>
		public StateDelegate StateDidStart;

		/// <summary>
		/// Delegate called whenever the state is about to stop
		/// </summary>
		public StateDelegate StateWillStop;

		/// <summary>
		/// Delegate called just after the state has stop
		/// </summary>
		public StateDelegate StateDidStop;


		#endregion

		#region Private Members

		/// <summary>
		/// Parent Context
		/// </summary>
		private StateContext myContext;

		/// <summary>
		/// Hold active flag of state
		/// </summary>
		private bool myActive;

		#endregion

		#region Properties

		/// <summary>
		/// Parent context of the state
		/// </summary>
		public StateContext context
		{get{ return myContext;}}

		/// <summary>
		/// Gets a value indicating whether this <see cref="CK.State"/> is active.
		/// </summary>
		/// <value><c>true</c> if is active; otherwise, <c>false</c>.</value>
		public bool isActive
		{get{return myActive;}}

		#endregion

		#region Flow Control Interface

		/// <summary>
		/// Start the state. 
		/// This method is automatically called
		/// by the parent state-context
		/// </summary>
		/// <param name="ctx">Parent StateContext</param>
		public void Start(StateContext ctx)
		{
			myContext = ctx;
			myActive = true;

			OnEnter();

			// Call Delegate
			if(StateDidStart != null)
				StateDidStart(this);
		}

		/// <summary>
		/// Stop the state. 
		/// This method is automatically called
		/// by the parent state-context. 
		/// Note that the state keeps holding a 
		/// reference to the parent state-context,
		/// to allow pause-restart implementations.
		/// </summary>
		public void Stop()
		{
			myActive = false;

			OnExit();

			// Call Delegate
			if(StateDidStop != null)
				StateDidStop(this);
		}

		#endregion

		#region Overridable Interface

		public virtual void OnEnter(){}
		public virtual void Update(float dt){}
		public virtual void OnExit(){}

		#endregion
	}
}