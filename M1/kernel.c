int main() {
 void printString (char*);
 void readString (char*); 
 void readSector (char*,int);
 void handleInterrupt21(int , int , int , int );
 int mod(int);
 int div(int);
 char line[80];
makeInterrupt21();
interrupt(0x21,1,line,0,0);
interrupt(0x21,0,line,0,0);
 while (1)
  {
 
 
 /* printString("Reading from input:\n\r");
 readString(line);
 printString("Line read is:\n\r");
 printString(line);
 */

}

return 0;
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
    if (al == 13) interrupt(0x10,0xe*0x100+10,0x0,0x0,0x0);
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


void handleInterrupt21(int ax, int bx, int cx, int dx) {

if (ax==0) {
 printString(bx) ; }
  else { if (ax==1)
            readString(bx) ;
             else { if (ax==2) readSector(bx,cx) ;
                        else printString("ERROR") ; } }
}




