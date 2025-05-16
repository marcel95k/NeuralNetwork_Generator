#pragma once

// Network parameters

	#define MIN_OUTPUTNEURONS	2
	#define MAX_OUTPUTNEURONS	30
	#define INPUTSIZE			400
	#define VALIDATION_SHARE	30


// Learning parameters

	#define DEFAULT_EPSILON			0.4		// 0.4
	#define DEFAULT_EPSILON_DECAY	0.9		// 0.9
	#define DEFAULT_MOMENTUMFACTOR	0.9		// 0.9
	#define DEFAULT_EPOCHS			100		// 100


// Drawing parameters

	#define DRAWRADIUS_MAX			20
	#define DRAWRADIUS_MIN			3


// Filenames and Foldernames

	#define SAVED_NETWORKS			"Networks/saved_networks.txt"
	#define NETWORKS				"Networks/"		
	#define TRAININGDATA			"Trainingdata/"
	#define VALIDATIONDATA			"Validationdata/"


// Debug

	//#define DEBUG_SHOW_TRAININGFILENAME
	//#define DEBUG_SHOW_VALIDATIONFILENAME
	//#define DEBUG_SHOW_FITNESSDETAILS


// ASSERT-Makro

	#define NN_ASSERT(condition, message)							\
	    do {														\
	        if (!(condition)) {										\
	            std::cerr << "Assertion failed: (" #condition		\
	                      << "), function " << __FUNCTION__			\
	                      << ", file " << __FILE__					\
	                      << ", line " << __LINE__ << ".\n"			\
	                      << "Message: " << message << std::endl;	\
							system("pause")			;				\
	        }														\
	    } while (false)												\
