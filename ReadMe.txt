Lendal the automated Length Value pair finder in arbitrary data

Released as open source by NCC Group Plc - http://www.nccgroup.com/

Developed by Ollie Whitehouse, ollie dot whitehouse at nccgroup dot com

Originally developed in 2006

http://www.github.com/nccgroup/lendal

Released under AGPL see LICENSE for more information


This will walk a file trying to find lengths and corsponding NULL terminated values

Examples:
	Command Line:
		Len.exe -D -i foo.foo 
		
	Result:
		Will do a DWORD search in foo.foo
		
				
	Command Line:
		Len.exe -B -i foo.foo -s 
		
	Result:
		Will do a byte search in foo.foo swapping the endian
		
		
		
	Command Line:
		Len.exe -A -i foo.foo 
		
	Result:
		Will do a BYTE, WORD and DWORD search in foo.foo
		
		
		
	Command Line:
		Len.exe -A -i foo.foo -s 
		
	Result:
		Will do a BYTE, WORD and DWORD search in foo.foo doing both little and big endian
		
		
		
	Command Line:
		Len.exe -A -i foo.foo -s -n
		
	Result:
		Will do a BYTE, WORD and DWORD search in foo.foo doing both little and big endian and not
		require a NULL terminator. This will potentially generate more false positives
		
		
		
	Command Line:
		Len.exe -A -i foo.foo -s -p 10
		
	Result:
		Will do a BYTE, WORD and DWORD search in foo.foo doing both little and big endian and not
		require a NULL terminator. Will also allow 10 non printale chars in the string. This will 
		potentially generate more false positives.
	
	Command Line:
		Len.exe -A -i foo.foo -m 10
		
	Result:
		Will do a BYTE, WORD and DWORD search in foo.foo needing a minimum of 10 chars in the output
		
	