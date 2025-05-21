#pragma once

/*------------------------------------------------------------*/
/* Reccuring constant variables throughout the entire program */
/*------------------------------------------------------------*/

/*Still left to implement:
	
*/



// Network parameters

	#define MIN_OUTPUTNEURONS	2			// 2
	#define MAX_OUTPUTNEURONS	30			// 30
	#define INPUTSIZE			400			// 400
	#define VALIDATION_SHARE	30			// 30


// Learning parameters

	#define DEFAULT_EPSILON			0.4		// 0.4
	#define DEFAULT_EPSILON_DECAY	0.9		// 0.9
	#define DEFAULT_MOMENTUMFACTOR	0.9		// 0.9
	#define DEFAULT_EPOCHS			100		// 100


// Drawing parameters

	#define DRAWRADIUS_MAX			20		// 20
	#define DRAWRADIUS_MIN			3		// 3


// Filenames and Foldernames

	#define SAVED_NETWORKS			"Networks/saved_networks.txt"	// "Networks/saved_networks.txt"
	#define NETWORKS				"Networks/"						// "Networks/"
	#define TRAININGDATA			"Trainingdata/"					// "Trainingdata/"	
	#define VALIDATIONDATA			"Validationdata/"				// "Validationdata/"


// Debug

	//#define DEBUG_SHOW_TRAININGFILENAME
	//#define DEBUG_SHOW_VALIDATIONFILENAME
	//#define DEBUG_SHOW_FITNESSDETAILS_CORRECT
	//#define DEBUG_SHOW_FITNESSDETAILS_WRONG


// ASSERT-Makro

#ifdef NDEBUG
	#define NN_ASSERT(conditon, message) ((void)0)
#else
	#define NN_ASSERT(condition, message)								\
	    do {															\
	        if (!(condition)) {											\
	            std::cerr << "\n\033[31mAssertion failed: (" #condition	\
	                      << ")\033[0m, function " << __FUNCTION__		\
	                      << ", file " << __FILE__						\
	                      << ", line " << __LINE__ << ".\n"				\
	                      << "Message: " << message << std::endl;		\
							system("pause")			;					\
	        }															\
			else if ((condition)) {										\
	            std::cerr << "\n\033[32mAssertion passed: (" #condition	\
	                      << ")\033[0m, function " << __FUNCTION__		\
	                      << ", file " << __FILE__						\
	                      << ", line " << __LINE__ << ".\n"				\
	                      << "Message: " << message << std::endl;		\
							system("pause")			;					\
	        }															\
	    } while (false)													\

#endif
