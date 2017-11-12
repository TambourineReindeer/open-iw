Beginning of packed resources: 37,888 (dec)

The packed files header is a simple string padded by spaces (0x20(hex)) where the first value of '504' is the number of files packed into the extender, the second number '1007' is currently unknown while the third number '10137' is the length of the file list in bytes.

String literals are escaped with 0x0A(hex) which is the line feed code or '\n' control character.

It seems that the first string starting at 38,400(dec) is "inner" and is a stand alone string and not an actual "lst" entry.  The list entries seem to actually begin directly after, and are of the format:
`<size in bytes> <file name>\n`
Where the 'line' has significant amounts of padding, seemingly for alignment either in memory or in the text file editor IW's developers used.

It appears the header, file list and each file are aligned somehow in the extender file, it is unclear what alignment is being used, as the final padding looks almost arbitrary.  This leaves significant amounts of trailing 0x00 padding between sections with no clear way to determine how much of it there really is, which inhibits our ability to extract the DOS version of the game files.

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

- ITF Perhaps an internal text format, with formatting or control indicators such as '\f1' and '\c'. Probably not related to [SAPScript Interchange Text Format]
- MMP [Truevision Bravado Board Video Output format] (animation format) - There is very little documentation of the format but IW may use this as a video format for whatever reason (this is the only reasonably fitting format found on the internet so far)
- REC [?] - Appears to be an audio format associated with the MMP files as it includes the same file name as the .mmp; however this could also be the video format and the MMP is the audio
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
