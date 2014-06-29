//
// CKCondition.cs
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
	/// Base class for condition. 
	/// Condition wrap boolean variable behaviour into an object.
	/// Useful for decision making system or any other conditions
	/// driven system.
	/// The value is only evaluated when the instance is either cast
	/// to a boolean, assigned to a boolean or get via the property
	/// ICondition.value. 
	/// IConditions can be modified/stacked via classic boolean operator 
	/// Not(!), And(&&) and Or(||).
	/// During the evaluation the ICondition will go through evaluation,
	/// of all linked IConditions.
	/// Subclasses need to override ICondition.Evaluate().
	/// Note that IConditions must be always reusable and keep in my that
	/// they're designed to seperate the construction and the evaluation
	/// of (dynamic) objects/data relations.
	/// </summary>
	public abstract class ICondition
	{
		#region Property

		/// <summary>
		/// Gets the value of the ICondition instance.
		/// </summary>
		/// <value><c>true</c> if value; otherwise, <c>false</c>.</value>
		public bool value
		{ 
			get
			{
				return this.Evaluate();
			}
		}

		#endregion

		#region Overridable Interface

		/// <summary>
		/// Method to override when creating a custom ICondition.
		/// This method must return the value of the condition.
		/// No assumptions on the usage of Evaluate should be made
		/// in order to ensure the most flexible usage.
		/// Mainly Evaluate is called when the value of the condition is 
		/// retrieved (either by the getter ICondtion.value, by a cast to a
		/// boolean or by assigment to a bool).
		/// </summary>
		protected abstract bool Evaluate();

		#endregion

		#region Operator Overloading

		public static ICondition operator !(ICondition a)
		{
			return new ConditionNOT(a);
		}

		public static ICondition operator &(ICondition a, ICondition b)
		{
			return new ConditionBinaryExpression(a,b,ConditionBinaryExpression.Operator.AND);
		}

		public static ICondition operator |(ICondition a, ICondition b)
		{
			return new ConditionBinaryExpression(a,b,ConditionBinaryExpression.Operator.OR);
		}

		public static bool operator true(ICondition a)
		{
			CK.Log.Info("Use true");
			return false;
		}

		public static bool operator false(ICondition b)
		{
			CK.Log.Info("Use false");
			return false;
		}

		#endregion
	}

	/// <summary>
	/// Inverse the value of an IConditon operand
	/// </summary>
	public class ConditionNOT : ICondition
	{
		#region Private Members

		/// <summary>
		/// The ICondition on which apply the NOT.
		/// </summary>
		private ICondition mOperand;

		#endregion
		
		#region Constructor
		
		/// <summary>
		/// Default constructor. 
		/// Operand can not be modified after construction.
		/// </summary>
		/// <param name="a">Operand.</param>
		public ConditionNOT(ICondition a)
		{
			mOperand = a;
		}
		
		#endregion

		#region ICondition override

		/// <summary>
		/// Override of ICondition.Evaluate()
		/// </summary>
		protected override bool Evaluate()
		{
			return !mOperand.value;
		}

		#endregion
	}

	/// <summary>
	/// Binary relationship between to ICondition.
	/// These are created when using & (&&) or | (||)
	/// operators on ICondition.
	/// The ConditionBinaryCompound keep a reference to
	/// the operand. In this way the expression creation
	/// and evaluation can be split in times. This also 
	/// allows recurent evaluation.
	/// Both operand are evaluated ever since the value of the first operand could
	/// give a result. This allow user implementing its own conditions to be sure that
	/// the evaluation code is processed every evaluation.
	/// </summary>
	public class ConditionBinaryExpression : ICondition
	{
		/// <summary>
		/// Operator for BinaryExpression
		/// </summary>
		public enum Operator
		{
			AND,
			OR
		}

		#region Private Members

		/// <summary>
		/// First operand of binary operator mOperator
		/// </summary>
		private ICondition mOperandA;

		/// <summary>
		/// Second operand of binary operator mOperator
		/// </summary>
		private ICondition mOperandB;

		/// <summary>
		/// Expression's operator.
		/// </summary>
		private Operator mOperator;

		#endregion

		#region Constructors

		/// <summary>
		/// Default constructor. Operands can not be modified after construction,
		/// nor the operator.
		/// Note that, since operator AND and OR are commutative and since 
		/// the evaluation of both operand is performed when the value is retrieved,
		/// the order of operand doesn't matter. 
		/// </summary>
		/// <param name="a">First operand.</param>
		/// <param name="b">Second operand.</param>
		/// <param name="op">Operator.</param>
		public ConditionBinaryExpression(ICondition a, ICondition b, Operator op)
		{
			mOperandA = a;
			mOperandB = b;

			mOperator = op;
		}

		#endregion

		#region ICondition override

		/// <summary>
		/// Override ICondition.Evaluate() to perform operation.
		/// Both operand are evaluated ever since the value of the first operand could
		/// give a result. This allow user implementing its own conditions to be sure that
		/// the evaluation code is processed every evaluation.
		/// </summary>
		protected override bool Evaluate()
		{
			switch(mOperator)
			{
			case Operator.AND:
				return mOperandA.value & mOperandB.value;
			
			case Operator.OR:
				return mOperandA.value | mOperandB.value;

			default:
				CK.Log.Error("Unhandled operator "+mOperator.ToString()+
				             " in CK.ConditionBinaryExpression", 
				             null,LogContextInfo.ENGINE);
				break;
			}

			return false;
		}

		#endregion
	}
}

