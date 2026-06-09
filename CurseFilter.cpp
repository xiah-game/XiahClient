#include "precompile.h"

#include <string.h>
#include <stdio.h>

#include "CurseFilter.h"

#define MAX_CURSE_STRING	400000
#define HUGE_NUMBER		1024

//static char curse_string[MAX_CURSE_STRING];
char *curse_string;
static bool bInit = false;

static inline void filterChar(const char *src, char *dst, const char *chars)
{
    char *srcp = (char *) src;
    char *dstp = dst;

    while (*srcp) {
	if (strchr(chars, *srcp) == NULL) {
	    *dstp = *srcp;   dstp += 1;
	} else {
	    int msb, lsb;
	    msb = *(unsigned char *)(srcp-1);
	    lsb = *(unsigned char *)srcp;
	    if (srcp > src && (
		(msb == 0x82 && lsb > 0x9e && lsb < 0xf2)	// hirakana
		|| (msb == 0x83 && lsb > 0x3f && lsb < 0x97)	// katakana
	       	|| (msb >= 0x88 && msb <= 0x9f && msb != 0x7f	// kanji 1 group
		    && lsb > 0x3f && lsb < 0xfd)
		|| (msb >= 0xe0 && msb <= 0xfc && msb != 0x7f	// kanji 2 group
		    && lsb > 0x3f && lsb < 0xfd))) {
		*dstp = *srcp;   dstp += 1;
	    }
	}
	srcp += 1;
    }
    *dstp = 0;
}

static inline char *getField(const char *buf, int delim, char *field)
{
    unsigned char *src = (unsigned char *)buf;
    while ((int)*src == delim && *src != 0) src++;
    while ((int)*src != delim && *src != 0) *field ++ = *src ++;

    *field = '\0';
    return (char *)src;
}

bool LoadCurses(const char *filename)
{
    static char comment_chars[] = { "$;\n" };
    char buf[128];
    FILE *file;

    if (bInit) return true;

	curse_string = new char [MAX_CURSE_STRING];
    char *ptr = curse_string;

    if (file = fopen(filename, "r"), !file) 
	{
		//fprintf(stderr, "! can't open [%s]\n", filename);
		return false;
    }

    while (fgets(buf, sizeof(buf), file)) 
	{
		// Decode
		for(int i=0;i<strlen(buf);i++)
		{
			if(buf[i] != 0x9 && buf[i] != 0x0a)
			{
				buf[i] = buf[i] ^ 0x1f;
			}
		}

		if (strchr(comment_chars, buf[0])) continue;

    	ptr += sprintf(ptr, buf);

    	if (ptr - curse_string >= MAX_CURSE_STRING) {
    	    //fprintf(stderr, "! %s is too long, pass.\n", filename);
    	    curse_string[MAX_CURSE_STRING - 1] = 0;
    	    break;
    	}
    }
    fclose(file);

    //fprintf(stderr, "* parsing [%s]. %d bytes read\n", filename, ptr - curse_string);

    return true;
}


bool UnloadCurses()
{
	delete [] curse_string;
	curse_string = NULL;
	return true;
}

bool IsCurse(const char * str)
{
    char string[128], line[128], word[128];
    char *ptr = curse_string;
    const char *sub, *subs, *subw;

    if (!str) return false;

    // filter characters to keep users from
    // speaking curses mixed with white spaces 
    // or punctuaion mark

    filterChar(str, string, "\t _-.:^");


    // optimized for multibyte code set.

    while (ptr = getField(ptr, '\n', line), line[0]) {
	getField(line, '\t', word);	
	sub = string;
	while (*sub && *(sub + 1)) {
	    subs = sub;	// current comparison pointer in string
	    subw = word;	// current comparison pointer in word
	    while (*subs && (*subs == *subw)) {
		subs++; subw++;
		if (*subw == 0) {
		    return true;		// reach to null, curse
		}
	    }
	    sub += *sub < 0 ? 2 : 1;
	}
    }

    return false;
}

char *ConvertString(char * str, int max_len)
{
    char result[HUGE_NUMBER], string[HUGE_NUMBER];
    char line[128], curse[128], replace[128];
    char *ptr = curse_string, *lptr, *cptr;

    if (!str) return NULL;

    if (!max_len || max_len >= sizeof(result)) return str;

    // skip converting, if no curse is found.
    // in fact, 'isCurse' and 'convert' are redundant.
    // but it can be ignored and more efficient 
    // because users speak a normal sentence 
    // much more than a curse

    if (!IsCurse(str)) return str;


    // filter characters to keep users from
    // speaking curses mixed with white spaces 
    // or punctuaion mark
    filterChar(str, string, "\t _-.:^");

    while (ptr = getField(ptr, '\n', line), line[0]) {

	lptr = line;
	lptr = getField(lptr, '\t', curse);		// get the first field	
	lptr = getField(lptr, '\t', replace);	// get the second field
	cptr = string;
	strcpy(result, string);

	while (cptr = strstr(string, curse), cptr) {
	    // if there is no matching replacement of curse word
	    if (!replace[0]) {
		str[0] = NULL;
		return str;
	    }

	    if ((max_len - 1) < (cptr - string) + strlen(replace) + strlen(cptr + strlen(curse))) {
                // string too long, remove the curses that linger around
                strncpy(result, string, cptr - string);
                strcpy(result + (cptr - string), cptr + strlen(curse));
            } else {               	    
	        strncpy(result, string, cptr - string);
	        strcpy(result + (cptr - string), replace);
	        strcpy(result + (cptr - string + strlen(replace)), cptr + strlen(curse));
            }
	    // refresh intermediate result
	    strcpy(string, result);
	}
    }

    result[max_len] = 0;		// prevent overflow
    strcpy(str, result);

    return str;
}
