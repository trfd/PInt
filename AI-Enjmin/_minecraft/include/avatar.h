#ifndef __AVATAR__
#define __AVATAR__

#include "engine/utils/types_3d.h"
#include "engine/render/camera.h"
#include "world.h"
#include "basic_physic_engine.h"

class NYAvatar
{
	public :

		NYVert3Df Position;
		NYVert3Df Speed;

		NYVert3Df MoveDir;
		bool Move;
		bool Jump;
		float Height;
		float Width;
		bool avance;
		bool recule;
		bool gauche;
		bool droite;
		bool Standing;
        bool Crouch;

		NYCamera * Cam;
		NYWorld * World;

		btRigidBody * Balle;

		NYAvatar(NYCamera * cam,NYWorld * world)
		{
			Position = NYVert3Df(0,0,0);
			Height = 10;
			Width = 3;
			Cam = cam;
			avance = false;
			recule = false;
			gauche = false;
			droite = false;
			Standing = false;
			Jump = false;
            Crouch = false;
			World = world;
			Balle = NULL;
		}


		void render(void)
		{
			glPushMatrix();
			glTranslatef(Position.X,Position.Y,Position.Z);
			glutSolidCube(Width);
			glPopMatrix();
		}

		/**
		  * Effectue le rendu de la balle. Il faut récupérer sa position et rotation
		  * dans le moteur physique et rendre un sphère de bonne taille à la bonne position / rotation.
		  */
		void renderBalle(void)
		{
			if(Balle)
			{
				btMotionState * state = Balle->getMotionState();
				btTransform t;
				state->getWorldTransform(t);
				btScalar m [16];
				t.getOpenGLMatrix(m);

				glEnable(GL_LIGHTING);
				glEnable(GL_COLOR_MATERIAL);
				glColor3d(0.2,0.2,0.2);
				glPushMatrix();
				glMultMatrixf(m);
				glutSolidSphere(Width,10,10);
				glPopMatrix();		
			}
		}

		/**
		  * Créer une nouvelle balle et l'ajoute au moteur. Detruire la balle
		  * si elle existe dejà
		  */
		void shoot(NYVert3Df & direction)
		{
			if(Balle)
				destroyShoot();
			Balle = NYBasicPhysicEngine::getInstance()->addSphereObject(true,Width,Position,10.0);
			btVector3 impulse(direction.X*500,direction.Y*500,direction.Z*500);
			Balle->applyImpulse(impulse,btVector3());	
			NYBasicPhysicEngine::getInstance()->setCollisionReportOnBody(Balle);
		}

		/**
		  * Permet de détruire la balle en cours, si elle existe. La retire du moteur physique
		  */
		void destroyShoot()
		{
			if(Balle)
			{
				NYBasicPhysicEngine::getInstance()->removeObject(Balle);
				SAFEDELETE(Balle);
			}
		}


        /// Note: This function has been modified
        /// to have a proper flying cam in replacement
        /// of FP Cam
		void update(float elapsed)
		{
            // Disable gravity for camera
			//NYVert3Df force = NYVert3Df(0,0,-1) * 100.0f;

            NYVert3Df force = NYVert3Df(0,0,0);

			if(!Standing)	
			{
			//	Jump = false;
			}


			if(Standing)	
			{
				if(avance)
					force += Cam->_Direction * 400;
				if(recule)
					force += Cam->_Direction * -400;
				if(gauche)
					force += Cam->_NormVec * -400;
				if(droite)
					force += Cam->_NormVec * 400;	
			}
			else	
			{
				if(avance)
					force += Cam->_Direction * 50;
				if(recule)
					force += Cam->_Direction * -50;
				if(gauche)
					force += Cam->_NormVec * -50;
				if(droite)
					force += Cam->_NormVec * 50;
			}

			if(Jump)
			{
				force += NYVert3Df(0,0,1) * 50.0f;// / elapsed;
				//Jump = false;
			}

            if(Crouch)
            {
                force -= NYVert3Df(0, 0, 1) * 50.0f;
                //Crouch = false;
            }

			//Speed += force * elapsed;

            Speed = force * 3.f; // No force
           

			NYVert3Df horSpeed = Speed;
			horSpeed.Z = 0;
			if(horSpeed.getSize() > 70.0f)
			{
				horSpeed.normalize();
				horSpeed *= 70.0f;
				Speed.X = horSpeed.X;
				Speed.Y = horSpeed.Y;
			}

			NYVert3Df oldPosition = Position;
			Position += (Speed * elapsed);

			NYCollision collidePrinc = 0x00;
			NYCollision collide = World->collide_with_world(Position,Width,Height,collidePrinc);
			if(collide & NY_COLLIDE_BOTTOM && Speed.Z < 0)
			{
				Position.Z = oldPosition.Z;
				Speed *= pow(0.01f,elapsed);
				Speed.Z = 0;
				Standing = true;
			}
			else
				Standing = false;

			if(collide & NY_COLLIDE_UP && !Standing && Speed.Z > 0)
			{
				Position.Z = oldPosition.Z;
				Speed.Z = 0;
			}

			collide = World->collide_with_world(Position,Width,Height,collidePrinc);


			//_cprintf("Collide  : %x ",collidePrinc);

			//Si je collide sur un coté
			if(collide & NY_COLLIDE_BACK && collide & NY_COLLIDE_RIGHT && collide & NY_COLLIDE_LEFT)
			{
				Position.Y = oldPosition.Y;
				Speed.Y = 0;
			}

			if(collide & NY_COLLIDE_FRONT && collide & NY_COLLIDE_RIGHT && collide & NY_COLLIDE_LEFT)
			{
				Position.Y = oldPosition.Y;
				Speed.Y = 0;
			}

			if(collide & NY_COLLIDE_RIGHT && collide & NY_COLLIDE_FRONT && collide & NY_COLLIDE_BACK)
			{
				Position.X = oldPosition.X;
				Speed.X = 0;
			}

			if(collide & NY_COLLIDE_LEFT && collide & NY_COLLIDE_FRONT && collide & NY_COLLIDE_BACK)
			{
				Position.X = oldPosition.X;
				Speed.X = 0;
			}

			//Si je collide sur un angle
			if(!(collide & NY_COLLIDE_BACK && collide & NY_COLLIDE_FRONT) && !(collide & NY_COLLIDE_LEFT && collide & NY_COLLIDE_RIGHT))
				if(collide & (NY_COLLIDE_BACK | NY_COLLIDE_FRONT | NY_COLLIDE_RIGHT | NY_COLLIDE_LEFT))
				{
					Position.Y = oldPosition.Y;
					Position.X = oldPosition.X;
				}

		}
};

#endif