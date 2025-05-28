#pragma once

#include "ExceptionHandling.h"
#include "Defines.h"

#include "Network.h"

namespace ERRORHANDLING {

	namespace FLAGCHECK {
		void checkNetBeforeTraining(const Network& _network);
		void checkNetBeforeSaving(const Network& _network);
		void checkNetBeforeDeleting(const Network& _network);
		void checkNetBeforeInitializing(const Network& _network);

	}
}

