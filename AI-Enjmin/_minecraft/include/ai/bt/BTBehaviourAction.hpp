///
///  BTBehaviourAction.hpp
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

#ifndef AI_BT_BehaviourAction_hpp
#define AI_BT_BehaviourAction_hpp

#include "../CKFunctor.hpp"
#include "BTBehaviour.hpp"

namespace ai
{
	namespace bt
	{
		#pragma region BehaviourAction

        struct IBehaviourAction
        {
            /// Called at each bt::Action 
            /// behaviour's steps
            virtual void run() = 0;

            /// Convenience interface
            /// This methods are not called by 
            /// the behaviour tree itself since
            /// action management can differt a lot.
            /// In this way, action manager
            /// can store IBehaviourAction pointer
            /// without regards of _Holder type
            /// and manage start/stop actions

            virtual void onStart(){}
            virtual void onTerminate(){}
        };

        template<typename _Holder>
		struct BehaviourAction : public IBehaviourAction
		{
            _Holder* holder = nullptr;
		};

		#pragma endregion
	}
}

#endif //AI_BT_BehaviourAction_hpp