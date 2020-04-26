void printString (char*);
void readString (char*);
void readSector (char*,int);
void writeSector(char*, int);
void handleInterrupt21(int , int , int , int );
void writeFile(char*, char*, int);
int readFile(char*, char*);
void deleteFile(char*);
void executeProgram(char*, int);
int mod(int);
int div(int);
void listFiles();
void clearBuffer(char*,int);
int findFreeSector(char*);
void terminate();
main() {
/*
  int i=0;
  char buffer1[13312];
  char buffer2[13312];
  buffer2[0]='h';
  buffer2[1]='e';
  buffer2[2]='l';
  buffer2[3]='l';
  buffer2[4]='o';
  for(i=5; i<13312; i++)
  buffer2[i]=0x0;

  makeInterrupt21();
  interrupt(0x21,8, "testW\0", buffer2, 1); //write file testW
  interrupt(0x21,3, "testW\0", buffer1, 0); //read file testW
  interrupt(0x21,0, buffer1, 0, 0); // print out  contents of testW

while (1) {}
*/


 char shell[];
 shell[0] = 's';
 shell[1] = 'h';
 shell[2] = 'e';
 shell[3] = 'l';
 shell[4] = 'l';
 shell[5] ='\0';

makeInterrupt21();
interrupt(0x21, 4, shell, 0x2000, 0);
terminate();



/*
char buffer[13312];
makeInterrupt21();
interrupt(0x21, 7, "messag\0", 0, 0); //delete messag
interrupt(0x21, 3, "messag\0", buffer, 0); // try to read messag
interrupt(0x21, 0, buffer, 0, 0); //print out the contents of buffer
while (1)
{

}

*/

return 0;
}


void handleInterrupt21(int ax, int bx, int cx, int dx) {

switch (ax)
{
  case 0: printString(bx);
  break;
  case 1: readString(bx) ;
  break;
  case 2: readSector(bx , cx);
  break;
  case 3: readFile(bx, cx);
  break;
  case 4: executeProgram(bx, cx);
  break;
  case 5: terminate();
  break;
  case 6: writeSector(bx, cx);
  break;
  case 7: deleteFile(bx);
  break;
  case 8: writeFile(bx, cx, dx);
  break;
  case 10: listFiles();
  break;
}
}

void readString(char* line)
{
 int i = 0;
 char in = 0x0;
 while (in != 0xd)
   {
    in = interrupt(0x16, 0x0, 0x0, 0x0, 0x0);
    *(line + i) = in;
    interrupt(0x10,0xe*0x100+in,0x0,0x0,0x0);
    i++;
   }
   interrupt(0x10,0xe*0x100+10,0x0,0x0,0x0);
 *(line + i) = 0x0;

}

void printString(char* string)
{
 int i = 0;
 while (*(string + i) != '\0')
   {
    char al = *(string + i);
    char ah = 0xe;
    int ax = ah * 256 + al;
    interrupt(0x10,ax,0,0,0);
    /* add LF to CR */

    if (al == 13)
    interrupt(0x10,0xe*0x100+10,0x0,0x0,0x0);
    if(*(string+i)=='\n')
    interrupt(0x10, 0xe*256+0xd, 0, 0, 0);

    i++;
   }
   clearBuffer(string,i);

}

void readSector(char* buffer, int sector){
    int relativeSector = mod(sector,18)+1;  //sector%18
    int head = mod(div(sector,18),2);   //(sector/18)%2
    int track = div(sector,36);     //sector/36
    interrupt(0x13, 2*256+1, buffer, track*256+relativeSector, head*256+0 );

}

  void writeSector(char* buffer, int sector)
  {
    int relativeSector = mod(sector,18)+1;  //sector%18
    int head = mod(div(sector,18),2);   //(sector/18)%2
    int track = div(sector,36);     //sector/36
    interrupt(0x13, 3*256+1, buffer, track*256+relativeSector, head*256+0 );
  }

int mod(int a, int b){
    while(a > b){
        a =a-b;
    }
    return a;
}//mod

int div(int a, int b){
    int q = 0;
    while((q*b) <= a){
        q++;
    }
    return q-1;
}//div



void writeFile (char* fileName, char* buffer, int secNum){
  int i, j, k, sec, tmp;
  char mapSec[512];
  char oneSec[512];
  char sectors[512];
  char dirSec[512];
  readSector(dirSec, 2);
  readSector(mapSec, 1);
  i = 0;
  j = 0;
  tmp = 0;
  while(secNum != j){
    if(mapSec[i] == 0){
      sectors[j] = i;
      mapSec[i] = 0xFF;
      j++;
    }
    i++;
  }
  for(i = 0; i < 16; i++){
    if(dirSec[i*32] == 0){
      for(j = 0; j < 6; j++){
        if(fileName[j] == '\0' ||  fileName[j] == '\n'){
          tmp = 1;
        }
        if(tmp == 0){
          dirSec[i*32 + j] = fileName[j];
        }else{
          dirSec[i*32 + j] = 0x00;
        }
      }
      sec = 0;
      for(j = 0; j < secNum; j++){
        dirSec[i*32 + 6 + j] = sectors[j];
        for(k = 0; k < 512; k++){
          oneSec[k] = buffer[sec*512 + k];
        }
        writeSector(oneSec, sectors[j]);
        sec++;
      }
      for(j = 0; j < 26-secNum; j++){
        dirSec[i*32 + 6 + secNum + j] = 0x00;
      }
      writeSector(dirSec, 2);
      writeSector(mapSec, 1);
      return;
    }
  }
}

/*
void writeFile(char* name, char* buffer, int secNum)
{

  char dir[512];
  char map[512];
  char tempBuffer[512];
  char test[512];
  int byte;
  int entry;
  char sectors[26];
  int sector= 0;
  int address= 0;
  int currBufferIndex=1;
  int freeSector;
  int difference;
  int t;
  int i;
  int k;
  int l;
  int h;
  int f;

  sectors[0] = '0';
  sectors[1] = '1';
  sectors[2] = '2';
  sectors[3] = '3';

  readSector(map,1);
  readSector(dir,2);

  for(entry=0;entry<16;entry++)
  {
    byte = entry*32;
    if(dir[byte]==0x00)
    {
      break;
    }
  }

  if(entry>=16)
  {
    printString("Not enough memory");
    return;
  }


  for(i=0;name[i]!='\0'&& name[i]!=0x00;i++)
  {
    dir[entry*32+i] = name[i];
  }

  if(i<6)
  {
    difference= 6- i;
    for(k=0;k<difference;k++)
    {
      dir[entry*32+k+i]=0x00;
    }
  }

  for(l=0;l<secNum;l++)
  {

    freeSector = findFreeSector(map);
    map[freeSector]=0xFF;
    dir[entry*32+6+l]=freeSector;

    for(k=0;k<512;k++)
    {
      tempBuffer[k] = buffer[currBufferIndex];
      currBufferIndex++;
    }

    writeSector(tempBuffer,freeSector);
    freeSector = 0;
    clearBuffer(tempBuffer,512);
  }

  for(h=secNum;h<26;h++)
  {
    dir[entry*32+6+h]=0x00;
  }

  for(f=0;f<32;f++)
  {
    test[f]=dir[entry*32+f];
  }
  printString(test);

  writeSector(map,1);
  writeSector(dir,2);

  // printString("finished writing");

}
*/

int readFile(char* filename, char* buf)
{
    int entry;
    char dir[512];
    int byte;
    int sectors[26];
    int sector = 0;
    int address =  0;
    int i;
    int k;


    readSector(dir ,2);
    entry = findFile(dir , filename);

    if(entry != -1)
    {
      byte = entry*32+6;

      for(k = 0 ;k<26;k++)
      {
        sectors[k] = dir[byte+k];
      }
      for(i = 0; sectors[i] != 0 ; i++)
      {
        readSector(buf+sector, sectors[i]);
        sector+=512;
      }
      return 1;
    }else
    {
      printString("File not found!!");
      return -1;
    }
}

void deleteFile(char* name)
{
  int entry;
  char dir[512];
  char map[512];
  int byte;
  int sectors[26];
  int sector = 0;
  int address =  0;
  int i;
  int k;
  int j;

  readSector(dir ,2);
  readSector(map, 1);

  entry = findFile(dir , name);

  if(entry < 0)
  {
    printString("There is no matching file");
    return;
  }

  if(entry != -1)
  {
    byte = entry*32;
    dir[byte] = 0x00;

    byte = byte+6;
    for(k = 0 ;k<26;k++)
    {
      sectors[k] = dir[byte+k];
    }
    for(i = 0; sectors[i] != 0 ; i++)
    {
      map[sectors[i]] = 0x00;
    }
    writeSector(dir, 2);
    writeSector(map, 1);

  }
  // terminate();
}

int findFile(char* dir, char* filename )
{
  int found = 0;
  int sector;
  int i;
  int j;
  int k;
  int l;

  for(i=0; i<16; i++)
  {
    // check if the sector is not free
    k=i*32;
    if(dir[k] !=0x00 )
    {
      for(j = 0; j<6 ; j++)
      {
        l = j+k;
        if(dir[l] == filename[j])
        {
          found = 1;
        }else
        {
        found = 0;
        break;
        }
      }

      if(found == 1)
      {
        return i;
      }
    }
  }
  if(found == 0)
  {
    return -1;
  }
}

void executeProgram(char* name, int segment)
{
  int i;
	char readingBuffer[13312];

	if(readFile(name, readingBuffer)>0)
  {
	for (i=0; i<13311;i++){
		putInMemory(segment,i,readingBuffer[i]);
	}

  // interrupt(0x21, 5, 0, 0, 0);
  }
  launchProgram(segment);
  interrupt(0x21, 5, 0, 0, 0);

}

void listFiles()
{
  int entry;
  char dir[512];
  int byte;
  char sectors[10];
  int sector = 0;
  int address =  0;
  char fileName[7];
  char size[3];
  char enter[2];
  int i;
  int j;
  int k;
  int l;
  int h;
  int m;
  int s;
  int t;

  sectors[0] = '0';
  sectors[1] = '1';
  sectors[2] = '2';
  sectors[3] = '3';
  sectors[4] = '4';
  sectors[5] = '5';
  sectors[6] = '6';
  sectors[7] = '7';
  sectors[8] = '8';
  sectors[9] = '9';

  readSector(dir ,2);

  for(i=0; i<16; i++)
  {
    // check if the sector is not free
    k=i*32;
    if(dir[k] !=0x0 )
    {
      for(j = 0; j<6 ; j++)
      {
        l = j+k;
        if(dir[l]!=0)
        {
          fileName[j] = dir[l];
        }
        else
        {
          j=5;
          l=j+k;
          break;
        }
      }
      printString(fileName);

      sector = 0;
      for(h=0; h<26;h++ )
      {
        m = l+h;
        if(dir[m]!=0)
        {
          sector = sector+1;
        }
        else
        {
          break;
        }
      }
      size[0] = ':';
      size[1] = sectors[sector];
      size[2] = ' ';
      printString(size);
      enter[0]='\n';
      enter[1]='\0';

      printString(enter);

    clearBuffer(fileName,6);
    clearBuffer(size,3);


    }
  }
  terminate();
}



void clearBuffer(char* buffer, int len){
	int i;
	for(i=0;i<len;i++){
		buffer[i] = 0x0;
	}
}


int findFreeSector(char* map)
{
  int i;
  for(i=0;i<512;i++)
  {
    if(map[i]=0x0)
    {
      return i;
    }
  }
}



void terminate()
{
    char shell[];
    shell[0] = 's';
    shell[1] = 'h';
    shell[2] = 'e';
    shell[3] = 'l';
    shell[4] = 'l';
    shell[5] ='\0';

    interrupt(0x21, 4, shell, 0x2000, 0);

}
