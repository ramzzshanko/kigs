#ifndef _MODULETHREAD_H_
#define _MODULETHREAD_H_

#include "ModuleBase.h"

/*! \defgroup Thread Thread module
 *  base thread management
*/

// ****************************************
// * ModuleThread class
// * --------------------------------------
/*!  \class ModuleThread
	 \ingroup Thread
*/
// ****************************************

class ModuleThread : public ModuleBase
{
public:

	DECLARE_CLASS_INFO(ModuleThread,ModuleBase,Thread)

	//! constructor
    ModuleThread(const kstl::string& name,DECLARE_CLASS_NAME_TREE_ARG);
         
	//! init module
    void Init(KigsCore* core, const kstl::vector<CoreModifiableAttribute*>* params) override;
	//! close module
	void Close() override;
            
	//! update module
	void Update(const Timer& timer, void* addParam) override;
               
protected:
	//! destructor
    virtual ~ModuleThread();    


}; 

#endif //_MODULETHREAD_H_
