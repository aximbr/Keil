/*Programa de teste em C++ para dispositivo embarcado */

#include "stm32f10x.h"


using namespace std;
	
	class mygpio {
		public:
			GPIO_TypeDef * gpio_name;
		  mygpio (GPIO_TypeDef *gpio_name) { }
		  void ligar ();
			void desligar ();
	};
	
	void mygpio::ligar() {
		
    if      (gpio_name == GPIOA) RCC->APB2ENR |=  (1U << 2);
    else if (gpio_name == GPIOB) RCC->APB2ENR |=  (1U << 3);
    else if (gpio_name == GPIOC) RCC->APB2ENR |=  (1U << 4);
    else if (gpio_name == GPIOD) RCC->APB2ENR |=  (1U << 5);
    else if (gpio_name == GPIOE) RCC->APB2ENR |=  (1U << 6);
    else if (gpio_name == GPIOF) RCC->APB2ENR |=  (1U << 7);
    else if (gpio_name == GPIOG) RCC->APB2ENR |=  (1U << 8);
	}
	
		void mygpio::desligar() {
		
    if      (gpio_name == GPIOA) RCC->APB2ENR &=  ~(1U << 2);
    else if (gpio_name == GPIOB) RCC->APB2ENR &=  ~(1U << 3);
    else if (gpio_name == GPIOC) RCC->APB2ENR &=  ~(1U << 4);
    else if (gpio_name == GPIOD) RCC->APB2ENR &=  ~(1U << 5);
    else if (gpio_name == GPIOE) RCC->APB2ENR &=  ~(1U << 6);
    else if (gpio_name == GPIOF) RCC->APB2ENR &=  ~(1U << 7);
    else if (gpio_name == GPIOG) RCC->APB2ENR &=  ~(1U << 8);
	}
		
	int main(void){
		
		mygpio porta_b(GPIOB);
		
		
		porta_b.ligar();
		
	}
