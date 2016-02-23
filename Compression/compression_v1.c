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
	int count = 0;
	int total = 0;
	while ((ch = fgetc(fileReader)) != EOF) total++;	

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
		
		fileReader = fopen(argv[2], "rb");
		while ((ch = fgetc(fileReader)) != EOF)
		{
			count++;
			if (ch == 0)
			{
				if (dupCount == 255)
				{
					putc(0, fileWriter);
					putc(dupCount, fileWriter);
					dupCount = 0;
				}
				dupCount++;
			}	
			else
			{
				if (dupCount > 0)
				{
					// 03 = 0 0 0
					// 2553 = 255 255 255
					putc(0, fileWriter);
					putc(dupCount, fileWriter);
					dupCount = 0;
				}
				putc(ch, fileWriter);
			
			}
			if ((total - 8) < count)
				break;			
		}
		// for last one
		if (dupCount > 0)
		{
			putc(0, fileWriter);
		}
	}
	else if(strcmp(argv[1],"d") == 0)
	{
		bool on = false;
		int i;
		strcpy(fileName, argv[3]);
		strcat(fileName, "_decomp");
		fileWriter = fopen(fileName, "wb");
		fileReader = fopen(argv[2], "rb");
		while((ch = fgetc(fileReader)) != EOF)
		{
			if (ch == 0)
			{
				on = true;		
			}
			else if (on)
			{
				for (i = 0; i < ch; i++)
				{
					putc(0, fileWriter);
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
			putc(0, fileWriter);
		}
	
		putc(0, fileWriter);
		putc(95, fileWriter);
		putc(105, fileWriter);
		putc(110, fileWriter);
		putc(105, fileWriter);
		putc(116, fileWriter);
		putc(0, fileWriter);
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

