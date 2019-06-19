#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t key[] = {0x36, 0x61, 0x3e, 0xf1, 0x69, 0x3c, 0x2e, 0x8e, 0x55, 0xcf, 0xc2, 0x86, 0x9c, 
                 0x3b, 0x94, 0x22, 0x64, 0x41, 0x0b, 0x6b, 0xe5, 0xac, 0xa9, 0x19, 0xd3, 0xd0,
                 0xaa, 0x9e, 0x49, 0x05, 0x03, 0x7f, 0x67, 0x42, 0x72, 0x50, 0xfd, 0xa0, 0x5e,
                 0x53, 0xef, 0x21, 0x59, 0x0c, 0x5c, 0xee, 0xae, 0xc0, 0xaf, 0xb9, 0xab, 0x15,
                 0xe6, 0xd4, 0x2e, 0xba, 0xa6, 0x59, 0x59, 0x6f, 0x5e, 0x5d, 0xef, 0x45, 0x1f,
                 0xe1, 0x95, 0x9c, 0x02, 0xf4, 0x70, 0x72, 0x95, 0xc9, 0xfd, 0xf2, 0xb8, 0xac,
                 0x33, 0xe7, 0x67, 0x5f, 0xfd, 0xcd, 0x34, 0x2c, 0x88, 0x5a, 0x85, 0x62, 0xca,
                 0x64, 0x3f, 0xba, 0xa9, 0xdd, 0x1c, 0x40, 0x7a, 0x9d, 0xb4, 0x6a, 0x10, 0x4a,
                 0x35, 0x0f, 0xbc, 0x6d, 0xbb, 0x70, 0x56, 0xa2, 0xcf, 0x54, 0x70, 0x04, 0x00,
                 0x79, 0x5f, 0x86, 0x5b, 0xa9, 0xea, 0x9a, 0xe3, 0x14, 0xf7};


int main(int argc, char** argv){
   //Load entire file into memory
   if(argc != 2){
      printf("Usage: ./decrypt FILENAME\n");
      exit(0);
   }

   FILE* f = fopen(argv[1],"r");
   if(!f){
       printf("Could not open file %s\n", argv[1]);
       exit(0);
   }
   fseek(f,0,SEEK_END);
   size_t sz = ftell(f);
   fseek(f,0,SEEK_SET);
   uint8_t *buf = (uint8_t*)valloc(sz);
   fread(buf,sz,1,f);
   fclose(f);
   for(int j=0; j < 1; j++){
   for(int k=65; k < 66; k++){
      int ki=sizeof(key)-1;
      uint8_t vstate = j;
//      printf("%d %d  abcdefghojklmnopksajhdkajhskdjh", j, k);
      for(size_t i=0; i < sz-k; i++){
         printf("%c", vstate ^ buf[i+k] ^ key[ki]);
         vstate = buf[i+k];        
         ki--;
         if(ki<0)
           ki = sizeof(key)-1;
      }
   }}
}
