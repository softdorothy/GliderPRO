
//============================================================================
//----------------------------------------------------------------------------
//								   Dynamics3.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include "RectUtils.h"


#define kBalloonStart		310
#define kCopterStart		8
#define kDartVelocity		6


dynaPtr		dinahs;
short		numDynamics;

extern	Rect		breadSrc[];
extern	short		numLights;
extern	Boolean		evenFrame;


//==============================================================  Functions
//--------------------------------------------------------------  HandleDynamics

// This is the master function that calls all the specific handlers above.

void HandleDynamics (void)
{
	short		i;
	
	for (i = 0; i < numDynamics; i++)
	{
		switch (dinahs[i].type)
		{
			case kSparkle:
			HandleSparkleObject(i);
			break;
			
			case kToaster:
			HandleToast(i);
			break;
			
			case kMacPlus:
			HandleMacPlus(i);
			break;
			
			case kTV:
			HandleTV(i);
			break;
			
			case kCoffee:
			HandleCoffee(i);
			break;
			
			case kOutlet:
			HandleOutlet(i);
			break;
			
			case kVCR:
			HandleVCR(i);
			break;
			
			case kStereo:
			HandleStereo(i);
			break;
			
			case kMicrowave:
			HandleMicrowave(i);
			break;
			
			case kBalloon:
			HandleBalloon(i);
			break;
			
			case kCopterLf:
			case kCopterRt:
			HandleCopter(i);
			break;
			
			case kDartLf:
			case kDartRt:
			HandleDart(i);
			break;
			
			case kBall:
			HandleBall(i);
			break;
			
			case kDrip:
			HandleDrip(i);
			break;
			
			case kFish:
			HandleFish(i);
			break;
			
			default:
			break;
		}
	}
}

//--------------------------------------------------------------  HandleDynamics

// This is the master function that calls all the various rendering handlersÉ
// above.

void RenderDynamics (void)
{
	short		i;
	
	for (i = 0; i < numDynamics; i++)
	{
		switch (dinahs[i].type)
		{
			case kToaster:
			RenderToast(i);
			break;
			
			case kBalloon:
			RenderBalloon(i);
			break;
			
			case kCopterLf:
			case kCopterRt:
			RenderCopter(i);
			break;
			
			case kDartLf:
			case kDartRt:
			RenderDart(i);
			break;
			
			case kBall:
			RenderBall(i);
			break;
			
			case kDrip:
			RenderDrip(i);
			break;
			
			case kFish:
			RenderFish(i);
			break;
			
			default:
			break;
		}
	}
}

//--------------------------------------------------------------  ZeroDinahs

// This clears all dynamics - zeros them all out.  Used to initialize them.

void ZeroDinahs (void)
{
	short		i;
	
	for (i = 0; i < kMaxDynamicObs; i++)
	{
		dinahs[i].type = kObjectIsEmpty;
		QSetRect(&dinahs[i].dest, 0, 0, 0, 0);
		QSetRect(&dinahs[i].whole, 0, 0, 0, 0);
		dinahs[i].hVel = 0;
		dinahs[i].vVel = 0;
		dinahs[i].count = 0;
		dinahs[i].frame = 0;
		dinahs[i].timer = 0;
		dinahs[i].position = 0;
		dinahs[i].room = 0;
		dinahs[i].byte0 = 0;
		dinahs[i].active = false;
	}
	numDynamics = 0;
}

//--------------------------------------------------------------  AddDynamicObject

// When a room is being drawn, various dynamic objects are pointed here.
// This function sets up the structures to handle them.

short AddDynamicObject (short what, Rect *where, objectType *who, 
		short room, short index, Boolean isOn)
{
	short		position, velocity;
	Boolean		lilFrame;
	
	if (numDynamics >= kMaxDynamicObs)
		return (-1);
	
	dinahs[numDynamics].type = what;
	switch (what)
	{
		case kSparkle:
		dinahs[numDynamics].dest = sparkleSrc[0];
		ZeroRectCorner(&dinahs[numDynamics].dest);
		QOffsetRect(&dinahs[numDynamics].dest, where->left, where->top);
		dinahs[numDynamics].whole = dinahs[numDynamics].dest;
		dinahs[numDynamics].hVel = 0;
		dinahs[numDynamics].vVel = 0;
		dinahs[numDynamics].count = 0;
		dinahs[numDynamics].frame = 0;
		dinahs[numDynamics].timer = RandomInt(60) + 15;
		dinahs[numDynamics].position = 0;
		dinahs[numDynamics].room = room;
		dinahs[numDynamics].byte0 = (Byte)index;
		dinahs[numDynamics].byte1 = 0;
		dinahs[numDynamics].moving = false;
		dinahs[numDynamics].active = isOn;
		break;
		
		case kToaster:
		dinahs[numDynamics].dest = breadSrc[0];
		CenterRectInRect(&dinahs[numDynamics].dest, where);
		VOffsetRect(&dinahs[numDynamics].dest, 
				where->top - dinahs[numDynamics].dest.top);
		dinahs[numDynamics].whole = dinahs[numDynamics].dest;
		dinahs[numDynamics].hVel = where->top + 2;	// hVel used as clip
		position = who->data.g.height;				// reverse engineer init. vel.
		velocity = 0;
		do
		{
			velocity++;
			position -= velocity;
		}
		while (position > 0);
		dinahs[numDynamics].vVel = -velocity;
		dinahs[numDynamics].count = velocity;		// count = initial velocity
		dinahs[numDynamics].frame = (short)who->data.g.delay * 3;
		dinahs[numDynamics].timer = dinahs[numDynamics].frame;
		dinahs[numDynamics].position = 0;			// launch/idle state
		dinahs[numDynamics].room = room;
		dinahs[numDynamics].byte0 = (Byte)index;
		dinahs[numDynamics].byte1 = 0;
		dinahs[numDynamics].moving = false;
		dinahs[numDynamics].active = isOn;
		break;
		
		case kMacPlus:
		dinahs[numDynamics].dest = plusScreen1;
		ZeroRectCorner(&dinahs[numDynamics].dest);
		QOffsetRect(&dinahs[numDynamics].dest, 
				where->left + playOriginH + 10, 
				where->top + playOriginV + 7);
		dinahs[numDynamics].whole = dinahs[numDynamics].dest;
		dinahs[numDynamics].hVel = 0;
		dinahs[numDynamics].vVel = 0;
		dinahs[numDynamics].count = 0;
		dinahs[numDynamics].frame = 0;
		dinahs[numDynamics].timer = 0;
		dinahs[numDynamics].position = 0;
		dinahs[numDynamics].room = room;
		dinahs[numDynamics].byte0 = (Byte)index;
		dinahs[numDynamics].byte1 = 0;
		dinahs[numDynamics].moving = false;
		dinahs[numDynamics].active = isOn;
		break;
		
		case kTV:
		dinahs[numDynamics].dest = tvScreen1;
		ZeroRectCorner(&dinahs[numDynamics].dest);
		QOffsetRect(&dinahs[numDynamics].dest, 
				where->left + playOriginH + 17, 
				where->top + playOriginV + 10);
		dinahs[numDynamics].whole = dinahs[numDynamics].dest;
		dinahs[numDynamics].hVel = 0;
		dinahs[numDynamics].vVel = 0;
		dinahs[numDynamics].count = 0;
		dinahs[numDynamics].frame = 0;
		dinahs[numDynamics].timer = 0;
		dinahs[numDynamics].position = 0;
		dinahs[numDynamics].room = room;
		dinahs[numDynamics].byte0 = (Byte)index;
		dinahs[numDynamics].byte1 = 0;
		dinahs[numDynamics].moving = false;
		dinahs[numDynamics].active = isOn;
		break;
		
		case kCoffee:
		dinahs[numDynamics].dest = coffeeLight1;
		ZeroRectCorner(&dinahs[numDynamics].dest);
		QOffsetRect(&dinahs[numDynamics].dest, 
				where->left + playOriginH + 32, 
				where->top + playOriginV + 57);
		dinahs[numDynamics].whole = dinahs[numDynamics].dest;
		dinahs[numDynamics].hVel = 0;
		dinahs[numDynamics].vVel = 0;
		dinahs[numDynamics].count = 0;
		dinahs[numDynamics].frame = 0;
		if (isOn)
			dinahs[numDynamics].timer = 200;
		else
			dinahs[numDynamics].timer = 0;
		dinahs[numDynamics].position = 0;
		dinahs[numDynamics].room = room;
		dinahs[numDynamics].byte0 = (Byte)index;
		dinahs[numDynamics].byte1 = 0;
		dinahs[numDynamics].moving = false;
		dinahs[numDynamics].active = isOn;
		break;
		
		case kOutlet:
		dinahs[numDynamics].dest = outletSrc[0];
		ZeroRectCorner(&dinahs[numDynamics].dest);
		QOffsetRect(&dinahs[numDynamics].dest, 
				where->left + playOriginH, 
				where->top + playOriginV);
		dinahs[numDynamics].whole = dinahs[numDynamics].dest;
		dinahs[numDynamics].hVel = numLights;
		dinahs[numDynamics].vVel = 0;
		dinahs[numDynamics].count = ((short)who->data.g.delay * 6) / kTicksPerFrame;
		dinahs[numDynamics].frame = 0;
		dinahs[numDynamics].timer = dinahs[numDynamics].count;
		dinahs[numDynamics].position = 0;			// launch/idle state
		dinahs[numDynamics].room = room;
		dinahs[numDynamics].byte0 = (Byte)index;
		dinahs[numDynamics].byte1 = 0;
		dinahs[numDynamics].moving = false;
		dinahs[numDynamics].active = isOn;
		break;
		
		case kVCR:
		dinahs[numDynamics].dest = vcrTime1;
		ZeroRectCorner(&dinahs[numDynamics].dest);
		QOffsetRect(&dinahs[numDynamics].dest, 
				where->left + playOriginH + 64, 
				where->top + playOriginV + 6);
		dinahs[numDynamics].whole = dinahs[numDynamics].dest;
		dinahs[numDynamics].hVel = 0;
		dinahs[numDynamics].vVel = 0;
		dinahs[numDynamics].count = 0;
		dinahs[numDynamics].frame = 0;
		if (isOn)
			dinahs[numDynamics].timer = 115;
		else
			dinahs[numDynamics].timer = 0;
		dinahs[numDynamics].position = 0;
		dinahs[numDynamics].room = room;
		dinahs[numDynamics].byte0 = (Byte)index;
		dinahs[numDynamics].byte1 = 0;
		dinahs[numDynamics].moving = false;
		dinahs[numDynamics].active = isOn;
		break;
		
		case kStereo:
		dinahs[numDynamics].dest = stereoLight1;
		ZeroRectCorner(&dinahs[numDynamics].dest);
		QOffsetRect(&dinahs[numDynamics].dest, 
				where->left + playOriginH + 56, 
				where->top + playOriginV + 20);
		dinahs[numDynamics].whole = dinahs[numDynamics].dest;
		dinahs[numDynamics].hVel = 0;
		dinahs[numDynamics].vVel = 0;
		dinahs[numDynamics].count = 0;
		dinahs[numDynamics].frame = 0;
		dinahs[numDynamics].timer = 0;
		dinahs[numDynamics].position = 0;
		dinahs[numDynamics].room = room;
		dinahs[numDynamics].byte0 = (Byte)index;
		dinahs[numDynamics].byte1 = 0;
		dinahs[numDynamics].moving = false;
		dinahs[numDynamics].active = isOn;
		break;
		
		case kMicrowave:
		dinahs[numDynamics].dest = microOn;
		ZeroRectCorner(&dinahs[numDynamics].dest);
		QOffsetRect(&dinahs[numDynamics].dest, 
				where->left + playOriginH + 14, 
				where->top + playOriginV + 13);
		dinahs[numDynamics].dest.right = dinahs[numDynamics].dest.left + 48;
		dinahs[numDynamics].whole = dinahs[numDynamics].dest;
		dinahs[numDynamics].hVel = 0;
		dinahs[numDynamics].vVel = 0;
		dinahs[numDynamics].count = 0;
		dinahs[numDynamics].frame = 0;
		dinahs[numDynamics].timer = 0;
		dinahs[numDynamics].position = 0;
		dinahs[numDynamics].room = room;
		dinahs[numDynamics].byte0 = (Byte)index;
		dinahs[numDynamics].byte1 = 0;
		dinahs[numDynamics].moving = false;
		dinahs[numDynamics].active = isOn;
		break;
		
		case kBalloon:
		dinahs[numDynamics].dest = balloonSrc[0];
		ZeroRectCorner(&dinahs[numDynamics].dest);
		QOffsetRect(&dinahs[numDynamics].dest, where->left, 0);
		dinahs[numDynamics].dest.bottom = kBalloonStart;
		dinahs[numDynamics].dest.top = dinahs[numDynamics].dest.bottom - 
				RectTall(&balloonSrc[0]);
		dinahs[numDynamics].whole = dinahs[numDynamics].dest;
		dinahs[numDynamics].hVel = 0;
		dinahs[numDynamics].vVel = -2;
		dinahs[numDynamics].count = ((short)who->data.h.delay * 6) / kTicksPerFrame;
		dinahs[numDynamics].frame = 0;
		dinahs[numDynamics].timer = dinahs[numDynamics].count;
		dinahs[numDynamics].position = 0;
		dinahs[numDynamics].room = room;
		dinahs[numDynamics].byte0 = (Byte)index;
		dinahs[numDynamics].byte1 = 0;
		dinahs[numDynamics].moving = false;
		dinahs[numDynamics].active = isOn;			// initially idle
		break;
		
		case kCopterLf:
		case kCopterRt:
		dinahs[numDynamics].dest = copterSrc[0];
		ZeroRectCorner(&dinahs[numDynamics].dest);
		QOffsetRect(&dinahs[numDynamics].dest, where->left, 0);
		dinahs[numDynamics].dest.top = kCopterStart;
		dinahs[numDynamics].dest.bottom = dinahs[numDynamics].dest.top + 
				RectTall(&copterSrc[0]);
		dinahs[numDynamics].whole = dinahs[numDynamics].dest;
		if (what == kCopterLf)
			dinahs[numDynamics].hVel = -1;
		else
			dinahs[numDynamics].hVel = 1;
		dinahs[numDynamics].vVel = 2;
		dinahs[numDynamics].count = ((short)who->data.h.delay * 6) / kTicksPerFrame;
		dinahs[numDynamics].frame = 0;
		dinahs[numDynamics].timer = dinahs[numDynamics].count;
		dinahs[numDynamics].position = dinahs[numDynamics].dest.left;
		dinahs[numDynamics].room = room;
		dinahs[numDynamics].byte0 = (Byte)index;
		dinahs[numDynamics].byte1 = 0;
		dinahs[numDynamics].moving = false;
		dinahs[numDynamics].active = isOn;			// initially idle
		break;
		
		case kDartLf:
		case kDartRt:
		dinahs[numDynamics].dest = dartSrc[0];
		ZeroRectCorner(&dinahs[numDynamics].dest);
		if (what == kDartLf)
		{
			QOffsetRect(&dinahs[numDynamics].dest, 
					kRoomWide - RectWide(&dartSrc[0]), where->top);
			dinahs[numDynamics].hVel = -kDartVelocity;
			dinahs[numDynamics].frame = 0;
		}
		else
		{
			QOffsetRect(&dinahs[numDynamics].dest, 0, where->top);
			dinahs[numDynamics].hVel = kDartVelocity;
			dinahs[numDynamics].frame = 2;
		}
		dinahs[numDynamics].whole = dinahs[numDynamics].dest;
		dinahs[numDynamics].vVel = 2;
		dinahs[numDynamics].count = ((short)who->data.h.delay * 6) / kTicksPerFrame;
		dinahs[numDynamics].timer = dinahs[numDynamics].count;
		dinahs[numDynamics].position = dinahs[numDynamics].dest.top;
		dinahs[numDynamics].byte0 = (Byte)index;
		dinahs[numDynamics].byte1 = 0;
		dinahs[numDynamics].moving = false;
		dinahs[numDynamics].active = isOn;			// initially idle
		break;
		
		case kBall:
		dinahs[numDynamics].dest = ballSrc[0];
		ZeroRectCorner(&dinahs[numDynamics].dest);
		QOffsetRect(&dinahs[numDynamics].dest, 
				where->left, where->top);
		dinahs[numDynamics].whole = dinahs[numDynamics].dest;
		dinahs[numDynamics].hVel = 0;
		position = who->data.h.length;			// reverse engineer init. vel.
		velocity = 0;
		evenFrame = true;
		lilFrame = true;
		do
		{
			if (lilFrame)
				velocity++;
			lilFrame = !lilFrame;
			position -= velocity;
		}
		while (position > 0);
		dinahs[numDynamics].vVel = -velocity;
		dinahs[numDynamics].moving = false;
		dinahs[numDynamics].count = -velocity;	// count = initial velocity
		dinahs[numDynamics].frame = 0;
		dinahs[numDynamics].timer = 0;
		dinahs[numDynamics].position = dinahs[numDynamics].dest.bottom;
		dinahs[numDynamics].room = room;
		dinahs[numDynamics].byte0 = (Byte)index;
		dinahs[numDynamics].byte1 = 0;
		dinahs[numDynamics].active = isOn;
		break;
		
		case kDrip:
		dinahs[numDynamics].dest = dripSrc[0];
		CenterRectInRect(&dinahs[numDynamics].dest, where);
		VOffsetRect(&dinahs[numDynamics].dest, 
				where->top - dinahs[numDynamics].dest.top);
		dinahs[numDynamics].whole = dinahs[numDynamics].dest;
		dinahs[numDynamics].hVel = dinahs[numDynamics].dest.top;	// remember
		dinahs[numDynamics].vVel = 0;
		dinahs[numDynamics].count = ((short)who->data.h.delay * 6) / kTicksPerFrame;
		dinahs[numDynamics].frame = 3;
		dinahs[numDynamics].timer = dinahs[numDynamics].count;
		dinahs[numDynamics].position = dinahs[numDynamics].dest.top + 
				who->data.h.length;
		dinahs[numDynamics].room = room;
		dinahs[numDynamics].byte0 = (Byte)index;
		dinahs[numDynamics].byte1 = 0;
		dinahs[numDynamics].moving = false;
		dinahs[numDynamics].active = isOn;
		break;
		
		case kFish:
		dinahs[numDynamics].dest = fishSrc[0];
		QOffsetRect(&dinahs[numDynamics].dest, 
				where->left + 10, where->top + 8);
		dinahs[numDynamics].whole = dinahs[numDynamics].dest;
		dinahs[numDynamics].hVel = ((short)who->data.h.delay * 6) / kTicksPerFrame;
		position = who->data.g.height;			// reverse engineer init. vel.
		velocity = 0;
		evenFrame = true;
		lilFrame = true;
		do
		{
			if (lilFrame)
				velocity++;
			lilFrame = !lilFrame;
			position -= velocity;
		}
		while (position > 0);
		dinahs[numDynamics].vVel = -velocity;
		dinahs[numDynamics].count = -velocity;	// count = initial velocity
		dinahs[numDynamics].frame = 0;
		dinahs[numDynamics].timer = dinahs[numDynamics].hVel;
		dinahs[numDynamics].position = dinahs[numDynamics].dest.bottom;
		dinahs[numDynamics].room = room;
		dinahs[numDynamics].byte0 = (Byte)index;
		dinahs[numDynamics].byte1 = 0;
		dinahs[numDynamics].moving = false;
		dinahs[numDynamics].active = isOn;
		break;
		
		default:
		return (-1);
		break;
	}
	
	numDynamics++;
	
	return (numDynamics - 1);
}

