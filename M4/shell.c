void doCommand(char* , char*);
void doCopyCommand(char*, char*);
void doCreateCommand(char*);
void clearBuffer(char*,int);
int div(int a, int b);
int main() {

int view;
int execute;
int delete;
int copy;
int dir;
int create;

int i;
int j;
int k;
int m;

char view1 [5];
char execute1 [8];
char delete1[7];
char copy1[5];
char dir1[4];

view1[0]='v';
view1[1]='i';
view1[2]='e';
view1[3]='w';
view1[4]='\0';

execute1[0]='e';
execute1[1]='x';
execute1[2]='e';
execute1[3]='c';
execute1[4]='u';
execute1[5]='t';
execute1[6]='e';
execute1[7]='\0';

delete1[0] = 'd';
delete1[1] = 'e';
delete1[2] = 'l';
delete1[3] = 'e';
delete1[4] = 't';
delete1[5] = 'e';
delete1[6] = '\0';

copy1[0] = 'c';
copy1[1] = 'o';
copy1[2] = 'p';
copy1[3] = 'y';
copy1[4] = '\0';

dir1[0] = 'd';
dir1[1] = 'i';
dir1[2] = 'r';
dir1[3] = '\0';

while (1)
{

char buffer [512];
char command[8];
char fileName[6];
char fileName2[6];

view = 0;
execute = 0;
delete = 0;
copy = 0;
dir = 0;
create = 0;

interrupt(0x21,0,"\n\0",0,0);
interrupt(0x21,0,"Shell =>\0",0,0);
interrupt(0x21,1,buffer,0,0);

//interrupt(0x21,0,command,0,0);
for(i = 0 ;i<8;i++)
{
  if(buffer[i]==" ")
  break;

  command[i] = buffer[i];
}

if( command[0] == view1[0] && command[1] == view1[1] && command[2] == view1[2] && command[3] == view1[3])
{
  view = 1;
  i = 5;
}
else if(command[0] == execute1[0] && command[1] == execute1[1] && command[2] == execute1[2] && command[3] == execute1[3] && command[4] == execute1[4] && command[5] == execute1[5] && command[6] == execute1[6])
{
  execute = 1;
  i = 8;

}else if(command[0] == delete1[0] && command[1] == delete1[1] && command[2] == delete1[2] && command[3] == delete1[3] && command[4] == delete1[4] && command[5] == delete1[5])
{
  delete = 1;
  i = 7;
}else if(command[0] == copy1[0] && command[1] == copy1[1] && command[2] == copy1[2] && command[3] == copy1[3])
{
  copy = 1;
  i = 5;
}
else if(command[0] == dir1[0] && command[1] == dir1[1] && command[2] == dir1[2])
{
    dir = 1;
    i=4;
}
else if(command[0] == 'c' && command[1] == 'r' && command[2] == 'e' && command[3] == 'a' && command[4] =='t' && command[5] == 'e')
{
  create = 1;
  i=7;
}
/*
// handling the errors NOT DONE YET
if(view && (buffer[i]==' ' || buffer[i]==0x00))
{
  interrupt(0x21,0,"Enter file name",0,0);
  interrupt(0x21,5,0,0,0);
}else if(execute && (buffer[i]==' ' || buffer[i]==0x00))
{
  interrupt(0x21,0,"Enter program name",0,0);
  interrupt(0x21,5,0,0,0);
}else if(delete && (buffer[i]==' ' || buffer[i]==0x00))
{
  interrupt(0x21,0,"Enter file name",0,0);
  interrupt(0x21,5,0,0,0);
}
else if(copy && (buffer[i]==' ' || buffer[i]==0x00))
{
  interrupt(0x21,0,"Enter the name of the file you want to copy",0,0);
  interrupt(0x21,5,0,0,0);
}
*/

for(j=0;buffer[j+i]!='\0'&& buffer[j+i]!=0x00;j++)
{
  fileName[j] = buffer[i+j];
}
j++;



if(view)
{
  doCommand(view1 , fileName);
}
  else if(execute)
  {
    // interrupt(0x21,0,fileName,0,0);
    doCommand(execute1 , fileName);
  }else if(delete)
  {
    doCommand(delete1, fileName); // not finished
  } else if(copy)
  {/*
    if(buffer[i+j]==' ' || buffer[i+j]==0x00)
    {
      interrupt(0x21,0,"Enter the name of the new file",0,0);
      interrupt(0x21,5,0,0,0);
    }*/
    for(m=0;buffer[j+m+i]!='\0'&& buffer[j+m+i]!=0x00;m++)
    {
      fileName2[m] = buffer[j+m+i];
    }
    doCopyCommand(fileName,fileName2);
  } else if(dir)
    {
      interrupt(0x21,20,0,0,0 ); //not finished

    }
   else if(create)
   {
     doCreateCommand(fileName);
   }
   else
    {
    interrupt(0x21,0,"Command not found!!\0",0,0);
    interrupt(0x21,0,"\n",0,0);
    interrupt(0x21,5,0,0,0);
    }
}
  return 0;
}

void doCommand(char *command , char *filename )
{
  int l = 0;
  int k;
  char dir [13213];

//interrupt(0x21,0,command[0],0,0);
  if(*(command) == 'v' && *(command+l+1) == 'i' && *(command+l+2) == 'e' && *(command+l+3) == 'w' )
  {
    interrupt(0x21, 3, filename, dir, 0);
    interrupt(0x21, 0 , dir, 0,0);
    interrupt(0x21,5,0,0,0);
  }else if (*(command) == 'e' && *(command+1) == 'x' && *(command+2) == 'e' && *(command+3) == 'c' && *(command+4) == 'u' && *(command+5) == 't' && *(command+6) == 'e' )
  {
      // interrupt(0x21,0,filename,0,0);
      // interrupt(0x21,0,"start exe",0,0);
      interrupt(0x21,4, filename, 0x2000, 0);
  } else if(*(command) == 'd' && *(command+1) == 'e' && *(command+2) == 'l' && *(command+3) == 'e' && *(command+4) == 't' && *(command+5) == 'e')
  {
      interrupt(0x21,7,filename,0,0);
      interrupt(0x21,5,0,0,0);

  }
  else
  {
    interrupt(0x21,0,"Command not found!!\0",0,0);
    interrupt(0x21,0,"\n",0,0);
  }
}

void doCopyCommand(char *fileName, char *newFileName)
{
  int numOfsectors=0;
  char buffer[13312];
  int i;
  interrupt(0x21,3,fileName,buffer,0);

  for(i=0;buffer[i]!=0x0;i++);

  numOfsectors =div(i,512)+1;
  interrupt(0x21,8,newFileName,buffer,numOfsectors);
  interrupt(0x21,0,"File has been copied\0",0,0);
  interrupt(0x21,5,0,0,0);
}

void doCreateCommand(char *fileName)
{
  char curLine[1000];
  char buffer[13312];
  int numOfsectors;
  int numOfChars=0;
  int i;
  int flag=1;
  int flag2=1;

  interrupt(0x21,0,"Start typing here:\n",0);
  while(flag)
  {
    clearBuffer(curLine,1000);
    interrupt(0x21,1,curLine,0,0);

    if(curLine[0]==0xd)
    {
      flag=0;
    }
    flag2=1;
    for(i=0;i<512 && flag && flag2;i++)
    {
      buffer[numOfChars]=curLine[i];
      numOfChars++;

      if(curLine[i]==0xd)
      {
        flag2=0;

      }
    }
  }
  numOfsectors = div(numOfChars,512)+1;
  interrupt(0x21,8,fileName,buffer,numOfsectors);
  interrupt(0x21,0,"create is done\0",0);
  interrupt(0x21,5,0,0,0);

}

int div(int a, int b)
{
    int q = 0;
    while(q*b <=a){
        q = q+1;
    }
    return q-1;

}

void clearBuffer(char* buffer, int len){
	int i;
	for(i=0;i<len;i++){
		buffer[i] = 0x0;
	}
}
