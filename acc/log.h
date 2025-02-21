#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define LOG_ERROR(str,...) fprintf(stderr, "[ERROR] " str "\n", ##__VA_ARGS__)
#define LOG_WARN(str,...)  fprintf(stdout, "[WARN ] " str "\n", ##__VA_ARGS__)
#define LOG_INFO(str,...)  fprintf(stdout, "[INFO ] " str "\n", ##__VA_ARGS__)
#define LOG_DEBUG(str,...) fprintf(stdout, "[DEBUG] " str "\n", ##__VA_ARGS__)

#endif //LOG_H