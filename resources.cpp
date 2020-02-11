#include "resources.h"

//@TODO will need to figure out logic for what happens if amount needed is greater
void Resource::Acquire(int amountNeeded)
{
	/*if (amountNeeded > _resourceCount)
		//do some stuff
		//subtract number necessary then check threshold
		//wait 
	else{
		_resourceCount -= amountNeeded;
	}*/
	
	_resourceCount -= amountNeeded;
	
	if (_resourceCount < _resourceThreshold){
		//Schedule an ordering event... may need to have an EA/EM with this class for that
	}
}

void Resource::Release(int amountToRelease)
{
	_resourceCount += amountToRelease;
}


bool Resource::isAvailable(int amountNeeded)
{
	if (amountNeeded > _resourceCount)
		return false;
	else{
		return true;
	}
}

void Resource::failResource()
{
	//@TODO write the algorithm for a resource failure essentially is just scheduling an event
	//so needed EA and EM
}

void Resource::restoreResource()
{
	//May probably need an associated Event to Execute function call. 
	_resourceCount++;
}
