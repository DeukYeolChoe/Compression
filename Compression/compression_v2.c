#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define ARRAY_SIZE 100

int main(int arc, char *argv[])
{
	FILE *fileReader;
	FILE *fileWriter;
	int ch;
	char fileName[ARRAY_SIZE];
	if (arc != 4)
	{
		printf("Please, follow this: ./compression [c or d] original new\n");
		printf("c-compression, d-depression\n");
		return 0;
	}	
	
	fileReader = fopen(argv[2], "rb");
	if (fileReader == NULL)
	{
		printf("Could not open the file, errno = %d\n", errno);
		return 0;
	}
	
	if (strcmp(argv[1],"c") == 0)
	{
		strcpy(fileName,argv[3]);
		strcat(fileName,"_comp");
		fileWriter = fopen(fileName, "wb");
		int dupCount = 0;
		int dupCount2 = 0;
		while ((ch = fgetc(fileReader)) != EOF)
		{
			if (ch == 0)
			{
				dupCount++;
			}
			else
			{
				if (dupCount > 0)
				{
					putc(0, fileWriter);
					putc(dupCount, fileWriter);
					dupCount = 0;
				}
			}
			
			if (ch == 255)
			{
				dupCount2++;
			}
			else
			{
				if (dupCount2 > 0)
				{
					putc(255, fileWriter);
					putc(dupCount2, fileWriter);
					dupCount2 = 0;
				}
			}
			if (ch != 0 && ch != 255)
				putc(ch, fileWriter);
		}
		// for last one
		if (dupCount > 0)
		{
			putc(0, fileWriter);
		}
		if (dupCount2 > 0)
		{
			putc(255, fileWriter);	
		}
	}
	else if(strcmp(argv[1],"d") == 0)
	{
		bool on = false;
		int i;
		strcpy(fileName, argv[3]);
		strcat(fileName, "_decomp");
		fileWriter = fopen(fileName, "wb");
		int temp = -1;
		while((ch = fgetc(fileReader)) != EOF)
		{
			if ((ch == 0 && !on) || (ch == 255 && !on))
			{
				temp = ch;
				on = true;		
			}
			else if (on)
			{
				for (i = 0; i < ch; i++)
				{
					putc(temp, fileWriter);
				}	
				on = false;
			}
			else
			{
				putc(ch, fileWriter);
			}
		}
		if (on)
		{
			putc(temp, fileWriter);
		}
	}
	else
	{
		printf("Invalid an option!");
		exit(1);
	}

	// change permission
	char cmd[ARRAY_SIZE];
	strcpy(cmd, "chmod 755 ");
	strcat(cmd, fileName);
	system(cmd);

	// close file descriptors
	close(fileReader);
	close(fileWriter);

	return 0;
}

