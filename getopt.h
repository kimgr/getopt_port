#ifndef INCLUDED_WIN32_GETOPT_H
#define INCLUDED_WIN32_GETOPT_H

#if defined(__cplusplus)
extern "C" {
#endif

extern const int no_argument;
extern const int required_argument;
extern const int optional_argument;

extern char* optarg;
extern int optind, opterr, optopt;

struct option {
  const char* name;
  int has_arg;
  int* flag;
  int val;
};

int getopt(int argc, char* const argv[], const char* optstring);

int getopt_long(int argc, char* const argv[],
  const char* optstring, const struct option* longopts, int* longindex);

#if defined(__cplusplus)
}
#endif

#endif // INCLUDED_WIN32_GETOPT_LONG
