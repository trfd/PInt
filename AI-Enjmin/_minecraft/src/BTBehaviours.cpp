///
///  BTBehaviours.cpp
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

#include <iostream>

#include "ai/bt/BTBehaviour.hpp"
#include "ai/bt/BTCondition.hpp"
#include "ai/bt/BTDecorator.hpp"
#include "ai/bt/BTSequence.hpp"
#include "ai/bt/BTSelector.hpp"
#include "ai/bt/BTFilter.hpp"
#include "ai/bt/BTParallel.hpp"
#include "ai/bt/BTMonitor.hpp"

namespace ai
{
	namespace bt
	{
		#pragma region Behaviour

		Behaviour::Behaviour()
		: m_state(BehaviourState::INVALID)
		{}

        Behaviour::~Behaviour()
        {}

        inline BehaviourState
        Behaviour::state() const
        {
            return m_state;
        }

        inline bool
        Behaviour::isTerminated() const
        {
            return (m_state != BehaviourState::RUNNING);
        }

		BehaviourState
		Behaviour::step()
		{
            std::cout << typeid(*this).name() << "\n";

			if(m_state == BehaviourState::INVALID)
				initialize();

			//m_state = doStep();

			if(m_state != BehaviourState::RUNNING)
				terminate();

			return m_state;
		}

		#pragma endregion

		#pragma region Condition

		Condition::Condition(const ConditionFunctor& cond_)
		: m_func(cond_)
		{}

		BehaviourState 
		Condition::doStep()
		{
			return (m_func() ? 
                    BehaviourState::SUCCESS :
					BehaviourState::FAILURE );
		}

		#pragma endregion

		#pragma region Decorator

		Decorator::Decorator(Behaviour* child_)
		: m_child(child_)
		{}

		#pragma endregion

        #pragma region Sequence

        BehaviourState Sequence::doStep()
        {
            BehaviourState state;

            for(Behaviour* b_ptr : this->children())
            {
                if((state = b_ptr->step()) != BehaviourState::SUCCESS)
                    return state;
            }

            return BehaviourState::SUCCESS;
        }

        #pragma endregion

        #pragma region Selector

        BehaviourState Selector::doStep()
        {
            BehaviourState state;

            for(Behaviour* b_ptr : this->children())
            {
                if((state = b_ptr->step()) != BehaviourState::FAILURE)
                    return state;
            }
            
            return BehaviourState::FAILURE;
        }

        #pragma endregion

        #pragma region Filter

        inline void Filter::addCondition(Behaviour* b_)
        {
            // equiv. to push_front
            this->insertChild(this->children().begin(),b_); 
        }

        inline void Filter::addAction(Behaviour* b_)
        {
            // equiv. to push_back
            this->addChild(b_);
        }

        #pragma endregion

        #pragma region Parallel

        BehaviourState Parallel::doStep()
        {
            BehaviourState state;
            
            int succCount = 0;
            int failCount = 0;

            for(Behaviour* b_ptr : this->children())
            {
                if(!b_ptr->isTerminated())
                    b_ptr->step();

                if(b_ptr->state() == BehaviourState::SUCCESS)
                {
                    succCount++;

                    if(_successPolicy == Policy::REQUIRE_ONE)
                        return BehaviourState::SUCCESS;
                }
                else if(b_ptr->state() == BehaviourState::FAILURE)
                {
                    failCount++;

                    if(_failurePolicy == Policy::REQUIRE_ONE)
                        return BehaviourState::FAILURE;
                }
            }

            if(succCount ==  this->childrenCount() && 
               _successPolicy == Policy::REQUIRE_ALL)
               return BehaviourState::SUCCESS;

            if(failCount == this->childrenCount() &&
               _failurePolicy == Policy::REQUIRE_ALL)
               return BehaviourState::FAILURE;

            return BehaviourState::RUNNING;

        }

        #pragma endregion

        #pragma region Monitor

        inline void Monitor::addCondition(Behaviour* b_)
        {
            // equiv. to push_front
            this->insertChild(this->children().begin(), b_);
        }

        inline void Monitor::addAction(Behaviour* b_)
        {
            // equiv. to push_back
            this->addChild(b_);
        }

        #pragma endregion
	}
}