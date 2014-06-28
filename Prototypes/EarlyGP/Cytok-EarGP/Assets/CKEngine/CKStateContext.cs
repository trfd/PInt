//
// CKStateContext.cs
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
	/// Context for state
	/// </summary>
	public class StateContext
	{
		#region Delegates

		public delegate void StateContextDelegate(StateContext ctx, State state);

		/// <summary>
		/// Delegate called whenever the current state is about to start
		/// </summary>
		public StateContextDelegate StateWillStart;
		
		/// <summary>
		/// Delegate called just after the current state has started
		/// </summary>
		public StateContextDelegate StateDidStart;
		
		/// <summary>
		/// Delegate called whenever the current state is about to stop
		/// </summary>
		public StateContextDelegate StateWillStop;
		
		/// <summary>
		/// Delegate called just after the current state has stop
		/// </summary>
		public StateContextDelegate StateDidStop;

		#endregion

		#region Private Members

		/// <summary>
		/// Holds the current state of context
		/// </summary>
		private State mCurrState;

		#endregion

		#region Public Interface
	
		/// <summary>
		/// Start the state
		/// </summary>
		/// <param name="newState">New state.</param>
		public virtual void Start(State newState)
		{
			// End previous state
			if(mCurrState != null)
			{
				mCurrState.Stop();

					// Unregister for current state callbacks
				UnregisterStateCallbacks();
			}

			// Start the new state

			mCurrState = newState;

				// Register for current state
			RegisterStateCallbacks();

			mCurrState.Start(this);
		}

		#endregion

		#region Private Methods

		/// <summary>
		/// Registers for delegate calls of current state
		/// </summary>
		void RegisterStateCallbacks()
		{
			mCurrState.StateWillStart += CurrentStateWillStart;
			mCurrState.StateDidStart  += CurrentStateDidStart;
			mCurrState.StateWillStop  += CurrentStateWillStop;
			mCurrState.StateDidStop   += CurrentStateDidStop;
		}

		/// <summary>
		/// Unregisters for delegate calls of current state
		/// </summary>
		void UnregisterStateCallbacks()
		{
			mCurrState.StateWillStart += CurrentStateWillStart;
			mCurrState.StateDidStart  += CurrentStateDidStart;
			mCurrState.StateWillStop  += CurrentStateWillStop;
			mCurrState.StateDidStop   += CurrentStateDidStop;
		}

		#region State Callbacks

		/// <summary>
		/// Delegate for current state's StateWillStart 
		/// </summary>
		void CurrentStateWillStart(State state)
		{
			// Check if state is current state:
			if(mCurrState != state)
				return;

			// Route callback to my delegate
			if(StateWillStart != null)
				StateWillStart(this,state);
		}

		/// <summary>
		/// Delegate for current state's StateDidStart 
		/// </summary>
		void CurrentStateDidStart(State state)
		{
			// Check if state is current state:
			if(mCurrState != state)
				return;
			
			// Route callback to my delegate
			if(StateDidStart != null)
				StateDidStart(this,state);
		}

		/// <summary>
		/// Delegate for current state's StateWillStop 
		/// </summary>
		void CurrentStateWillStop(State state)
		{
			// Check if state is current state:
			if(mCurrState != state)
				return;
			
			// Route callback to my delegate
			if(StateWillStop != null)
				StateWillStop(this,state);
		}

		/// <summary>
		/// Delegate for current state's StateDidStop 
		/// </summary>
		void CurrentStateDidStop(State state)
		{
			// Check if state is current state:
			if(mCurrState != state)
				return;
			
			// Route callback to my delegate
			if(StateDidStop != null)
				StateDidStop(this,state);
		}

		#endregion

		#endregion
	}
}