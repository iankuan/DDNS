#include "utility/file_ops.h"

bool fexist( char* fname)
{
	static int exist;
	exist = access( fname, F_OK);
	if( exist != -1){
		dlog("file %s exist\n", fname);
		return true;
	}
	else{
		dlog("file %s not exist\n", fname);
		return false;
	}
}

bool isdir( char* dname)
{
	struct stat st;
	if( stat( dname, &st) == 0 && S_ISDIR( st.st_mode)){
		dlog("%s is a dir\n", dname);
		return true;
	}
	else{
		dlog("%s isn't a dir\n", dname);
		return false;
	}
}

bool mkd( char* dname, mode_t mode )
{
	if( fexist( dname)){///<TODO: we should know the relative or absolute path
		dlog("dir %s already exist\n", dname);
		return false;
	}
	else{
		dlog("dir %s not exist\n", dname);
		mkdir( dname, mode);
		return true;
	}
}

void apen_dir_file( char* dir, char** file, int n, char** out_files)
{
	assert( fexist( dir) && "dir not exist");

	for ( int i = 0; i < n; i++){
		strcpy( out_files[i], dir);
		if( out_files[i][ strlen( out_files[i]) - 1] != '/' ) strcat( out_files[i], "/");
		//assert( fexist( file[i]) && "file not exist");
		strcat( out_files[i], basename( file[i]));
	}
}

void apen_suffix( char** in, int n, char* suffix)
{
	for ( int i = 0; i < n; i++)
		strcat( in[i], suffix);
}

char* ret_cwd( void)
{
	char* cwd = ( char*) malloc( sizeof( char) * PATH_LIMIT);

	syserr( !getcwd( cwd, PATH_LIMIT), "getcwd");
	return cwd;
}

char* ret_realpath( char* path)
{
	char* real = (char*) malloc( sizeof( char) * PATH_LIMIT);

	realpath( path, real);
	dlog("realpath( %s)  = %s\n", path, real);

	return real;
}
