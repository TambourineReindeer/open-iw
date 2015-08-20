Beginning of packed resources: 37,888 (dec)

It appears that 0x20 (hex) is a padding value and that the ASCI entries of number literals are QWORD aligned (64-bits)

String literals are escaped with 0x0A (hex) / 10 (dec) control character as opposed to the C style of using a NULL 0x00 (hex) to end a string.

The first QWORD is an 8-byte ASCI representation of a number that indicates where the "inner.exe.lst" contents begin inside the extended region of the DOS binary file.

In our case (version 1.2) this is a set of four bytes equal to 0x20 and three bytes that equal ASCI "504" and a final 0x20 padding byte before the next QWORD.

The beginning of the "inner.exe.lst" (located at 38,400 (dec)) starts with a string literal entry of "inner" with a single 0x20 padding value before an ASCI number is listed and padded by 0x20 again with a new text entry listed directly after, another "inner" entry with two 0x20 padding bytes before a new ASCI number is listed, after this is a single 0x20 padding and the first meaningful file name is listed "1.dump"

This leads me to believe the structure of the "inner.exe.lst" is fairly simple and straight forward:

<some file name> <some number>

Where the  filename is the name of the file stored inside a later region in the extended area, and the number is likely the size in bytes of the binary blob this file is.

