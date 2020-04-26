void printString(char*);
void readString(char*);
void readSector(char* buffer, int sector);
void writeSector(char* buffer, int sector);
int mod(int a, int b);
int div(int a, int b);
void handleInterrupt21 (int AX, int BX, int CX, int DX);
void handleTimerInterrupt(int, int);
void readFile(int* fileName,int* buffer);
void printChar(char ch);
void prtInt(int i);
int strComp(int* buffer, char* fileName);
void executeProgram(char* name);
void terminate();
void deleteFile(char* name);
void writeFile(int* name, int* buffer, int numberOfSectors);
void getDirectory();
void clearBuffer(char*,int);
void exe(char*, int);
void prtInt(int);
void listFiles();

struct entry
{
  int active;
  int sp;
  char *name;
  int waiting;
  int time;
};

struct entry processTable[8];
int currentProcess;

main()
{    // testing deleteFile
    /*
    char buffer[13312];
    makeInterrupt21();
    interrupt(0x21, 7, "messag\0", 0, 0);
    interrupt(0x21, 3, "messag\0", buffer, 0);
    interrupt(0x21, 0, buffer, 0, 0);
    */

    //testing writeFile
/*
    int i=0;
    char buffer1[13312];
    char buffer2[13312];
    buffer2[0]='h'; buffer2[1]='e'; buffer2[2]='l';  buffer2[3]='l'; buffer2[4]='o';
    for(i=5; i<13312; i++) buffer2[i]=0x0;
    makeInterrupt21();
    interrupt(0x21,8, "testW\0", buffer2, 1); //write file testW
    interrupt(0x21,3, "testW\0", buffer1, 0); //read file testW
    interrupt(0x21,0, buffer1, 0, 0); // print out  contents of testW
*/

    int i;
    for(i = 0; i < 8; i ++)
    {
      processTable[i].active = 0;
      processTable[i].sp = 0xFF00;
      processTable[i].waiting = -1;
      processTable[i].time = 0;
    }
    currentProcess = 0;
    makeInterrupt21();
    makeTimerInterrupt();
    interrupt(0x21, 4, "hello1\0", 0, 0);
    interrupt(0x21, 4, "hello2\0", 0, 0);

    // interrupt(0x21, 4, "shell\0", 0x2000, 0);


    // printString("I came back!");
    while(1);
}

void handleInterrupt21 (int ax, int bx, int cx, int dx)
{
   // printString("Hello World from Interrupt21!\0");
    if (ax == 0){
            printString(bx);
        }
        else if (ax ==1){
            readString(bx);
        }
        else if(ax == 2){
            readSector(bx,cx);
        }
        else if(ax == 3){
            readFile(bx, cx);
        }
        else if (ax == 4){
            executeProgram(bx);
        }
        else if (ax == 5){
            terminate();
        }
        else if (ax == 6){
            writeSector(bx,cx);
        }
         else if (ax == 7){
            deleteFile(bx,cx);
        }
        else if (ax == 8){
            writeFile(bx,cx,dx);
        }else if(ax==20)
        {
          listFiles();
        } else
        {
            printString("Invalid interrupt!\0");
        }
}

void handleTimerInterrupt(int segment, int sp)
{

    int i;
    int newSp;
    int newSegment;
    int flag = 0;
    int c=0;

    processTable[currentProcess].time++;

    if(processTable[currentProcess].time==100)
    {
      processTable[currentProcess].sp = sp;
      processTable[currentProcess].time = 0;

      for(i=currentProcess+1; 1;i++)
      {
        c++;
        if(c==7)
        {
          break;
        }
        if(i==7)
        {
          i=0;
        }
        flag = 0;
        if(processTable[i].active == 1 )
        {
          currentProcess = i;
          printString(processTable[currentProcess].name);
          newSegment = (i+2)*0x1000;
          newSp = processTable[currentProcess].sp;
          flag = 1;
          break;
        }
      }
    }
    if(!flag)
    {
      printString("notFlag\0");
      returnFromTimer(segment,sp);
    }
    else if(flag)
    {
      printString("flag\0");
      returnFromTimer(newSegment,newSp);
    }
   // if(flag)


}

void executeProgram(char* name)
{
    int i;
    int j;
    int segment;
    char readingBuffer[13312];

    for(j = 0 ; j < 8; j++){
      setKernelDataSegment();
     if(processTable[j].active == 0){
       currentProcess = j;
       segment = (j+2)*0x1000;
       processTable[j].active = 1;
       processTable[j].name = name;
       restoreDataSegment();
       break;
     }
   }

    readFile(name, readingBuffer);

    for (i=0; i<13312;i++){
        putInMemory(segment,i,readingBuffer[i]);
    }
    initializeProgram(segment);
}

void readFile(char* fileName,char* buffer){
    int fileFound;
    int nameCt = 0;
    int index, k,h;
    int sectors[27];
    int j = 0;
    int i;
    int buffAddress = 0;


    /* Read in the directory sector */
    readSector(buffer, 2);

    /* Try to find the file name */
    fileFound = strComp(buffer,fileName);

    if (fileFound!=0){

        index = fileFound*32+6;
        for (j=0;j<26;j++){
            sectors[j] = buffer[index+j];

        }

        sectors[26] = 0;
        k = 0;
        while(sectors[k]!=0x0){
            readSector(buffer+buffAddress,sectors[k]);
            buffAddress += 512;
            k++;
        }

    }
    else{
        printString("File Not Found!");
        return;
    }

}

void readSector(char* buffer, int sector)
{

        int relSector = mod(sector,18) + 1;
        int op = div(sector,18);
        int head = mod(op,2);
        int track = div(sector,36);
        interrupt(0x13,2*256+1,buffer,track*256+relSector,head*256+0);
}

void writeSector(char* buffer, int sector)
{

        int relSector = mod(sector,18) + 1;
        int op = div(sector,18);
        int head = mod(op,2);
        int track = div(sector,36);
        interrupt(0x13,3*256+1,buffer,track*256+relSector,head*256+0);
}

int strComp(char* buffer, char* fileName)
{
    int i, j;

    int checkFound = 0;


     for (i = 0; i < 16; i++)
    {
         /* If there is a file at the location */
        if (buffer[32*i] != 0x0)
        {
            /* Check the first characters */
            for (j=0; j < 6; j++)
            {
                /* This is a match */
                /*printString("Checking: ");
                printChar(buffer[j+32*i]);
                printString(" with ");
                printChar(fileName[j]);*/

                if (buffer[j+32*i] == '\0' || buffer[j+32*i] == '\r' || buffer[j+32*i] == '\n' || fileName[j] == '\0' || fileName[j] == '\r' || fileName[j] == '\n'){
                    break;
                }
                else if (buffer[j+32*i] == fileName[j])
                {
                    checkFound = 1;
                }
                else
                {
                    checkFound = 0;
                    break;
                }
            }

            if (checkFound == 1)
            {

                 return i;
            }
        }
    }
     if (checkFound == 0)
     {
         for (i=0;i<13312;i++)
         {
            buffer[i] = 0x0;
         }
        return 0;
     }
}


int mod(int a, int b)
{
    while(a >= b){
        a = a - b;
    }
    return a;
}

int div(int a, int b)
{
    int q = 0;
    while(q*b <=a){
        q = q+1;
    }
    return q-1;

}
/*
void printString(char* chars)
{
    int i = 0;
    while(chars[i] != '\0'){
            int ch = chars[i];
                interrupt(0x10, 0xe*256+ch, 0, 0, 0);
            i++;
    }
}
*/

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
/*
void readString(char* buff)
{
    int dashn = 0xa; //line feed
    int endStr = 0x0; // end of string
    int enter = 0xd; //enter
    int backsp = 0x8; //backspace
    int dashr = 0xd;
    int loop = 1;
    int count = 2;
    buff[0] = dashr;
    buff[1] = dashn;
    while(loop){


            int ascii = interrupt(0x16,0,0,0,0); //read interrupt
            if (ascii == enter){
                buff[count] = 0x0;
                buff[count+1] = dashr;
                buff[count+2] = dashn;
                return;
            }
            else if (ascii == backsp){
                if (count > 1){
                    buff[count] = 0x0;
                    count--;
                    interrupt(0x10,0xe*256+0x8,0,0,0);
                    count++;
                    interrupt(0x10,0xe*256+0x0,0,0,0);
                    count--;
                    interrupt(0x10,0xe*256+0x8,0,0,0);
               }
            }
            else{
                buff[count] = ascii;
                interrupt(0x10, 0xe*256+ascii, 0, 0, 0);
                count++;
            }
    }
}
*/
 void terminate(){

  /*  char shell[6];
    shell[0] = 's';
    shell[1] = 'h';
    shell[2] = 'e';
    shell[3] = 'l';
    shell[4] = 'l';
    shell[5] = 0x0;
    interrupt(0x21,4,shell,0x3000,0);
    */
    setKernelDataSegment();
    processTable[currentProcess].active = 0;
    restoreDataSegment();
    while (1)
    {
    }
}

void deleteFile(char* name)
{
    char map[512];
    char dir[512];
    char sectors[26];
    int file, index, i, scount = 0;

    readSector(map,1);
    readSector(dir,2);

    //printString("HERE");
    file=strComp(dir,name);
    if (file !=0)
    {
         dir[file*32]=0x0;
         index = (file*32)+6;
         for(i=0; i<26; i++)
         {
            if(dir[index+i]==0x0) break;
            sectors[i]= dir[index+i];
            scount++;
         }
         for(i=0; i<scount; i++)
         {
            map[sectors[i]]=0x0;
         }

         writeSector(map,1);
         writeSector(dir,2);

    }
    else
    {
        printString("File not found!");
    }
}

void writeFile(char* name, char* buffer, int numSecs)
{
    char map[512];
    char dir[512];
    int i,j,k,l, buffAdd=0;

    readSector(map,1);
    readSector(dir,2);

    // set the file's name in the directory
    for(i=0; i<16; i++)
    {
        if(dir[i*32]==0x0)
        {
            for(j=0; j<6; j++)
            {
                if(name[j]!=0x0)
                {
                    dir[(i*32)+j]=name[j];
                }
                else
                {
                    dir[(i*32)+j]=0x0;
                    break;
                }
            }
            for(; j<6; j++) dir[(i*32)+j]=0x0;
        break;
        }
    }

    //search through the map for empty secs
    for(k=0; k<512; k++)
    {
        if(map[k]==0x0)
        {
            map[k]=0xFF;
            dir[(i*32)+6+l] = k;
            writeSector(buffer+buffAdd,k);
            buffAdd+=512;
            l++;
            if(l==numSecs) break;
        }
    }
    if(k==512)
    {
        printString("The map is full !");
        return;
    }
    while(l<26)
    {
        dir[(i*32)+6+l]=0x0;
        l++;
    }
    writeSector(map,1);
    writeSector(dir,2);
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

void printChar(char ch){
    char* chars[2];
    chars[0] = ch;
    chars[1] = '\0';
    printString(chars);
}
void prtInt(int i){
    int* chars[2];
    chars[0] = i;
    chars[1] = '\0';
    printString(chars);

}
