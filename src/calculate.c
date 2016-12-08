//
// calcute.c
// Copyright (C)  2016 jskyzero
//

#include "calculate.h"
#include <string.h>  // for strchr() strlen()
#include <stdlib.h>  // for malloc() free()
#include <ctype.h>   // for isdigit()
#include <limits.h>  // for INT_MIN


int int_add(int num1, int num2, int* have_error);
int int_sub(int num1, int num2, int* have_error);
int int_mul(int num1, int num2, int* have_error);
int int_div(int num1, int num2, int* have_error);

int expr(const char* cal_str, int size, int* have_error);
int term(const char* cal_str, int size, int* have_error);
int factor(const char* cal_str, int size, int* have_error);
int integer(const char* cal_str, int size, int* have_error);
int number(const char* cal_str, int size, int* have_error);
int digit(char ch, int* have_error);

// #include <stdio.h>
// void show(const char* cal_str, int size, const char* str, int* p) {
//   if (*p == 0) {
//     char a[100] = {0};
//     strncpy(a, cal_str, size);
//     printf("%s %s\n", str, a);
//   }
//   if (*p == -1) {
//     char a[100] = {0};
//     strncpy(a, cal_str, size);
//     printf("   %s %s\n", str, a);
//   }
// }

int expr(const char* cal_str, int size, int* have_error) {
  // show(cal_str, size, "expr", have_error);
  if (*have_error > 0) return 0;
  char* char_ptr;
  int left, try_error = -1;
  char_ptr = strchr(cal_str, '+');
  if (char_ptr  && (char_ptr - cal_str) <= size ) {
    left = term(cal_str, char_ptr - cal_str, &try_error);
    if (try_error <= 0) {
      term(cal_str, char_ptr - cal_str, have_error);
      return int_add(
          left, expr(char_ptr + 1, size - (char_ptr - cal_str) - 1, have_error),
          have_error);
    }
  }
  try_error = -1;
  char_ptr = strchr(cal_str, '-');
  if (char_ptr  && (char_ptr - cal_str) <= size ) {
    left = term(cal_str, char_ptr - cal_str, &try_error);
    if (try_error <= 0) {
      term(cal_str, char_ptr - cal_str, have_error);
      return int_sub(
          left, expr(char_ptr + 1, size - (char_ptr - cal_str) - 1, have_error),
          have_error);
    }
  }

  return term(cal_str, size, have_error);
}

int term(const char* cal_str, int size, int* have_error) {
  // show(cal_str, size, "term", have_error);
  if (*have_error > 0) return 0;
  char* char_ptr;
  int left, try_error = -1;
  char_ptr = strchr(cal_str, '*');
  if (char_ptr  && (char_ptr - cal_str) <= size ) {
    left = factor(cal_str, char_ptr - cal_str, have_error);
    if (try_error <= 0) {
      left = factor(cal_str, char_ptr - cal_str, &try_error);
      return int_mul(
          left, term(char_ptr + 1, size - (char_ptr - cal_str) - 1, have_error),
          have_error);
    }
  }
  try_error = -1;
  char_ptr = strchr(cal_str, '/');
  if (char_ptr  && (char_ptr - cal_str) <= size ) {
    left = factor(cal_str, char_ptr - cal_str, &try_error);
    if (try_error <= 0) {
      left = factor(cal_str, char_ptr - cal_str, have_error);
      return int_div(
          left, term(char_ptr + 1, size - (char_ptr - cal_str) - 1, have_error),
          have_error);
    }
  }

  return factor(cal_str, size, have_error);
}

int factor(const char* cal_str, int size, int* have_error) {
  // show(cal_str, size, "factor", have_error);
  if (*have_error > 0) return 0;
  if ('(' == cal_str[0] && ')' == cal_str[size - 1]) {
    return expr(cal_str + 1, size - 2, have_error);
  }
  return integer(cal_str, size, have_error);
}


int integer(const char* cal_str, int size, int* have_error) {
  if (*have_error > 0) return 0;
  
  if (0 == strncmp(cal_str, "-2147483648", 11)) return INT_MIN;
  if ('+' == cal_str[0]) return number(cal_str + 1, size - 1, have_error);
  if ('-' == cal_str[0]) return -number(cal_str + 1, size - 1, have_error);
  return number(cal_str, size, have_error);
}

int number(const char* cal_str, int size, int* have_error) {
  if (*have_error > 0) return 0;
  if (size <= 0) {
    *have_error = EXPRESSION_ERROR;
    return 0;
  }

  return size == 1 ? digit(cal_str[0], have_error)
                   : int_add(int_mul(number(cal_str, size - 1, have_error), 10,
                                     have_error),
                             digit(cal_str[size - 1], have_error), have_error);
}

int digit(char ch, int* have_error) {
  if (*have_error > 0) return 0;
  if (!isdigit(ch)) {
    *have_error = EXPRESSION_ERROR;
    return 0;
  } else {
    return ch - '0';
  }
}

int int_add(int num1, int num2, int* have_error) {
  if (*have_error > 0) return 0;
  int ans = num1 + num2;
  if ((ans ^ num1) >= 0 || (ans ^ num2) >= 0) {
    return ans;
  } else {
    *have_error = OVER_FLOW_ERROR;
    return 0;
  }
}

int int_sub(int num1, int num2, int* have_error) {
  if (*have_error > 0) return 0;
  return int_add(num1, -num2, have_error);
}

int int_mul(int num1, int num2, int* have_error) {
  if (*have_error > 0) return 0;
  int ans = 0;
  while (num2--) {
    ans = int_add(ans, num1, have_error);
  }
  return ans;
}

int int_div(int num1, int num2, int* have_error) {
  if (*have_error > 0) return 0;
  if (num2 == 0) {
    if (num1 != 0) {
      return 0;
    } else {
      *have_error = DIVIDE_BY_ZERO_ERROR;
      return 0;
    }
  } else {
    return num1 / num2;
  }
}

int calculate(const char* input_str) {
  size_t len = strlen(input_str);
  char* cal_str = (char*)malloc((len + 1) * sizeof(char));
  int i, j;  // input_str index and cal_str index
  int result;

  // remove white spaces
  for (i = 0, j = 0; i < len; ++i) {
    if (!isspace(input_str[i])) {
      cal_str[j] = input_str[i];
      ++j;
    }
  }
  cal_str[j] = '\0';
  // start
  int have_error = 0;
  result = expr(cal_str, j, &have_error);
  free(cal_str);
  if (have_error) {
    show_error_message(have_error);
    return 0;
  } else
    return result;
}