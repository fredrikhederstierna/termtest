#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#include <readline/readline.h>
#include <readline/history.h>

//-------------------------------
static const char* __keywords[] =
{
  "read", "write", "poll", "quit", "\0"
};

//-----------------------
static char * __dupstr(const char* s)
{
  int slen = strnlen(s,1024) + 1;
  char *copy = (char*) malloc(slen);
  if (copy == NULL) {
    fprintf(stderr, "ERROR: Out of memory, exit(1).\n");
    exit(1);
  }
  strcpy(copy, s);
  return copy;
}

//--------------------------------
static char* __custom_match_generator(const char* text, int state)
{
  static int keyword_list_index = 0;
  static int text_len = 0;

  // if reset list index and check len
  if (state == 0) {
    text_len = strlen(text);
    keyword_list_index = 0;
  }

  // iterate through keywords list, check EOL
  const char *keyword = NULL;
  do {
    keyword = __keywords[ keyword_list_index++ ];
    if (strncmp(keyword, text, text_len) == 0) {
      // if found match return copy
      return __dupstr(keyword);
    }
  } while (strlen(keyword) > 0);

  // no names matched, return NULL
  return ((char *)NULL); 
}

//--------------------------
static char** __custom_completion(const char *text, int start, int end)
{
  (void)end; // remove warning  
  char **matches = (char **)NULL;

  if (start == 0) {
    matches = rl_completion_matches(text, &__custom_match_generator);
  }
  else {
    //rl_bind_key('\t', rl_abort);
    rl_bind_key('\t', rl_insert);
  }

  // this will suppress extra ' ' put after keyword
  //rl_completion_append_character = '\0';
  rl_completion_suppress_append = 1;

  return matches;
}

//---------------------------
int main (int argc, char **argv)
{
#if 1
  int vflag = 0;
  char *pvalue = NULL;

  opterr = 0;

  // parse command line args
  int c;
  while ((c = getopt(argc, argv, "vp:")) != -1)
    switch (c)
      {
      case 'v':
        vflag = 1;
        break;
      case 'p':
        pvalue = optarg;
        break;
      case '?': {
        if (optopt == 'c')
          fprintf(stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint(optopt))
          fprintf(stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
      }
      default:
        exit(1);
      }

  if (vflag) {
    printf("version 0.1\n");
    exit(0);
  }

  if (pvalue) {
    printf("open port pvalue %s\n", pvalue);
  }
  else {
    printf("no port specified, use -p PORT\n");
    exit(1);
  }

  int i;
  for (i = optind; i < argc; i++) {
    printf ("Non-option argument %s\n", argv[i]);
    exit(1);
  }
#endif

  // args parsed, now exec readline stuff

#if 1
  rl_attempted_completion_function = __custom_completion;
  //clear_history();
  
  char *buf = NULL;
  while ((buf = readline("TERMTEST> ")) != NULL) {
    //enable auto-complete
    rl_bind_key('\t', rl_complete);
    // check null
    if (buf) {
      // check keyword
      printf("keyword [%s]\n", buf);
      // do command
      if (strcmp(buf, "quit") == 0) {
        printf("Exit, bye.\n");
        break;
      }
      // add buffer to readline history
      if (buf[0] != 0) {
        add_history(buf);
      }
    }
  }
  
  free(buf);
#endif

  return 0;
}
