///
///  BTBehaviour.hpp
///
///  Created by Baptiste Dupy on 10/08/2014.
///  Contact:
///          Mail:       <baptiste.dupy@gmail.com>
///          GitHub:     trfd <https://github.com/trfd>
///          Website:    <http://www.trf-d.com>
///
///  Copyright (C) 2014 Baptiste (trfd) Dupy
///
///  This program is free software: you can redistribute it and/or modify
///  it under the terms of the GNU General Public License as published by
///  the Free Software Foundation, either version 3 of the License, or
///  (at your option) any later version.
///
///  This program is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///  GNU General Public License for more details.
///
///  You should have received a copy of the GNU General Public License
///  along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
///
///

#ifndef AI_BT_Behaviour_hpp
#define AI_BT_Behaviour_hpp

#include "BTBehaviourState.hpp"

namespace ai
{
	namespace bt
	{
		#pragma region Behaviour

		/// <summary> 
		///
		/// </summary>
		class Behaviour
		{
		public:
			
			#pragma region Constructor/Destructor

			/// <summary>
			/// Default Constructor.
			/// <summary>
			/// Initialize state to BehaviourState::INVALID
			Behaviour();

			virtual ~Behaviour();

			#pragma endregion

            #pragma region Accessors

            inline BehaviourState state() const;
            
            inline bool isTerminated() const;

            #pragma endregion

			#pragma region BehaviourTree Interface

			BehaviourState step();

			#pragma endregion
		
		protected:
		
			#pragma region Override Interface

			/// <summary>
			/// Called before initialization
			/// </summary>
			virtual void initialize(){}

			/// <summary>
			/// Called after a step success or failure
			/// </summary>
			virtual void terminate(){} 

			/// <summary>
			/// Overridable step. Called during Behaviour::step
			/// </summary>
			virtual BehaviourState doStep() = 0;

			#pragma endregion

		private:

			BehaviourState m_state;

		};

		#pragma endregion
	}
}

#endif //AI_BT_Behaviour_hpp
