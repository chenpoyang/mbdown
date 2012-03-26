#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "global.h"

void m_tolower(char *str)
{
	int i, len; 

	assert(str != NULL);

	len = strlen(str);
	for (i = 0; i < len; ++i)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			str[i] = 'a' + str[i] - 'A';
		}
	}
}
