#include "PrecompiledHeaders.h"
#include "ThreadRunMethod.h"
#include "Core.h"


IMPLEMENT_CLASS_INFO(ThreadRunMethod)


ThreadRunMethod::ThreadRunMethod(const kstl::string& name,CLASS_NAME_TREE_ARG) : CoreModifiable(name,PASS_CLASS_NAME_TREE_ARG)
{
	myThread=0;
}

ThreadRunMethod::~ThreadRunMethod()
{

}    

bool	ThreadRunMethod::addItem(const CMSP& item, ItemPosition pos DECLARE_LINK_NAME)
{
	if(item->isSubType(Thread::myClassID))
	{
		// there's already a thread in there
		if((myThread)&&(item !=myThread))
		{
			CMSP toDel(myThread, GetRefTag{});
			removeItem(toDel PASS_LINK_NAME(linkName));
		}
		myThread=(Thread*)item.get();
	}
	return CoreModifiable::addItem(item,pos PASS_LINK_NAME(linkName));
}

bool	ThreadRunMethod::removeItem(const CMSP& item DECLARE_LINK_NAME)
{
	if(item->isSubType(Thread::myClassID))
	{
		if(item == myThread)
		{
            if(myThread->GetCurrentState() == Thread::RUNNING)
                EndThread();
            
			myThread=0;
		}
	}
	return CoreModifiable::removeItem(item PASS_LINK_NAME(linkName));
}
 
void ThreadRunMethod::InitModifiable()
{
	CoreModifiable::InitModifiable();

	if(myThread)
	{
		StartThread();
	}
}

