#define FUSE_USE_VERSION 28

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdlib.h>

#define TIME_SIZE 30
#define LOG_SIZE 1000

#define PREF_ATOZ 1
#define PREF_RX 2
#define PREF_AISA 3

#define ENC_ATBASH 1
#define ENC_ROT13 2
#define ENC_VIG 3

const int INF = 1000;
static const char *dirpath = "/home/david/Downloads";
static const char *logpath = "/home/david/SinSeiFS.log";

static const char* info = "INFO";
static const char* warn = "WARNING";
 
int isEncoded(const char* path) {
	if (strstr(path, "AtoZ_") != NULL) return PREF_ATOZ;
	if (strstr(path, "RX_") != NULL) return PREF_RX;
	if (strstr(path, "A_is_a_") != NULL) return PREF_AISA;
	
	return 0;
}
 
void encode(char* s, int method) {
	int len = strlen(s);
	int i;
	
	if (method == ENC_ATBASH) {
		for (i = 0; i < len; i++) {
			if (s[i] >= 'A' && s[i] <= 'Z')
				s[i] = 'Z' - s[i] + 'A';
     			else if (s[i] >= 'a' && s[i] <= 'z')
     				s[i] = 'z' - s[i] + 'a';
		}
	} else if (method == ENC_ROT13) {
		for (i = 0; i < len; i++) {
			if (s[i] >= 'A'&& s[i] <= 'Z')
				s[i] = ((s[i]-'A'+13)%26)+'A';
    			else if (s[i] >= 'a'&& s[i] <= 'z')
    				s[i] = ((s[i]-'a'+13)%26)+'a';
		}
	} else if (method == ENC_VIG) {
		char* key = "SISOP";
		
		for (i = 0; i < len; i++) {
			if (s[i] >= 'A' && s[i] <= 'Z')
				s[i] = ((s[i] - 'A' + (key[i % ((sizeof(key) - 1))] - 'A')) % 26) + 'A';
			else if (s[i] >= 'a' && s[i] <= 'z')
				s[i] = ((s[i] - 'a' + (key[i % ((sizeof(key) - 1))] - 'A')) % 26) + 'a';
		}
	}
}

void decode(char* s, int method) {
	int len = strlen(s);
	int i;
	
	if (method == ENC_ATBASH) {
		for (i = 0; i < len; i++) {
			if (s[i] >= 'A' && s[i] <= 'Z')
				s[i] = 'A' - s[i] + 'Z';
			else if (s[i] >= 'a' && s[i] <= 'z')
				s[i] = 'a' - s[i] + 'z';
		}
	} else if (method == ENC_ROT13) {
		for (i = 0; i < len; i++) {
			if (s[i] >= 'A' && s[i] <= 'Z')
				s[i] = ((s[i] - 'A' - 13) % 26) + 'A';
			else if (s[i] >= 'a' && s[i] <= 'z')
				s[i] = ((s[i] - 'a' - 13) % 26) + 'a';
		}
	} else if (method == ENC_VIG) {
		char* key = "SISOP";
		
		for (i = 0; i < len; i++) {
			if (s[i] >= 'A' && s[i] <= 'Z')
				s[i] = ((s[i] - 'A' - (key[i % ((sizeof(key) - 1))] - 'A') + 26) % 26) + 'A';
			else if (s[i] >= 'a' && s[i] <= 'z')
				s[i] = ((s[i] - 'a' - (key[i % ((sizeof(key) - 1))] - 'A') + 26) % 26) + 'a';
		}
	}
}

void logInfo(char *command, char *desc) {
    time_t t = time(NULL);
    struct tm* lt = localtime(&t);

    char currTime[TIME_SIZE];
    strftime(currTime, TIME_SIZE, "%d%m%Y-%H:%M:%S", lt);

    char log[LOG_SIZE];
    sprintf(log, "%s::%s:%s::%s", warn, currTime, command, desc);

    FILE *out = fopen(logpath, "a");
    fprintf(out, "%s\n", log);
    fclose(out);

    return;
}

void logWarn(char *command, char *desc) {
    time_t t = time(NULL);
    struct tm* lt = localtime(&t);

    char currTime[TIME_SIZE];
    strftime(currTime, TIME_SIZE, "%d%m%Y-%H:%M:%S", lt);

    char log[LOG_SIZE];
    sprintf(log, "%s::%s:%s::%s", warn, currTime, command, desc);

    FILE *out = fopen(logpath, "a");
    fprintf(out, "%s\n", log);
    fclose(out);

    return;
}

void logEncode(char *dir1, char *dir2) {
    char buff[1024], cmd[32];
    if (dir1[0] != '\0') {
        strcpy(cmd, "RENAME");
        sprintf(buff, "%s::%s", dir1, dir2);
        logInfo(cmd, buff);
    }
    else {
        strcpy(cmd, "CREATE");
        sprintf(buff, "%s", dir2);
        logWarn(cmd, buff);
    }
}

void getFileDetail(const char *completeFileName, char *name, char *ext) {
    int index = 0;
    int i = 0;
    while(completeFileName[i]) {
        if (completeFileName[i] == '.') {
            break;
        }
        name[index++] = completeFileName[i];
        i++;
    }
    name[index] = '\0';
    index = 0;
    while (completeFileName[i]) {
        ext[index++] = completeFileName[i];
        i++;
    }
    ext[index] = '\0';
}

int encDirName(const char *basePath, const char* folderName) {
    char encryptedName[512];
    strcpy(encryptedName, folderName);
    encode(encryptedName, ENC_ATBASH);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, folderName);
    sprintf(t_path, "%s/%s", basePath, encryptedName);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

int encDirNameRXmk(const char *basePath, const char* folderName) {
    char encryptedName[512];
    strcpy(encryptedName, folderName);
    encode(encryptedName, ENC_ATBASH);
    encode(encryptedName, ENC_ROT13);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, folderName);
    sprintf(t_path, "%s/%s", basePath, encryptedName);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

int encDirNameRXrn(const char *basePath, const char* folderName) {
    char encryptedName[512];
    strcpy(encryptedName, folderName);
    encode(encryptedName, ENC_ATBASH);
    encode(encryptedName, ENC_VIG);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, folderName);
    sprintf(t_path, "%s/%s", basePath, encryptedName);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

int decDirName(const char *basePath, const char* folderName) {
    char decryptedName[512];
    strcpy(decryptedName, folderName);
    decode(decryptedName, ENC_ATBASH);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, folderName);
    sprintf(t_path, "%s/%s", basePath, decryptedName);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

int decDirNameRXmk(const char *basePath, const char* folderName) {
    char decryptedName[512];
    strcpy(decryptedName, folderName);
    decode(decryptedName, ENC_ROT13);
    decode(decryptedName, ENC_ATBASH);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, folderName);
    sprintf(t_path, "%s/%s", basePath, decryptedName);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

int decDirNameRXrn(const char *basePath, const char* folderName) {
    char decryptedName[512];
    strcpy(decryptedName, folderName);
    decode(decryptedName, ENC_VIG);
    decode(decryptedName, ENC_ATBASH);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, folderName);
    sprintf(t_path, "%s/%s", basePath, decryptedName);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

int encodeFile(char *basePath, char *name) {
    char fileName[512], ext[64];
    getFileDetail(name, fileName, ext);
    encode(fileName, ENC_ATBASH);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, name);
    sprintf(t_path, "%s/%s%s", basePath, fileName, ext);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

int encodeFileRXmk(char *basePath, char *name) {
    char fileName[1024], ext[64];
    getFileDetail(name, fileName, ext);
    encode(fileName, ENC_ATBASH);
    encode(fileName, ENC_ROT13);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, name);
    sprintf(t_path, "%s/%s%s", basePath, fileName, ext);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

int encodeFileRXrn(char *basePath, char *name) {
    char fileName[1024], ext[64];
    getFileDetail(name, fileName, ext);
    encode(fileName, ENC_ATBASH);
    encode(fileName, ENC_VIG);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, name);
    sprintf(t_path, "%s/%s%s", basePath, fileName, ext);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

int decFilename(char *basePath, char *name) {
    char fileName[1024], ext[64];
    getFileDetail(name, fileName, ext);
    decode(fileName, ENC_ATBASH);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, name);
    sprintf(t_path, "%s/%s%s", basePath, fileName, ext);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

int decFilenameRXmk(char *basePath, char *name) {
    char fileName[1024], ext[64];
    getFileDetail(name, fileName, ext);
    decode(fileName, ENC_ROT13);
    decode(fileName, ENC_ATBASH);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, name);
    sprintf(t_path, "%s/%s%s", basePath, fileName, ext);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

int decFilenameRXrn(char *basePath, char *name) {
    char fileName[1024], ext[64];
    getFileDetail(name, fileName, ext);
    decode(fileName, ENC_VIG);
    decode(fileName, ENC_ATBASH);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, name);
    sprintf(t_path, "%s/%s%s", basePath, fileName, ext);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

int encDirRec(char *basePath, int depth) {
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir) return 0;
    int count = 0;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
        strcpy(path, basePath);
        strcat(path, "/");
        strcat(path, dp->d_name);

        struct stat path_stat;
        stat(path, &path_stat);
        if (!S_ISREG(path_stat.st_mode)) {
            // Folder
            if (depth > 0) {
                count += encDirRec(path, depth - 1);
                encDirName(basePath, dp->d_name);
            }
        }
        else {
            // File
            if (encodeFile(basePath, dp->d_name) == 0) count++;
        }
    }
    closedir(dir);
    return count;
}

int encDirRecRXmk(char *basePath) {
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir) return 0;
    int count = 0;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
        strcpy(path, basePath);
        strcat(path, "/");
        strcat(path, dp->d_name);

        struct stat path_stat;
        stat(path, &path_stat);
        if (!S_ISREG(path_stat.st_mode)) {
            // Folder
            count += encDirRecRXmk(path);
            encDirNameRXmk(basePath, dp->d_name);
        }
        else {
            // File
            if (encodeFileRXmk(basePath, dp->d_name) == 0) count++;
        }
    }
    closedir(dir);
    return count;
}

int encDirRecRXrn(char *basePath, int depth) {
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir) return 0;
    int count = 0;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
        strcpy(path, basePath);
        strcat(path, "/");
        strcat(path, dp->d_name);

        struct stat path_stat;
        stat(path, &path_stat);
        if (!S_ISREG(path_stat.st_mode)) {
            // Folder
            if (depth > 0) {
                count += encDirRecRXrn(path, depth - 1);
                encDirNameRXrn(basePath, dp->d_name);
            }
        }
        else {
            // File
            if (encodeFileRXrn(basePath, dp->d_name) == 0) count++;
        }
    }
    closedir(dir);
    return count;
}

int decDirRec(char *basePath, int depth) {
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir) return 0;
    int count = 0;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
        strcpy(path, basePath);
        strcat(path, "/");
        strcat(path, dp->d_name);

        struct stat path_stat;
        stat(path, &path_stat);
        if (!S_ISREG(path_stat.st_mode)) {
            // Folder
            if (depth > 0) {
                count += decDirRec(path, depth - 1);
                decDirName(basePath, dp->d_name);
            }
        }
        else {
            // File
            if (decFilename(basePath, dp->d_name) == 0) count++;
        }
    }
    closedir(dir);
    return count;
}

int decDirRecRXmk(char *basePath) {
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir) return 0;
    int count = 0;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
        strcpy(path, basePath);
        strcat(path, "/");
        strcat(path, dp->d_name);

        struct stat path_stat;
        stat(path, &path_stat);
        if (!S_ISREG(path_stat.st_mode)) {
            // Folder
            count += decDirRecRXmk(path);
            decDirNameRXmk(basePath, dp->d_name);
        }
        else {
            // File
            if (decFilenameRXmk(basePath, dp->d_name) == 0) count++;
        }
    }
    closedir(dir);
    return count;
}

int decDirRecRXrn(char *basePath, int depth) {
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir) return 0;
    int count = 0;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
        strcpy(path, basePath);
        strcat(path, "/");
        strcat(path, dp->d_name);

        struct stat path_stat;
        stat(path, &path_stat);
        if (!S_ISREG(path_stat.st_mode)) {
            // Folder
            if (depth > 0) {
                count += decDirRecRXrn(path, INF);
                decDirNameRXrn(basePath, dp->d_name);
            }
        }
        else {
            // File
            if (decFilenameRXrn(basePath, dp->d_name) == 0) count++;
        }
    }
    closedir(dir);
    return count;
}

void hideRX(int flag, char *path){
    char name[1024];
    sprintf(name, "%s/.map", path);
    FILE *fptr = fopen(name, "w");
    if(fptr == NULL) exit(EXIT_FAILURE);
    fprintf(fptr, "%d", flag);
    fclose(fptr);
}

static int fs_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];
    // Concat path
    sprintf(fpath, "%s%s", dirpath, path);
    res = lstat(fpath, stbuf);
    if (res == -1) return -errno;
    return 0;
}

static int fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0) {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else sprintf(fpath, "%s%s", dirpath, path);

    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;

    dp = opendir(fpath);

    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if(filler(buf, de->d_name, &st, 0)) break;
    }
    closedir(dp);
    return 0;
}

int checkRX(char *path){
    char newpath[1024];
    strcpy(newpath, path);
    
    int len = strlen(path);
    
    for (int i = len-1; i >= 3 ; i--) {
        if (path[i-3] == 'R' && path[i-2] == 'X' && path[i-1] == '_'){
            for(int j = i; j < len; j++){
                if(newpath[j] == '/'){
                    newpath[j] = '\0';
                    break;
                }
            }
            strcat(newpath, "/.map");
            FILE *fptr = fopen(newpath, "r");
            char ch;
            fscanf(fptr, "%c", &ch);
            fclose(fptr);
            if(ch == '1')return 1;
            else return 0;
        }
    }
    return -1;
}

static int fs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0) {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else sprintf(fpath, "%s%s", dirpath, path);

    int fd;
    int res;
    (void) fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1) res = -errno;

    close(fd);

    return res;
}

static int fs_rename(const char *from, const char *to, unsigned int flags) {
    int res;
    if (flags) return -EINVAL;
    
    char fpath[1000], tpath[1000];
    if (strcmp(from, "/") == 0) {
        from = dirpath;
        sprintf(fpath, "%s", from);
    }
    else sprintf(fpath, "%s%s", dirpath, from);
    if (strcmp(to, "/") == 0) {
        to = dirpath;
        sprintf(tpath, "%s", to);
    }
    else sprintf(tpath, "%s%s", dirpath, to);

    // Check if it is folder
    struct stat path_stat;
    stat(fpath, &path_stat);
    if (!S_ISREG(path_stat.st_mode)) {
        if (isEncoded(fpath) == PREF_ATOZ && isEncoded(tpath) == PREF_AISA) {
            decDirRec(fpath, 0);
            logEncode(fpath, tpath);
        } else if (isEncoded(fpath) == PREF_AISA && isEncoded(tpath) == PREF_ATOZ) {
            encDirRec(fpath, 0);
            logEncode(fpath, tpath);
        } else if (isEncoded(fpath) == PREF_ATOZ && isEncoded(tpath) != PREF_ATOZ) {
            logEncode(fpath, tpath);
            int count = decDirRec(fpath, INF);
        } else if (isEncoded(fpath) != PREF_ATOZ && isEncoded(tpath) == PREF_ATOZ) {
            logEncode(fpath, tpath);
            int count = encDirRec(fpath, INF);
        } else if (isEncoded(fpath) == PREF_RX && isEncoded(tpath) == PREF_AISA) {
            decDirRecRXrn(fpath, 0);
        } else if (isEncoded(fpath) == PREF_AISA && isEncoded(tpath) == PREF_RX) {
            encDirRecRXrn(fpath, 0);
        } else if(isEncoded(fpath) != PREF_RX && isEncoded(tpath) == PREF_RX){
            encDirRecRXrn(fpath, INF);
            logEncode(fpath, tpath);
        } else if(isEncoded(fpath) == PREF_RX && isEncoded(tpath) != PREF_RX){
            decDirRecRXrn(fpath, INF);
            logEncode(fpath, tpath);
        } else if (isEncoded(fpath) == PREF_AISA && isEncoded(tpath) != PREF_AISA) {
            logEncode(fpath, tpath);
        } else if (isEncoded(fpath) != PREF_AISA && isEncoded(tpath) == PREF_AISA) {
            logEncode(fpath, tpath);
        }
    }

    res = rename(fpath, tpath);
    if (res == -1) return -errno;
    return 0;
}

static int fs_mknod(const char *path, mode_t mode, dev_t rdev)
{
	char fpath[1000];
	if (strcmp(path, "/") == 0) {
		path = dirpath;
		sprintf(fpath, "%s", path);
	}
	else sprintf(fpath, "%s%s", dirpath, path);

	int res;

	if (S_ISREG(mode)) {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(fpath, mode);
	else
		res = mknod(fpath, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}

static int fs_mkdir(const char *path, mode_t mode) {
    int res;
    
    char fpath[1000];
    
    if (strcmp(path, "/") == 0) {
        path = dirpath;
        sprintf(fpath, "%s", path);
    } else sprintf(fpath, "%s%s", dirpath, path);

    res = mkdir(fpath, mode);
    if (res == -1) return -errno;

    if (isEncoded(fpath) == PREF_ATOZ){
        logEncode("", fpath);
    } else if (isEncoded(fpath) == PREF_AISA) {
        logEncode("", fpath);
    } else if(isEncoded(fpath) == PREF_RX){
        logEncode("", fpath);
        hideRX(1, fpath);
    }
    
    return 0;
}

static const struct fuse_operations fs_oper = {
	.mkdir = fs_mkdir,
	.rename = fs_rename,
   	.mknod = fs_mknod,
   	.getattr = fs_getattr,
   	.readdir = fs_readdir,
	.read = fs_read
};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &fs_oper, NULL);
}
