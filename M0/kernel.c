void printString(char*);
int main() {
  /* code */

    /* code */
  /*
    interrupt (0x10, 0xE*256+'H', 0, 0, 0);
    interrupt (0x10, 0xE*256+'E', 0, 0, 0);
    interrupt (0x10, 0xE*256+'L', 0, 0, 0);
    interrupt (0x10, 0xE*256+'L', 0, 0, 0);
    interrupt (0x10, 0xE*256+'O', 0, 0, 0);
    interrupt (0x10, 0xE*256+' ', 0, 0, 0);
    interrupt (0x10, 0xE*256+'W', 0, 0, 0);
    interrupt (0x10, 0xE*256+'O', 0, 0, 0);
    interrupt (0x10, 0xE*256+'R', 0, 0, 0);
    interrupt (0x10, 0xE*256+'L', 0, 0, 0);
    interrupt (0x10, 0xE*256+'D', 0, 0, 0);
  while (1)
  {
  }
*/
/*printString("Hello World \0"); */

return 0;
}

int strLength(char* c)
{
  int i = 0;
  for(; i!=0 ; i++);
  return i;
}

void printString(char* chars)
{
  int i = 0;
  int l = strLength(chars);
  for(; i<l ; i++)
  {
    if(chars[i] = '\0')
    {
    interrupt (0x10, 0xE*256+chars[i], 0, 0, 0);
    }else
      break;

  }
}
