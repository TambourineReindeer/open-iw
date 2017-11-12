Beginning of packed resources: 37,888 (dec)

It appears that 0x20 (hex) is a padding value and that the ASCI entries of number literals are QWORD aligned (64-bits)

String literals are escaped with 0x0A (hex) / 10 (dec) control character as opposed to the C style of using a NULL 0x00 (hex) to end a string.

The first QWORD is an 8-byte ASCI representation of a number that indicates where the "inner.exe.lst" contents begin inside the extended region of the DOS binary file.

In our case (version 1.2) this is a set of four bytes equal to 0x20 and three bytes that equal ASCI "504" and a final 0x20 padding byte before the next QWORD.

It seems that the first string starting at 38,400(dec) is "inner" and is a stand alone string and not an actual "lst" entry.  The list entries seem to actually begin directly after, and are of the format:
`<size in bytes> <file name>\n`
Where the 'line' has significant amounts of padding, seemingly for alignment either in memory or in the text file editor IW's developers used.

---

0x2E (ASCII 46 for periods) is used in file path strings for '..' (next directory up). However it seems also to be used for something else, like a separator between menu strings where it has three instances instead of two (see offset 0x76b38).

Another interesting note is what appears to be a variable called "g_ptrs". This may be an array or some object that holds pointers to various other game objects. It is shown in inner.exe.lst as "g_ptrs.bin" and after that at hex 0x76ea4, directly after a list of map files. Afterwards, g_ptrs is separated by blank spaces and several 4-digit hex values (the pattern is broken by a six-digit value further down) separated by several spaces as well. These may be offsets (don't appear to be sizes as they don't match with inner.exe.lst) from the starting position of g_ptrs to somewhere else in the file where these map files are stored.

---

## Data formats
Known data formats so far:

 - LBM [Interleaved Bitmap] (image file) - Internally these are stored as ".bin"
 - MOD [ProTracker module] (music file)
 - LST - List of all files included in the executable
 - CFG - Text document the executable reads and writes from for game configuration
 - SAV - Inner Worlds save file format; this and the configuration file are the only files that the executable writes out to

File format guesses:

- ITF [SAPScript Interchange Text Format] (text file)
- MMP [Truevision Bravado Board Video Output format] (animation format) - There is very little documentation of the format but IW may use this as a video format for whatever reason (this is the only reasonably fitting format found on the internet so far)
- REC [?] - Appears to be an audio format associated with the MMP files it includes the same file name as the .mmp; however this could also be the video format and the MMP is the audio
- MAP [internal map/level format]
- EOL - "End of level" - its specific purpose is currently unknown

Unknown formats:

 - RM5 - There is only one of these, "castle.rm5"
 - DUMP - A data dump of some sort but as of yet unknown as to its purpose
 - [audio] - stored internally as ".bin" and are raw waveforms stored as 8-bit signed PCM Uncompressed Mono; appear to be prefixed with s_*
 - [font] - unknown, but probably an internal font format, presumably stored in "fonts.bin" (defines the actual font data?) and "g_fonts2.bin" (the bitmap data for the font)
 - others? - To be seen after browsing file offsets from the .lst file

---

### Known file directories within the exectuable

 - ../game/
 - ../game/inner/
 - ../game/inner/e1
 - ../game/inner/e2
 - ../game/inner/e3
 - ../list/
 - root directory? (/sleepy/? see below)
 - directory commands are called from (hence '..')

#### Other miscellaneous directories (probably not useful)

 - /dev/mem (Linux physical memory file)
 - /home/baford/include/sleepy/list (this is one of the developers' Linux home directories, unknown as to why it was included in the file)

---

### Notes on original development
The game itself was written in C++ with libc5 and compiled with G++ 2.7.2 and BinUtils 2.6 into DOS and Linux executables. The developers used a custom-built DOS extender called MOSS (available at the University of Utah website) which uses a form of ELF executables and allows the main executable and any other resource files to be attached to it.
