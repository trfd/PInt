//
// WorldGrid.cs
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
using UnityEditor;
using System.Collections;

[CustomEditor(typeof(WorldGrid))]
public class WorldGridEditor : Editor 
{
	GridIndex m_selectIdx;

	Vector2 currIdx;

	GridCell.Wall currWallType;

	public override void OnInspectorGUI()
	{
		base.OnInspectorGUI();

		CK.Editor.Utility.DrawMethodGUIButton(target);

		EditorGUILayout.Space();

		EditorGUILayout.BeginHorizontal();

		currIdx = EditorGUILayout.Vector2Field ("Index",currIdx);

		EditorGUILayout.EndHorizontal();

		currWallType = (GridCell.Wall) EditorGUILayout.EnumMaskField("Wall",currWallType);

		if((int)currWallType == -1)
		{
			currWallType = 
				GridCell.Wall.TOP | GridCell.Wall.BOTTOM | GridCell.Wall.DIAG_DOWN | 
				GridCell.Wall.DIAG_UP | GridCell.Wall.LEFT | GridCell.Wall.RIGHT ;		          
		}

		if(GUILayout.Button("Add/Remove Wall"))
		{
			currIdx.Set(Mathf.Round(currIdx.x) , Mathf.Round(currIdx.y));


			
			((WorldGrid) target).SwitchWall((int)currIdx.x,(int)currIdx.y, currWallType);
		}
	}


}
