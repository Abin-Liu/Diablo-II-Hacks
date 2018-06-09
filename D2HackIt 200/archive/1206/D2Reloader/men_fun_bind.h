//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#ifndef __MEM_FUNCTION_BIND_H__
#define __MEM_FUNCTION_BIND_H__

template<class ClsType, class ValType, class ResType>
class const_mem_fun_binder : public std::unary_function<ValType, ResType>
{
public:
	explicit const_mem_fun_binder(ClsType& cls, ResType (ClsType::*op)(const ValType&))
		:cls_(cls), op_(op){}
		ResType operator()(const ValType& val) const
		{
			(cls_.*op_)(val);
			// cannt compile with vc6.0, shit
//			return (cls_.*op_)(val);
		}
private:
	ClsType& cls_;
	ResType (ClsType::*op_)(const ValType&);
};

template<class ClsType, class ValType, class ResType>
class mem_fun_binder : public std::unary_function<ValType, ResType>
{
public:
	explicit mem_fun_binder(ClsType& cls, ResType (ClsType::*op)(ValType&))
		:cls_(cls), op_(op){}
		ResType operator()(ValType& val) const
		{
			return (cls_.*op_)(val);
		}
private:
	ClsType& cls_;
	ResType (ClsType::*op_)(ValType&);
};

template<class ClsType, class ValType1, class ValType2, class ResType>
class bin_mem_fun_binder : public std::binary_function<ValType1, ValType2, ResType>
{
public:
	explicit bin_mem_fun_binder(ClsType& cls, ResType (ClsType::*op)(ValType1&, ValType2&))
		:cls_(cls), op_(op){}
		ResType operator()(ValType1& val1, ValType2& val2) const
		{
			return (cls_.*op_)(val1, val2);
		}
private:
	ClsType& cls_;
	ResType (ClsType::*op_)(ValType1&, ValType2&);
};

template<class ClsType, class ValType, class ResType>
inline const_mem_fun_binder<ClsType, ValType, ResType> const_mem_fun_bind(ClsType& cls, ResType (ClsType::*op)(const ValType&))
{
	return const_mem_fun_binder<ClsType, ValType, ResType>(cls, op);
}

template<class ClsType, class ValType, class ResType>
inline mem_fun_binder<ClsType, ValType, ResType> mem_fun_bind(ClsType& cls, ResType (ClsType::*op)(ValType&))
{
	return mem_fun_binder<ClsType, ValType, ResType>(cls, op);
}

template<class ClsType, class ValType1, class ValType2, class ResType>
inline bin_mem_fun_binder<ClsType, ValType1, ValType2, ResType> mem_fun_bind(ClsType& cls, ResType (ClsType::*op)(ValType1&, ValType2&))
{
	return bin_mem_fun_binder<ClsType, ValType1, ValType2, ResType>(cls, op);
}

#endif // __MEM_FUNCTION_BIND_H__
