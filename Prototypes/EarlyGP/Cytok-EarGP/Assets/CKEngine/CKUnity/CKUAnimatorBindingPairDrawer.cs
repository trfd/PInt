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

namespace CK
{
	namespace Unity
	{
		/*
		/// <summary>
		/// CustomPropertyDrawer allow display of Animator binding pairs
		/// </summary>
		[CustomPropertyDrawer(typeof(CK.Unity.AnimatorBindingPair))]
		public class AnimatorBindingPairDrawer : PropertyDrawer
		{
			string[] mStateTypeName;
			Dictionary<string,System.Type> mStateTypeMap;

			/// <summary>
			/// Encapsulate cast of attribute
			/// </summary>
			/// <value>The expose type attribute.</value>
			//public ExposeTypeAttribute exposeTypeAttribute
			//{ get{ return (ExposeTypeAttribute) this.attribute;}}

			public override float GetPropertyHeight(SerializedProperty property, GUIContent label)
			{
				return Editor.Utility.defaultTextHeight;
			}

			public override void OnGUI(Rect position,SerializedProperty property , GUIContent content)
			{
				if(mStateTypeName == null || mStateTypeMap == null)
					FetchStateTypes();

				SerializedProperty name = property.FindPropertyRelative ("stateName");
				SerializedProperty type = property.FindPropertyRelative ("stateType");

				/*
				mCurrSelectedIndex = EditorGUILayout.Popup(mCurrSelectedIndex, exposeTypeAttribute.exposedNames);

				// Set the value 
				fieldInfo.SetValue(property.serializedObject.targetObject, 
				                   exposeTypeAttribute.exposedTypes[mCurrSelectedIndex]);



				//EditorGUILayout.LabelField("Hello");

				// Split the total space in two for textfield and popup
				float hwidth = position.width * 0.5f;

				Rect textFieldPosition = position;
				textFieldPosition.height = Editor.Utility.defaultTextHeight;
				textFieldPosition.width = hwidth;

				Rect popupPosition = position;
				popupPosition.height = Editor.Utility.defaultTextHeight;
				popupPosition.x = hwidth;
				popupPosition.width = hwidth;

				EditorGUI.BeginChangeCheck();

				name.stringValue = EditorGUI.TextField(textFieldPosition,"State", name.stringValue);

				DrawPopup(property);



				EditorGUI.EndChangeCheck();

			}

			/// <summary>
			/// Draw the popup exposing types.
			/// </summary>
			void DrawPopup(SerializedProperty property)
			{
				// Current type of AnimatorBindingPair
				System.Type currType = fieldInfo.GetValue(property.serializedObject.targetObject);

				// int idx = 

				//mCurrSelectedIndex = EditorGUI.Popup(popupPosition,mCurrSelectedIndex,mStateTypeName);
			}

			/// <summary>
			/// Retrieve all types inheriting from CK.State in order to 
			/// expose these in the popup
			/// </summary>
			void FetchStateTypes()
			{
				System.Type[] types = CK.Utils.Reflection.GetListChildrenTypes(CK.State);
				mStateTypeName = new string[types.Length];
				mStateTypeMap = new Dictionary<string, System.Type>();

				for(int i=0 ; i< mStateTypeName.Length; i++)
				{
					mStateTypeName[i] = types[i].Name;
					mStateTypeMap.Add(mStateTypeName[i],types[i]);
				}
			}
		}
		*/
	}
}

#endif