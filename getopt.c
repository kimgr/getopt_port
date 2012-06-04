#include "getopt.h"

#include <stddef.h>
#include <string.h>
#include <assert.h>

char *optarg;
int optopt;
int optind = 1; /* The variable optind [...] shall be initialized to 1 by the system */
int opterr;

static char* curchar = NULL;

/* Implemented based on http://pubs.opengroup.org/onlinepubs/000095399/functions/getopt.html */
int getopt(int argc, char* argv[], const char *optstring)
{
  int optchar = -1;
  const char* optdecl = NULL;

  optarg = NULL;
  opterr = 0;
  optopt = 0;

  /* This is not specified, but we need it to avoid overrunning the argv bounds */
  if (optind >= argc)
    return -1;

  /* If, when getopt() is called argv[optind] is a null pointer, getopt() shall return -1 without changing optind. */
  if (argv[optind] == NULL)
    return -1;

  /* If, when getopt() is called *argv[optind]  is not the character '-', getopt() shall return -1 without changing optind. */
  if (*argv[optind] != '-')
    return -1;

  /* If, when getopt() is called argv[optind] points to the string "-", getopt() shall return -1 without changing optind. */
  if (strcmp(argv[optind], "-") == 0)
    return -1;

  /* If, when getopt() is called argv[optind] points to the string "--", getopt() shall return -1 after incrementing optind. */
  if (strcmp(argv[optind], "--") == 0)
  {
    ++optind;
    return -1;
  }

  if (curchar == NULL || *curchar == '\0')
    curchar = argv[optind] + 1;

  optchar = *curchar;

  /*  The getopt() function shall return the next option character (if one is found) from argv
      that matches a character in optstring, if there is one that matches */
  optdecl = strchr(optstring, optchar);
  if (optdecl)
  {
    if (optdecl[1] == ':')
    {
      optarg = ++curchar;
      if (*optarg == '\0')
      {
        if (optind < argc - 1)
        {
          optarg = argv[++optind];
        }
        else
        {
          optopt = optchar;
          optchar = (optstring[0] == ':') ? ':' : '?';
        }
      }
      else
      {
        curchar = NULL;
      }
    }
  }
  else
  {
    /* If getopt() encounters an option character that is not contained in optstring, it shall return the question-mark ( '?' ) character. */
    /* [...] getopt() shall set the variable optopt to the option character that caused the error. */
    optopt = optchar;
    optchar = '?';
  }

  if (curchar == NULL || *++curchar == '\0')
    ++optind;

  return optchar;
}

int getopt_long(int argc, char * argv[], 
      const char *optstring, const struct option *longopts, 
      int *longindex)
{
  return -1;
}
