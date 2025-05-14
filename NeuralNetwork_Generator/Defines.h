#pragma once

// Network parameters

	#define MIN_OUTPUTNEURONS	2
	#define MAX_OUTPUTNEURONS	30
	#define INPUTSIZE			400


// Learning parameters

	#define DEFAULT_EPSILON			0.4
	#define DEFAULT_EPSILON_DECAY	0.9
	#define DEFAULT_MOMENTUMFACTOR	0.9
	#define DEFAULT_EPOCHS			100


// Drawing parameters

	#define DRAWRADIUS_MAX			20
	#define DRAWRADIUS_MIN			3


// Foldernames

	#define TRAININGDATA			"Trainingdata/"
	#define VALIDATIONDATA			"Validationdata/"


// Debug

	//#define DEBUG_SHOW_TRAININGFILENAME
