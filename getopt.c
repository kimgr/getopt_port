#include "getopt.h"

#include <stddef.h>
#include <string.h>
#include <assert.h>

char *optarg;
int optopt;
int optind = 1; /* The variable optind [...] shall be initialized to 1 by the system */
int opterr;

static char* optcursor = NULL;

/* Implemented based on http://pubs.opengroup.org/onlinepubs/000095399/functions/getopt.html */
int getopt(int argc, char* argv[], const char *optstring)
{
  int optchar = -1;
  const char* optdecl = NULL;

  optarg = NULL;
  opterr = 0;
  optopt = 0;

  /* Unspecified, but we need it to avoid overrunning the argv bounds */
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

  if (optcursor == NULL || *optcursor == '\0')
    optcursor = argv[optind] + 1;

  optchar = *optcursor;

  /*  The getopt() function shall return the next option character (if one is found) from argv
      that matches a character in optstring, if there is one that matches */
  optdecl = strchr(optstring, optchar);
  if (optdecl)
  {
    /* [I]f a character is followed by a colon, the option takes an argument */
    if (optdecl[1] == ':')
    {
      optarg = ++optcursor;
      if (*optarg == '\0')
      {
        /* If the option was the last character in the string pointed to by an element of argv, 
            then optarg shall contain the next element of argv, and optind shall be incremented 
            by 2. If the resulting value of optind is greater than argc, this indicates a missing 
            option-argument, and getopt() shall return an error indication.

            Otherwise, optarg shall point to the string following the option character in that 
            element of argv, and optind shall be incremented by 1.
        */
        if (optind < argc - 1)
        {
          optarg = argv[++optind];
          optcursor = optarg + 1;
        }
        else
        {
          /* If it detects a missing option-argument, it shall return the colon character ( ':' ) if the first character of optstring was a colon, or a question-mark character ( '?' ) otherwise. */
          /* [...] getopt() shall set the variable optopt to the option character that caused the error. */
          optopt = optchar;
          optchar = (optstring[0] == ':') ? ':' : '?';
        }
      }
      else
      {
        optcursor = NULL;
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

  if (optcursor == NULL || *++optcursor == '\0')
    ++optind;

  return optchar;
}

int getopt_long(int argc, char * argv[], 
      const char *optstring, const struct option *longopts, 
      int *longindex)
{
  return -1;
}
