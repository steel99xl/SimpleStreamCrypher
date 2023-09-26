#include <iostream>
#include <fstream>

void Help(){

  std::cout << "Simple Crypt" << std::endl;
  std::cout << "./SimpleCrypt -s [Key] | Set Key" << std::endl;
  std::cout << "./SimpleCrypt -e [Message] | Encrypt Message" << std::endl;
  std::cout << "./SimpleCrypt -d [Message] | Encrypt Message" << std::endl;
  std::cout << "./SimpleCrypt -e [KeySize] [Message] [Key] | Encrypt Message" << std::endl;
  std::cout << "./SimpleCrypt -d [KeySize] [Message] [Key] | Encrypt Message" << std::endl;

}



typedef struct simplecrypt{
  char* Key;
  int KeySize;

  void SetKey(const char* NewKey, int Size){
      this->Key = new char[Size];
      this->KeySize = Size;
      for(int i = 0; i < Size; i++){
       this->Key[i] = NewKey[i];
      }
  }

  char GetKey(int Index){
    int Temp;
    if(Index > this->KeySize){
      Temp = Index - this->KeySize;
      if(Temp > this->KeySize){
        Temp = Temp%this->KeySize;
      }
      return this->Key[Temp];
    } else {
      return this->Key[Index];
    }


  }

} simplecrypt;


void SaveKey(const char *Key){
  std::ofstream File;

  File.open("/tmp/SC.key", std::ios::out);

  if(File.is_open()){
    File << Key;
    File.close();
  }

}


void LoadKey(simplecrypt* SC){
  std::fstream File;
  int InitSize = 512;
  char *TKey = new char[InitSize];
  int Size = InitSize;
  int Count = 0;
  char c;

  File.open("/tmp/SC.key", std::ios::in);
  if(File.is_open()){
    while(File.get(c)){
      if(Count >= Size){
        Size *=2;
        
        char *temp = new char[Size];

        for(int i = 0; i < Count; i++){
          temp[i] = TKey[i];

        }
          delete[] TKey;

          TKey = temp;

      }
      TKey[Count] = c;
      Count += 1;

    }

    SC->SetKey(TKey, Size);

    File.close();

  }
}


void Printer(const char *MSG, simplecrypt SC){
  bool running = true;
  int count = 0;
  // lol idk, e
  char E;

  while(running){

    switch(MSG[count]){
      case('\0') : {
                     running = false;
                   } break;
      default : {

                  E = MSG[count] ^ SC.GetKey(count);
                  if((int)E > 127) {
                    E -=128;
                  }
                  if((int)E < 33){
                    E +=32;
                  }
                  std::cout << E;
                  /*
                  if (std::isprint(static_cast<unsigned char>(E))) {
                    // Check if the character is printable ASCII
                    std::cout << E;
                  } else {
                    // Character is not printable ASCII, use escape sequence
                    std::cout << "\\x" << std::hex << static_cast<int>(E) << std::dec;
                  }
                  */
                  
                } break;

    };
    count = count + 1;
    
  }

  std::cout << std::endl;


};

int main(int argc, char** argv){

  simplecrypt SC;
  
  switch(argc){

    case(3) :{
             if(argv[1][0] == '-'){
               switch(argv[1][1]){
                 case('s') : {
                               SaveKey(argv[2]);
                             } break;
                 case('e') : {
                               std::cout << "Encrypted Message | ";
                               LoadKey(&SC);
                               Printer(argv[2],SC);

                             } break;
                 case('d') : {
                               std::cout << "Decrypted Message | ";
                               LoadKey(&SC);
                               Printer(argv[2],SC);

                             } break;
               };

             } else { Help(); }
             } break;

    case(5) :{
             if(argv[1][0] == '-'){
               switch(argv[1][1]){
                 case('e') : {
                               std::cout << "Encrypted Message | ";
                               SC.SetKey(argv[4],atoi(argv[2])-1);
                               Printer(argv[3], SC);


                             } break;
                 case('d') : {
                               std::cout << "Decrypted Message | ";
                               SC.SetKey(argv[4],atoi(argv[2])-1);
                               Printer(argv[3], SC);

                             } break;
               };

             } else { Help(); }
             } break;

    default: {
               Help();
             }

  };


  return 0;
}
