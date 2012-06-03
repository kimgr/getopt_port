#include "getopt.h"

#include <stddef.h>
#include <string.h>
#include <assert.h>

char *optarg;
int optopt, optind = 1;
int opterr; // we don't use this

static char* nextchar;

//// Implemented based on http://pubs.opengroup.org/onlinepubs/000095399/functions/getopt.html
//// except I never got it to work... :-(
//int getopt(int argc, char* argv[], const char *optstring)
//{
//  int optchar = -1;
//  const char* optdecl = NULL;
//  char* cur_argv = NULL;
//
//  optarg = NULL;
//  opterr = 0;
//  optopt = 0;
//
//  if (nextchar && *nextchar)
//    return *(++nextchar);
//
//  if (optind >= argc)
//    return -1;
//
//  cur_argv = argv[optind];
//  if (cur_argv == NULL)
//    return -1;
//
//  if (cur_argv[0] != '-')
//    return -1;
//
//  // If we get this far, the current argv starts with a dash. Break if that's all it is.
//  if (cur_argv[1] == '\0')
//    return -1;
//
//  assert(cur_argv[0] == '-');
//
//  if (cur_argv[1] != '-') // --
//  {
//    optchar = cur_argv[1];
//    optdecl = strchr(optstring, optchar);
//    if (optdecl)
//    {
//      if (optdecl[1] == ':')
//      {
//        nextchar = NULL;
//        optarg = cur_argv + 2;
//        if (*optarg == '\0')
//        {
//          if (optind < argc - 1)
//          {
//            optarg = argv[++optind];
//          }
//          else
//          {
//            optopt = optchar;
//            optchar = (optstring[0] == ':') ? ':' : '?';
//          }
//        }
//      }
//      else
//      {
//        if (strlen(cur_argv + 1) > 1)
//        {
//          nextchar = cur_argv + 1;
//          return (*nextchar);
//        }
//      }
//    }
//    else
//    {
//      nextchar = NULL;
//      optopt = optchar;
//      optchar = '?'; // unrecognized option
//    }
//  }
//
//  ++optind;
//  
//  return optchar;
//}

// Implemented based on http://pubs.opengroup.org/onlinepubs/000095399/functions/getopt.html
// except I never got it to work... :-(
int getopt(int argc, char* argv[], const char *optstring)
{
  int optchar = -1;
  const char* optdecl = NULL;

  optarg = NULL;
  opterr = 0;
  optopt = 0;

  if (optind >= argc)
    return -1;

  if (nextchar && *nextchar)
    return *(++nextchar);

  nextchar = argv[optind];
  if (nextchar == NULL)
    return -1;

  if (nextchar[0] != '-')
    return -1;

  // If the argv is only a dash, don't touch optind
  // and return -1
  if (nextchar[1] == '\0')
    return -1;

  assert(nextchar[0] == '-');

  if (nextchar[1] == '-') // -- means increase optind and return -1
  {
    ++optind;
    return -1;
  }

  optchar = nextchar[1];
  optdecl = strchr(optstring, optchar);
  if (optdecl)
  {
    if (optdecl[1] == ':')
    {
      optarg = nextchar + 2;
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

      nextchar = NULL;
    }
  }
  else
  {
    nextchar = NULL;
    optopt = optchar;
    optchar = '?'; // unrecognized option
  }

  if (*nextchar == '\0')
    ++optind;

  return optchar;
}

int getopt_long(int argc, char * argv[], 
      const char *optstring, const struct option *longopts, 
      int *longindex)
{
  return -1;
}
