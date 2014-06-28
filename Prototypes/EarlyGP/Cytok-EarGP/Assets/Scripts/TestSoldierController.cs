using UnityEngine;
using System.Collections;

[RequireComponent (typeof (Rigidbody))]
[RequireComponent (typeof (CapsuleCollider))]

public class TestSoldierController : MonoBehaviour 
{
	#region Variables
	
	//Components
	protected Animator animator;
	public GameObject camera;
	public Camera cam;
	
	//action variables
	bool canReload = true;
	public bool reloading = true;
	bool isReloading;
	bool canItem;
	public bool item = true;
	bool canSignal;
	public bool signal = true;
	bool canAbility;
	public bool ability = true;
	bool canMelee;
	public bool melee = true;
	bool isMelee;

	/*
	//cover variables
	bool covering = true;
	int cover = 0;
	bool canCover = true;
	bool covered = false;
	
	//jumping variables
	public float gravity = -19.62f;
	bool canJump;
	bool isJumping;
	bool jumped = false;
	bool isGrounded;
	bool falling;
	public float jumpSpeed = 12;
	public float doublejumpSpeed = 12;
	public bool doublejumping = true;
	bool canDoubleJump;
	bool isDoubleJumping;
	bool doublejumped = false;
	
	// Used for continuing momentum while in air
	public bool aircontrol = true;
	public float inAirSpeed = 8f;
	float maVelXelocity = 2f;
	float minVelocity = -2f;
	
	//rolling variables
	public float rollSpeed = 8;
	public bool rolling = true;
	bool canRoll = true;
	bool isRolling = false;
	bool rolled = false;
	float rolldamp = .16f;
	float rollduration;
	*/
	//movement variables
	bool canMove = true;
	public float walkSpeed = 1.35f;
	bool canwalk = true;
	float moveSpeed;
	public float runSpeed = 6f;
	public float rotationSpeed = 20f;
	bool isMoving = false;
	public bool walking = true;
	bool areWalking;
	Vector3 inputVec;
	Vector3 newVelocity;
	Vector3 platformSpeed;
	bool platformAnimated;
	Quaternion platformFacing;
	
	//aiming/shooting variables
	bool canAim;
	bool canFire;
	public bool aiming = true;
	bool isAiming = false;
	public bool grenading = true;
	bool isGrenading;
	bool canGrenade = true;
	int weaponType = 0;
	
	//Weapon Prefabs
	GameObject rifle; 
	GameObject heavy;
	
	#endregion
	
	#region Initialization
	
	void Awake()
	{
		//Get the weapons
		rifle = GameObject.FindGameObjectWithTag("Rifle");
		heavy = GameObject.FindGameObjectWithTag("Heavy");
	}
	
	void Start () 
	{
		//set the animator component
		animator = GetComponentInChildren<Animator>();
		
		//sets the weight on any additional layers to 1
		if(animator.layerCount >= 2)
		{
			animator.SetLayerWeight(1, 1);
		}
		
		//Get the camera
		camera = GameObject.FindGameObjectWithTag("MainCamera");
		cam = camera.GetComponent<Camera>();
		
		//enables pistol, disables other weapons
		//pistol.SetActive(true);
		rifle.SetActive(true);
		heavy.SetActive(false);
		
		//sets the Weapon to 1 in the animator
		weaponType = 1;
		StartCoroutine( COSwitchWeapon("Weapon", 1));

		canAim = true;
		canItem = true;
		canGrenade = true;
		canFire = true;
	}
	
	#endregion
	
	#region CORoutines
	
	//function to play a one shot animation 
	public IEnumerator COPlayOneShot ( string paramName )
	{
		animator.SetBool( paramName, true );
		yield return null;
		animator.SetBool( paramName, false );
	}
	
	//function to switch weapons
	public IEnumerator COSwitchWeapon ( string weaponname, int weaponnumber )
	{	
		//sets Weapon to 0 first to reset
		animator.SetInteger( weaponname, 0 );
		yield return null;
		yield return null;
		animator.SetInteger( weaponname, weaponnumber );
	}
	
	//function to reload
	public IEnumerator COReload ( int weapon )
	{	
		//sets Weapon to 0 first to reset
		animator.SetBool( "Reload", true );
		yield return null;
		animator.SetBool( "Reload", false );
		float wait = 0;
		
		if (weaponType == 1 || weaponType == 2)
		{
			wait = 1.85f;
		}
		
		if (weaponType == 3 || weaponType == 4)
		{
			wait = 3f;
		}
		
		yield return new WaitForSeconds(wait);
		isReloading = false;
	}
	
	//function to grenade
	public IEnumerator COGrenade()
	{	
		//sets Weapon to 0 first to reset
		animator.SetBool( "Grenade", true );
		yield return null;
		animator.SetBool( "Grenade", false );
		yield return new WaitForSeconds(1);
		isGrenading = false;
	}
	
	//function to Melee
	public IEnumerator COMelee()
	{	
		rigidbody.velocity = new Vector3(0,0,0);
		canMove = false;
		isMoving = false;
		animator.SetTrigger( "Melee" );
		yield return new WaitForSeconds(.7f);
		isMelee = false;
		canMove = true;
	}
	
	#endregion
	
	#region WeaponSwitching
	
	void WeaponSwitch()
	{	
		weaponType= (weaponType+1)%3;
		
		if (weaponType == 1)
		{
			rifle.SetActive(true);
			heavy.SetActive(false);
			StartCoroutine( COSwitchWeapon("Weapon", 1));
		}
		
		if (weaponType == 2)
		{

		
			rifle.SetActive(false);
			heavy.SetActive(true);
			StartCoroutine( COSwitchWeapon("Weapon", 2));
		}

	}
	
	#endregion
	
	#region UpdateMovement
	
	float UpdateMovement()
	{	
		float x = Input.GetAxisRaw("Horizontal");
		float z = Input.GetAxisRaw("Vertical");
		Vector3 inputVec = new Vector3(x, 0, z);
		Vector3 motion = inputVec;
		
		//if(isGrounded)
		//{
			//reduce input for diagonal movement
			motion *= (Mathf.Abs(inputVec.x) == 1 && Mathf.Abs(inputVec.z) == 1)?.7f:1;
			
			//apply velocity based on platform speed to prevent sliding
			float platformVelocity = platformSpeed.magnitude * .4f;
			Vector3 platformAdjust = platformSpeed * platformVelocity;
			
			//set speed by walking / running
			if (areWalking)
			{
				//canAim = false;
				
				//check if we are on a platform and if its animated, apply the platform's velocity
				if (!platformAnimated)
				{
					newVelocity = motion * walkSpeed + platformAdjust;
				}
				else
				{
					newVelocity = motion * walkSpeed + platformAdjust;
				}
			}
			else
			{
				//check if we are on a platform and if its animated, apply the platform's velocity
				if (!platformAnimated)
				{
					newVelocity = motion * runSpeed + platformAdjust;
				}
				else
				{
					newVelocity = motion * runSpeed + platformSpeed;
				}
			}
		/*
		}
		else
		{
			//if we are falling use momentum
			newVelocity = rigidbody.velocity;
		}
		*/
		// limit velocity to x and z, by maintaining current y velocity:
		newVelocity.y = rigidbody.velocity.y;
		rigidbody.velocity = newVelocity;
		
		// Rotation
		if (inputVec != Vector3.zero && !isAiming)
		{
			transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(inputVec), Time.deltaTime * rotationSpeed);
		}
		
		//if the right mouse button is held look at the mouse cursor
		if (isAiming)
		{
			//make character point at mouse
			Quaternion targetRotation;
			float rotationSpeed = 40f;
			Vector3 mousePos = Input.mousePosition;
			mousePos = cam.ScreenToWorldPoint(new Vector3(mousePos.x,mousePos.y,cam.transform.position.y - transform.position.y));
			targetRotation = Quaternion.LookRotation(mousePos - new Vector3(transform.position.x,0,transform.position.z));
			transform.eulerAngles = Vector3.up * Mathf.MoveTowardsAngle(transform.eulerAngles.y,targetRotation.eulerAngles.y,(rotationSpeed * Time.deltaTime) * rotationSpeed);
		}
		
		if (canAim)
		{
			//if the right joystick is moved, use that for facing
			if (Mathf.Abs(Input.GetAxis("AimHorizontal")) > .1 || Mathf.Abs(Input.GetAxis("AimVertical")) > .1 && canAim)
			{
				float aimH = Input.GetAxisRaw("AimHorizontal");
				float aimV = Input.GetAxisRaw("AimVertical");
				Vector3 rotationDirection = new Vector3(aimH, 0 , aimV);
				rotationDirection = rotationDirection.normalized;
				var rotation = Quaternion.LookRotation(rotationDirection);
				transform.rotation = Quaternion.Lerp(transform.rotation, rotation, Time.time);
			}
		}
		/*
		//calculate the rolling time
		rollduration -= rolldamp;
		
		if (rollduration > 0)
		{
			isRolling = true;
		}
		else
		{
			isRolling = false;
		}
		
		if (isRolling)
		{
			Vector3 localforward = transform.TransformDirection(0, 0, 1);
			rigidbody.velocity = localforward * rollSpeed;
		}
		*/
		//return a movement value for the animator
		return inputVec.magnitude;
	}
	
	#endregion

	#region AirControl
	/*
	void AirControl()
	{
		float x = Input.GetAxisRaw("Horizontal");
		float z = Input.GetAxisRaw("Vertical");
		Vector3 inputVec = new Vector3(x, 0, z);
		Vector3 motion = inputVec;
		
		motion *= (Mathf.Abs(inputVec.x) == 1 && Mathf.Abs(inputVec.z) == 1)?.7f:1;
		
		//newVelocity = motion * runSpeed + platformSpeed;
		
		//allow some control the air
		rigidbody.AddForce(motion * inAirSpeed, ForceMode.Acceleration);
		
		//limit the amount of velocity we can achieve
		float VelX = 0;
		float VelZ = 0;
		
		if (rigidbody.velocity.x > maVelXelocity)
		{
			VelX = rigidbody.velocity.x - maVelXelocity;
			
			if (VelX < 0)
			{
				VelX = 0;
			}
			
			rigidbody.AddForce(new Vector3(-VelX, 0, 0), ForceMode.Acceleration);
		}
		
		if (rigidbody.velocity.x < minVelocity)
		{
			VelX = rigidbody.velocity.x - minVelocity;
			
			if (VelX > 0)
			{
				VelX = 0;
			}
			
			rigidbody.AddForce(new Vector3(-VelX, 0, 0), ForceMode.Acceleration);
		}
		
		if (rigidbody.velocity.z > maVelXelocity)
		{
			VelZ = rigidbody.velocity.z - maVelXelocity;
			
			if (VelZ < 0)
			{
				VelZ = 0;
			}
			
			rigidbody.AddForce(new Vector3(0, 0, -VelZ), ForceMode.Acceleration);
		}
		
		if (rigidbody.velocity.z < minVelocity)
		{
			VelZ = rigidbody.velocity.z - minVelocity;
			
			if (VelZ > 0)
			{
				VelZ = 0;
			}
			
			rigidbody.AddForce(new Vector3(0, 0, -VelZ), ForceMode.Acceleration);
		}
	}
*/	
	#endregion

	#region Grounding
	/*
	void CheckForGrounded()
	{
		float distanceToGround;
		float threshold = .45f;
		RaycastHit hit;
		
		Vector3 offset = new Vector3(0,.4f,0);
		if (Physics.Raycast((transform.position + offset), -Vector3.up, out hit, 100f))
		{
			distanceToGround = hit.distance;
			
			if (distanceToGround < threshold)
			{
				isGrounded = true;
				
				//moving platforms
				if (hit.transform.tag == "Platform")
				{
					//get platform script from collided platform
					Platform platformScript = hit.transform.GetComponent<Platform>();
					
					//check if the platform is moved with physics or if it is animated and get velocity from it
					if (platformScript.animated)
					{
						platformSpeed = platformScript.velocity;
						platformAnimated = true;
					}
					
					if (!platformScript.animated)
					{
						platformSpeed = hit.transform.rigidbody.velocity;
					}
					
					//get the platform rotation to pass into our character when they are on a platform
					platformFacing = hit.transform.rotation;
				}
				else
				{
					//if we are not on a platform, reset platform variables
					platformSpeed = new Vector3(0,0,0);
					platformFacing.eulerAngles = new Vector3(0, 0, 0);
					Platform platformScript = null;
					float platformVelocity = 0f;
				}
			}
			else
			{
				isGrounded = false;
			}
		}
	}
	*/
	#endregion
	
	#region Cover
	/*
	void CoverUpdate()
	{			
		if (covering)
		{
			//check if we press cover button
			if (Input.GetButtonDown("Cover") && canCover && !covered)
			{
				//set variables
				animator.SetBool("Moving", false);
				Input.ResetInputAxes();
				isMoving = false;
				
				animator.SetBool("Moving", false);
				covered = true;
				canReload = true;
				canCover = false;
				canItem = false;
				canMelee = false;
				canFire = false;
				canItem = false;
				canGrenade = false;
				canJump = false;
				cover = 1;
				animator.SetInteger("Cover", 1);
				rigidbody.velocity = new Vector3(0,0,0);
			}
			else
			{   
				//if we are already in cover and press the cover button, get out of cover
				if (Input.GetButtonDown("Cover") && covered == true)
				{
					//set the animation back to idle
					animator.SetInteger("Cover", 3);
					
					//set variables
					cover = 0;
					covered = false;
					canCover = true;
					canAbility = true;
					canAim = true;
					canItem = true;
					canGrenade = true;
					canFire = true;
				}
			}
		}
	}
	*/
	#endregion
	
	#region Jumping
	/*
	void JumpingUpdate()
	{
		//If the character is on the ground
		if (isGrounded)
		{
			//set the animation back to idle
			animator.SetInteger("Jumping", 0);
			
			//set variables
			jumped = false;
			canJump = true;
			isJumping = false;
			jumped = false;
			doublejumped = false;
			canRoll = true;
			canCover = true;
			canItem = true;
			canSignal = true;
			canAbility = true;
			canAim = true;
			canFire = true;
			canMelee = true;
			canDoubleJump = false;
			falling = false;
			
			//check if we press jump button
			if ( canJump && Input.GetButtonDown("Jump") && cover != 1 )
			{
				// Apply the current movement to launch velocity
				rigidbody.velocity += jumpSpeed * Vector3.up;
				
				//set variables
				animator.SetTrigger("Jump");
				canJump = false;
				isJumping = true;
				canDoubleJump = true;
				jumped = true;
				animator.SetInteger("Jumping", 2);
			}
		}
		else
		{    
			//set bools
			canDoubleJump = true;
			canRoll = false;
			canCover = false;
			canAbility = false;
			canCover = false;
			canItem = false;
			canMelee = false;
			canSignal = false;
			canReload = true;
			
			if (!falling && !jumped)
			{
				//set the animation back to idle
				animator.SetInteger("Jumping", 2);
				falling = true;
			}
			
			//if double jumping is allowed and jump is pressed, do a double jump
			if ( canDoubleJump && doublejumping && Input.GetButtonDown("Jump") && doublejumped != true && doublejumping)
			{
				// Apply the current movement to launch velocity
				rigidbody.velocity += doublejumpSpeed * Vector3.up;
				
				//set the animation to double jump
				animator.SetInteger("Jumping", 3);
				
				//set variables
				canJump = false;
				doublejumped = true;
				isJumping = true;
				falling = false;
				jumped = false;
			}
		}
	}
	*/
	#endregion
	
	#region Update
	
	void Update() 
	{
		if(animator)
		{

			
			//check if we aren't in cover and can move
			if (canMove)
			{
				// if we are not in cover, move character
				moveSpeed = UpdateMovement();
			}
			
			//if we are moving, set our animator
			if (moveSpeed > 0)
			{
				isMoving = true;
				animator.SetBool("Moving", true);
			}
			else
			{
				isMoving = false;
				animator.SetBool("Moving", false);
			}
			
			if((Input.GetButton("Fire1")  || Input.GetAxis("Fire1") > 0)&& canFire)// && cover != 1 && covering) 
			{
				isAiming = true;
				Fire();
			}
			else
			{
				isAiming = false;
			}
			

			
			if (canAim)
			{
				if(Mathf.Abs(Input.GetAxis("AimHorizontal")) > .1 || Mathf.Abs(Input.GetAxis("AimVertical")) > .1 && canAim )//&& cover != 1 && covering) 
				{
					isAiming = true;
				}
			}
			
			if(Input.GetButton("Ability") && canAbility && ability) 
			{
				Ability();
			}
			
			if(Input.GetButton("Item") && canItem && item) 
			{
				Item();
			}
			
			if(Input.GetAxis("Item") > 0 && canItem && item) 
			{
				Item();
			}
			
			if(Input.GetButton("Grenade") && canGrenade && grenading) 
			{
				Grenade();
			}
			
			if(Input.GetAxis("Grenade") > 0 && canGrenade && grenading && !isGrenading) 
			{
				Grenade();
			}
			
			if(Input.GetButton("Reload") && canReload && reloading && !isReloading) 
			{
				Reload();
			}
			
			if(Input.GetButton("Signal") && canSignal && signal) 
			{
				Signal();
			}
			
			if(Input.GetAxis("Signal") < 0 && canSignal && signal) 
			{
				Signal();
			}
			
			if(Input.GetButtonDown("Melee") && canMelee && melee) 
			{
				Melee();
			}
			
			if(Input.GetButton("Pain")) 
			{
				Pain();
			}
			
			if(Input.GetButton("Knockback")) 
			{
				Knockback();
			}
			
			if(Input.GetButton("Dazed")) 
			{
				Dazed();
			}
			
			if(Input.GetKeyUp(KeyCode.X))
			{
				Death();
			}
			else
			{
				animator.SetInteger("Death", 0);
			}
			/*
			if(Input.GetButtonDown("Rolling") && rolling  && canRoll && !isRolling) 
			{
				Rolling();
				rollduration = rollSpeed;
			}
			*/
			
			if(Input.GetButton("Run") && walking && canwalk) 
			{
				animator.SetBool("Walking", false);
				areWalking = false;
				canAim = true;
			}
			else
			{
				areWalking = true;
				animator.SetBool("Walking", true);

				canAim = true;
			}
			
			if(Input.GetButtonDown("WeaponSwitch")) 
			{
				WeaponSwitch();
			}
			
			if(Input.GetButton("Weapon1")) 
			{
				//enables pistol, disables other weapons
			
				rifle.SetActive(true);
				heavy.SetActive(false);
				
				//sets the Weapon to 1 in the animator
				weaponType = 1;
				StartCoroutine( COSwitchWeapon("Weapon", 1));
			}
			
			if(Input.GetButton("Weapon2")) 
			{
				//enables rifle, disables other weapons
			
				rifle.SetActive(false);

				heavy.SetActive(true);
				
				//sets the Weapon to 2 in the animator
				weaponType = 2;
				StartCoroutine( COSwitchWeapon("Weapon", 2));
			}
		}
	}
	
	#endregion
	
	#region Misc Methods
	/*
	void Rolling()
	{
		StartCoroutine( COPlayOneShot("Rolling") );
		covered = false;
		canCover = false;
		cover = 0;
		animator.SetInteger("Cover", 0);
		isRolling = true;
	}
	*/
	void Fire()
	{
		StartCoroutine( COPlayOneShot("Fire") );
	}
	
	void Ability()
	{
		StartCoroutine( COPlayOneShot("Ability") );
	}
	
	void Item()
	{
		StartCoroutine( COPlayOneShot("Item") );
	}
	
	void Grenade()
	{
		StartCoroutine( COGrenade() );
		isGrenading = true;
	}
	
	void Reload()
	{
		StartCoroutine( COReload(weaponType) );
		isReloading = true;
	}
	
	void Signal()
	{
		StartCoroutine( COPlayOneShot("Signal") );
	}
	
	void Melee()
	{
		StartCoroutine( COMelee() );
		isMelee = true;
	}
	
	void Pain()
	{
		StartCoroutine( COPlayOneShot("Pain") );
	}
	
	void Knockback()
	{
		StartCoroutine( COPlayOneShot("Knockback") );
	}
	
	void Dazed()
	{
		StartCoroutine( COPlayOneShot("Dazed") );
	}
	
	//plays a random death# animation between 1-3
	void Death()
	{
		//stop character movement
		animator.SetBool("Moving", false);
		Input.ResetInputAxes();
		isMoving = false;
		int deathnumber = 0;
		deathnumber = (Random.Range(1, 4));
		animator.SetInteger("Death",deathnumber);
	}
	
	#endregion
	
	#region Fixed/Late Updates
	
	void FixedUpdate ()
	{
		/*
		//gravity
		rigidbody.AddForce(0, gravity, 0, ForceMode.Acceleration);
		
		if (aircontrol)
			AirControl();
			*/
	}
	
	void LateUpdate()
	{
		//Get local velocity of charcter
		float VelXel = transform.InverseTransformDirection(rigidbody.velocity).x;
		float VelZel = transform.InverseTransformDirection(rigidbody.velocity).z;
		
		//Update animator with movement values
		animator.SetFloat("Velocity X", VelXel / runSpeed);
		animator.SetFloat("Velocity Z", VelZel / runSpeed);
	}
	
	#endregion
}