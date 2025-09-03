// DCArray.h

#include <new>
#include <typeinfo>

  #define SHOW_Version "DCArray"
  #define METHOD "ClearCode TM" // CREATED IN THE 90'S -> ONE GIANT STEP BETTER THAN CLEAN CODE.
  #define THE_CopyRightNotice "`Copyright (c) 2020-2025 Denis Cote ThinknSpeak TM"

  using namespace std;

  typedef char  CHR;
  typedef char* PSTR;
  typedef const char* CSTR;
  typedef unsigned char  BYTE;
  typedef unsigned char* PBYTE;
  typedef unsigned const char* CBYTE;
  #define ReCast(A) reinterpret_cast< A >

  #define null NULL  
  #define BOOL bool
  #define ZeroMemory(ptr, sz) memset((ptr), 0, (sz))



// MIN/MAX IMPLEMENTATION
  #ifndef min
  #define min(a,b) ((a)<(b)?(a):(b))
  #endif
  #ifndef max
  #define max(a,b) ((a)>(b)?(a):(b))
  #endif

// DCARRAY CLASS
  template<class TYPE, class ARG_TYPE = const TYPE&>
  class DCArray   // : public CObject 
  {
   public:
    TYPE* ArrayStack;     // THE INTERNAL ARRAY
    int   StackCount;     // # of elements in use (upperBound - 1)
    int   StackSize;      // ALLOCATED + EMPTY + IN USE 
    int   PageSize;       // HOW MANY PER ALLOCATION
    int   SizeOfType;     // SIZE OF THE template TYPE 

    DCArray(){
      SizeOfType                = sizeof(TYPE);
      ArrayStack                = NULL;
      StackCount                = 
      StackSize                 = 0;
      PageSize                  = 32;
    }
    ~DCArray(){
      DeleteAll();
    }
   // Attributes
    bool IsEmpty                  (){ 
      return StackCount == 0; 
    }
    int  GetCount                 (){ 
      return StackCount; 
    }
    int  GetUpperBound            (){ 
      return StackCount-1; 
    }
   // CREATE ENTRIES 
    void ClearCreateType          (int ThisRec){
      ZeroMemory((void*)(ArrayStack + ThisRec), SizeOfType);
      CreateType(ThisRec);
    }
    void CreateType               (int ThisRec){
      #pragma push_macro("new")
      #undef new
        ::new( (void*)( ArrayStack + ThisRec ) ) TYPE;
      #pragma pop_macro("new")
    }
    void ClearDeleteType          (int ThisRec){
      DeleteType(ThisRec);
      ZeroMemory((void*)(ArrayStack + ThisRec), SizeOfType);
    }
    void DeleteType               (int ThisRec){
      (ArrayStack + ThisRec)->~TYPE();
    }
    void RemoveAll                (){ DeleteAll(); }
    void DeleteAll                (){
  		if(ArrayStack            != NULL){
  			for(int Indx = 0; Indx  < StackSize; Indx++ ){
  				(ArrayStack + Indx)->~TYPE();
        }
  			delete[] (PBYTE)ArrayStack;
  			ArrayStack              = NULL;
  		}
  		StackCount                = StackSize = 0;
    }
    void PreAllocate              (int ReqSize){
    	if(ReqSize                < 1){
    		ReqSize                 = 1;
      }
      int Totalneed             = ReqSize;
      if(ArrayStack            == NULL){
    		int AllocSize           = PageSize;
        if(ReqSize             >= PageSize){
          AllocSize             = PageSize + ReqSize;
        }
    		ArrayStack              = (TYPE*) new BYTE[AllocSize * SizeOfType];
    		ZeroMemory((void*)ArrayStack, AllocSize * SizeOfType);

    		for(int Indx = 0;Indx   < AllocSize; Indx++ ){
          CreateType(StackCount + Indx);
        }
    		StackSize               = AllocSize;

    	} else 
      if(Totalneed              < StackSize){
        // PREALLOCATED + PRETYPED
    	} else {                    // ADD MORE MILK
    		int ThisNewSize         = Totalneed + PageSize;

    		TYPE* ThisNewData       = (TYPE*) new BYTE[ThisNewSize * SizeOfType];
    		ZeroMemory  ((void*)(ThisNewData),        (ThisNewSize * SizeOfType));
    		memcpy      ((ThisNewData), (ArrayStack), (StackSize   * SizeOfType));

        delete[] (PBYTE)ArrayStack;
    		ArrayStack              = ThisNewData;
        int ThisNewCount        = ThisNewSize - StackSize;
    		for(int Indx = 0; Indx  < ThisNewCount; Indx++ ){
          CreateType(StackSize  + Indx);
        }
    		StackSize               = ThisNewSize;
    	}
    }
    int           Add             (ARG_TYPE ThisElement){ 
      int ThisIndex             = StackCount;
   		PreAllocate(ThisIndex + 1);
    	ArrayStack[ThisIndex]     = ThisElement;
  		return StackCount++; 
    }
    int           Push            (ARG_TYPE ThisElement){ 
      return Add (ThisElement); 
    }
   // Accessing elements
    void          Copy            (const DCArray& ThisArray){
    	if(this                  != &ThisArray){
        RemoveAll();
    		PreAllocate(ThisArray.StackCount);

        TYPE*       ThisDst     = ArrayStack; 
        const TYPE* ThisSrc     = ThisArray.ArrayStack;
        int         ThisCount   = ThisArray.StackCount;     
      	while(ThisCount--){
	        *ThisDst++            = *ThisSrc++;
        }
        StackCount              = ThisArray.StackCount;
    	}
    }
    int           Find            (ARG_TYPE ThisElement, int ThisStart = 0){ 
      // TYPE* RetType             = ReCast(TYPE*)(NULL);
          // return (TYPE)*ReCast(TYPE*)(NULL);
        // ON FAIL...
        // TYPE ThisRet = TYPE();
        // return ThisRet;

      CSTR ThisType             = typeid(ThisElement).name();
      BOOL IsPChar              = (NULL != strstr(ThisType, "char *"));
      BOOL IsStr                = (NULL != strstr(ThisType, "Str"));

      if((!(IsStr))
      && ((!(IsPChar)))
      ){
        for(int Indx              = ThisStart; Indx < StackCount; Indx++ ){
          bool ThisResult         = (ArrayStack[Indx] == ThisElement);
          if(ThisResult){
            return Indx;
          }
        }
      } else {
        for(int Indx              = ThisStart; Indx < StackCount; Indx++ ){
          CSTR ThisResult         = strstr((CSTR)ArrayStack[Indx], (CSTR)ThisElement);
          if(ThisResult){
            return Indx;
          }
        }
      }
      return -1;
    }
    TYPE          Pop             (){ 
      int ThisIndex             = StackCount - 1;
      TYPE ThisRet              =  ArrayStack[ThisIndex];
      RemoveAt(ThisIndex);
      return ThisRet;
    }
    BOOL          RemoveAt        (int ThisIndex, int ThisCount = 1){
      if((ThisIndex             < 0)
      || (ThisIndex            >= StackCount)
      ){
        return false;
      }
      if(ThisCount              < 1){
        return false;
      }
    	int UpperBound            = (ThisIndex  + ThisCount);
      if(UpperBound            >= StackCount){
        UpperBound              = StackCount;
      }
    	int MoveCount             = (StackCount - UpperBound);
    	// THE RIGHT STUFF: WE MUST CLEAR AND DESTROY THOSE IN THE OVERLAP
      for(int Indx = 0; Indx    < ThisCount; Indx++ ){
    		ClearDeleteType(ThisIndex + Indx);
      }
    	if(MoveCount){
    		memmove((ArrayStack     + ThisIndex), 
   			(ArrayStack             + UpperBound), 
        (MoveCount              * SizeOfType));
    	}
    	// THE RIGHT STUFF: WE MUST CLEAR AND REINITIALIZE THE GENERATED TAIL
      for(int Indx = 0; Indx    < ThisCount; Indx++ ){
        int ThisRec             = ThisIndex + MoveCount + Indx;
        CreateType(ThisRec);
      }
    	StackCount               -= ThisCount;
      return true;
    }

    void          InsertAt        (int ThisIndex, ARG_TYPE ThisElement, int ThisCount = 1){
      if(ThisIndex              < 0){
        ThisIndex               = 0;
      }
      if(ThisCount              < 1){
        ThisIndex               = 1;
      }
      PreAllocate(StackCount    + ThisCount);
    	// DESTROY THE ONE THAT WILL BE OVERWRITTEN BY THE OVERLAPPED MOVE
      for(int Indx = 0; Indx    < ThisCount; Indx++ ){
        int ThisRec             = (StackCount + Indx);
        ClearDeleteType(ThisRec);
      }
      // MOVE FROM AT TO THE NUMBER OF COUNT TO INSERT.
      int TheRecDst             = ThisIndex   + ThisCount;
      int TheRecSrc             = ThisIndex;
      int TheRecCnt             = StackCount  - ThisIndex;
   		memmove((ArrayStack       + TheRecDst), 
        			(ArrayStack       + TheRecSrc), 
              (TheRecCnt        * SizeOfType));

      for(int Indx = 0;Indx     < ThisCount;Indx++){
        int ThisRec             = ThisIndex   + Indx;
        ClearCreateType(ThisRec);
        ArrayStack[ThisRec]     = ThisElement;
      }
      StackCount               += ThisCount;
    }
    void          InsertAt        (int ThisStartIndex, DCArray* ThisArray){
    	if(ThisArray->GetSize()   > 0){

        int InsertCount         = ThisArray->GetSize();
        PreAllocate(StackCount  + InsertCount);

    		for(int Indx = 0; Indx  < ThisArray->GetSize(); Indx++){
    			InsertAt((ThisStartIndex  + Indx), ThisArray->GetAt(Indx));
        }
        StackCount             += InsertCount;
    	}
    }
    void          SetAt           (int ThisIndex, ARG_TYPE ThisElement){
      if((ThisIndex             < 0)
      || (ThisIndex            >= StackSize)
      ){
        return;
      }
   		ArrayStack[ThisIndex]     = ThisElement;
    }
    TYPE          GetAt           (int ThisIndex){
      return ElementAt(ThisIndex); 
    }
    TYPE          ElementAt       (int ThisIndex){
      if((ThisIndex             < 0)
      || (ThisIndex            >= StackSize)
      ){
        return TYPE();
      }
   		return ArrayStack[ThisIndex];
	  }
    inline TYPE   operator[]      (int ThisIndex){ 
      return ElementAt(ThisIndex); 
    }

  };
