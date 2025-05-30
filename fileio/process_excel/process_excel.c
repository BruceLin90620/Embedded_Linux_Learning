#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

/*
./process_excel data.csv result.csv
argc = 3
argv[0] = "./process_excel"
argv[1] = data.csv
argv[2] = result.csv
*/
static int read_line(int fd, unsigned char *buf) {
    unsigned char c;
    int len;
    int i = 0;
    int err = 0;

    while (1) {
        len = read(fd, &c, 1);
        if (len <= 0) {
            err = -1;
            break;
        }
        else {
            if (c != '\n' && c != '\r') {
                buf[i] = c;
                i++;
            }
            else {
                err = 0;
                break;
            }
        }
    }

    buf[i] = '\0';

    if (err && (i == 0)) {
        return -1;
    }
    else {
        return i;
    }
}

void process_data(unsigned char *data_buf, unsigned char *result_buf) {
    char name[100];
    int scores[3];
    int sum;
    char *levels[] = {"A+", "A", "B"};
    int level;

    if (data_buf[0] == 0xef) {
        strcpy(result_buf, data_buf);
    }
    else {
        sscanf(data_buf, "%[^,], %d, %d, %d, ", name, &scores[0], &scores[1], &scores[2]);
        sum = scores[0] + scores[1] + scores[2];
        
        if (sum >= 270)
            level = 0;
        else if (sum >= 240)
            level = 1;
        else
            level = 2;

            sprintf(result_buf, "%s, %d, %d, %d, %d. %s", name, scores[0], scores[1], scores[2], sum, levels[level]);
    }
}

int main(int argc, char **argv) {

    int fd_data, fd_result;
    int i;
    int len;
    unsigned char data_buf[1000];
    unsigned char result_buf[1000];

    if (argc != 3) {
        printf("Usage: %s <data csv file> <result csv file>\n", argv[0]);
        return -1;
    }

    fd_data = open(argv[1], O_RDONLY);
    if (fd_data < 0) {
        printf("cannot open file %s\n", argv[1]);
        perror("open");
        return -1;
    }
    else {
        printf("data file fd = %d\n", fd_data);
    }

    fd_result = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd_result < 0) {
        printf("cannot create file %s\n", argv[2]);
        perror("create");
        return -1;
    }
    else {
        printf("result file fd = %d\n", fd_result);
    }

    while (1) {
        len = read_line(fd_data, data_buf);
        if (len == -1) {
            break;
        }

        if (len != 0) {
            process_data(data_buf, result_buf);

            write(fd_result, result_buf, strlen(result_buf));

            write(fd_result, "\r\n", 2);
        }
    }

    close(fd_data);
    close(fd_result);

    return 0;
}