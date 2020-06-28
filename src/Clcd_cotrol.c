#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#define MAX_SIZE 32
// Pin mapping
#define LCD_D4 2
#define LCD_D5 3
#define LCD_D6 1
#define LCD_D7 4
#define LCD_RS 7
#define LCD_EN 0
#define BT0 12
#define BT1 13
#define BT2 14
#define BT3 21
#define BT4 22
#define BT5 23
#define BT6 24
#define BT7 10
#define BT8 11
#define BT9 26
#define EQL 27
#define PLUS 5
#define MINUS 6

char buf[MAX_SIZE];
char overflow[9] = "Overflow";
char invalid[18] = "Invalid operation";
int num_idx = 0;                // parsing 에서 사용, len으로 사용
int op_idx = 0;           // parsing 에서 사용, len으로 사용
int cursor = 0;
int error = 1;
void write4bits(unsigned char command)
{
    digitalWrite(LCD_D4, (command & 1));
    command >>= 1;
    digitalWrite(LCD_D5, (command & 1));
    command >>= 1;
    digitalWrite(LCD_D6, (command & 1));
    command >>= 1;
    digitalWrite(LCD_D7, (command & 1));
    digitalWrite(LCD_EN, 1);
    delayMicroseconds(10);
    digitalWrite(LCD_EN, 0);
    delayMicroseconds(10);
}

void sendDataCmd4(unsigned char data)
{
    write4bits(((data >> 4) & 0x0f));
    write4bits((data & 0x0f));
    delayMicroseconds(100);
}

void putCmd4(unsigned char cmd)
{
    digitalWrite(LCD_RS, 0);
    sendDataCmd4(cmd);
}

void putChar(char c) {

    digitalWrite(LCD_RS, 1);

    sendDataCmd4(c);
}

void initialize_textlcd() {	//CLCD 초기화
    pinMode(LCD_RS, OUTPUT); digitalWrite(LCD_RS, 0);
    pinMode(LCD_EN, OUTPUT); digitalWrite(LCD_EN, 0);
    pinMode(LCD_D4, OUTPUT); digitalWrite(LCD_D4, 0);
    pinMode(LCD_D5, OUTPUT); digitalWrite(LCD_D5, 0);
    pinMode(LCD_D6, OUTPUT); digitalWrite(LCD_D6, 0);
    pinMode(LCD_D7, OUTPUT); digitalWrite(LCD_D7, 0);

    // 버튼이 눌러지지 않았을 때 1로 인식한다.
    // digitalRead(BT0)         << 해당 스위치상태를 가져오는 함수로 1이면
    pinMode(BT0, INPUT);    pullUpDnControl(BT0, PUD_DOWN);
    pinMode(BT1, INPUT);    pullUpDnControl(BT1, PUD_DOWN);
    pinMode(BT2, INPUT);    pullUpDnControl(BT2, PUD_DOWN);
    pinMode(BT3, INPUT);    pullUpDnControl(BT3, PUD_DOWN);
    pinMode(BT4, INPUT);    pullUpDnControl(BT4, PUD_DOWN);
    pinMode(BT5, INPUT);    pullUpDnControl(BT5, PUD_DOWN);
    pinMode(BT6, INPUT);    pullUpDnControl(BT6, PUD_DOWN);
    pinMode(BT7, INPUT);    pullUpDnControl(BT7, PUD_DOWN);
    pinMode(BT8, INPUT);    pullUpDnControl(BT8, PUD_DOWN);
    pinMode(BT9, INPUT);    pullUpDnControl(BT9, PUD_DOWN);
    pinMode(EQL, INPUT);    pullUpDnControl(EQL, PUD_DOWN);
    pinMode(PLUS, INPUT);    pullUpDnControl(PLUS, PUD_UP);
    pinMode(MINUS, INPUT);    pullUpDnControl(MINUS, PUD_UP);
    delay(35);

    // 4비트 단위 전송(reg to memory) 2줄
    putCmd4(0x28);  // 4비트 2줄
    putCmd4(0x28); putCmd4(0x28);

    putCmd4(0x0e); // 디스플레이온 커서 온 
    putCmd4(0x02); // 커서 홈 
    delay(2);
    putCmd4(0x01); // 표시 클리어 
    delay(2);
}
void print_overflow()
{
    int i;
    initialize_textlcd();
    for (i = 0; i < 8; i++)
    {
        putChar(overflow[i]);
    }
    delay(2000);
    initialize_textlcd();
}

void print_invalid()
{
    int i;
    initialize_textlcd();
    for (i = 0; i < 17; i++)
    {
        putChar(invalid[i]);
        if(i == 15) putCmd4(0xC0);
    }
    delay(2000);
    initialize_textlcd();
}
int Input()
{
    int i;
    while (digitalRead(EQL) == 0)
    {
        if (cursor > 32) return 0;  // 입력 overflow 발생
        if (digitalRead(BT0) == 1)
        {
            buf[cursor++] = '0';
            putChar('0');
            delay(500);
        }
        if (digitalRead(BT1) == 1)
        {
            buf[cursor++] = '1';
            putChar('1');        // 1 char는 1 byte 즉, 8bit로 되어있으니까
            delay(500);
        }
        if (digitalRead(BT2) == 1)
        {
            buf[cursor++] = '2';
            putChar('2');        // 1 char는 1 byte 즉, 8bit로 되어있으니까
            delay(500);
        }
        if (digitalRead(BT3) == 1)
        {
            buf[cursor++] = '3';
            putChar('3');
            delay(500);
        }
        if (digitalRead(BT4) == 1)
        {
            buf[cursor++] = '4';
            putChar('4');        // 1 char는 1 byte 즉, 8bit로 되어있으니까
            delay(500);
        }
        if (digitalRead(BT5) == 1)
        {
            buf[cursor++] = '5';
            putChar('5');        // 1 char는 1 byte 즉, 8bit로 되어있으니까
            delay(500);
        }
        if (digitalRead(BT6) == 1)
        {
            buf[cursor++] = '6';
            putChar('6');
            delay(500);
        }
        if (digitalRead(BT7) == 1)
        {
            buf[cursor++] = '7';
            putChar('7');
            delay(500);
        }
        if (digitalRead(BT8) == 1)
        {
            buf[cursor++] = '8';
            putChar('8');
            delay(500);
        }
        if (digitalRead(BT9) == 1)
        {
            buf[cursor++] = '9';
            putChar('9');
            delay(500);
        }
        if (digitalRead(PLUS) == 0)
        {
            buf[cursor++] = '+';
            putChar('+');        // 1 char는 1 byte 즉, 8bit로 되어있으니까
            delay(500);
        }
        if (digitalRead(MINUS) == 0)
        {
            buf[cursor++] = '-';
            putChar('-');        // 1 char는 1 byte 즉, 8bit로 되어있으니까
            delay(500);
        }
        if (cursor == 16) putCmd4(0xC0);
    }
    putChar('='); cursor++;
    return 1;
}
int Parsing()
{
    int number[MAX_SIZE / 2];
    char operate[MAX_SIZE / 2];
    int len = strlen(buf); int i;
    char num_buf[MAX_SIZE];          // 연산자가 나오기 전까지의 number를 담아주는 char buf 
    int num_len = 0;
    // 숫자를 받아들인 길이가 2 이상이면 atoi 값 넣기
    // 숫자를 받아들인 길이가 1이면 (x  - '0') 값 넣기 으로
    for (i = 0; i < len; i++)
    {
        if (i == 0 || (buf[i] >= '0' && buf[i] <= '9'))         // 숫자이면
        {
            num_buf[num_len++] = buf[i];
        } 
        else{                             // 연산자인 경우
            if (i == 1)
            {
                if (buf[0] == '+' || buf[0] == '-')
                {
                    return 0;
                }
            }
            if (num_len >= 2)
            {
                number[num_idx++] = atoi(num_buf);
            }
            else if(num_len == 1){
                number[num_idx++] = num_buf[0] - '0';
            }
            num_len = 0;
            // 연산자 처리
            operate[op_idx++] = buf[i];   
        }
    }
    if (num_len >= 2)
    {
        number[num_idx++] = atoi(num_buf);
    }
    else {
        number[num_idx++] = num_buf[0] - '0';
    }
    printf("num_idx :  %d\n", num_idx);
    printf("op_idx :  %d\n", op_idx);
    if (op_idx >= num_idx) return 0;
    else
    {
        return Calc(number, operate);
    }
}

int Calc(int number[], char operate[])
{
    int i;
    int result = 0;
    error = 0;
    result = number[0];
    for (i = 0; i < op_idx; i++)
    {
        if (operate[i] == '+')
        {
            result += number[i + 1];
        }
        if (operate[i] == '-')
        {
            result -= number[i + 1];
        }
    }
    return result;
}

void init(int version)     // 모든 상태 초기화
{
    // init version 1은 초기화만 해주기
    // init version 2는 숫자 버튼을 누를 경우에 넘어가기
    num_idx = 0;
    op_idx = 0;
    cursor = 0;
    error = 1;
    memset(buf, '\0', sizeof(buf));         // NULL문자로 초기화 시키지않으면 계쏙 BUF에 남아서 strlen(buf) 때문에 오류가 생김
    if (version == 1)
    {
        initialize_textlcd();
        return;
    }
    while (!digitalRead(BT0) && !digitalRead(BT1) && !digitalRead(BT2) && !digitalRead(BT3)
        && !digitalRead(BT4) && !digitalRead(BT5) && !digitalRead(BT6) && !digitalRead(BT7)
        && !digitalRead(BT8) && !digitalRead(BT9))            // 어떤 숫자가 눌렸을 때 화면이 지워지고 그 다음 숫자부터 시작
    {
        continue;
    }
    initialize_textlcd();
    delay(500);                          // delay 해줘야 다시 눌러진 숫자부터 표시된다.
}

void start()
{
    int i;
    int result;
    while (1)
    {
        int check_over = 0;                   // overflow이면 화면 새로운 창이된 후 바로 숫자를 쓸 수 있게 해줌
        char s1[MAX_SIZE];
        result = 0;
        if (Input() == 0)
        {
            print_overflow();
            init(1);
        }
        else
        {
            // = (EQL) 버튼을 누른 경우
            result = Parsing();
            if (result == 0 && error == 1)         // number, operate 배열에 각 각 숫자와 연산자가 들어간다.
            {
                // 올바르지 않은 수식인 경우
                print_invalid();
                init(1);
            }
            else {
                sprintf(s1, "%d", result);
                int s1_len = strlen(s1);
                for (i = 0; i < s1_len; i++)
                {
                    if (cursor + i == 16) putCmd4(0xC0);
                    if (cursor + i > 32) { // 출력 중 overlfow가 발생하는 경우
                        print_overflow();
                        check_over = 1;
                        break;
                    }
                    putChar(s1[i]);
                }
                if (check_over) init(1);
                init(2);
            }
        }
    }
}

int main(int argc, char** argv) {
    wiringPiSetup();                // wiringPi로 핀을 사용하겠다는 시작의미
    initialize_textlcd();           // LCD 세팅
    start();
}
