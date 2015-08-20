
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_BYTES	32


struct storage_entry_s {
	char	name[MAX_NAME_BYTES];
	int		size;

	/* Use a linked list of "entry" structs to store the values. */
	struct storage_entry_s * prev;
	struct storage_entry_s * next;
};

struct file_storage_s {

	/* Use a linked list of "entry" structs to store the values. */
	struct storage_entry_s * first;
	struct storage_entry_s * last;

	/* Header values */
	int		file_pos; /* The offset from after this value that the file contents are stored in the binary */
	int		header_vals[2]; /* Two unknown values listed in the header for the file storage */
	char	header_filename[MAX_NAME_BYTES]; /* The name of the file */
};

/* Free the storage entry, link the previous and next entries together */
static void free_storage_entry(struct storage_entry_s * se)
{
	if (se) {
		struct storage_entry_s * cache_se_prev = se->prev;
		struct storage_entry_s * cache_se_next = se->next;

		/* Free the memory that `se` occupies */		
		free(se);

		/* Do any linking that needs to be done so the list remains continuous */
		if (cache_se_prev && cache_se_next) {
			/* Link the neighbour entries together */
			cache_se_prev->next = cache_se_next;
			cache_se_next->prev = cache_se_prev;
		}
		else {
			/* One or both were NULL */
			if (cache_se_prev) {
				cache_se_prev->next = 0;
			}
			if (cache_se_next) {
				cache_se_next->prev = 0;
			}
		}
	}
}

/* Initialise a recursive memory free() on the storage structure */
static void free_file_storage(struct file_storage_s * fs)
{
	if (!fs) return;

	/* if first and last are the same and are not NULL */
	if (fs->first == fs->last && fs->first) {
		free(fs->first);
		fs->first = 0;
		fs->last = 0;
	}
	/* if they are not the same and first is not NULL */
	else if (fs->first) {
		struct storage_entry_s * se = fs->first;

		while (se)
		{
			struct storage_entry_s * se_next = se->next;
			free_storage_entry(se);
			se = se_next;
		}

		fs->first = 0;
		fs->last = 0;
	}
}


static void print_help()
{
	fputs("Inner Worlds file extractor\n", stdout);
	fputs("By \"Jamie Breeze\" <jamie.c.breeze@gmail.com> on August 2015\n", stdout);
	fputs("\tUSAGE: iw-extract [options] <binary_name>\n\n", stdout);

	fputs("-h | --help\tThis help printout\n", stdout);
	fputs("-o\tOffset of \'file\'\n", stdout);
	
	fflush(stdout);
}


int main(int argc, char **argv)
{
	struct file_storage_s file;
	int file_offset = 37888;		// inner.exe.lst entry offset
	int file_content_offset = 38400;// inner.exe.lst contents offset
	FILE *fp = 0;

	/* Initialise the structure to empty */
	memset(&file, 0, sizeof(struct file_storage_s));

	/* Process the command line parameters */
	if (argc <= 1) {
		/* Print out some help information */
		print_help();
		return 0;
	}
	else {
		/* Process the relevant parameters */
		for (int i=1; i<argc; i++)
		{
			if (!strcmp("--help", argv[i]) || !strcmp("-h", argv[i])) {
				print_help();
				/* Ignore any other options and do not process any binaries */
				return 0;
			}
			else if (!strcmp("-o", argv[i])) {
				file_offset = 37888;
				/* TODO: Grab the argv[i+1] as the actual result. */
			}
		}

		/* Open the binary file, if that fails print an error */
		fp = fopen(argv[argc-1], "rb");
		if (!fp) {
			fprintf(stderr, "ERROR: Failed to open the file \"\"\n", argv[argc-1]);
		}
	}

	/* If we opened the file, do some work in it */
	if (fp) {
		/* TODO: do relevant work inside the .EXE */
		char test[24];

		fseek(fp, file_offset, SEEK_SET);
		fread(test, 24, 1, fp);
		fprintf(stdout, "test =\"%s\"\n", test);
		
		/* Cleanup everything we did */
		free_file_storage(&file);
		fclose(fp);
	}

	return 0;
}
