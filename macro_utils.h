#ifndef MACRO_UTILS_H
#define MACRO_UTILS_H

#define MULT_SCALE_TO_PERCENT(mult)		((mult)*100)
#define PERCENT_TO_MULT_SCALE(percent)	(((float)(percent)/100))
#define NK_TXT(txt) txt, strlen(txt)
#define NK_TXT_PAD(txt) txt" "
#define RAND(max) (rand() % max)
#define RANDRANGE(min, max) (RAND(max + 1 - min) + min)
#define M_PI 3.14

#define DEG_TO_RAD 0.0174533

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

#endif