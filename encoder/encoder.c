#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MIN_INT32 -2147483648
#define MAX_INT32  2147483647

#define MAX_WIDTH	52
#define LENGTH		5000
#define FREQLEN		95

typedef struct element { char *str; double cpx;	} element;
typedef enum bool { false, true } bool;

bool check_int (char *s);
bool check_char (char *s);
int index_of (char *s, char c);
char least_seen (char *s);
char most_seen (char *s);
void make_cycle (char *s, int n);
int divide_length (char *s);
void strdel (char *s, int n);
char *revstring (char *s);

void enc_char (char *s, char *msg);
void enc_number (char *s);
void enc_word (char *s);

double complexity (const char *s);
void sorting (element *elements, int t);
element *partition (char *msg, int n);
char *restructuring (element *elements, int n);

int main()
{
	char str[MAX_WIDTH], str_nxt[MAX_WIDTH], msg[LENGTH]; 
	int n, nr = 0, cr = 0, wd = 0;

	char *r_msg;
	element *msg_r;

	fgets (str, MAX_WIDTH, stdin); 
	str[strlen (str) - 1] = '\0';
  	
	if (strcmp (str,"END") == 0)
		return 0;
	else
	{
		do
		{
			fgets (str_nxt, MAX_WIDTH, stdin);        
			str_nxt[strlen (str_nxt) - 1] = '\0';

			if (strcmp (str_nxt,"END") != 0)
			{	
				if (check_char (str))       
				{	
					cr++;
					enc_char (str, msg);
					strcat (msg, str);
				}
				else
					if (check_int (str))
					{
						nr++;
						enc_number (str);
						strcat (msg, str);
					}
					else
					{
						wd++;
						enc_word (str);
						strcat (msg, str);
					}
			}
			else
			{
				n = atoi (str);
				if (n == 0)
				{
					printf ("ERROR!\n");
					return 0;
				}
			}
			strcpy (str, str_nxt);

		} while (strcmp (str_nxt,"END") != 0);

		printf ("%d %d %d\n", wd, cr, nr);
		printf ("%s\n", msg);                 
		
		if (n > strlen (msg))
		{
			printf("The message cannot be crypted.\n");
			return 0;
		}

		msg_r = partition (msg, n);
		sorting (msg_r, n);
		r_msg = restructuring (msg_r, n);
		printf ("%s\n", r_msg);                
	} 

    
	free (r_msg);                            
	for (int index = 0; index < n; ++index)
		free (msg_r[index].str);
	free (msg_r);
	
	return 0;
}

bool check_int (char *s)
{
	int len = strlen(s);

	if ( !(isdigit(s[0]) || (s[0] == '-' && len > 1)) )
		return false;

	for (int index = 1; index < len; ++index)
		if (! isdigit (s[index]) || s[index] == '0')
				return false;

	long int x = atol (s);
	if ( x <  MIN_INT32 || x > MAX_INT32) return false; 

	return true;

}

bool check_char (char *s)
{
	if (s[1] == '\0')
		if (isdigit (s[0])) return false;
		else return true;
	else return false;
}

int index_of (char *s, char c)
{
	char *ptr = strchr(s, c);
	if (ptr)
		return (int) (ptr - s);
	else
		return -1;
}

char least_seen (char *s) 
{
	int freq[FREQLEN];
	
	int min = MAX_INT32;
	int index_min = -1;

	int len = strlen (s);
	
	memset(freq, 0, FREQLEN * sizeof(int));
	
	for (int index = 0; index < len; ++index)	
		freq[s[index] - 32]++;

	for (int index = 0; index < FREQLEN; ++index) 
		if (freq[index] > 0)
		{
			if (min > freq[index])
			{
				min = freq[index];
				index_min = index;
			}
			else
				if (min == freq[index] && (index_of(s, index + 32) < index_of (s, index_min + 32))) 
					{
						min = freq[index];
						index_min = index;
					}
		}

	if (index_min < 0) return (char)index_min;
	else return (char)index_min + 32;
}

char most_seen (char *s) 
{
	int freq[FREQLEN];

	int max = MIN_INT32;
	int index_max = -1;

	int len = strlen(s);

	memset(freq, 0, FREQLEN * sizeof(int));

	for (int index = 0; index < len; ++index)
		freq[s[index] - 32]++;

	for (int index = 0; index < FREQLEN; ++index)
		if (freq[index] > 0)
		{
			if (max < freq[index])
			{
				max = freq[index];
				index_max = index;
			}
			else
				if (max == freq[index] && (index_of(s, index + 32) < index_of(s, index_max + 32)))
				{
					max = freq[index];
					index_max = index;
				}
		}

	if (index_max < 0) return (char)index_max;
	else return (char)index_max + 32;
}

void enc_char (char *s, char *msg) 
{
	char chr;
	if (strlen (msg) != 0) 
	{
		chr = s[0];
		s[0] = most_seen (msg);
		s[1] = chr;
		s[2] = least_seen (msg);
		s[3] = '\0';
	}
}

void make_cycle (char *s, int n) 
{
	char c;
	int len = strlen(s);
	for (int count = 0; count < n; ++count)
	{
		c = s[0];
		for (int index = 1; index < len; ++index)
			s[index - 1] = s[index];
		s[len - 1] = c;
	}
}

void enc_number (char *s) 
{
	int len = strlen (s);

	if (s[0] == '-')
	{
		char min[MAX_WIDTH];

		strdel (s,1);
		strcpy (min, s);
		
		for (int count = 0; count < len; ++count)
		{
			make_cycle (s, 1);

			if(strcmp (min, s) >= 0)
				strcpy (min, s);
		}		

		strcpy (s, min);
	}
	else
	{
		char max[MAX_WIDTH];
		
		strcpy (max, s);

		for (int count = 0; count < len; ++count)
		{
			 make_cycle (s, 1);

			if(strcmp (max, s) <= 0)
				strcpy (max, s);
		}

		strcpy (s, max);
	}
}

int divide_length (char *s) 
{
	int len = strlen (s);

	for (int d = len - 1; d > 1; --d)
		if (len % d == 0) return d;
	return 1;
}

void strdel (char *s, int n) 
{
	int len = strlen (s) - n + 1;

	for(int index = 0; index < len; ++index)
		s[index] = s[n+index];
}

char *revstring (char *s) 
{
	int i = strlen (s) - 1;
	int k = 0;

	char c;
	while (i > k)
	{
		c = s[i];
		s[i] = s[k];
		s[k] = c;

		i--;
		k++;
	} 

	return s;
}

void enc_word (char *s) 
{
	char copy[MAX_WIDTH] = "";

	int len = strlen (s);
	int   d = divide_length(s);
	
	strcpy (copy, s);
	strdel (copy, d);

	for (int index = 0 ; index < len; ++index) 
		if (isdigit (s[index]) != 0)
		{
			revstring (copy);
			break;
		}
	
	strncat (copy, s, d);
	strcpy (s, copy);
}

double complexity (const char *s) 
{
	int len = strlen (s);
	if (len == 0) return 0;

	int sum = 0;
	for (int index = 0; index < len; ++index) sum += s[index];

	return sum / (double) len;
}

void sorting (element *elements, int n) 
{
	element tmp; 
	bool bubble = true;

	while (bubble)
	{
		bubble = false;
		for (int index = 0; index < n - 1; ++index)
		{
			if (elements[index].cpx < elements[index + 1].cpx)
			{
				tmp = elements[index];
				elements[index] = elements[index + 1];
				elements[index + 1] = tmp;
				bubble = true;
			}
			else
				if ( elements[index].cpx == elements[index + 1].cpx && 
					 strcmp (elements[index].str, elements[index + 1].str) > 0
				   )
				{
					tmp = elements[index];
					elements[index] = elements[index + 1];
					elements[index + 1] = tmp;
					bubble = true;
				} 
		}
	}
}

element *partition (char *msg, int n)
{
	element *elements;
	elements = (element *)calloc (n, sizeof (element));

	int len = strlen(msg);
	int ltok = len / n;
	int rtok = len % n;
	int k = 0;
	while (strlen(msg) != 0)
	{
		if ((k + 2) * ltok + rtok > len) ltok += rtok; 
		
		elements[k].str = (char *)calloc (ltok + 1, sizeof (char));
		strncpy (elements[k].str, msg, ltok);
		elements[k].str[ltok] = '\0';

		elements[k].cpx = complexity (elements[k].str);
		strdel (msg, ltok);
		k++;
	}

	return elements;
}

char *restructuring (element *elements, int n) 
{
	int len = 0;
	for (int index = 0; index < n; ++index)
		len += strlen (elements[index].str);
	
	char *s = (char *) calloc (len + 1, sizeof (char));
	int i = n - 1;
	int j = 0;

	while (i > j)
	{
		strcat (s, elements[j].str);
		strcat (s, elements[i].str);		
		i--;
		j++;
	}
	if (i == j)
		strcat (s, elements[i].str);
	
	return s;
}
