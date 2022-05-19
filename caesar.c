#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_SIZE 513 //문자열(512) + null

const char match_table[54] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

//오류메세지 처리
void die(char *message)
{
    printf("%s", message);
    exit(1);
}

int main(int argc, char *argv[])
{

    //입력 오류 처리
    if (argc != 3)
        die("./<file name> <key> <file path>");
    if (!isdigit(argv[1][0]) || !isdigit(argv[1][1]))
    { // isdigit은 문자열이 digit일 아닐 경우 0을 반환하므로 !을 붙인다.
        if (isdigit(argv[1][0]) && argv[1][1] == '\0')
            ;
        else
            die("<key> must be digit");
    }

    printf("I HATE UTF-8 FILE, I ONLY LIKE ANSII FILE !!!\n");
    int key;
    //무효한 키 처리 -> 제자리 이동, 의미없는 큰 수
    if (argv[1][1] == '\0')
        key = (argv[1][0] - 48);
    else
        key = (argv[1][0] - 48) * 10 + (argv[1][1] - 48);
    if (key < 1 || key > 51) //알파벳 a(0) ~ Z(51)
        die("<key> must be greater than 0 or less than 52");

    //파일 오픈
    FILE *F_string = fopen(argv[2], "r");
    FILE *cryted_file = fopen("cryted_file.txt", "wt");
    if (F_string == NULL || cryted_file == NULL)
        die("The file didn't open");

    //파일 내용 받아오기
    char string[MAX_SIZE]; // END_offset 512
    fscanf(F_string, "%[^\n]s", string);

    //파일 사이즈 체크
    int file_size = ftell(F_string); // stringEND_offset 변환 file_size - 1
    if (file_size > 512)
        die("Too big file");

    // right shift치환 암호
    for (int i = 0; string[i] != '\0'; i++)
    {
        if (!isalpha(string[i]))
            continue;
        char *CUR_offset = strchr(match_table, string[i]);
        if (CUR_offset + key > &match_table[51])
            string[i] = *(CUR_offset + key - 52);
        else
            string[i] = *(CUR_offset + key);
    }

    //파일쓰기 & 닫기
    fwrite(string, file_size, 1, cryted_file);
    fclose(F_string), fclose(cryted_file);
    return 0;
}