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
using System.Collections;

[System.Serializable]
public class GridCell
{
	public enum Wall
	{
		NONE		= 1,
		TOP			= 2,
		LEFT		= 4,
		RIGHT		= 8,
		BOTTOM		= 16,
		DIAG_UP		= 32,
		DIAG_DOWN   = 64
	}

	public Wall wall;
	public int floor;
}

[System.Serializable]
public class GridIndex
{
	public int x
	{ get; set; }

	public int y
	{ get; set; }

	public GridIndex()
	{
		x = 0;
		y = 0;
	}

	public GridIndex(int px, int py)
	{
		x = px;
		y = py;
	}
}


public class WorldGrid : CK.Behaviour 
{
	public int width = 50;
	public int height = 50;

	public float size = 0;

	public GameObject floorCellTemplate;
	public GameObject wallCellTemplate;

	GridCell[,] m_grid;

	GameObject[,] m_floorCells;
	GameObject[,,] m_wallCells;

	[GUIButton("Generate")]
	public void Generate()
	{
		m_grid = new GridCell[width,height];

		for(int i = 0 ; i < width ; i++)
		{
			for(int j = 0 ; j < height ; j++)
			{
				m_grid[i,j] = new GridCell()
				{ wall = GridCell.Wall.NONE,
				  floor = 1
				};
			}
		}

		m_floorCells = new GameObject[width,height];

		m_wallCells = new GameObject[width,height,6];
	}

	[GUIButton("Update")]
	public void UpdateGrid()
	{
		for(int i = 0 ; i < width ; i++)
		{
			for(int j = 0 ; j < height ; j++)
			{
				if(m_grid[i,j].floor == 0 && m_floorCells[i,j] != null)
					DestroyImmediate(m_floorCells[i,j]);
				else if(m_grid[i,j].floor == 1 && m_floorCells[i,j] == null)
				{
					m_floorCells[i,j] =(GameObject) GameObject.Instantiate(floorCellTemplate);
					m_floorCells[i,j].transform.parent = this.transform;
				}

				if(m_floorCells[i,j] != null)
					m_floorCells[i,j].transform.position = new Vector3(i*size, j*size, 0f);
			}
		}
	}

	[GUIButton("Clear")]
	public void Clear()
	{
		for(int i=0 ; i<transform.childCount ; )
		{
			DestroyImmediate(transform.GetChild(i).gameObject);
		}


	}

	public GridCell GetCell(int x, int y)
	{
		if( x < 0 || x >= width || 
	   	   y < 0 || y >= height)
	 		return null;

		return m_grid[x,y];
	}

	public void SwitchWall(int x, int y, GridCell.Wall wall)
	{
		Debug.Log("Set wall of type: "+wall+" on cell: ("+x+","+y+")");

		if( x < 0 || x >= width || 
		   y < 0 || y >= height)
			return;

		if(wall != m_grid[x,y].wall)
		{
			int newWallID = (int) wall;
			int prevWallID = (int) m_grid[x,y].wall;

			int newWall,prevWall;

			for(int k = 1 ; k <= 6 ; k++)
			{
				newWall  = ((1<<k)  & newWallID) >> k;
				prevWall = ((1<<k) & prevWallID) >> k;

				if(newWall == 1 && prevWall == 0)
				{
					CreateWall(x,y,(GridCell.Wall) (newWall<<k));
				}
				else if(newWall ==0 && prevWall == 1)
				{
					DestroyImmediate(m_wallCells[x,y,k-1]);
				}
			}

			m_grid[x,y].wall = wall;
		}
	}

	bool isPowerOfTwo(int x)
	{
		return ((x != 0) && ((x & (~x + 1)) == x));
	}

	private void CreateWall(int x, int y , GridCell.Wall wallType)
	{
		if(!isPowerOfTwo((int)wallType))
		{
			Debug.Log("Error in CreateWall : wallType must be a non-zero power of two");
			return;
		}

		int wallIdx = GetWallIndex(wallType);

		Debug.Log("Index: "+wallIdx+" for type: "+wallType);

		m_wallCells[x,y,wallIdx] = (GameObject) Instantiate(wallCellTemplate);

		m_wallCells[x,y,wallIdx].transform.parent = this.transform;

		m_wallCells[x,y,wallIdx].transform.position = new Vector3( x * size , y * size) +
			0.5f* size * (Vector3) GetWallOffset(wallType);

		m_wallCells[x,y,wallIdx].transform.rotation = 
			Quaternion.AngleAxis(GetWallRotation(wallType) * Mathf.Rad2Deg ,Vector3.forward);
	}

	public Vector2 GetWallOffset(GridCell.Wall wallType)
	{
		if(!isPowerOfTwo((int) wallType))
		{
			return Vector2.zero;
		} 

		switch(wallType)
		{
		case GridCell.Wall.NONE:
		case GridCell.Wall.DIAG_UP:
		case GridCell.Wall.DIAG_DOWN:
			return Vector2.zero;
		case GridCell.Wall.TOP:
			return Vector2.up;
		case GridCell.Wall.BOTTOM:
			return -Vector2.up;
		case GridCell.Wall.LEFT:
			return -Vector2.right;
		case GridCell.Wall.RIGHT:
			return Vector2.right;
		default:
			Debug.LogError("Incorrect walltype: "+wallType);
			break;
		}

		return Vector2.zero;
	}

	public float GetWallRotation(GridCell.Wall wallType)
	{
		if(!isPowerOfTwo((int) wallType))
		{
			return 0f;
		} 
		
		switch(wallType)
		{
		case GridCell.Wall.NONE:
		case GridCell.Wall.TOP:
		case GridCell.Wall.BOTTOM:
			return 0f;

		case GridCell.Wall.DIAG_UP:
			return Mathf.PI * 0.25f;
		case GridCell.Wall.DIAG_DOWN:
			return 3f * Mathf.PI * 0.25f;
		
		case GridCell.Wall.LEFT:
		case GridCell.Wall.RIGHT:
			return Mathf.PI * 0.5f;
		default:
			Debug.LogError("Incorrect walltype: "+wallType);
			break;
		}
		
		return 0f;
	}

	public int GetWallIndex(GridCell.Wall wallType)
	{
		if(!isPowerOfTwo((int) wallType) ||
		   wallType == GridCell.Wall.NONE)
		{
			return -1;
		} 

		int val = (int) wallType;
		int k = -1;

		while((val&0x01) != 0x01)
		{
			val = val>>1;
			k++;
		}

		if(val != 1)
			Debug.LogWarning("Incorrect Wall value: "+wallType);

		return k;
	}

	public GridIndex GetIndexFromMouse(Vector2 mousePos)
	{
		Vector3 wMousePos = Camera.main.ScreenToWorldPoint(
			(Vector3)mousePos + Camera.main.nearClipPlane * Vector3.forward);

		return GetIndexFromPosition(wMousePos);
	}

	public GridIndex GetIndexFromPosition(Vector2 pos)
	{
		int x = Mathf.RoundToInt(pos.x/size);
		int y = Mathf.RoundToInt(pos.y/size);

		if( x < 0 || x >= width || 
		    y < 0 || y >= height)
			return null;

		return new GridIndex(x,y);
	}

	public void OnDrawGizmos()
	{
		DrawGrid();
	}

	void DrawGrid()
	{
		Gizmos.color = Color.white;

		for(int i = 0 ; i < width ; i++)
		{
			Gizmos.DrawLine(new Vector3( (i-0.5f) * size , -0.5f * size), 
			                new Vector3( (i-0.5f) * size , size * (width - 0.5f)));
		}

		for(int j = 0 ; j < height ; j++)
		{
			Gizmos.DrawLine(new Vector3( size * -0.5f , (j - 0.5f) * size), 
			                new Vector3( size * (width - 0.5f), (j - 0.5f) * size));
		}

		if(m_wallCells == null)
			return;

		for(int i = 0 ; i < width ; i++)
		{
			for(int j = 0 ; j < height ; j++)
			{
				for(int k= 0 ; k < 6 ; k++)
				{
					if(m_wallCells[i,j,k] == null)
						continue;
				
					Matrix4x4 rotationMatrix = Matrix4x4.TRS(transform.position + 
					                                         0.5f* size *(Vector3) GetWallOffset((GridCell.Wall)(1<<k+1)) +
					                        				 new Vector3(i*size , j*size), 
					                                         Quaternion.AngleAxis(GetWallRotation((GridCell.Wall) (1<<k+1)) * Mathf.Rad2Deg,Vector3.forward),
					                                         transform.lossyScale);
					Gizmos.matrix = rotationMatrix;

					Gizmos.DrawCube(Vector2.zero, 
					                new Vector3(1f,0.1f,0.1f));
				}
			}
		}
	}


}
