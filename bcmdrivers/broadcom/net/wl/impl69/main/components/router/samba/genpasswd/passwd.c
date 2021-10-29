#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <time.h>
#include <unistd.h>

/*
 * If we don't include this prototype, then gcc assumes
 * crypt returns an "int" which means our 64-bit char *
 * pointer can get truncated to 32-bits, causing a
 * segfault.
 */
char *crypt(const char *key, const char *salt);

int
i64c(int i)
{
	if (i <= 0)
		return ('.');
	if (i == 1)
		return ('/');
	if (i >= 2 && i < 12)
		return ('0' - 2 + i);
	if (i >= 12 && i < 38)
		return ('A' - 12 + i);
	if (i >= 38 && i < 63)
		return ('a' - 38 + i);
	return ('z');
}

char *
crypt_make_salt(void)
{
	time_t now;
	static unsigned long x;
	static char result[3];

	time(&now);
	x += now + getpid() + clock();
	result[0] = i64c(((x >> 18) ^ (x >> 6)) & 077);
	result[1] = i64c(((x >> 12) ^ x) & 077);
	result[2] = '\0';
	return result;
}

/* function to support encryption password for login */
char *
pw_encrypt(const char *clear, const char *salt)
{
	static char cipher[128];
	char *cp;

	cp = (char *) crypt(clear, salt);
	/* if crypt (a nonstandard crypt) returns a string too large,
	 * truncate it so we don't overrun buffers and hope there is
	 * enough security in what's left */

	if (strlen(cp) > sizeof(cipher)-1) {
		cp[sizeof(cipher)-1] = 0;
	}

	strcpy(cipher, cp);
	return cipher;
}

int
create_login_cfg()
{
	struct passwd pw;
	FILE *fsPwd = NULL, *fsGrp = NULL;

	fsPwd = fopen("passwd", "w+");

	if ( fsPwd != NULL ) {
		pw.pw_name = "admin";
		pw.pw_passwd = pw_encrypt("1234", crypt_make_salt());
		pw.pw_uid = 0;
		pw.pw_gid = 0;
		pw.pw_gecos = "Administrator";
		pw.pw_dir = "/";
		pw.pw_shell = "/bin/sh";
		putpwent(&pw, fsPwd);

		pw.pw_name = "support";
		pw.pw_passwd = pw_encrypt("1234", crypt_make_salt());
		pw.pw_uid = 0;
		pw.pw_gid = 0;
		pw.pw_gecos = "Technical Support";
		pw.pw_dir = "/";
		pw.pw_shell = "/bin/sh";
		putpwent(&pw, fsPwd);

		pw.pw_name = "user";
		pw.pw_passwd = pw_encrypt("1234", crypt_make_salt());
		pw.pw_uid = 0;
		pw.pw_gid = 0;
		pw.pw_gecos = "Normal User";
		pw.pw_dir = "/";
		pw.pw_shell = "/bin/sh";
		putpwent(&pw, fsPwd);

		pw.pw_name = "nobody";
		pw.pw_passwd = pw_encrypt("1234", crypt_make_salt());
		pw.pw_uid = 0;
		pw.pw_gid = 0;
		pw.pw_gecos = "nobody for ftp";
		pw.pw_dir = "/";
		pw.pw_shell = "/bin/sh";
		putpwent(&pw, fsPwd);
		fclose(fsPwd);

 		fsGrp = fopen("group", "w+");
		if ( fsGrp != NULL ) {
			fprintf(fsGrp, "root::0:root,admin,support,user\n");
			fclose(fsGrp);
			return 0; /* Success */
		}
	}

	return -1; /* Fail */
}

int main(int argc,const char *argv[])
{
	create_login_cfg();
}
