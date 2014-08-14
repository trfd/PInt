///
///  main.cpp
///  for unit testing
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

#include <iostream>

#define PERMANENT_PROMPT

#include "ai/bt/BTBehaviourTree.hpp"
#include "ai/bt/BTBehaviourTreeBuilder.hpp"

#include "ai/bt/BTFilter.hpp"

using namespace ai::bt;

class A
{
public:
    virtual ~A(){}
};

class B
    : public A
{
    int a;
};

class DebugBehaviour : public Behaviour
{
    BehaviourState debugState = BehaviourState::RUNNING;
protected:
    virtual void initialize() override
    {
        std::cout << "Initialize \n";
    }

    virtual void terminate() override
    {
        std::cout << "Terminate \n";
    }

    virtual BehaviourState doStep() override
    {
        std::cout << "Step \n";
        return debugState;
    }
};

int main(void)
{
    A* b = new B();

	std::cout << "Start Unit Testing: \n";

    ai::bt::BehaviourTree bt =
        BehaviourTreeBuilder(new DebugBehaviour()).end();

    bt.step();

#ifdef PERMANENT_PROMPT
	while (true)
	{}
#endif

	return 0;

}
	
