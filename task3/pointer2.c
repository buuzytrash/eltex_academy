
#include <stdio.h>
int main(void)
{
  float x = 5.0;

  float y = 6.0;
  printf("x = %f\n", x);
  printf("y = %f\n", y);
  //   printf("x = %f, &x = %p, sizeof = %ld\n", x, &x, sizeof(x));
  //   printf("y = %f, &y = %p, sizeof = %ld\n", y, &y, sizeof(y));
  float* xp = &x + 1; // смещаем адрес на 1, получаем указатель на &y, т.к. в
                      // данном случае в памяти адрес y находится сразу за
                      // адресом x, возможно применение xp++

  //   printf("%p %p %ld %ld\n", &x, &x + 1, sizeof(&x + 1), sizeof(&x));
  float* yp = &y;
  //   printf("%ld - %p, %ld - %p\n", sizeof(xp), &xp, sizeof(yp), &yp);
  printf("Результат: %f\n", *xp + *yp);
}