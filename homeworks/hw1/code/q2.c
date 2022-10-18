// ename built on baadal vm using file Build_ename.sh given in tlpi code distribution in lib directory
// some error handling code borrowed from tlpi error_functions.h
static char *ename[] = {
    /*   0 */ "",
    /*   1 */ "EPERM", "ENOENT", "ESRCH", "EINTR", "EIO", "ENXIO",
    /*   7 */ "E2BIG", "ENOEXEC", "EBADF", "ECHILD",
    /*  11 */ "EAGAIN/EWOULDBLOCK", "ENOMEM", "EACCES", "EFAULT",
    /*  15 */ "ENOTBLK", "EBUSY", "EEXIST", "EXDEV", "ENODEV",
    /*  20 */ "ENOTDIR", "EISDIR", "EINVAL", "ENFILE", "EMFILE",
    /*  25 */ "ENOTTY", "ETXTBSY", "EFBIG", "ENOSPC", "ESPIPE",
    /*  30 */ "EROFS", "EMLINK", "EPIPE", "EDOM", "ERANGE",
    /*  35 */ "EDEADLK/EDEADLOCK", "ENAMETOOLONG", "ENOLCK", "ENOSYS",
    /*  39 */ "ENOTEMPTY", "ELOOP", "", "ENOMSG", "EIDRM", "ECHRNG",
    /*  45 */ "EL2NSYNC", "EL3HLT", "EL3RST", "ELNRNG", "EUNATCH",
    /*  50 */ "ENOCSI", "EL2HLT", "EBADE", "EBADR", "EXFULL", "ENOANO",
    /*  56 */ "EBADRQC", "EBADSLT", "", "EBFONT", "ENOSTR", "ENODATA",
    /*  62 */ "ETIME", "ENOSR", "ENONET", "ENOPKG", "EREMOTE",
    /*  67 */ "ENOLINK", "EADV", "ESRMNT", "ECOMM", "EPROTO",
    /*  72 */ "EMULTIHOP", "EDOTDOT", "EBADMSG", "EOVERFLOW",
    /*  76 */ "ENOTUNIQ", "EBADFD", "EREMCHG", "ELIBACC", "ELIBBAD",
    /*  81 */ "ELIBSCN", "ELIBMAX", "ELIBEXEC", "EILSEQ", "ERESTART",
    /*  86 */ "ESTRPIPE", "EUSERS", "ENOTSOCK", "EDESTADDRREQ",
    /*  90 */ "EMSGSIZE", "EPROTOTYPE", "ENOPROTOOPT",
    /*  93 */ "EPROTONOSUPPORT", "ESOCKTNOSUPPORT",
    /*  95 */ "EOPNOTSUPP/ENOTSUP", "EPFNOSUPPORT", "EAFNOSUPPORT",
    /*  98 */ "EADDRINUSE", "EADDRNOTAVAIL", "ENETDOWN", "ENETUNREACH",
    /* 102 */ "ENETRESET", "ECONNABORTED", "ECONNRESET", "ENOBUFS",
    /* 106 */ "EISCONN", "ENOTCONN", "ESHUTDOWN", "ETOOMANYREFS",
    /* 110 */ "ETIMEDOUT", "ECONNREFUSED", "EHOSTDOWN", "EHOSTUNREACH",
    /* 114 */ "EALREADY", "EINPROGRESS", "ESTALE", "EUCLEAN",
    /* 118 */ "ENOTNAM", "ENAVAIL", "EISNAM", "EREMOTEIO", "EDQUOT",
    /* 123 */ "ENOMEDIUM", "EMEDIUMTYPE", "ECANCELED", "ENOKEY",
    /* 127 */ "EKEYEXPIRED", "EKEYREVOKED", "EKEYREJECTED",
    /* 130 */ "EOWNERDEAD", "ENOTRECOVERABLE", "ERFKILL", "EHWPOISON"
};

#define MAX_ENAME 133

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// noreturn attribute only for gcc
__attribute__ ((__noreturn__)) void errExit(const char *userMsg)
{
    printf("ERROR [%s %s]: %s\n", errno>0 && errno<=MAX_ENAME ? ename[errno] : "?UNKNOWN?", strerror(errno), userMsg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int fd1 = open("hw1f1", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd1==-1)
    {
        errExit("opening fd1 for file hw1f1");
    }
    int fd2 = dup(fd1);
    if (fd2==-1)
    {
        errExit("duplicating fd1");
    }
    int fd3 = open("hw1f1", O_RDWR);
    if (fd3==-1)
    {
        errExit("opening fd3 for file hw1f1");
    }
    int fd4 = open("hw1f1", O_WRONLY | O_APPEND);
    if (fd4==-1)
    {
        errExit("opening fd4 for file hw1f1");
    }
    if (write(fd1, "Hello,", 6) != 6)
    {
        errExit("couldn't write \"Hello\", completely to file hw1f1");
    }
    if (write(fd2, " world", 6) != 6)
    {
        errExit("couldn't write \" world\", completely to file hw1f1");
    }
    if (lseek(fd2, 0, SEEK_SET) == -1)
    {
        errExit("couldn't seek fd2 to beginning of file hw1f1");
    }
    if (write(fd4, " Hej da!", 8) != 8)
    {
        errExit("couldn't write \" Hej da!\", completely to file hw1f1");
    }
    if (write(fd1, "HELLO,", 6) != 6)
    {
        errExit("couldn't write \"HELLO\", completely to file hw1f1");
    }
    if (write(fd3, "Gidday", 6) != 6)
    {
        errExit("couldn't write \"Gidday\", completely to file hw1f1");
    }
    if (lseek(fd4, 0, SEEK_SET) == -1)
    {
        errExit("couldn't seek fd4 to beginning of file hw1f1");
    }
    if (write(fd4, " Var ar vi?", 11) != 11)
    {
        errExit("couldn't write \" Var ar vi?\", completely to file hw1f1");
    }
}
