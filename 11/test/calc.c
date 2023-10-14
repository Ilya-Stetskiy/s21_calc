#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <math.h>
#include "a.h"
// mod % cosA sinB  tanC acosD asinE atanF sqrtG lnH logI
// priority ()- 0 +- 1 mod 2 */ 3 ^ 4 func 5

EXPORT void printer(char *str)
{
    printf("%s\n", str);
}
double precision(double number)
{

    return number = round(number * 100) / 100;
}

EXPORT char what_the_func(char *func)
{
    char value;
    if (strstr(func, "cos") != NULL)
        value = 'A';
    else if (strstr(func, "sin") != NULL)
        value = 'B';
    else if (strstr(func, "tan") != NULL)
        value = 'C';
    else if (strstr(func, "acos") != NULL)
        value = 'D';
    else if (strstr(func, "asin") != NULL)
        value = 'E';
    else if (strstr(func, "atan") != NULL)
        value = 'F';
    else if (strstr(func, "sqrt") != NULL)
        value = 'G';
    else if (strstr(func, "ln") != NULL)
        value = 'H';
    else if (strstr(func, "log") != NULL)
        value = 'I';

    return value;
}
void calc(Stack_f *Stack_double, char func)
{

    double right_token = Fpop(Stack_double), res;
    switch (func)
    {
    case 'A':
        res = cos(right_token);
        break;
    case 'B':
        res = sin(right_token);
        break;
    case 'C':
        res = tan(right_token);
        break;
    case 'D':
        res = acos(right_token);
        break;
    case 'E':
        res = asin(right_token);
        break;
    case 'F':
        res = atan(right_token);
        break;
    case 'G':
        res = sqrt(right_token);
        break;
    case 'H':
        res = log(right_token);
        break;
    case 'I':
        res = log10(right_token);
        break;

    default:

    {
        double left_token = Fpop(Stack_double);

        switch (func)
        {
        case '+':
            res = right_token + left_token;
            break;
        case '-':
            res = left_token - right_token;
            break;
        case '*':
            res = right_token * left_token;
            break;
        case '/':
            res = left_token / right_token;
            break;
        case '^':
            res = powf(right_token, left_token);
            break;
        case '%':
            res = fmod(right_token, left_token);
            break;

        default:
            // exit(ERROR_ON_CALC);
            break;
        }
    }
    break;
    }

    Fpush(Stack_double, res);
    // printf("\nres=%.8g",Fpeek(Stack_double));
}
void push_func(Stack_t *stack, Stack_t *stack_prior, char spec, int priority, Stack_f *Stack_double)
{
    // printf("(top= %d)  ",stack->top);
    if (stack->top == 0 || spec == '(')
    {
        push(stack, spec);
        push(stack_prior, priority);
    }
    else if (spec == ')')
    {
        while (peek(stack) != '(')
        {
            calc(Stack_double, pop(stack));
            pop(stack_prior);
        }
        pop(stack);
        pop(stack_prior);
        if (peek(stack_prior) == 5)
        {
            calc(Stack_double, pop(stack));
            pop(stack_prior);
        }
    }

    else // if (priority<=peek(stack_prior))
    {
        while (stack->top >= 1 && priority <= peek(stack_prior))
        {
            calc(Stack_double, pop(stack));
            pop(stack_prior);
        }
        push(stack, spec);
        push(stack_prior, priority);
    }
}

EXPORT double not_main(char *input)
{
    printf("input=%s\n", input);
    char result[256];
    char signs[7] = "+-*/^m";
    char flfunc[6] = "acstl";
    // char funcs[5][4];
    double values;
    // int funcs_token=0;
    char temp[25];

    Stack_t *stack = createStack();
    Stack_t *stack_prior = createStack();
    Stack_f *stack_double = createFStack();
    int last_sign=3;// 0-не знак, 1 + 2 - 3- прочее
    // scanf("%s", input);
    for (size_t i = 0; i < strlen(input); i++)
    {
        if (input[i] >= '0' && input[i] <= '9')
        {

            values = atof(&input[i]);
            if (last_sign==2) values=-values;
            Fpush(stack_double, values);
            printf("fpeek = %lf\n", Fpeek(stack_double));
            while (((input[i] >= '0' && input[i] <= '9') || input[i] == '.'))
                i++;
            printf("%ld - длина элементф\n", strlen(temp));
            last_sign=0;
        }
        if (strchr(signs, input[i]) != NULL)
        {
            if (input[i] == 'm')
            {
                push_func(stack, stack_prior, '%', 2, stack_double);
                last_sign=3;
            }
            else if (input[i] == '+' || input[i] == '-')
                if (last_sign ==0) {push_func(stack, stack_prior, input[i], 1, stack_double);last_sign=3;}
                else if(input[i] == '-') {
                    if (last_sign == 2) last_sign=1;
                    else last_sign=2;
                }
                else last_sign=1;
            else if (input[i] == '*' || input[i] == '/')
                {push_func(stack, stack_prior, input[i], 3, stack_double);last_sign=3;}
            else if (input[i] == '^')
                {push_func(stack, stack_prior, input[i], 4, stack_double);last_sign=3;}
        }
        if (strchr(flfunc, input[i]) != NULL)
        {last_sign=0;
            int j = 0;
            for (; input[i] != '('; i++)
            {
                temp[j] = input[i];
                j++;
            }
            temp[j] = '\0';
            // strcpy(funcs[funcs_token],temp);
            // funcs_token++;
            push_func(stack, stack_prior, what_the_func(temp), 5, stack_double);
        }
        if (input[i] == '(' || input[i] == ')')
        {last_sign=3;
            push_func(stack, stack_prior, input[i], 0, stack_double);
        };
        if (input[i] == '=')
            break;
    }
    printf("hyi");

    //======================================================================================
    size_t size_stack = stack->top;
    for (size_t i = 0; i < size_stack; i++)
    {
        printf("pop stack=%c", peek(stack));
        calc(stack_double, pop(stack));
    }
    printf("hyi");
    double res = Fpeek(stack_double);
    if (stack_double->top == 1)
        sprintf(result, "%.8g", Fpop(stack_double));
    else
        sprintf(result, "error");
    // for (int i=0;i<values_token; i++){
    // printf("value %d = %f\n", i,values[i]);
    // }
    // for (int i=0;i<funcs_token; i++){
    // printf("funcs %d = %s\n", i,funcs[i]);
    // }

    deleteStack(&stack);
    deleteStack(&stack_prior);
    deleteFStack(&stack_double);
    printf("res!!=%lf", res);
    return res;
}

EXPORT double credit_a(char *summ, char *years, char *month, char *percent)
{
    double p = atof(percent) / 100 / 12, month_pay;
    month_pay = atof(summ) * (p + p / (pow((1 + p), atof(month) + 12 * atof(years)) - 1));
    return month_pay;
}

EXPORT double credit_d(char *summ_c, char *years_c, char *month_c, char *percent_c)
{
    double sum = atof(summ_c), month = 12 * atof(years_c) + atof(month_c), percent = atof(percent_c);
    double base = sum / month, res = sum, p = percent / 100 / 12, dop;
    FILE *data;
    data = fopen("result.txt", "w+");
    fprintf(data, " № | payment\n");
    for (int i = 1; i <= month; i++)
    {
        dop = sum * p;
        fprintf(data, "%3d| %.2F\n", i, dop + base);
        sum -= base;
        res += dop;
    }
    fclose(data);
    return res;
}

EXPORT double vklad_not(char *summ_c, int days, char *percent_c, char *history, int *capital, int *nalog_date, int nal_len, char *nal_per_c, int *new_year, int flag, int flag_capital)
{
    double res;

    double sum_res, nalog_res;

    long double sum = atof(summ_c);
    double percent = atof(percent_c), nal_percent = atof(nal_per_c);

    char *token = NULL;
    printf("%f\n", nal_percent);
    char sep[5] = " \n\0";
    for (int i = 1; i < 60; i++)
    {
        printf("%d (%d)", capital[i], capital[i] - capital[i - 1]);
    }
    double *change = (double *)malloc(2 * sizeof(double));
    int num = 0;
    if (history[0] != '\0')
    {
        token = strpbrk(history, sep);
        change[num] = atof(history);
        num++;
        change[num] = atof(token);
        num++;
        token = strpbrk(token + sizeof(char), sep);
        while (token != NULL)
        {
            printf("\n token %s, %d\n", token, token[0]);
            change = (double *)realloc(change, (num + 3) * sizeof(double));
            change[num] = atof(token);
            token = strpbrk(token + sizeof(char), sep);
            change[num + 1] = atof(token);
            token = strpbrk(token + sizeof(char), sep);
            num += 2;
        }
    }

    double *sort_change = (double *)malloc((num + 1) * sizeof(double));
    for (int i = 0; i < num; i += 2)
    {
        double min = 100000;
        int point_of_min;
        for (int j = 0; j < num; j += 2)
        {
            if (change[j] < min && change[j] >= 0)
            {
                min = change[j];
                point_of_min = j;
            }
        }
        sort_change[i] = change[point_of_min];
        sort_change[i + 1] = change[point_of_min + 1];
        change[point_of_min] = -1;
    }

    int current_day = 0, last_day = 0, cur_change = 0, cur_capit = 0, cur_nal = 0, cur_year = 0;
    long double res_capit = 0, sum_nal = 0, last_nal = 0;
    res = 0;
    printf("hui!!!\n");

    for (; current_day <= days; current_day++)
    {

        if (current_day == nalog_date[cur_nal])
        {
            printf("NALOGI!!!\n");
            // res+=precision(sum*(current_day-last_day)*percent/new_year[cur_year]/100);

            if (res - last_nal > 1000000 * 0.12)
            {
                sum_nal += precision((res - last_nal - 1000000 * 0.12) * nal_percent / 100);
            }

            printf("res=%.2F, sum=%.2LF d_days=%d, sum_nal=%LF, d_res=%.2LF\n", res, sum, current_day - last_day, sum_nal, res - last_nal);
            last_nal = res;
            // last_day=current_day;
            cur_nal++;
            res += precision(sum * (current_day - last_day) * percent / new_year[cur_year] / 100);
            cur_year++;
            last_day = current_day;
        }
        if (current_day == sort_change[cur_change])
        {
            printf("hyinya\n");
            res += precision(sum * (current_day - last_day) * percent / new_year[cur_year] / 100);
            last_day = current_day;
            sum += sort_change[cur_change + 1];
            cur_change += 2;
        }
        if (current_day == capital[cur_capit])
        {
            res += precision(sum * (current_day - last_day) * percent / new_year[cur_year] / 100);
            printf("res=%.2F, sum=%.2LF d_days=%d, cur_capit=%d, d_res=%.2LF\n", res, sum, current_day - last_day, cur_capit, res - res_capit);
            last_day = current_day;
            if (flag_capital == 1)
                sum += res - res_capit;
            res_capit = res;
            cur_capit++;
        }
    }
    res += precision(sum * (days - last_day) * percent / new_year[cur_year] / 100);
    if (res - last_nal > 1000000 * 0.12)
        sum_nal += precision((res - last_nal - 1000000 * 0.12) * nal_percent / 100);
    printf("nalog=%LF\n", sum_nal);

    sum_res = sum;
    nalog_res = sum_nal;

    free(change);
    free(sort_change);

    if (flag == 3)
    {
        res = sum_res;
    }
    else if (flag == 2)
    {
        res = nalog_res;
    }
    return res;
}

int main()
{
    // printf("%lf",vklad_not("50000",365, "4.7","5 1000 100 -2000","));
}
