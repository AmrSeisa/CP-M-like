int main() {
 void printString (char*);
 void readString (char*);
 void readSector (char*,int);
 void handleInterrupt21(int , int , int , int );
 int readFile(char*, char*);
 void executeProgram(char*, int);
 int mod(int);
 int div(int);
void terminate();



makeInterrupt21();
interrupt(0x21, 4, "shell\0", 0x2000, 0);

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

}

void readSector(char* buffer, int sector){
    int relativeSector = mod(sector,18)+1;  //sector%18
    int head = mod(div(sector,18),2);   //(sector/18)%2
    int track = div(sector,36);     //sector/36
    interrupt(0x13, 2*256+1, buffer, track*256+relativeSector, head*256+0 );

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

    if(entry!=0)
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
    if(dir[k] !=0 )
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
    return 0;
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

	launchProgram(segment);
  // interrupt(0x21, 5, 0, 0, 0);
  }
  interrupt(0x21, 5, 0, 0, 0);

}

void terminate()
{
  shell[0] = 's';
    shell[1] = 'h';
    shell[2] = 'e';
    shell[3] = 'l';
    shell[4] = 'l';
    shell[5] ='\0';

    interrupt(0x21, 4, shell, 0x2000, 0);

}
