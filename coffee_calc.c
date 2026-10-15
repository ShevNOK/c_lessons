#include <stdio.h>

int main() {
	double dose = 0.0;
	double ratio = 0.0;

	printf("введите два значения вес молотого кофе и коэффицент");
if (scanf("%lf %lf", &dose, &ratio) != 2){
	printf("Ошибка: нужно ввести два числа!\n");
	return 1;
}
double yield = dose * ratio;
printf("Для %.1f г зерна при коэффициенте 1:%.1f нужно получить %.1f г эспрессо.\n",
           dose, ratio, yield);
if (ratio < 1.5) {
        printf("Тип напитка: Ристретто (плотный и концентрированный)\n");
    } else if (ratio <= 2.5) {
        printf("Тип напитка: Классический эспрессо (баланс вкуса)\n");
    } else {
        printf("Тип напитка: Лунго (длинный пролив)\n");
    }
    return 0;
}
