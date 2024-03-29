#pragma once

#include <stdio.h>

enum NYCubeType
{
	CUBE_HERBE = 0,
	CUBE_EAU   = 1,
	CUBE_TERRE = 2, 
	CUBE_AIR   = 3
};

class NYCube
{
	public :
		bool _Draw;
		NYCubeType _Type;
		static const int CUBE_SIZE = 10;

		NYCube()
		{
			_Draw = true;
			_Type = CUBE_AIR;
		}

		bool isSolid(void)
		{
			return (_Type != CUBE_AIR);
		}

		void saveToFile(FILE * fs)
		{
			fputc(_Draw ? 1 : 0,fs);
			fputc(_Type,fs);
		}

		void loadFromFile(FILE * fe)
		{
			_Draw = fgetc(fe) ? true : false;
			_Type = (NYCubeType)fgetc(fe);
		}
};