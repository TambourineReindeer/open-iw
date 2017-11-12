
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <cstdint>
#include <cstdio>
#include <string.h>

// Oh noes, I'm a bad programmer! >:c
using namespace std;


class StorageEntry
{
public:

	string		name;
	size_t		size;

	StorageEntry( string name, size_t size ) : name(name), size(size)
	{}

};

struct FileStorage
{
public:

	list<StorageEntry>	file_list;

	// Header values
	size_t	file_count;			// The number of files in the list
	size_t	unknown;			// Unknown value listed in the header for the file storage
	size_t	list_length;		// Length of the list of files, in bytes
	string	header_filename;	// The name of the file

	FileStorage()
	{
		file_count = 0;
		unknown = 0;
		list_length = 0;
		header_filename = "";
	}

	~FileStorage()
	{
		file_list.clear();
	}
};


void print_help()
{
	cout << "Inner Worlds file extractor\n";
	cout << "By \"Rexhunter99\" <rexhunter99@gmail.com> on August 2015\n\n";

	cout << "USAGE: iw-extract [options] <binary_name>\n";
	cout << "--help\n-h\n";
	cout << "\tPrints the help\n";
	cout << "--offset\n-o\n";
	cout << "\tOffset of packed data\n";

	cout << endl;
}


int main(int argc, char *argv[])
{
	FileStorage file;
	fstream f;
	size_t file_header_offset = 37888;	// inner.exe.lst header offset
	size_t file_list_offset = 38400;	// inner.exe.lst list offset
	size_t file_content_offset = 0;		// inner.exe.lst contents offset

	// Process the command line parameters
	if (argc <= 1) {
		// Print out some help information
		print_help();
		return 0;
	}
	else
	{
		// Process the relevant parameters
		for (int i=1; i<argc; i++)
		{
			if (!strcmp("--help", argv[i]) || !strcmp("-h", argv[i])) {
				print_help();
				// Ignore any other options and do not process any binaries
				return 0;
			}
			else if (!strcmp("--offset", argv[i]) || !strcmp("-o", argv[i])) {
				sscanf(argv[i+1], "%d", &file_list_offset);
				i++;
			}
		}

		// Open the binary file, if that fails print an error
		f.open(argv[argc-1], ios::in | ios::binary);

		if ( !f.is_open() )
		{
			cerr << "ERROR: Failed to open the file \"" << argv[argc-1] << "\"" << endl;
		}
	}

	// If we opened the file, do some work in it
	if ( f.is_open() )
	{
		// TODO: do relevant work inside the .EXE
		fstream out;

		// Jump the the offset we were told to
		f.seekg( file_header_offset );

		// Read in the header
		f >> file.file_count;
		f >> file.unknown;
		f >> file.list_length;

		f >> file.header_filename;

		cout << "File Count: \"" << file.file_count << "\"" << endl;
		cout << "File Unknown: \"" << file.unknown << "\"" << endl;
		cout << "File List Length: \"" << file.list_length << "\"" << endl;
		cout << "File Name: \"" << file.header_filename << "\"" << endl;

		// Read in the entries
		f.seekg( file_list_offset );

		out.open(file.header_filename, ios::out);

		if (out.is_open())
		{
			size_t entry_size = 0;
			string entry_name;

			// "inner"
			f >> entry_name;
			out << entry_name << endl;

			for (size_t i=0; i<file.file_count-1; i++)
			{
				f >> entry_size;
				f >> entry_name;

				out << entry_size << " " << entry_name << endl;
			}

			out.close();
		}

		f.close();
	}

	return 0;
}
