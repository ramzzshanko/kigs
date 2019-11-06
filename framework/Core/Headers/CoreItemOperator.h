#ifndef _COREITEMOPERATOR_H
#define _COREITEMOPERATOR_H

#include "CoreVector.h"
#include "CoreValue.h"
#include "AsciiParserUtils.h"

#include "kstlunordered_map.h"

struct CoreItemOperatorStruct
{
	char	myOp;
	int		myPos;
	int		mySize;
	CoreItem* myOp1, *myOp2;
	int		myPriority;
};

struct CoreItemEvaluationContext
{
	kstl::unordered_map<unsigned int, RefCountedBaseClass*>	myVariableList;
};

extern	CoreItemEvaluationContext*	myCurrentCoreItemEvaluationContext;
extern void	SetCoreItemOperatorContext(CoreItemEvaluationContext* set);
extern void	ReleaseCoreItemOperatorContext();

typedef     CoreVector* (*CoreItemOperatorCreateMethod)();

template<typename operandType>
class CoreItemOperator : public CoreVector
{
public:

	class SpecificOperator
	{
	public:
		kstl::string	myKeyWord;	// 3 letter keyword
		CoreItemOperatorCreateMethod	myCreateMethod;
	};
	class ConstructContext
	{
	public:
		kstl::unordered_map<kstl::string, CoreItemOperatorCreateMethod>	myMap;
		CoreModifiable*											myTarget;
	};

	virtual inline operator bool() const
	{
		KIGS_ERROR("cast operator called on base CoreItem", 2);
		return false;
	}

	virtual inline operator kfloat() const
	{
		KIGS_ERROR("cast operator called on base CoreItem", 2);
		return 0.0f;
	}

	virtual inline operator int() const
	{
		KIGS_ERROR("cast operator called on base CoreItem", 2);
		return 0;
	}

	virtual inline operator unsigned int() const
	{
		KIGS_ERROR("cast operator called on base CoreItem", 2);
		return 0;
	}

	virtual inline operator kstl::string() const
	{
		KIGS_ERROR("cast operator called on base CoreItem", 2);
		return "";
	}

	virtual inline operator usString() const
	{
		KIGS_ERROR("cast operator called on base CoreItem", 2);
		return "";
	}

	virtual inline operator Point2D() const
	{
		Point2D result;
		KIGS_ERROR("cast operator called on base CoreItem", 2);
		return result;
	}

	virtual inline operator Point3D() const
	{
		Point3D result;
		KIGS_ERROR("cast operator called on base CoreItem", 2);
		return result;
	}

	static void	defaultOperandTypeInit(operandType& _value)
	{
		_value = (operandType)0;
	}

	static CoreItem&	Construct(const kstl::string& formulae, CoreModifiable* target, kstl::vector<SpecificOperator>* specificList=0);
	static CoreItem&	Construct(const kstl::string& formulae, CoreModifiable* target, const kstl::unordered_map<kstl::string, CoreItemOperatorCreateMethod>&	myMap);
	static void	ConstructContextMap(kstl::unordered_map<kstl::string, CoreItemOperatorCreateMethod>&	myMap, kstl::vector<SpecificOperator>* specificList = 0);

protected:



	static CoreItem&	Parse(AsciiParserUtils& formulae, ConstructContext& context);
	static kstl::vector<CoreItemOperatorStruct>	FindFirstLevelOperators(AsciiParserUtils& formulae, ConstructContext& context);
	static kstl::vector<kstl::string>	FindFirstLevelParams(AsciiParserUtils& formulae, ConstructContext& context);
	static CoreItemOperator<operandType>* getOperator(const kstl::string& keyword, ConstructContext& context);
	static CoreItem* getVariable(const kstl::string& keyword);
};


template<>
inline CoreItemOperator<bool>::operator kfloat() const
{
	bool result = (bool)*this;
	return result?1.0f:0.0f;
}

template<>
inline CoreItemOperator<bool>::operator int() const
{
	bool result = (bool)*this;
	return result ? 1 : 0;
}

template<>
inline CoreItemOperator<bool>::operator unsigned int() const
{
	bool result = (bool)*this;
	return result ? 1 : 0;
}

template<>
inline CoreItemOperator<bool>::operator kstl::string() const
{
	bool result = (bool)*this;
	return result ? "true" : "false";
}

template<>
inline CoreItemOperator<bool>::operator usString() const
{
	bool result = (bool)*this;
	return result ? "true" : "false";
}


template<>
inline CoreItemOperator<kfloat>::operator bool() const
{
	kfloat result = (kfloat)*this;
	return result ? true : false;
}

template<>
inline CoreItemOperator<kfloat>::operator int() const
{
	kfloat result = (kfloat)*this;
	return (int)result;
}

template<>
inline CoreItemOperator<kfloat>::operator unsigned int() const
{
	kfloat result = (kfloat)*this;
	return (unsigned int)result;
}

template<>
inline void	CoreItemOperator<kstl::string>::defaultOperandTypeInit(kstl::string& _value)
{
	_value = "";
}

template<>
inline void	CoreItemOperator<Point2D>::defaultOperandTypeInit(Point2D& _value)
{
	_value.Set(0.0f, 0.0f);
}

template<>
inline void	CoreItemOperator<Point3D>::defaultOperandTypeInit(Point3D& _value)
{
	_value.Set(0.0f, 0.0f,0.0f);
}


// just evaluate each operand and return the last evaluated one
template<typename operandType>
class InstructionListOperator : public CoreItemOperator<operandType>
{
public:

	virtual inline operator operandType() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperandEnd = CoreVector::myVector.end();

		operandType	result;
		while (itOperand != itOperandEnd)
		{
			result = (operandType)((const CoreItem&)*((CoreItem*)(*itOperand)));
			++itOperand;
		}
		return result;
	}

};

template<typename operandType>
class AddOperator : public CoreItemOperator<operandType>
{
public:

	virtual inline operator operandType() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperandEnd = CoreVector::myVector.end();

		operandType	result((operandType)((CoreItem&)*((CoreItem*)(*itOperand))));
		++itOperand;
		while (itOperand != itOperandEnd)
		{
			result += (operandType)((CoreItem&)*((CoreItem*)(*itOperand)));
			++itOperand;
		}
		return result;
	}

};

template <  >
inline AddOperator<kstl::string>::operator kstl::string() const
{
	kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();
	kstl::vector<RefCountedBaseClass*>::const_iterator itOperandEnd = CoreVector::myVector.end();

	kstl::string	result("");

	while (itOperand != itOperandEnd)
	{
		result += (kstl::string)((CoreItem&)*((CoreItem*)(*itOperand)));
		++itOperand;
	}
	return result;
}

template<typename operandType>
class SubOperator : public CoreItemOperator<operandType>
{
public:

	virtual inline operator operandType() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperandEnd = CoreVector::myVector.end();

		operandType	result((operandType)((CoreItem&)*((CoreItem*)(*itOperand))));
		++itOperand;
		while (itOperand != itOperandEnd)
		{
			result -= (operandType)((CoreItem&)*((CoreItem*)(*itOperand)));
			++itOperand;
		}
		return result;
	}

};

template <  >
inline SubOperator<kstl::string>::operator kstl::string() const
{
	kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

	kstl::string	result((kstl::string)((CoreItem&)*((CoreItem*)(*itOperand))));

	// no sub on string
	return result;
}


template<typename operandType>
class MultOperator : public CoreItemOperator<operandType>
{
public:

	virtual inline operator operandType() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperandEnd = CoreVector::myVector.end();

		operandType	result((operandType)((const CoreItem&)*((CoreItem*)(*itOperand))));
		++itOperand;
		while (itOperand != itOperandEnd)
		{
			result *= (operandType)((const CoreItem&)*((CoreItem*)(*itOperand)));
			++itOperand;
		}
		return result;
	}

};

template <  >
inline MultOperator<kstl::string>::operator kstl::string() const
{
	kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

	kstl::string	result((kstl::string)((CoreItem&)*((CoreItem*)(*itOperand))));

	// no mult on string
	return result;
}


template<typename operandType>
class DivOperator : public CoreItemOperator<operandType>
{
public:

	virtual inline operator operandType() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperandEnd = CoreVector::myVector.end();

		operandType	result((operandType)((CoreItem&)*((CoreItem*)(*itOperand))));
		++itOperand;
		while (itOperand != itOperandEnd)
		{
			result /= (operandType)((CoreItem&)*((CoreItem*)(*itOperand)));
			++itOperand;
		}
		return result;
	}

};

template <  >
inline DivOperator<kstl::string>::operator kstl::string() const
{
	kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

	kstl::string	result((kstl::string)((CoreItem&)*((CoreItem*)(*itOperand))));

	// no div on string
	return result;
}


template<typename operandType>
class AbsOperator : public CoreItemOperator<operandType>
{
public:

	virtual inline operator operandType() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();
		
		operandType	result((operandType)((CoreItem&)*((CoreItem*)(*itOperand))));

		return (result<(operandType)0) ? (-result) : result;
	}

	static CoreVector* create()
	{
		return new AbsOperator<operandType>();
	}

};

template <  >
inline AbsOperator<kstl::string>::operator kstl::string() const
{
	kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

	kstl::string	result((kstl::string)((CoreItem&)*((CoreItem*)(*itOperand))));

	// no abs on string
	return result;
}


template<typename operandType>
class MaxOperator : public CoreItemOperator<operandType>
{
public:

	virtual inline operator operandType() const
	{

		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperandEnd = CoreVector::myVector.end();

		operandType	result((operandType)((CoreItem&)*((CoreItem*)(*itOperand))));
		++itOperand;
		while (itOperand != itOperandEnd)
		{
			operandType current = (operandType)((CoreItem&)*((CoreItem*)(*itOperand)));
			if (current > result)
				result = current;
			++itOperand;
		}
		return result;
	}

	static CoreVector* create()
	{
		return new MaxOperator<operandType>();
	}
};

template <  >
inline MaxOperator<kstl::string>::operator kstl::string() const
{
	kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

	kstl::string	result((kstl::string)((CoreItem&)*((CoreItem*)(*itOperand))));

	// no max on string
	return result;
}

template<typename operandType>
class MinOperator : public CoreItemOperator<operandType>
{
public:

	virtual inline operator operandType() const
	{

		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperandEnd = CoreVector::myVector.end();

		operandType	result((operandType)((CoreItem&)*((CoreItem*)(*itOperand))));
		++itOperand;
		while (itOperand != itOperandEnd)
		{
			operandType current = (operandType)((CoreItem&)*((CoreItem*)(*itOperand)));
			if (current < result)
				result = current;
			++itOperand;
		}
		return result;
	}

	static CoreVector* create()
	{
		return new MinOperator<operandType>();
	}
};

template <  >
inline MinOperator<kstl::string>::operator kstl::string() const
{
	kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

	kstl::string	result((kstl::string)((CoreItem&)*((CoreItem*)(*itOperand))));

	// no min on string
	return result;
}



template<typename operandType>
class SinusOperator : public CoreItemOperator<operandType>
{
public:

	virtual inline operator operandType() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

		operandType	result((operandType)((CoreItem&)*((CoreItem*)(*itOperand))));

		return sinf(result);
	}

	static CoreVector* create()
	{
		return new SinusOperator<operandType>();
	}
};

template<typename operandType>
class CosinusOperator : public CoreItemOperator<operandType>
{
public:

	virtual inline operator operandType() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

		operandType	result((operandType)((CoreItem&)*((CoreItem*)(*itOperand))));

		return cosf(result);
	}

	static CoreVector* create()
	{
		return new CosinusOperator<operandType>();
	}

};

template<typename operandType>
class TangentOperator : public CoreItemOperator<operandType>
{
public:

	virtual inline operator operandType() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

		operandType	result((operandType)((CoreItem&)*((CoreItem*)(*itOperand))));

		return tanf(result);
	}

	static CoreVector* create()
	{
		return new TangentOperator<operandType>();
	}

};


template<typename operandType>
class NegOperator : public CoreItemOperator<operandType>
{
public:

	virtual inline operator operandType() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();


		operandType	result;
		
		((CoreItem*)(*itOperand))->getValue(result);

		return -result;
	}

};

template <  >
inline NegOperator<kstl::string>::operator kstl::string() const
{
	kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

	kstl::string	result((kstl::string)((CoreItem&)*((CoreItem*)(*itOperand))));

	// no neg on string
	return result;
}

// boolean
class NotOperator : public CoreItemOperator<bool>
{
public:

	virtual inline operator bool() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

		bool	result(false);

		if ( ((bool)((CoreItem&)*((CoreItem*)(*itOperand))))==false)
		{
			result=true;
		}

		return result;
	}

};



class EqualOperator : public CoreItemOperator<bool>
{
public:

	virtual inline operator bool() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();
		
		bool	result(false);
		
		if (CoreVector::myVector.size() == 2)
		{
			CoreItem&	op1(((CoreItem&)*((CoreItem*)(*itOperand))));
			++itOperand;
			CoreItem&	op2(((CoreItem&)*((CoreItem*)(*itOperand))));

			if (op1 == op2)
			{
				result = 1;
			}
		}

		return result;
	}

};


class NotEqualOperator : public CoreItemOperator<bool>
{
public:

	virtual inline operator bool() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

		bool	result(false);

		if (CoreVector::myVector.size() == 2)
		{
			CoreItem&	op1(((CoreItem&)*((CoreItem*)(*itOperand))));
			++itOperand;
			CoreItem&	op2(((CoreItem&)*((CoreItem*)(*itOperand))));

			if(! (op1 == op2))
			{
				result = true;
			}
		}

		return result;
	}

};

class AndOperator : public CoreItemOperator<bool>
{
public:

	virtual inline operator bool() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

		bool	result(false);

		if (CoreVector::myVector.size() == 2)
		{
			bool	op1((bool)((CoreItem&)*((CoreItem*)(*itOperand))));
			++itOperand;
			bool	op2((bool)((CoreItem&)*((CoreItem*)(*itOperand))));

			if (op1&&op2)
			{
				result = true;
			}
		}

		return result;
	}

};


class OrOperator : public CoreItemOperator<bool>
{
public:

	virtual inline operator bool() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

		bool	result(false);

		if (CoreVector::myVector.size() == 2)
		{
			bool	op1((bool)((CoreItem&)*((CoreItem*)(*itOperand))));
			++itOperand;
			bool	op2((bool)((CoreItem&)*((CoreItem*)(*itOperand))));

			if (op1 || op2)
			{
				result = true;
			}
		}

		return result;
	}

};

class SupOperator : public CoreItemOperator<bool>
{
public:

	virtual inline operator bool() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

		bool	result(false);

		if (CoreVector::myVector.size() == 2)
		{
			kfloat	op1((kfloat)((CoreItem&)*((CoreItem*)(*itOperand))));
			++itOperand;
			kfloat	op2((kfloat)((CoreItem&)*((CoreItem*)(*itOperand))));

			if (op1 > op2)
			{
				result = true;
			}
		}

		return result;
	}

};

class SupEqualOperator : public CoreItemOperator<bool>
{
public:

	virtual inline operator bool() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

		bool	result(false);

		if (CoreVector::myVector.size() == 2)
		{
			kfloat	op1((kfloat)((CoreItem&)*((CoreItem*)(*itOperand))));
			++itOperand;
			kfloat	op2((kfloat)((CoreItem&)*((CoreItem*)(*itOperand))));

			if (op1 >= op2)
			{
				result = true;
			}
		}

		return result;
	}

};


class InfOperator : public CoreItemOperator<bool>
{
public:

	virtual inline operator bool() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

		bool	result(false);

		if (CoreVector::myVector.size() == 2)
		{
			kfloat	op1((kfloat)((CoreItem&)*((CoreItem*)(*itOperand))));
			++itOperand;
			kfloat	op2((kfloat)((CoreItem&)*((CoreItem*)(*itOperand))));

			if (op1 < op2)
			{
				result = true;
			}
		}

		return result;
	}

};

class InfEqualOperator : public CoreItemOperator<bool>
{
public:

	virtual inline operator bool() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

		bool	result(false);

		if (CoreVector::myVector.size() == 2)
		{
			kfloat	op1((kfloat)((CoreItem&)*((CoreItem*)(*itOperand))));
			++itOperand;
			kfloat	op2((kfloat)((CoreItem&)*((CoreItem*)(*itOperand))));

			if (op1 <= op2)
			{
				result = true;
			}
		}

		return result;
	}

};


template<typename operandType>
class AffectOperator : public CoreItemOperator<operandType>
{
public:

	virtual inline operator operandType() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperandEnd = CoreVector::myVector.end();

		CoreItem& result=(*((CoreItem*)(*itOperand)));
		
		++itOperand;
		if (itOperand != itOperandEnd)
		{
			result= (operandType)((CoreItem&)*((CoreItem*)(*itOperand)));
		}
		return (operandType)result;

	}

};


template<typename operandType>
class DynamicVariableOperator : public CoreItemOperator<operandType>
{
public:

	DynamicVariableOperator(const kstl::string& varname) : myVarName(varname)
	{

	}

	virtual operator operandType() const;

protected:

	
	kstl::string	myVarName;

};



template<typename operandType>
class CoreModifiableAttributeOperator : public CoreItemOperator<operandType>
{
public:

	CoreModifiableAttributeOperator(const kstl::string& path, CoreModifiable* target) : myAttributePath(path), myTarget(target), myAttribute(0), myArrayAttributeIndex(-1), myIsMethod(0){};

	// construct from an existing attribute
	CoreModifiableAttributeOperator(CoreModifiableAttribute* attr) : myTarget(0), myAttribute(attr), myArrayAttributeIndex(-1), myIsMethod(0) 
	{
		myAttributePath = "";
	};

	virtual operator operandType() const;

	virtual inline bool operator==(const CoreItem& other) const
	{
		return (((kfloat)(*this)) == (kfloat)other);
	}

	virtual CoreItem& operator=(const operandType& other);

protected:

	void GetAttribute() const;

	kstl::string				myAttributePath;
	CoreModifiable*				myTarget;

	union
	{
		CoreModifiableAttribute*	myAttribute;
		unsigned int				myMethodID;
	}; 

	short						myArrayAttributeIndex;
	short						myIsMethod;
};

template<>
inline bool CoreModifiableAttributeOperator<kstl::string>::operator == (const CoreItem& other) const
{
	kstl::string	othervalue;
	other.getValue(othervalue);

	kstl::string	thisvalue;
	getValue(thisvalue);

	return (thisvalue == othervalue);
}

template<>
inline bool CoreModifiableAttributeOperator<Point2D>::operator == (const CoreItem& other) const
{
	Point2D	othervalue;
	other.getValue(othervalue);

	Point2D	thisvalue;
	getValue(thisvalue);

	return (thisvalue == othervalue);
}


template<>
inline bool CoreModifiableAttributeOperator<Point3D>::operator == (const CoreItem& other) const
{
	Point3D	othervalue;
	other.getValue(othervalue);
	Point3D	thisvalue;
	getValue(thisvalue);

	return (thisvalue == othervalue);
}


template<typename operandType>
class IfThenElseOperator : public CoreItemOperator<operandType>
{
public:

	virtual inline operator operandType() const
	{
		kstl::vector<RefCountedBaseClass*>::const_iterator itOperand = CoreVector::myVector.begin();

		operandType	result(0);

		bool	test((bool)((CoreItem&)*((CoreItem*)(*itOperand))));
		
		if (test != 0)
		{
			if (CoreVector::myVector.size() > 1)
			{
				++itOperand;
				result = (operandType)((CoreItem&)*((CoreItem*)(*itOperand)));
			}
		}
		else
		{
			if (CoreVector::myVector.size() > 2)
			{
				++itOperand;
				++itOperand;

				result = (operandType)((CoreItem&)*((CoreItem*)(*itOperand)));
			}
		}

		
		return result;
	}
	static CoreVector* create()
	{
		return new IfThenElseOperator<operandType>();
	}
};


#endif // _COREITEMOPERATOR_H