//
// AnimatorBindingPairDrawer.cs
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

#if UNITY_EDITOR

using UnityEngine;
using UnityEditor;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Reflection;

namespace CK
{
	namespace Unity
	{
		/// <summary>
		/// CustomPropertyDrawer allow display of Animator binding pairs
		/// </summary>
		[CustomPropertyDrawer(typeof(CK.Utils.TypeWrapper))]
		public class TypeWrapperDrawer : PropertyDrawer
		{
			public override float GetPropertyHeight(SerializedProperty property, GUIContent label)
			{
				return Editor.Utils.defaultTextHeight * 2f;
			}

			public override void OnGUI(Rect position,SerializedProperty property , GUIContent content)
			{
				SerializedProperty nameProp = property.FindPropertyRelative("myName");
				SerializedProperty dirtyProp = property.FindPropertyRelative("myTypeDirty");

				if(nameProp == null || dirtyProp == null)
					throw new System.NullReferenceException();

				// Get the actual type wrapped
				string currTypeName = nameProp.stringValue;

				// Set the display rectangle
				Rect textFieldRect = position;
				textFieldRect.height = Editor.Utils.defaultTextHeight;

				Rect helpRect = position;
				helpRect.height = Editor.Utils.defaultTextHeight;
				helpRect.y += Editor.Utils.defaultTextHeight;

				EditorGUI.BeginChangeCheck();

				// Update the wrapper's type
				nameProp.stringValue = EditorGUI.TextField(textFieldRect,"Type",currTypeName);

				EditorGUI.EndChangeCheck();

				// Check if the type is valid
				// Otherwise reset to InvalidType
				if(!CK.Utils.TypeWrapper.Exists(nameProp.stringValue))
				{
					nameProp.stringValue = typeof(CK.Utils.InvalidType).Name;
					EditorGUI.HelpBox(helpRect,"Type not found",MessageType.Error);
				}
				else
					EditorGUI.HelpBox(helpRect,CK.Utils.TypeWrapper.FindType(nameProp.stringValue).FullName,
					                  MessageType.Info);

				// Set type to dirty 
				dirtyProp.boolValue = true;
			}
		
		}

	}
}

#endif