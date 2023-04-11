#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STACK_SIZE 100

//스택 생성
typedef int element;
typedef struct {
    element stack[MAX_STACK_SIZE];
    int top;
} StackType;

//스택 초기화 함수
void init(StackType* s)
{
    s->top = -1;
}

//공백 상태 검출 함수
int is_empty(StackType* s)
{
    return (s->top == -1);
}

//포화 상태 검출 함수
int is_full(StackType* s)
{
    return (s->top == (MAX_STACK_SIZE - 1));
}

StackType* s;

//삽임 함수
void push(StackType* s, element item)
{
    if (is_full(s)) {
        fprintf(stderr, "스택포화 에러 \n");
        return;
    }
    else    s->stack[++(s->top)] = item;

}

//삭제 함수
element pop(StackType* s)
{
    if (is_empty(s)) {
        fprintf(stderr, "스택공백 에러 \n");
        exit(1);
    }
    else   return s->stack[(s->top)--];

}


//피크 함수
element peek(StackType* s)
{
    if (is_empty(s)) {
        fprintf(stderr, "스택공백 에러 \n");
        exit(1);
    }
    else    return s->stack[s->top];

}

// 우선순위 반환 메서드
int prec(char op)
{
    switch (op) {
    case '(':
    case ')':
        return 0;
    case '{':
    case '}':
        return 1;
    case '[':
    case ']':
        return 2;
    case '+':
    case '-':
        return 3;
    case '*':
    case '/':
        return 4;
    }
    return -1;
}

int isoper(char c)
{
    return 0 <= prec(c);
}


//후위 표기식 계산
int eval(char* exp)
{
    int op1, op2, value = 0, i = 0;
    int len = strlen(exp);
    char ch;
    StackType s;

    init(&s); //스택 초기화

    for (i = 0; i < len; i++) {
        ch = exp[i];    // 문자형 변수 ch에 후위표기식을 대입

        if (isdigit(ch)) { //ch가 비연산자 이면
            value = ch - '0';
            while (isdigit(exp[i + 1])) {
                i++;
                value *= 10;
                value += exp[i] - '0';
            }
            push(&s, value); //스택에 value 삽입
        }
        else if (isoper(ch)) {
            op2 = pop(&s);
            op1 = pop(&s);
            switch (ch) {
            case '+':
                push(&s, op1 + op2);
                break;
            case '-':
                push(&s, op1 - op2);
                break;
            case '*':
                push(&s, op1 * op2);
                break;
            case '/':
                push(&s, op1 / op2);
                break;
            }
        }
    }
    return pop(&s);
}

// 중위표기식 -> 후위표기식 변환 함수
void infix_to_postfix(char exp[], char cexp[])
{
    // exp[] : 받아올 중위표기식,  cexp[] : 후위표기식으로 변환하여 저장할 배열
    int i = 0;
    int j = 0;
    char ch, top_op;
    int len = (int)strlen(exp);

    StackType s;
    init(&s); //스택초기화

    for (i = 0; i < len; i++) {
        ch = exp[i];
        if (isdigit(ch)) {
            cexp[j++] = ch;
            while (isdigit(exp[i + 1])) {
                i++;
                cexp[j++] = exp[i];
            }
            cexp[j++] = ' ';
        }
        else if (isoper(ch)) {
            switch (ch) {
                // 연산자일 경우
            case '+':
            case '-':
            case '*':
            case '/':
                //스택에 있는 연산자의 우선순위가 더 크거나 같으면 출력
                while (!is_empty(&s) && (prec(ch) <= prec(peek(&s)))) {
                    cexp[j++] = pop(&s);
                    cexp[j++] = ' ';
                }
                push(&s, ch);
                break;

                // 괄호의 경우
            case '(':           //왼쪽 괄호
                push(&s, ch);
                break;
            case ')':           //오른쪽 괄호
                top_op = pop(&s);

                while (top_op != '(') {          //왼쪽 괄호를 만날 때 까지 출력
                    cexp[j++] = top_op;
                    cexp[j++] = ' ';
                    top_op = pop(&s);
                }
                break;
            case '{':           //왼쪽 괄호
                push(&s, ch);
                break;
            case '}':           //오른쪽 괄호
                top_op = pop(&s);

                while (top_op != '{') {          //왼쪽 괄호를 만날 때 까지 출력
                    cexp[j++] = top_op;
                    cexp[j++] = ' ';
                    top_op = pop(&s);
                }
                break;
            case '[':           //왼쪽 괄호
                push(&s, ch);
                break;
            case ']':           //오른쪽 괄호
                top_op = pop(&s);

                while (top_op != '[') {          //왼쪽 괄호를 만날 때 까지 출력
                    cexp[j++] = top_op;
                    cexp[j++] = ' ';
                    top_op = pop(&s);
                }
                break;
            }
        }
    }

    while (!is_empty(&s)) {
        cexp[j++] = pop(&s);
        cexp[j] = NULL;
    }
}

int main()
{
    int result = 0;

    char a[100];
    char b[100];
    //a는 입력받은 식 , b는 후위표기식으로 변환 될 배열

    printf("Enter an Infix expression : ");
    scanf_s("%s", a, sizeof(a));
    infix_to_postfix(a, b);
    printf("Postfix expression: %s \n", b);

    result = eval(b);

    printf("\n");
    printf("Result : %d \n", result);
}