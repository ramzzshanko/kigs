#ifndef _COREACTIONKEYFRAME_H_
#define _COREACTIONKEYFRAME_H_

#include "CoreAction.h"
#include "CoreValue.h"

class CoreActionKeyFrameBase : public CoreAction
{
public:

	CoreActionKeyFrameBase() : CoreAction(),myKeyFrameCount(0),myLastKeyIndex(-1),myTimeArray(0)
	{
	}

	virtual void	setStartTime(kdouble t)
	{
		CoreAction::setStartTime(t);
		myLastKeyIndex=-1;
	}

	virtual ~CoreActionKeyFrameBase()
	{
		if(myTimeArray)
		{
			delete[]	myTimeArray;
		}
	}


protected:

	virtual bool	protectedUpdate(kdouble time);

	virtual void	protectedSetValue(int index)=0;

	unsigned int		myKeyFrameCount;
	int					myLastKeyIndex;

	kdouble*			myTimeArray;

};

template<typename dataType>
class CoreActionKeyFrame : public CoreActionKeyFrameBase
{
public:

	CoreActionKeyFrame() : CoreActionKeyFrameBase(),myKeyFrameArray(0)
	{}

	virtual void init(CoreSequence* sequence,CoreVector* params)
	{
		myTarget = sequence->getTarget();

		kstl::string readstring;
		(*params)[0]->getValue(readstring);
		myTarget = checkSubTarget(readstring);

		myParamID = CharToID::GetID(readstring);

		// stock in list before creating the final array
		kstl::vector<dataType>	L_values;
		kstl::vector<kdouble>	L_times;
		float readfloat;
		dataType	readPoint;

		unsigned int i;
		for (i = 1; i < params->size(); i += 2) // read each (time + val) couples
		{
			(*params)[i]->getValue(readfloat);
			L_times.push_back(readfloat);
			(*params)[i + 1]->getValue(readPoint);
			L_values.push_back(readPoint);

		}

		if (L_values.size() == 0)
		{
			// mhh not good
			return;
		}

		myKeyFrameCount = (unsigned int)L_values.size();
		myDuration = L_times[L_times.size() - 1];

		myKeyFrameArray = new dataType[L_values.size()];
		myTimeArray = new kdouble[L_values.size()];

		for (i = 0; i < L_values.size(); i++)
		{
			myKeyFrameArray[i] = L_values[i];
			myTimeArray[i] = L_times[i];
		}
	}


	virtual ~CoreActionKeyFrame()
	{
		if(myKeyFrameArray)
		{
			delete[]	myKeyFrameArray;
		}
	}

protected:

	inline void	protectedSetValue(int index)
	{
		myTarget->setValue(myParamID, myKeyFrameArray[index]);
	}

	dataType*			myKeyFrameArray;
	
};

#endif //_COREACTIONKEYFRAME_H_