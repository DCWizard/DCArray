#include "DCArray.h"
DCArray<PSTR,       PSTR> CheckArray; 
DCArray<PSTR,       PSTR> TestArray;
DCArray<int,        int>  IntArray;

int main(){
  printf("%s\n", "Welcome to DCArray");

  CheckArray.Add("A");
  CheckArray.Add("B");
  CheckArray.Add("C");
  CheckArray.Add("D");
  CheckArray.Add("E");
  // CheckArray.RemoveAt(2);
  // for(int Indx = 0;Indx < CheckArray.GetCount();Indx++){
  //   printf("Array Entry: [%d]:%s\n", Indx, CheckArray[Indx]);
  // }

  TestArray.Add("Test First value");  
  TestArray.Add("11");
  TestArray.Add("22");
  TestArray.Add("33");
  TestArray.Add("44");
  TestArray.Add("55");
  TestArray.Add("66");
  TestArray.Add("77");
  TestArray.Add("88");
  TestArray.Add("99");
  TestArray.Add("000");
  TestArray.Add("55");
  TestArray.Add("111");
  TestArray.Add("222");
  TestArray.Add("333");
  TestArray.Add("444");
  TestArray.Add("555");
  TestArray.Add("666");
  TestArray.Add("777");
  TestArray.Add("888");
  TestArray.Add("999");
  TestArray.Add("0000");
  TestArray.Add("1111");
  TestArray.Add("2222");

  for(int Indx = 0;Indx < TestArray.StackCount;Indx++){
    printf("Array Entry: [%d]:%s\n", Indx, TestArray[Indx]);
  }
  PSTR ThisStr;

  TestArray.RemoveAt(5, 5);
  printf("Removed Entry:[5-10]\n");
  printf("Removed Entry:[4]  %s\n", TestArray.GetAt(4));
  printf("Removed Entry:[5]  %s\n", TestArray.GetAt(5));
  printf("Removed Entry:[6]  %s\n", TestArray.GetAt(6));
  printf("Removed Entry:[7]  %s\n", TestArray.GetAt(7));
  printf("Removed Entry:[8]  %s\n", TestArray.GetAt(8));
  printf("Removed Entry:[9]  %s\n", TestArray.GetAt(9));
  printf("Removed Entry:[10] %s\n", TestArray.GetAt(10));
  printf("Removed Entry:[11] %s\n", TestArray.GetAt(11));


  ThisStr = TestArray.Pop();
  printf("Popped Entry: %s\n", ThisStr);

  TestArray.InsertAt(5, "Test Insert starting at 5", 5);
  printf("Inserted Entry:[4]  %s\n", TestArray.GetAt(4));
  printf("Inserted Entry:[5]  %s\n", TestArray.GetAt(5));
  printf("Inserted Entry:[6]  %s\n", TestArray.GetAt(6));
  printf("Inserted Entry:[7]  %s\n", TestArray.GetAt(7));
  printf("Inserted Entry:[8]  %s\n", TestArray.GetAt(8));
  printf("Inserted Entry:[9]  %s\n", TestArray.GetAt(9));
  printf("Inserted Entry:[10] %s\n", TestArray.GetAt(10));
  printf("Inserted Entry:[11] %s\n", TestArray.GetAt(11));

  for(int Indx  = 0;Indx < TestArray.StackSize;Indx++){
    printf("String Entry: [%d]:%s\n", Indx, TestArray[Indx]);
  }


  int ThisRec   = TestArray.Find("Insert");
  if(ThisRec   != -1){
    printf("Found Entry: [%d]:%s\n", ThisRec, TestArray[ThisRec]);
  }


  TestArray.Copy(CheckArray);
  for(int Indx  = 0;Indx < TestArray.StackCount;Indx++){
    printf("Copied Entry: [%d]:%s\n", Indx, TestArray[Indx]);
  }


  for(int Indx  = 0; Indx < 55;Indx++){ 
    IntArray.Push(Indx);  
  }
  // int IntRet    = IntArray.Pop();
  for(int Indx  = 0;Indx < IntArray.StackSize;Indx++){
    printf("Int Entry: [%d]:%d\n", Indx, IntArray[Indx]);
  }

  ThisRec       = IntArray.Find(28);
  if(ThisRec   != -1){
    printf("Found int Entry: [%d]:%d\n", ThisRec, IntArray[ThisRec]);
  }


  return 0;
}

