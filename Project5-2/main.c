#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STACK_SIZE 100

//���� ����
typedef int element;
typedef struct {
    element stack[MAX_STACK_SIZE];
    int top;
} StackType;

//���� �ʱ�ȭ �Լ�
void init(StackType* s)
{
    s->top = -1;
}

//���� ���� ���� �Լ�
int is_empty(StackType* s)
{
    return (s->top == -1);
}

//��ȭ ���� ���� �Լ�
int is_full(StackType* s)
{
    return (s->top == (MAX_STACK_SIZE - 1));
}

StackType* s;

//���� �Լ�
void push(StackType* s, element item)
{
    if (is_full(s)) {
        fprintf(stderr, "������ȭ ���� \n");
        return;
    }
    else    s->stack[++(s->top)] = item;

}

//���� �Լ�
element pop(StackType* s)
{
    if (is_empty(s)) {
        fprintf(stderr, "���ð��� ���� \n");
        exit(1);
    }
    else   return s->stack[(s->top)--];

}


//��ũ �Լ�
element peek(StackType* s)
{
    if (is_empty(s)) {
        fprintf(stderr, "���ð��� ���� \n");
        exit(1);
    }
    else    return s->stack[s->top];

}

// �켱���� ��ȯ �޼���
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


//���� ǥ��� ���
int eval(char* exp)
{
    int op1, op2, value = 0, i = 0;
    int len = strlen(exp);
    char ch;
    StackType s;

    init(&s); //���� �ʱ�ȭ

    for (i = 0; i < len; i++) {
        ch = exp[i];    // ������ ���� ch�� ����ǥ����� ����

        if (isdigit(ch)) { //ch�� �񿬻��� �̸�
            value = ch - '0';
            while (isdigit(exp[i + 1])) {
                i++;
                value *= 10;
                value += exp[i] - '0';
            }
            push(&s, value); //���ÿ� value ����
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

// ����ǥ��� -> ����ǥ��� ��ȯ �Լ�
void infix_to_postfix(char exp[], char cexp[])
{
    // exp[] : �޾ƿ� ����ǥ���,  cexp[] : ����ǥ������� ��ȯ�Ͽ� ������ �迭
    int i = 0;
    int j = 0;
    char ch, top_op;
    int len = (int)strlen(exp);

    StackType s;
    init(&s); //�����ʱ�ȭ

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
                // �������� ���
            case '+':
            case '-':
            case '*':
            case '/':
                //���ÿ� �ִ� �������� �켱������ �� ũ�ų� ������ ���
                while (!is_empty(&s) && (prec(ch) <= prec(peek(&s)))) {
                    cexp[j++] = pop(&s);
                    cexp[j++] = ' ';
                }
                push(&s, ch);
                break;

                // ��ȣ�� ���
            case '(':           //���� ��ȣ
                push(&s, ch);
                break;
            case ')':           //������ ��ȣ
                top_op = pop(&s);

                while (top_op != '(') {          //���� ��ȣ�� ���� �� ���� ���
                    cexp[j++] = top_op;
                    cexp[j++] = ' ';
                    top_op = pop(&s);
                }
                break;
            case '{':           //���� ��ȣ
                push(&s, ch);
                break;
            case '}':           //������ ��ȣ
                top_op = pop(&s);

                while (top_op != '{') {          //���� ��ȣ�� ���� �� ���� ���
                    cexp[j++] = top_op;
                    cexp[j++] = ' ';
                    top_op = pop(&s);
                }
                break;
            case '[':           //���� ��ȣ
                push(&s, ch);
                break;
            case ']':           //������ ��ȣ
                top_op = pop(&s);

                while (top_op != '[') {          //���� ��ȣ�� ���� �� ���� ���
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
    //a�� �Է¹��� �� , b�� ����ǥ������� ��ȯ �� �迭

    printf("Enter an Infix expression : ");
    scanf_s("%s", a, sizeof(a));
    infix_to_postfix(a, b);
    printf("Postfix expression: %s \n", b);

    result = eval(b);

    printf("\n");
    printf("Result : %d \n", result);
}