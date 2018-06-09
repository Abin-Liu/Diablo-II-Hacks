#ifndef __AUTO_HANDLE_H__
#define  __AUTO_HANDLE_H__

template<class HandleType, class dtor = AutoHandlePolicy<HandleType> >
class AutoHandle
{
	AutoHandle(const AutoHandle&);
	AutoHandle operator=(const AutoHandle&);
public:
	AutoHandle(HandleType h=NULL):h_(h){}
	~AutoHandle() { dtor::Release(h_); }
	operator HandleType() { return h_; }
	AutoHandle& operator=(HandleType h) {
		dtor::Release(h_);
		h_ = h;
		return *this;
	}
private:
	HandleType h_;
};

template<class T>
struct AutoHandlePolicy {};

template<>
struct AutoHandlePolicy<HANDLE> {
	static void Release(HANDLE h) { if(h) CloseHandle(h); }
};

template<>
struct AutoHandlePolicy<HMODULE> {
	static void Release(HMODULE h) { if(h) FreeLibrary(h); }
};

template<>
struct AutoHandlePolicy<LPBYTE> {
	static void Release(LPBYTE h) { if(h) VirtualFree(h, 0, MEM_RELEASE); }
};

#endif // __AUTO_HANDLE_H__
