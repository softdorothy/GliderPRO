//============================================================================
//----------------------------------------------------------------------------
//								GliderStructs.h
//----------------------------------------------------------------------------
//============================================================================


#include <QDOffscreen.h>


typedef struct
{
	Point		topLeft;				// 4
	short		distance;				// 2
	Boolean		initial;				// 1
	Boolean		state;					// 1		              F. lf. dn. rt. up
	Byte		vector;					// 1		| x | x | x | x | 8 | 4 | 2 | 1 |
	Byte		tall;					// 1
} blowerType;							// total = 10

typedef struct
{
	Rect		bounds;					// 8
	short		pict;					// 2
} furnitureType;						// total = 10

typedef struct
{
	Point		topLeft;				// 4
	short		length;					// 2 grease spill
	short		points;					// 2 invis bonus
	Boolean		state;					// 1
	Boolean		initial;				// 1
} bonusType;							// total = 10

typedef struct
{
	Point		topLeft;				// 4
	short		tall;					// 2 invis transport
	short		where;					// 2
	Byte		who;					// 1
	Byte		wide;					// 1
} transportType;						// total = 10

typedef struct
{
	Point		topLeft;				// 4
	short		delay;					// 2
	short		where;					// 2
	Byte		who;					// 1
	Byte		type;					// 1
} switchType;							// total = 10

typedef struct
{
	Point		topLeft;				// 4
	short		length;					// 2
	Byte		byte0;					// 1
	Byte		byte1;					// 1
	Boolean		initial;				// 1
	Boolean		state;					// 1
} lightType;							// total = 10

typedef struct
{
	Point		topLeft;				// 4
	short		height;					// 2 toaster, pict ID
	Byte		byte0;					// 1
	Byte		delay;					// 1
	Boolean		initial;				// 1
	Boolean		state;					// 1
} applianceType;						// total = 10

typedef struct
{
	Point		topLeft;					// 4
	short		length;						// 2
	Byte		delay;						// 1
	Byte		byte0;						// 1
	Boolean		initial;					// 1
	Boolean		state;						// 1
} enemyType;								// total = 10

typedef struct
{
	Rect		bounds;						// 8
	short		pict;						// 2
} clutterType;								// total = 10

typedef struct
{
	short		what;						// 2
	union
	{
		blowerType		a;
		furnitureType	b;
		bonusType		c;
		transportType	d;
		switchType		e;
		lightType		f;
		applianceType	g;
		enemyType		h;
		clutterType		i;
	} data;									// 10
} objectType, *objectPtr;					// total = 12

typedef struct
{
	Str31			banner;					// 32		= 32
	Str15			names[kMaxScores];		// 16 * 10	= 160
	long			scores[kMaxScores];		// 4 * 10	= 40
	unsigned long	timeStamps[kMaxScores];	// 4 * 10	= 40
	short			levels[kMaxScores];		// 2 * 10	= 20
} scoresType;								// total 	= 292

typedef struct
{
	short		version;					// 2
	short		wasStarsLeft;				// 2
	long		timeStamp;					// 4
	Point		where;						// 4
	long		score;						// 4
	long		unusedLong;					// 4
	long		unusedLong2;				// 4
	short		energy;						// 2
	short		bands;						// 2
	short		roomNumber;					// 2
	short		gliderState;				// 2
	short		numGliders;					// 2
	short		foil;						// 2
	short		unusedShort;				// 2
	Boolean		facing;						// 1
	Boolean		showFoil;					// 1
} gameType;									// total = 40

typedef struct
{
	short		unusedShort;				// 2
	Byte		unusedByte;					// 1
	Boolean		visited;					// 1
	objectType	objects[kMaxRoomObs];		// 24 * 12
} savedRoom, *saveRoomPtr;					// total = 292

typedef struct
{
	FSSpec		house;						// 70
	short		version;					// 2
	short		wasStarsLeft;				// 2
	long		timeStamp;					// 4
	Point		where;						// 4
	long		score;						// 4
	long		unusedLong;					// 4
	long		unusedLong2;				// 4
	short		energy;						// 2
	short		bands;						// 2
	short		roomNumber;					// 2
	short		gliderState;				// 2
	short		numGliders;					// 2
	short		foil;						// 2
	short		nRooms;						// 2
	Boolean		facing;						// 1
	Boolean		showFoil;					// 1
	savedRoom	savedData[];				// 4
} game2Type, *gamePtr;						// total = 114

typedef struct
{
	Str27		name;						// 28
	short		bounds;						// 2
	Byte		leftStart;					// 1
	Byte		rightStart;					// 1
	Byte		unusedByte;					// 1
	Boolean		visited;					// 1
	short		background;					// 2
	short		tiles[kNumTiles];			// 2 * 8
	short		floor, suite;				// 2 + 2
	short		openings;					// 2
	short		numObjects;					// 2
	objectType	objects[kMaxRoomObs];		// 24 * 12
} roomType, *roomPtr;						// total = 348

typedef struct
{
	short		version;					// 2
	short		unusedShort;				// 2
	long		timeStamp;					// 4
	long		flags;						// 4 (bit 0 = wardBit)
	Point		initial;					// 4
	Str255		banner;						// 256
	Str255		trailer;					// 256
	scoresType	highScores;					// 292
	gameType	savedGame;					// 40
	Boolean		hasGame;					// 1
	Boolean		unusedBoolean;				// 1
	short		firstRoom;					// 2
	short		nRooms;						// 2
	roomType	rooms[];					// 348 * nRooms
} houseType, *housePtr, **houseHand;		// total = 866 +

typedef struct
{
	Rect		src, mask, dest, whole;
	Rect		destShadow, wholeShadow;
	Rect		clip, enteredRect;
	long		leftKey, rightKey;
	long		battKey, bandKey;
	short		hVel, vVel;
	short		wasHVel, wasVVel;
	short		vDesiredVel, hDesiredVel;
	short		mode, frame, wasMode;
	Boolean		facing, tipped;
	Boolean		sliding, ignoreLeft, ignoreRight;
	Boolean		fireHeld, which;
	Boolean		heldLeft, heldRight;
	Boolean		dontDraw, ignoreGround;
} gliderType, *gliderPtr;

typedef struct
{
	Rect		bounds;
	short		action;
	short		who;
	Boolean		isOn, stillOver;
	Boolean		doScrutinize;
} hotObject, *hotPtr;

typedef struct
{
	Rect		dest;
	GWorldPtr	map;
	short		where;
	short		who;
} savedType, *savedPtr;

typedef struct
{
	Rect		bounds;
	short		mode;
} sparkleType, *sparklePtr;

typedef struct
{
	Rect		dest, whole;
	short		start;
	short		stop;
	short		mode;
	short		loops;
	short		hVel, vVel;
} flyingPtType, *flyingPtPtr;

typedef struct
{
	Rect		dest, src;
	short		mode;
	short		who;
} flameType, *flamePtr;

typedef struct
{
	Rect		dest, src;
	short		mode, where;
	short		who, link;
	Boolean		toOrFro, active;
} pendulumType, *pendulumPtr;

typedef struct
{
	Boolean		left;
	Boolean		top;
	Boolean		right;
	Boolean		bottom;
} boundsType, *boundsPtr, **boundsHand;

typedef struct
{
	Rect		dest;
	short		mode, count;
	short		hVel, vVel;
} bandType, *bandPtr;

typedef struct
{
	short		srcRoom, srcObj;
	short		destRoom, destObj;
} linksType, *linksPtr;

typedef struct
{
	Rect		dest;
	short		mapNum, mode;
	short		who, where;
	short		start, stop;
	short		frame, hotNum;
	Boolean		isRight;
} greaseType, *greasePtr;

typedef struct
{
	Rect		dest, src;
	short		mode, who;
	short		link, where;
} starType, *starPtr;

typedef struct
{
	Rect		bounds;
	short		frame;
} shredType, *shredPtr;

typedef struct
{
	Rect		dest;
	Rect		whole;
	short		hVel, vVel;
	short		type, count;
	short		frame, timer;
	short		position, room;
	Byte		byte0, byte1;
	Boolean		moving, active;
} dynaType, *dynaPtr;

typedef struct
{
	short		roomNum;	// room # object in (real number)
	short		objectNum;	// obj. # in house (real number)
	short		roomLink;	// room # object linked to (if any)
	short		objectLink;	// obj. # object linked to (if any)
	short		localLink;	// index in master list if exists
	short		hotNum;		// index into active rects (if any)
	short		dynaNum;	// index into dinahs (if any)
	objectType	theObject;	// actual object data
} objDataType, *objDataPtr;

typedef struct
{
	long		frame;
	char		key;
	char		padding;
} demoType, *demoPtr;

typedef struct
{
	short		room;
	short		object;
} retroLink, *retroLinkPtr;


