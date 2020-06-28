#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <wiringPi.h>
int hex2int(char ch);
void shuffle(int *arr, int num);
int main (int argc, char* argv[])
{
    wiringPi.h 를 사용하기 위한 Setup 함수입니다.
    wiringPiSetup();
    argv[1]을 type에 넣으면서 App 1 , App 2를 구분할 때 사용할 것입니다.
    char* type = argv[1];
    입력 받은 argument의 개수가 입력 형식에 벗어난 경우 에러 핸들링 하는 코드입니다.
    if(argc != 2 && argc != 3)
    {
      printf("Please input 1 or 2 arguments\n");
      exit(1);
    } else if(type[0] == '1' && argc == 2)                    // assignment 1
    {
        int i, j;
        사용할 LED pin 번호들을 배열에 넣어 놓습니다.
        int LED[7] = { 26, 14, 13, 12 ,21, 22, 23 };
        7 세그먼트를 배열에 담아둡니다.
        int mapping[16][7] = {
            {1, 1, 1, 1, 1, 1, 0},  //0 
            {0, 1, 1, 0, 0, 0 ,0},  //1 
            {1, 1, 0, 1, 1, 0, 1},  //2 
            {1,1,1,1,0,0,1},        //3
            {0,1,1,0,0,1,1},        //4
            {1,0,1,1,0,1,1},        //5
            {1,0,1,1,1,1,1},        //6
            {1,1,1,0,0,0,0},
            {1,1,1,1,1,1,1},
            {1,1,1,1,0,1,1},
            {1,1,1,1,1,0,1},
            {0,0,1,1,1,1,1},
            {1,0,0,1,1,1,0},
            {0,1,1,1,1,0,1},
            {1,0,0,1,1,1,1},
            {1,0,0,0,1,1,1},
        };
        15가지 글자들을 랜덤하게 출력시키기 위한 배열을 셋팅합니다.
        int shuffle_arr[15];
        for(i = 0; i < 15; i++)
        {
            shuffle_arr[i] = i;
        }
        shuffle(shuffle_arr, 15);
        모든 핀을 출력모드로 설정합니다.
        for(i = 0; i < 7; i++)
        {
            pinMode(LED[i], OUTPUT);
        }
        0~9, a~f 까지 순서대로 출력하는 이중 for문입니다
        mapping 배열을 적절히 만들어 놓은 후 이중 for문으로 제어하였습니다.
        딜레이는 500으로 0.5초마다 다음 차례로 넘어갑니다.
        for (i = 0; i < 16; i++) {
            for(j = 0; j < 7; j++)
            {
                digitalWrite(LED[j], mapping[i][j]);
            }
            delay(500);                        // mS
        }
        15개의 숫자가 랜덤으로 나열되어있는 shuffle_arr의 값대로
        mapping 배열의 첫 번째 인덱스에 랜덤하게 들어가서 중복없이 무작위로
        LED에 값을 보여줄 수 있습니다.
        for(i = 0; i < 15; i++)
        {
            for(j = 0; j < 7; j++)
            {
                digitalWrite(LED[j], mapping[shuffle_arr[i]][j]);
            }
            delay(500);                        // mS
        }
    }
    else if(type[0] == '2' && argc == 3)
    {
        int i, a1, a2;
        16진수 표시로 4글자가 아닌 입력을 받는 경우의 에러를 잡아줍니다.
        if(strlen(argv[2]) != 4)
        {
            printf("Please input hexa form 0x~~ \n");
        }
        int LED[7] = { 23, 22, 21, 12 ,13, 14, 26 };
        char* hexa = argv[2];
        16진수인 0x로 시작하지 않으면 에러를 잡아줍니다.
        if(hexa[0] != '0' || hexa[1] != 'x')
        {
            printf("Please input hexa form 0x~~ \n");
            exit(1);
        }
        0x7f 이상의 값을 입력받았을 경우 오류를 출력합니다.
        if(hexa[2] < '0' || hexa[2] > '7')
        {
            printf("A number higher than 0x7F is not allowed\n");
            exit(1);
        }
        if((hexa[3] < '0' || hexa[3] > '9') && (hexa[3] < 'a' || hexa[3] > 'f')
&& (hexa[3] < 'A' || hexa[3] > 'F'))
        {
            printf("Please input any hexa number at second index\n");
            exit(1);
        }
        16진수를 10진수로 변환하여 저장합니다.
        a1 = hex2int(hexa[2]);
        a2 = hex2int(hexa[3]);
        10진수를 2진수로 변환하는 코드입니다.
        2로 나누고 남은 값을 배열에 저장하고 0보다 클때까지 
        2로 나누며 반복합니다.
        int result[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        for(i = 3; 0 < a1 ; i--)
        {
            result[i] = a1 % 2;
            a1 = a1 / 2;
        }
        for(i = 7; 0 < a2 ; i--)
        {
            result[i] = a2 % 2;
            a2 = a2 / 2;
        }
        2진수로 변환된 result 배열이 1일 때 LED를 키고
        2진수로 변환된 result 배열이 0일 때 LED를 꺼줍니다.
        for(i = 1; i <= 7; i++)
        {
            if(result[i]) digitalWrite(LED[i - 1], HIGH);
            else digitalWrite(LED[i - 1], LOW);
        }
    }
    else{
        printf("Only 1 and 2 are available for options\n");
        exit(1);
    }
    return 0;
}
int hex2int(char ch)
{
    if (ch >= 'A' && ch <= 'F')
    {
        ch += 32;
    }
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return -1;
}

void shuffle(int *arr, int num)
{
    srand(time(NULL));
    int temp;
    int rn, i;
    for (i=0; i < num-1; i++)
    {
        rn = rand() % (num - i) + i;
        temp = arr[i];
        arr[i] = arr[rn];
        arr[rn] = temp;
    }
}

