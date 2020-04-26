void doCommand(char* , char*);
int main() {

int view;
int execute;
int delete;
int dir;

char view1 [5];
char execute1 [8];
char delete1[7];
char dir1[4];

while (1)
{
int i;
int j;
int k;
char buffer [512];
char command[8];
char fileName[6];

view = 0;
execute = 0;
delete = 0;
dir = 0;


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

dir1[0] = 'd';
dir1[1] = 'i';
dir1[2] = 'r';
dir1[3] = '\0';


interrupt(0x21,0,"\n\0",0,0);
interrupt(0x21,0,"Shell =>\0",0,0);
interrupt(0x21,1,buffer,0,0);


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
}
else if(command[0] == delete1[0] && command[1] == delete1[1] && command[2] == delete1[2] && command[3] == delete1[3] && command[4] == delete1[4] && command[5] == delete1[5])
{
  delete = 1;
  i = 7;
}else if(command[0] == dir1[0] && command[1] == dir1[1] && command[2] == dir1[2])
{
    dir = 1;
    i=4;
}


for(j=0;j<7;j++)
{
  fileName[j] = buffer[i+j];
}
if(fileName[i+1]=='\0' && view)
{
  // interrupt(0x21,0,"Enter file name\0",0,0);
}else if(fileName[i+1] =='\0' && execute)
{
  interrupt(0x21,0,"Enter program name\0",0,0);
}else if(fileName[i+1] == '\0' && delete)
{

}

for(k=0;k<512;k++)
{
  buffer[i] = '\0';
}

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
    doCommand(delete1, fileName);
  }
  else if(dir)
  {
    char emptyBuffer[1];
    doCommand(dir1, emptyBuffer );// not complete
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
      interrupt(0x21,5,0,0,0);

  }
  else if(*(command) == 'd' && *(command+1) == 'e' && *(command+2) == 'l' && *(command+3) == 'e' && *(command+4) == 't' && *(command+5) == 'e')
  {
    interrupt(0x21,7,filename,0,0);
    interrupt(0x21,5,0,0,0);

  }
  else if(*(command) == 'd' && *(command+1) == 'i' && *(command+2) == 'r')
  {
    interrupt(0x21,10,0,0,0 );
  }
  else
  {
    interrupt(0x21,0,"Command not found!!\0",0,0);
    interrupt(0x21,0,"\n",0,0);
    interrupt(0x21,5,0,0,0);
  }
}
