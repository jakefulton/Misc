//Jake Fulton ID: 51804736
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <stdint.h>

#define MAX_FILES 512

//file struct will store information about a file
typedef struct
{
    char *name;
    char *group;
    nlink_t links;
    char time[150];
    char *user;
    off_t size;
    char mode[10];
} file;

void exiting(char *error_message)
{
    perror(error_message);
    exit(1);
}

char *get_group_name(struct stat statbuf)
{
    gid_t gid = statbuf.st_gid;
    struct group *grp = getgrgid(gid);
    return grp->gr_name ? grp->gr_name: "";
}

char *get_user_name(struct stat statbuf)
{
    uid_t uid = statbuf.st_uid;
    struct passwd *pw = getpwuid(uid);
    return pw->pw_name ? pw->pw_name: "";
}

void *get_time(struct stat statbuf, char *time_str)
{
    time_t last_mod = statbuf.st_mtime;
    struct tm *tm;
    tm = localtime(&last_mod);
    strftime(time_str, 150, "%b %d %R", tm);
}

static char file_type(mode_t mode)
{
    if (S_ISDIR(mode))
        return 'd';
    else if (S_ISCHR(mode))
        return 'c';
    else if (S_ISBLK(mode))
        return 'b';
    else if (S_ISFIFO(mode))
        return 'f';
    else if (S_ISSOCK(mode))
        return 's';
    else if (S_ISLNK(mode))
        return 'l';
    else
        return '-';
}

void get_mode(struct stat statbuf, char *mode_str)
{
    mode_t mode = statbuf.st_mode;
    mode_str[0] = file_type(mode);
    mode_str[1] = mode & S_IRUSR ? 'r' : '-';
    mode_str[2] = mode & S_IWUSR ? 'w' : '-';
    mode_str[3] = mode & S_IXUSR ? 'x' : '-';
    mode_str[4] = mode & S_IRGRP ? 'r' : '-';
    mode_str[5] = mode & S_IWGRP ? 'w' : '-';
    mode_str[6] = mode & S_IXGRP ? 'x' : '-';
    mode_str[7] = mode & S_IROTH ? 'r' : '-';
    mode_str[8] = mode & S_IWOTH ? 'w' : '-';
    mode_str[9] = mode & S_IXOTH ? 'x' : '-';
    mode_str[10] = '\0';
}

void get_file_information(char*dir_path, struct dirent *entry,file files[], int index)
{
    struct stat statbuf;
    char full_path[512];
    char error_message[512];
    if (index >= MAX_FILES)
    {
        printf("INDEX: %d\n", index);
        exiting("Too many files in directory. Please increase MAX_FILES");
    }
    sprintf(full_path, "%s/%s", dir_path, entry->d_name);
    if ( stat(full_path, &statbuf) == -1 )
    {
        sprintf(error_message, "stat() error. Possible broken link: %s", full_path);
        exiting(error_message);
    }
    files[index].name = entry->d_name; 
    files[index].group = get_group_name(statbuf);
    files[index].links = statbuf.st_nlink;
    files[index].user = get_user_name(statbuf);
    files[index].size = statbuf.st_size;
    get_time(statbuf, files[index].time);
    get_mode(statbuf, files[index].mode);
}

int max_size_length(int n)
{
    //Determine how many characters in an integer
    if (n < 10) return 1;
    return 1 + max_size_length (n/10);
}

void print_files(file files[], int number_of_files)
{
    //find out how many characters to leave for size field
    int width = max_size_length(files[0].size); 
    for (int i = 0; i < number_of_files; ++i)
    {
        printf("%s %d %s %s %*d %s %s\n", files[i].mode, files[i].links, files[i].user,
        files[i].group, width, files[i].size, files[i].time, files[i].name);
    }
}

int compare(const void *a, const void *b)
{
    //Decides which of two files comes first in file list
    int l = ((file *)a)->size;
    int r = ((file *)b)->size;
    return ( r-l );
}

int main()
{
    DIR *dir;
    struct dirent *entry;
    char dir_name[1024];
    int number_of_files = 0;
    file files[MAX_FILES];

    //Get current working directory
    if(!getcwd(dir_name, sizeof(dir_name)))
        exiting("getcwd() erorr");

    //Open directory
    if( !(dir = opendir(dir_name)) )
        exiting("opendir() error");

    //Iterate through files in directory
    for( ; entry = readdir(dir); number_of_files++) 
    {
        //Gather information about file
        get_file_information(dir_name, entry, files, number_of_files); 
    }

    //Sort the file list by size (largest first)
    qsort(files, number_of_files, sizeof(file), compare);

    //Print file information
    print_files(files, number_of_files);

    //Close directory
    closedir(dir);
    return 0;
}
