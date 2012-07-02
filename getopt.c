#include "getopt.h"

#include <stddef.h>
#include <string.h>
#include <assert.h>

const int no_argument = 0;
const int required_argument = 1;
const int optional_argument = 2;

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
    goto no_more_optchars;

  /* If, when getopt() is called argv[optind] is a null pointer, getopt() shall return -1 without changing optind. */
  if (argv[optind] == NULL)
    goto no_more_optchars;

  /* If, when getopt() is called *argv[optind]  is not the character '-', getopt() shall return -1 without changing optind. */
  if (*argv[optind] != '-')
    goto no_more_optchars;

  /* If, when getopt() is called argv[optind] points to the string "-", getopt() shall return -1 without changing optind. */
  if (strcmp(argv[optind], "-") == 0)
    goto no_more_optchars;

  /* If, when getopt() is called argv[optind] points to the string "--", getopt() shall return -1 after incrementing optind. */
  if (strcmp(argv[optind], "--") == 0)
  {
    ++optind;
    goto no_more_optchars;
  }

  if (optcursor == NULL || *optcursor == '\0')
    optcursor = argv[optind] + 1;

  optchar = *optcursor;

  /*  The getopt() function shall return the next option character (if one is found) from argv
      that num_matches a character in optstring, if there is one that num_matches */
  optdecl = strchr(optstring, optchar);
  if (optdecl)
  {
    /* [I]f a character is followed by a colon, the option takes an argument */
    if (optdecl[1] == ':')
    {
      optarg = ++optcursor;
      if (*optarg == '\0')
      {
        /* GNU extension: Two colons mean an option takes an
           optional arg; if there is text in the current argv-element (i.e., in the same
           word as the option name itself, for example, "-oarg"), then it is returned in
           optarg, otherwise optarg is set to zero. */
        if (optdecl[2] != ':')
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
            optcursor = NULL;
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
          optarg = NULL;
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

no_more_optchars:
  optcursor = NULL;
  return -1;
}

static const char* get_arg_end(const char* argument)
{
  while (*argument != '\0' && *argument != '=')
    ++argument;

  return argument;
}

int getopt_long(int argc, char * argv[], 
      const char *optstring, const struct option *longopts, 
      int *longindex)
{
  const struct option* o = longopts;
  const struct option* match = NULL;
  int num_matches = 0;
  size_t argument_name_length = 0;
  const char* current_argument = NULL;

  if (optind >= argc)
    return -1;

  if (strlen(argv[optind]) < 3 || strncmp(argv[optind], "--", 2) != 0)
    return getopt(argc, argv, optstring);

  /* It's an option; starts with -- and is longer than 2 chars */
  current_argument = argv[optind] + 2;
  argument_name_length = strcspn(current_argument, "=");
  for (; o->name; ++o)
  {
    if (strncmp(o->name, current_argument, argument_name_length) == 0)
    {
      match = o;
      ++num_matches;
    }
  }

  /* Unknown option or ambiguous match */
  if (num_matches != 1)
    return '?';

  /* If longindex is not NULL, it points to a variable which is set to the index of the long option relative to longopts */
  if (longindex)
    *longindex = (match - longopts);

  /* If flag is NULL, then getopt_long() shall return val. 
    Otherwise, getopt_long() returns 0, and flag shall point to a variable which shall be 
    set to val if the option is found, but left unchanged if the option is not found. */
  if (match->flag)
    *(match->flag) = match->val;

  if (match->has_arg != no_argument)
  {
    optarg = strchr(argv[optind], '=');
    if (optarg != NULL)
      ++optarg;

    if (optarg == NULL && optind < argc - 1)
    {
      optarg = argv[++optind];
    }
  }

  if (match->has_arg == required_argument && optarg == NULL)
    return ':';

  ++optind;
  return match->flag ? 0 : match->val;
}
