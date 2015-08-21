Beginning of packed resources: 37,888 (dec)

It appears that 0x20 (hex) is a padding value and that the ASCI entries of number literals are QWORD aligned (64-bits)

String literals are escaped with 0x0A (hex) / 10 (dec) control character as opposed to the C style of using a NULL 0x00 (hex) to end a string.

The first QWORD is an 8-byte ASCI representation of a number that indicates where the "inner.exe.lst" contents begin inside the extended region of the DOS binary file.

In our case (version 1.2) this is a set of four bytes equal to 0x20 and three bytes that equal ASCI "504" and a final 0x20 padding byte before the next QWORD.

The beginning of the "inner.exe.lst" (located at 38,400 (dec)) starts with a string literal entry of "inner" with a single 0x20 padding value before an ASCI number is listed and padded by 0x20 again with a new text entry listed directly after, another "inner" entry with two 0x20 padding bytes before a new ASCI number is listed, after this is a single 0x20 padding and the first meaningful file name is listed "1.dump"

This leads me to believe the structure of the "inner.exe.lst" is fairly simple and straight forward:
`<some file name> <some number>`
Where the  filename is the name of the file stored inside a later region in the extended area, and the number is likely the size in bytes of the binary blob this file is.

## EDIT: 21st 8 2015
It seems that the first string starting at 38,400 is "inner" and is a stand alone string and not an actual "lst" entry.  The list entries seem to actually begin directly after, and are of the format:
`<size in bytes> <file name>\n`
Where the 'line' has significant amounts of padding, seemingly for alignment either in memory or in the text file editor IW's developers used.

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
 - [audio] - stored internally as ".bin" but may be .wav or some other common audio format; appear to be prefixed with s_*
 - [font] - unknown, also masked as ".bin" but could be .fon, .ttf, or .otf (probably .fon or some other bitmap-based font however)
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