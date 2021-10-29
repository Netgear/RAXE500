#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



#define MIN(a,b) (((a) < (b)) ? (a) : (b))

int rline(char *b) 
{
  int i;
  int n;
  i = 0;
  do {
    n = read(STDIN_FILENO, b+i, 1);
    i++;
  } while ((n > 0) && (b[i-1] != '\n') && (b[i-1] != '\0'));
  b[--i] = 0;
  return(i);
}

int main ()
{
  char buf[128];
  int i;
  long int off;
  int len;
  int new;
  char fname[128];
  char flen[128];
  off = 0;
  i = 0;

  while (1)
    {
      if (rline(fname) == 0) {
        return(0);
      }
      printf ("filename %s\n", fname);
      rline(flen);
      len = (int) strtoul (flen, NULL, 10);
      printf ("size %s parsed to %d\n", flen, len);
      new = creat(fname, 0777);
      i = 0;
      while (0 != (i = read (STDIN_FILENO, buf, MIN (128, len))))
	{
	  len = len - i;
	  printf ("%d bytes %d remain\n", i, len);
          write(new, buf, i);
	}
      close(new);
      i = 0;
    }
}

