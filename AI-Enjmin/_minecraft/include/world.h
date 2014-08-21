#ifndef __WORLD_H__
#define __WORLD_H__

#include "gl/glew.h"
#include "gl/freeglut.h" 
#include "engine/utils/types_3d.h"
#include "cube.h"
#include "chunk.h"
#include "my_physics.h"

typedef uint8 NYCollision;
#define NY_COLLIDE_UP     0x01
#define NY_COLLIDE_BOTTOM 0x02
#define NY_COLLIDE_LEFT   0x04
#define NY_COLLIDE_RIGHT  0x08
#define NY_COLLIDE_FRONT  0x10
#define NY_COLLIDE_BACK   0x20
#define NY_COLLIDE_IN     0x40

#define MAT_SIZE 10 //en nombre de chunks
#define MAT_HEIGHT 2 //en nombre de chunks
#define MAT_SIZE_CUBES (MAT_SIZE * NYChunk::CHUNK_SIZE)
#define MAT_HEIGHT_CUBES (MAT_HEIGHT * NYChunk::CHUNK_SIZE)


class NYWorld
{
public :
	NYChunk * _Chunks[MAT_SIZE][MAT_SIZE][MAT_HEIGHT];
	int _MatriceHeights[MAT_SIZE_CUBES][MAT_SIZE_CUBES];
	int _MatriceHeightsTmp[MAT_SIZE_CUBES][MAT_SIZE_CUBES];
	float _FacteurGeneration;
	std::vector<btRigidBody*> _ExplodedCubes;
	std::vector<NYCubeType> _ExplodedCubesTypes;

	NYWorld()
	{
		_FacteurGeneration = 1.0;

		//On cr�e les chunks
		for(int x=0;x<MAT_SIZE;x++)
			for(int y=0;y<MAT_SIZE;y++)
				for(int z=0;z<MAT_HEIGHT;z++)
					_Chunks[x][y][z] = new NYChunk(x,y,z);

		for(int x=0;x<MAT_SIZE;x++)
			for(int y=0;y<MAT_SIZE;y++)
				for(int z=0;z<MAT_HEIGHT;z++)
				{
					NYChunk * cxPrev = NULL;
					if(x > 0)
						cxPrev = _Chunks[x-1][y][z];
					NYChunk * cxNext = NULL;
					if(x < MAT_SIZE-1)
						cxNext = _Chunks[x+1][y][z];

					NYChunk * cyPrev = NULL;
					if(y > 0)
						cyPrev = _Chunks[x][y-1][z];
					NYChunk * cyNext = NULL;
					if(y < MAT_SIZE-1)
						cyNext = _Chunks[x][y+1][z];

					NYChunk * czPrev = NULL;
					if(z > 0)
						czPrev = _Chunks[x][y][z-1];
					NYChunk * czNext = NULL;
					if(z < MAT_HEIGHT-1)
						czNext = _Chunks[x][y][z+1];

					_Chunks[x][y][z]->setVoisins(cxPrev,cxNext,cyPrev,cyNext,czPrev,czNext);
				}

					
	}

	inline NYCube * getCube(int x, int y, int z)
	{	
		if(x < 0)x = 0;
		if(y < 0)y = 0;
		if(z < 0)z = 0;
		if(x >= MAT_SIZE * NYChunk::CHUNK_SIZE) x = (MAT_SIZE * NYChunk::CHUNK_SIZE)-1;
		if(y >= MAT_SIZE * NYChunk::CHUNK_SIZE) y = (MAT_SIZE * NYChunk::CHUNK_SIZE)-1;
		if(z >= MAT_HEIGHT * NYChunk::CHUNK_SIZE) z = (MAT_HEIGHT * NYChunk::CHUNK_SIZE)-1;

		return &(_Chunks[x / NYChunk::CHUNK_SIZE][y / NYChunk::CHUNK_SIZE][z / NYChunk::CHUNK_SIZE]->_Cubes[x % NYChunk::CHUNK_SIZE][y % NYChunk::CHUNK_SIZE][z % NYChunk::CHUNK_SIZE]);
	}

	inline NYChunk * getChunkFromCube(int x, int y, int z)
	{	
		if(x < 0)x = 0;
		if(y < 0)y = 0;
		if(z < 0)z = 0;
		if(x >= MAT_SIZE * NYChunk::CHUNK_SIZE) x = (MAT_SIZE * NYChunk::CHUNK_SIZE)-1;
		if(y >= MAT_SIZE * NYChunk::CHUNK_SIZE) y = (MAT_SIZE * NYChunk::CHUNK_SIZE)-1;
		if(z >= MAT_HEIGHT * NYChunk::CHUNK_SIZE) z = (MAT_HEIGHT * NYChunk::CHUNK_SIZE)-1;

		return _Chunks[x / NYChunk::CHUNK_SIZE][y / NYChunk::CHUNK_SIZE][z / NYChunk::CHUNK_SIZE];
	}

	void updateCube(int x, int y, int z)
	{	
		if(x < 0)x = 0;
		if(y < 0)y = 0;
		if(z < 0)z = 0;
		if(x >= MAT_SIZE * NYChunk::CHUNK_SIZE)x = (MAT_SIZE * NYChunk::CHUNK_SIZE)-1;
		if(y >= MAT_SIZE * NYChunk::CHUNK_SIZE)y = (MAT_SIZE * NYChunk::CHUNK_SIZE)-1;
		if(z >= MAT_HEIGHT * NYChunk::CHUNK_SIZE)z = (MAT_HEIGHT * NYChunk::CHUNK_SIZE)-1;

		int xChnk = x / NYChunk::CHUNK_SIZE;
		int yChnk = y / NYChunk::CHUNK_SIZE;
		int zChnk = z / NYChunk::CHUNK_SIZE;

		NYChunk * chk = _Chunks[xChnk][yChnk][zChnk];
		
		chk->updateStructure();

		for(int i=0;i<6;i++)
			if(chk->Voisins[i])
			{
				chk->Voisins[i]->updateStructure();
			}
	}

	void updateStructure(void)
	{
		for(int x=0;x<MAT_SIZE;x++)
			for(int y=0;y<MAT_SIZE;y++)
				for(int z=0;z<MAT_HEIGHT;z++)
					if(!_Chunks[x][y][z]->_UpToDate)
					{
						_Chunks[x][y][z]->updateStructure();
					}
	}

	void deleteCube(int x, int y, int z, bool autoUpdate = true)
	{
		NYCube * cube = getCube(x,y,z);
		cube->_Draw = false;
		cube->_Type = CUBE_AIR;
		NYChunk * chk = getChunkFromCube(x,y,z);
		chk->_UpToDate = false;
		for(int i=0;i<6;i++)
			if(chk->Voisins[i])
				chk->Voisins[i]->_UpToDate = false;
	
		if(autoUpdate)
			updateStructure();
	}

	//Cr�ation d'une pile de cubes
	//only if zero permet de ne g�n�rer la  pile que si sa hauteur actuelle est de 0 (et ainsi de ne pas reg�n�rer de piles existantes)
	void load_pile(int x, int y, int height, bool onlyIfZero = true)
	{
		if(height < 1)
			height = 1;
		if(height >= MAT_HEIGHT_CUBES)
			height = MAT_HEIGHT_CUBES-1;

		if(_MatriceHeights[x][y] != 0 && onlyIfZero)
			return;

		for(int z=0;z<height;z++)
		{
			getCube(x,y,z)->_Draw = true;
			getCube(x,y,z)->_Draw = true;
			if(z>0)
				getCube(x,y,z)->_Type = CUBE_TERRE;
			else
				getCube(x,y,z)->_Type = CUBE_EAU;
		}

		if(height-1>0)
		{
			getCube(x,y,height-1)->_Draw = true;
			getCube(x,y,height-1)->_Type = CUBE_HERBE;
		}

		for(int z=height;z<MAT_HEIGHT_CUBES;z++)
		{
			getCube(x,y,z)->_Draw = true;
			getCube(x,y,z)->_Type = CUBE_AIR;
		}

		_MatriceHeights[x][y] = height;
	}

	//Creation du monde entier, en utilisant le mouvement brownien fractionnaire
	void generate_piles(int x1, int y1,
		int x2, int y2, 
		int x3, int y3,
		int x4, int y4, int prof, int profMax = -1)
	{
		if((x3-x1)<=1 && (y3-y1)<=1)
			return;

		int largeurRandom = (int)(MAT_HEIGHT_CUBES/(prof*_FacteurGeneration));
		if(largeurRandom == 0)
			largeurRandom = 1;

		if(profMax >= 0 && prof >= profMax)
		{
			Log::log(Log::ENGINE_INFO,("End of generation at prof " + toString(prof)).c_str());
			return;
		}

		//On se met au milieu des deux coins du haut
		int xa = (x1+x2)/2;
		int ya = (y1+y2)/2;
		int heighta = (_MatriceHeights[x1][y1]+ _MatriceHeights[x2][y2])/2;
		if((x2-x1)>1)
		{	
			//if(_MatriceHeights[xa][ya] != 0)
				//Log::log(Log::ENGINE_INFO,("heighta before generation " + toString(_MatriceHeights[xa][ya])).c_str());
			heighta += (rand() % largeurRandom) - (largeurRandom/2);
			load_pile(xa,ya,heighta);
		}
		else
			heighta = _MatriceHeights[xa][ya];

		

		//Au milieu des deux coins de droite
		int xb = (x2+x3)/2;
		int yb = (y2+y3)/2;
		int heightb = (_MatriceHeights[x2][y2]+ _MatriceHeights[x3][y3])/2;
		if((y3-y2)>1)
		{
			//if(_MatriceHeights[xb][yb] != 0)
				//Log::log(Log::ENGINE_INFO,("heightb before generation " + toString(_MatriceHeights[xb][yb])).c_str());
			heightb += (rand() % largeurRandom) - (largeurRandom/2);
			load_pile(xb,yb,heightb);
		}
		else
			heightb = _MatriceHeights[xb][yb];

		//Au milieu des deux coins du bas
		int xc = (x3+x4)/2;
		int yc = (y3+y4)/2;
		int heightc = (_MatriceHeights[x3][y3]+ _MatriceHeights[x4][y4])/2;
		heightc += (rand() % largeurRandom) - (largeurRandom/2);
		if((x3-x4)>1)
		{
			//if(_MatriceHeights[xc][yc] != 0)
				//Log::log(Log::ENGINE_INFO,("heightc before generation " + toString(_MatriceHeights[xc][yc])).c_str());
			load_pile(xc,yc,heightc);
		}
		else
			heightc = _MatriceHeights[xc][yc];

		//Au milieu des deux coins de gauche
		int xd = (x4+x1)/2;
		int yd = (y4+y1)/2;
		int heightd = (_MatriceHeights[x4][y4]+ _MatriceHeights[x1][y1])/2;
		heightd += (rand() % largeurRandom) - (largeurRandom/2);
		if((y3-y1)>1)
		{	
			//if(_MatriceHeights[xd][yd] != 0)
				//Log::log(Log::ENGINE_INFO,("heightd before generation " + toString(_MatriceHeights[xd][yd])).c_str());
			load_pile(xd,yd,heightd);
		}
		else
			heightd = _MatriceHeights[xd][yd];

		//Au milieu milieu
		int xe = xa;
		int ye = yb;
		if((x3-x1)>1 && (y3-y1)>1)
		{
			int heighte = (heighta+heightb+heightc+heightd)/4;
			heighte += (rand() % largeurRandom) - (largeurRandom/2);
			load_pile(xe,ye,heighte);
		}
		

		//On genere les 4 nouveaux quads
		generate_piles(x1,y1,xa,ya,xe,ye,xd,yd,prof+1,profMax);
		generate_piles(xa,ya,x2,y2,xb,yb,xe,ye,prof+1,profMax);
		generate_piles(xe,ye,xb,yb,x3,y3,xc,yc,prof+1,profMax);
		generate_piles(xd,yd,xe,ye,xc,yc,x4,y4,prof+1,profMax);
	}


	void lisse(void)
	{
		int sizeWidow = 4;
		memset(_MatriceHeightsTmp,0x00,sizeof(int)*MAT_SIZE_CUBES*MAT_SIZE_CUBES);
		for(int x=0;x<MAT_SIZE_CUBES;x++)
		{
			for(int y=0;y<MAT_SIZE_CUBES;y++)
			{
				//on moyenne sur une distance
				int nb = 0;
				for(int i=(x-sizeWidow < 0 ? 0 : x-sizeWidow) ; i < (x+sizeWidow >= MAT_SIZE_CUBES ? MAT_SIZE_CUBES-1 : x+sizeWidow) ; i++)
				{
					for(int j=(y-sizeWidow < 0 ? 0 : y-sizeWidow) ; j <( y+sizeWidow >= MAT_SIZE_CUBES ? MAT_SIZE_CUBES-1 : y+sizeWidow) ; j++)
					{
						_MatriceHeightsTmp[x][y] += _MatriceHeights[i][j];
						nb++;
					}
				}
				if(nb)
					_MatriceHeightsTmp[x][y] /= nb;
			}
		}

		//On reset les piles
		for(int x=0;x<MAT_SIZE_CUBES;x++)
		{
			for(int y=0;y<MAT_SIZE_CUBES;y++)
			{
				load_pile(x,y,_MatriceHeightsTmp[x][y],false);
			}
		}


	}

	


	void init_world(int profmax = -1)
	{
		_cprintf("Creation du monde %f \n",_FacteurGeneration);

		srand(6665);

		//Reset du monde
		for(int x=0;x<MAT_SIZE;x++)
			for(int y=0;y<MAT_SIZE;y++)
				for(int z=0;z<MAT_HEIGHT;z++)
					_Chunks[x][y][z]->reset();
		memset(_MatriceHeights,0x00,MAT_SIZE_CUBES*MAT_SIZE_CUBES*sizeof(int));

		//On charge les 4 coins
		load_pile(0,0,MAT_HEIGHT_CUBES/2);
		load_pile(MAT_SIZE_CUBES-1,0,MAT_HEIGHT_CUBES/2);
		load_pile(MAT_SIZE_CUBES-1,MAT_SIZE_CUBES-1,MAT_HEIGHT_CUBES/2);	
		load_pile(0,MAT_SIZE_CUBES-1,MAT_HEIGHT_CUBES/2);

		//On g�n�re a partir des 4 coins
		generate_piles(0,0,
			MAT_SIZE_CUBES-1,0,
			MAT_SIZE_CUBES-1,MAT_SIZE_CUBES-1,
			0,MAT_SIZE_CUBES-1,1,profmax);	

		lisse();
		lisse();


		for(int x=0;x<MAT_SIZE;x++)
			for(int y=0;y<MAT_SIZE;y++)
				for(int z=0;z<MAT_HEIGHT;z++)
					_Chunks[x][y][z]->disableHiddenCubes();
		
		add_world_to_vbo();
	}

	NYCube * pick(NYVert3Df  pos, NYVert3Df  dir, NYPoint3D * point)
	{
		return NULL;
	}

    //Boites de collisions plus petites que deux cubes
	NYCollision collide_with_world(NYVert3Df pos, float width, float height, NYCollision & collisionPrincipale)
	{
		NYCollision collision = 0x00;		

		int x = (int)(pos.X / NYCube::CUBE_SIZE);
		int y = (int)(pos.Y / NYCube::CUBE_SIZE);
		int z = (int)(pos.Z / NYCube::CUBE_SIZE);

		int xNext = (int)((pos.X + width/2.0f) / NYCube::CUBE_SIZE);
		int yNext = (int)((pos.Y + width/2.0f) / NYCube::CUBE_SIZE);
		int zNext = (int)((pos.Z + height/2.0f)/ NYCube::CUBE_SIZE);

		int xPrev = (int)((pos.X - width/2.0f) / NYCube::CUBE_SIZE);
		int yPrev = (int)((pos.Y - width/2.0f) / NYCube::CUBE_SIZE);
		int zPrev = (int)((pos.Z - height/2.0f)/ NYCube::CUBE_SIZE);

		//De combien je d�passe dans les autres blocs
		float xDepNext = 0;
		if(xNext != x)
			xDepNext = abs((xNext * NYCube::CUBE_SIZE) - (pos.X + width/2.0f));
		float xDepPrev = 0;
		if(xPrev != x)
			xDepPrev = abs((xPrev * NYCube::CUBE_SIZE) - (pos.X + width/2.0f));
		

		float yDepNext = 0;
		if(yNext != y)
			yDepNext = abs((yNext * NYCube::CUBE_SIZE) - (pos.Y + width/2.0f));
		float yDepPrev = 0;
		if(yPrev != y)
			yDepPrev = abs((yPrev * NYCube::CUBE_SIZE) - (pos.Y + width/2.0f));
		

		float zDepNext = 0;
		if(zNext != z)
			zDepNext = abs((zNext * NYCube::CUBE_SIZE) - (pos.Z + height/2.0f));
		float zDepPrev = 0;
		if(zPrev != z)
			zDepPrev = abs((zPrev * NYCube::CUBE_SIZE) - (pos.Z + height/2.0f));

		if(x < 0)
			x = 0;
		if(y < 0)
			y = 0;
		if(z < 0)
			z = 0;

		if(xPrev < 0)
			xPrev = 0;
		if(yPrev < 0)
			yPrev = 0;
		if(zPrev < 0)
			zPrev = 0;

		if(xNext < 0)
			xNext = 0;
		if(yNext < 0)
			yNext = 0;
		if(zNext < 0)
			zNext = 0;

		if(x >= MAT_SIZE_CUBES)
			x = MAT_SIZE_CUBES-1;
		if(y >= MAT_SIZE_CUBES)
			y = MAT_SIZE_CUBES-1;
		if(z >= MAT_HEIGHT_CUBES)
			z = MAT_HEIGHT_CUBES-1;

		if(xPrev >= MAT_SIZE_CUBES)
			xPrev = MAT_SIZE_CUBES-1;
		if(yPrev >= MAT_SIZE_CUBES)
			yPrev = MAT_SIZE_CUBES-1;
		if(zPrev >= MAT_HEIGHT_CUBES)
			zPrev = MAT_HEIGHT_CUBES-1;

		if(xNext >= MAT_SIZE_CUBES)
			xNext = MAT_SIZE_CUBES-1;
		if(yNext >= MAT_SIZE_CUBES)
			yNext = MAT_SIZE_CUBES-1;
		if(zNext >= MAT_HEIGHT_CUBES)
			zNext = MAT_HEIGHT_CUBES-1;

		//Est on dans un cube plein ?
		if(getCube(x,y,z)->isSolid())
			collision |= NY_COLLIDE_IN; 

		//Collisions droite et gauche
		//On checke ou se trouvent les sommets de la box
		//On checke les coins top et bottom en meme temps

		//Sommets arri�res gauches
		if(getCube(xPrev,yPrev,zPrev)->isSolid() ||
		   getCube(xPrev,yPrev,zNext)->isSolid())
		{
			collision |= NY_COLLIDE_LEFT;
			collision |= NY_COLLIDE_BACK;
			if(xDepPrev > yDepPrev)
				collisionPrincipale |= NY_COLLIDE_LEFT;
			else 
				collisionPrincipale |= NY_COLLIDE_BACK;
		}

		//Sommets avants gauches
		if(getCube(xPrev,yNext,zPrev)->isSolid() ||
			getCube(xPrev,yNext,zNext)->isSolid())
		{
			 collision |= NY_COLLIDE_LEFT;
			 collision |= NY_COLLIDE_FRONT;
			 if(xDepPrev > yDepNext)
				 collisionPrincipale |= NY_COLLIDE_LEFT;
			 else 
				 collisionPrincipale |= NY_COLLIDE_FRONT;
		}

		//Sommets arri�re droits
		if(getCube(xNext,yPrev,zPrev)->isSolid() ||
			getCube(xNext,yPrev,zNext)->isSolid())
		{
			collision |= NY_COLLIDE_RIGHT;
			collision |= NY_COLLIDE_BACK;
			if(xDepNext > yDepPrev)
				collisionPrincipale |= NY_COLLIDE_RIGHT;
			else 
				collisionPrincipale |= NY_COLLIDE_BACK;
		}

		//Sommets avant droits
		if(getCube(xNext,yNext,zPrev)->isSolid() ||
			getCube(xNext,yNext,zNext)->isSolid())
		{
			collision |= NY_COLLIDE_RIGHT;
			collision |= NY_COLLIDE_FRONT;
			if(xDepNext > yDepNext)
				collisionPrincipale |= NY_COLLIDE_RIGHT;
			else 
				collisionPrincipale |= NY_COLLIDE_FRONT;
		}
			
		//Pour le bottom on checke tout, meme le milieu 
		if(getCube(xPrev,y    ,zPrev)->isSolid() ||
			getCube(xPrev,yPrev,zPrev)->isSolid() ||
			getCube(xPrev,yNext,zPrev)->isSolid() ||
			getCube(xNext,y    ,zPrev)->isSolid() ||
			getCube(xNext,yPrev,zPrev)->isSolid() ||
			getCube(xNext,yNext,zPrev)->isSolid() ||
			getCube(x    ,y    ,zPrev)->isSolid() ||
			getCube(x    ,yPrev,zPrev)->isSolid() ||
			getCube(x    ,yNext,zPrev)->isSolid())
			collision |= NY_COLLIDE_BOTTOM;

		//Pour le up on checke tout, meme le milieu 
		if(getCube(xPrev,y    ,zNext)->isSolid() ||
			getCube(xPrev,yPrev,zNext)->isSolid() ||
			getCube(xPrev,yNext,zNext)->isSolid() ||
			getCube(xNext,y    ,zNext)->isSolid() ||
			getCube(xNext,yPrev,zNext)->isSolid() ||
			getCube(xNext,yNext,zNext)->isSolid() ||
			getCube(x    ,y    ,zNext)->isSolid() ||
			getCube(x    ,yPrev,zNext)->isSolid() ||
			getCube(x    ,yNext,zNext)->isSolid())
			collision |= NY_COLLIDE_UP;

		return collision;
	}

	bool getRayCollision(NYVert3Df & debSegment, NYVert3Df & finSegment,
						 NYVert3Df & inter,
						 int &xCube, int&yCube, int&zCube)
	{
		float len = (finSegment - debSegment).getSize();

		int x = (int)(debSegment.X/ NYCube::CUBE_SIZE);
		int y = (int)(debSegment.Y/ NYCube::CUBE_SIZE);
		int z = (int)(debSegment.Z/ NYCube::CUBE_SIZE);

		int l = (int)(len/ NYCube::CUBE_SIZE)+1; 

		int xDeb = x - l;
		int yDeb = y - l;
		int zDeb = z - l;

		int xFin = x + l;
		int yFin = y + l;
		int zFin = z + l;

		if(xDeb < 0)
			xDeb = 0;
		if(yDeb < 0)
			yDeb = 0;
		if(zDeb < 0)
			zDeb = 0;

		if(xFin >= MAT_SIZE_CUBES)
			xFin = MAT_SIZE_CUBES-1;
		if(yFin >= MAT_SIZE_CUBES)
			yFin = MAT_SIZE_CUBES-1;
		if(zFin >= MAT_HEIGHT_CUBES)
			zFin = MAT_HEIGHT_CUBES-1;

		float minDist = -1;
		NYVert3Df interTmp;
		for(x=xDeb;x<=xFin;x++)
			for(y=yDeb;y<=yFin;y++)
				for(z=zDeb;z<=zFin;z++)
				{
					if(getCube(x,y,z)->isSolid())
					{
						if(getRayCollisionWithCube(debSegment,finSegment,x,y,z,interTmp))
						{
							if((debSegment-interTmp).getMagnitude() < minDist || minDist == -1)
							{
								minDist = (debSegment-interTmp).getMagnitude();
								inter = interTmp;
								xCube = x;
								yCube = y;
								zCube = z;

							}
						}
					}
				}

		if(minDist != -1)
			return true;

		return false;

	}

	/**
	  * De meme cette fonction peut �tre grandement opitimis�e, on a priviligi� la clart�
	  */
	bool getRayCollisionWithCube(NYVert3Df & debSegment, NYVert3Df & finSegment,
						         int x,int y,int z,
								 NYVert3Df & inter)
	{


		float minDist = -1;
		NYVert3Df interTemp;

		//Face1
		if(intersecDroiteCubeFace(debSegment,finSegment,
			                   NYVert3Df((x+0)*NYCube::CUBE_SIZE,(y+0)*NYCube::CUBE_SIZE,(z+0)*NYCube::CUBE_SIZE),
							   NYVert3Df((x+1)*NYCube::CUBE_SIZE,(y+0)*NYCube::CUBE_SIZE,(z+0)*NYCube::CUBE_SIZE),
							   NYVert3Df((x+1)*NYCube::CUBE_SIZE,(y+0)*NYCube::CUBE_SIZE,(z+1)*NYCube::CUBE_SIZE),
							   NYVert3Df((x+0)*NYCube::CUBE_SIZE,(y+0)*NYCube::CUBE_SIZE,(z+1)*NYCube::CUBE_SIZE),
							   interTemp))
		{
			if((interTemp-debSegment).getMagnitude() < minDist || minDist == -1)
			{
				minDist = (interTemp-debSegment).getMagnitude();
				inter = interTemp;
			}
		}

		//Face2
		if(intersecDroiteCubeFace(debSegment,finSegment,
			NYVert3Df((x+0)*NYCube::CUBE_SIZE,(y+1)*NYCube::CUBE_SIZE,(z+0)*NYCube::CUBE_SIZE),
			NYVert3Df((x+1)*NYCube::CUBE_SIZE,(y+1)*NYCube::CUBE_SIZE,(z+0)*NYCube::CUBE_SIZE),
			NYVert3Df((x+1)*NYCube::CUBE_SIZE,(y+1)*NYCube::CUBE_SIZE,(z+1)*NYCube::CUBE_SIZE),
			NYVert3Df((x+0)*NYCube::CUBE_SIZE,(y+1)*NYCube::CUBE_SIZE,(z+1)*NYCube::CUBE_SIZE),
			interTemp))
		{
			if((interTemp-debSegment).getMagnitude() < minDist || minDist == -1)
			{
				minDist = (interTemp-debSegment).getMagnitude();
				inter = interTemp;
			}
		}

		//Face3
		if(intersecDroiteCubeFace(debSegment,finSegment,
			NYVert3Df((x+0)*NYCube::CUBE_SIZE,(y+0)*NYCube::CUBE_SIZE,(z+0)*NYCube::CUBE_SIZE),
			NYVert3Df((x+1)*NYCube::CUBE_SIZE,(y+0)*NYCube::CUBE_SIZE,(z+0)*NYCube::CUBE_SIZE),
			NYVert3Df((x+1)*NYCube::CUBE_SIZE,(y+1)*NYCube::CUBE_SIZE,(z+0)*NYCube::CUBE_SIZE),
			NYVert3Df((x+0)*NYCube::CUBE_SIZE,(y+1)*NYCube::CUBE_SIZE,(z+0)*NYCube::CUBE_SIZE),
			interTemp))
		{
			if((interTemp-debSegment).getMagnitude() < minDist || minDist == -1)
			{
				minDist = (interTemp-debSegment).getMagnitude();
				inter = interTemp;
			}
		}

		//Face4
		if(intersecDroiteCubeFace(debSegment,finSegment,
			NYVert3Df((x+0)*NYCube::CUBE_SIZE,(y+0)*NYCube::CUBE_SIZE,(z+1)*NYCube::CUBE_SIZE),
			NYVert3Df((x+1)*NYCube::CUBE_SIZE,(y+0)*NYCube::CUBE_SIZE,(z+1)*NYCube::CUBE_SIZE),
			NYVert3Df((x+1)*NYCube::CUBE_SIZE,(y+1)*NYCube::CUBE_SIZE,(z+1)*NYCube::CUBE_SIZE),
			NYVert3Df((x+0)*NYCube::CUBE_SIZE,(y+1)*NYCube::CUBE_SIZE,(z+1)*NYCube::CUBE_SIZE),
			interTemp))
		{
			if((interTemp-debSegment).getMagnitude() < minDist || minDist == -1)
			{
				minDist = (interTemp-debSegment).getMagnitude();
				inter = interTemp;
			}
		}

		//Face5
		if(intersecDroiteCubeFace(debSegment,finSegment,
			NYVert3Df((x+0)*NYCube::CUBE_SIZE,(y+0)*NYCube::CUBE_SIZE,(z+0)*NYCube::CUBE_SIZE),
			NYVert3Df((x+0)*NYCube::CUBE_SIZE,(y+1)*NYCube::CUBE_SIZE,(z+0)*NYCube::CUBE_SIZE),
			NYVert3Df((x+0)*NYCube::CUBE_SIZE,(y+1)*NYCube::CUBE_SIZE,(z+1)*NYCube::CUBE_SIZE),
			NYVert3Df((x+0)*NYCube::CUBE_SIZE,(y+0)*NYCube::CUBE_SIZE,(z+1)*NYCube::CUBE_SIZE),
			interTemp))
		{
			if((interTemp-debSegment).getMagnitude() < minDist || minDist == -1)
			{
				minDist = (interTemp-debSegment).getMagnitude();
				inter = interTemp;
			}
		}

		//Face6
		if(intersecDroiteCubeFace(debSegment,finSegment,
			NYVert3Df((x+1)*NYCube::CUBE_SIZE,(y+0)*NYCube::CUBE_SIZE,(z+0)*NYCube::CUBE_SIZE),
			NYVert3Df((x+1)*NYCube::CUBE_SIZE,(y+1)*NYCube::CUBE_SIZE,(z+0)*NYCube::CUBE_SIZE),
			NYVert3Df((x+1)*NYCube::CUBE_SIZE,(y+1)*NYCube::CUBE_SIZE,(z+1)*NYCube::CUBE_SIZE),
			NYVert3Df((x+1)*NYCube::CUBE_SIZE,(y+0)*NYCube::CUBE_SIZE,(z+1)*NYCube::CUBE_SIZE),
			interTemp))
		{
			if((interTemp-debSegment).getMagnitude() < minDist || minDist == -1)
			{
				minDist = (interTemp-debSegment).getMagnitude();
				inter = interTemp;
			}
		}
		

		if (minDist < 0)
			return false;

		return true;
	}

	/**
	  * Cr�e une explosiont : suppression de tous les cubes de la matrice dans un rayon donn�, pour les ajouter 
	  * sous forme de box au moteur physique. Conserver les body dans un vector pour pouvoir effectuer leur rendu.
	  */
	void createExplosion(NYVert3Df & position, float size)
	{
		int x = (int)(position.X/ NYCube::CUBE_SIZE);
		int y = (int)(position.Y/ NYCube::CUBE_SIZE);
		int z = (int)(position.Z/ NYCube::CUBE_SIZE);

		int l = (int)(size/ NYCube::CUBE_SIZE)+1; 

		int xDeb = x - l;
		int yDeb = y - l;
		int zDeb = z - l;

		int xFin = x + l;
		int yFin = y + l;
		int zFin = z + l;

		if(xDeb < 0)
			xDeb = 0;
		if(yDeb < 0)
			yDeb = 0;
		if(zDeb < 0)
			zDeb = 0;

		if(xFin >= MAT_SIZE_CUBES)
			xFin = MAT_SIZE_CUBES-1;
		if(yFin >= MAT_SIZE_CUBES)
			yFin = MAT_SIZE_CUBES-1;
		if(zFin >= MAT_HEIGHT_CUBES)
			zFin = MAT_HEIGHT_CUBES-1;

		for(x=xDeb;x<=xFin;x++)
			for(y=yDeb;y<=yFin;y++)
				for(z=zDeb;z<=zFin;z++)
				{
					float dist = (position - NYVert3Df(x*NYCube::CUBE_SIZE,y*NYCube::CUBE_SIZE,z*NYCube::CUBE_SIZE)).getMagnitude();
					if(dist < size*size)
					{
						if(getCube(x,y,z)->isSolid())
						{
							_ExplodedCubesTypes.push_back(getCube(x,y,z)->_Type);
							deleteCube(x,y,z,false);
							btRigidBody * cube = NYBasicPhysicEngine::getInstance()->addBoxObject(true,
								NYVert3Df(NYCube::CUBE_SIZE/2,
								NYCube::CUBE_SIZE/2,
								NYCube::CUBE_SIZE/2),
								NYVert3Df(x*NYCube::CUBE_SIZE + NYCube::CUBE_SIZE/2,
								y*NYCube::CUBE_SIZE+ NYCube::CUBE_SIZE/2,
								z*NYCube::CUBE_SIZE+ NYCube::CUBE_SIZE/2),
								10.0f);
							cube->applyImpulse(btVector3(0,0,500),btVector3(0,0,0));
							_ExplodedCubes.push_back(cube);
						}
					}
					
				}
		
		updateStructure();
	}

	void render_world_vbo(void)
	{
		for(int x=0;x<MAT_SIZE;x++)
			for(int y=0;y<MAT_SIZE;y++)
				for(int z=0;z<MAT_HEIGHT;z++)
				{
					glPushMatrix();
					glTranslatef((float)(x*NYChunk::CHUNK_SIZE*NYCube::CUBE_SIZE),(float)(y*NYChunk::CHUNK_SIZE*NYCube::CUBE_SIZE),(float)(z*NYChunk::CHUNK_SIZE*NYCube::CUBE_SIZE));
					_Chunks[x][y][z]->render();	
					glPopMatrix();
				}

		for(int i=0;i<_ExplodedCubes.size();i++)
		{
			btMotionState * state = _ExplodedCubes[i]->getMotionState();
			btTransform t;
			state->getWorldTransform(t);
			btScalar m [16];
			t.getOpenGLMatrix(m);

			glEnable(GL_LIGHTING);
			glEnable(GL_COLOR_MATERIAL);
			switch(_ExplodedCubesTypes[i])
			{
				case CUBE_TERRE:
					glColor3f(101.0f/255.0f,74.0f/255.0f,0.0f/255.0f);
					break;
				case CUBE_HERBE:
					glColor3f(1.0f/255.0f,112.0f/255.0f,12.0f/255.0f);
					break;
				case CUBE_EAU:
					glColor3f(0.0f/255.0f,48.0f/255.0f,255.0f/255.0f);
					break;
			}

			glPushMatrix();
			glMultMatrixf(m);
			glutSolidCube(NYCube::CUBE_SIZE);
			glPopMatrix();		
		}
	}

	void add_world_to_vbo(void)
	{
		int totalNbVertices = 0;
		
		for(int x=0;x<MAT_SIZE;x++)
			for(int y=0;y<MAT_SIZE;y++)
				for(int z=0;z<MAT_HEIGHT;z++)
				{
					_Chunks[x][y][z]->toVbo();
					totalNbVertices += _Chunks[x][y][z]->_NbVertices;
				}

		Log::log(Log::ENGINE_INFO,(toString(totalNbVertices) + " vertices in VBO").c_str());
	}

	/*OLDIES*/
	void render_world_old_school(void)
	{
		glEnable(GL_COLOR_MATERIAL);
		glPushMatrix();
		//glTranslated(-(MAT_SIZE/2)*BLOC_SIZE,-(MAT_SIZE/2)*BLOC_SIZE,-(MAT_HEIGHT/2)*BLOC_SIZE);
		for(int x=0;x<MAT_SIZE_CUBES;x++)
		{
			for(int y=0;y<MAT_SIZE_CUBES;y++)
			{
				for(int z=0;z<MAT_HEIGHT_CUBES;z++)
				{
					NYCube * cube = getCube(x,y,z);
					if(cube->_Draw && cube->_Type != CUBE_AIR)
					{
						switch(cube->_Type)
						{
							case CUBE_TERRE:
								glColor3f(101.0f/255.0f,74.0f/255.0f,0.0f/255.0f);
								break;
							case CUBE_HERBE:
								glColor3f(1.0f/255.0f,112.0f/255.0f,12.0f/255.0f);
								break;
							case CUBE_EAU:
								glColor3f(0.0f/255.0f,48.0f/255.0f,255.0f/255.0f);
								break;
						}

						glPushMatrix();
						glTranslated(x*NYCube::CUBE_SIZE,y*NYCube::CUBE_SIZE,z*NYCube::CUBE_SIZE);
						glutSolidCube(NYCube::CUBE_SIZE);
						glPopMatrix();
					}
				}
			}
		}
		glPopMatrix();
		glDisable(GL_COLOR_MATERIAL);
	}

	/**
	  * Ajoute tous les chunks au moteur physique
	  */
	void addToPhysic(void)
	{
		for(int x=0;x<MAT_SIZE;x++)
			for(int y=0;y<MAT_SIZE;y++)
				for(int z=0;z<MAT_HEIGHT;z++)
				{
					_Chunks[x][y][z]->updateStructure();
				}
	}

};



#endif