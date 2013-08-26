//
//  cbz.h
//  cbz
//
//  Created by Trevor Downs on 2013-08-24.
//
//

#ifndef cbz_cbz_h
#define cbz_cbz_h

#endif


#ifdef DEBUG
#define THEBUG(string, args...) printf(string, args)
#else
#define THEBUG(string, args...) //empty macro for no action
#endif

#define ZIP				"zip"
#define ZIP_FLAGS		"-mrT9"
#define ZIP_EXCLUDE		"-x \"*.DS_Store\" \"*[Tt]humbs.db\" \"Icon?\""
#define FILE_EXTENSION	".cbz"

int main(int argc, const char * argv[]);