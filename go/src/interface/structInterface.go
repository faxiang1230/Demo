package main

import (
	"fmt"
	"reflect"
)

type Dog struct {
	Name string
	Age  int
}

func (dog *Dog) Eat() {
	fmt.Printf("%s is eating.", dog.Name)
}

func (dog *Dog) Run() {
	fmt.Printf("%s is running.", dog.Name)
}

func (dog Dog) Sleep() {
	fmt.Printf("%s is sleeping.", dog.Name)
}

func (dog Dog) Jump() {
	fmt.Printf("%s is jumping.", dog.Name)
}

func main() {
	doggy := Dog{"doggy", 2}
	checkFieldAndMethod(doggy)

	fmt.Println("")
	tommy := &Dog{"tommy", 2}
	checkFieldAndMethod(tommy)
}

func checkFieldAndMethod(input interface{}) {
	inputType := reflect.TypeOf(input)
	fmt.Println("Type of input is :", inputType.Name())
	inputValue := reflect.ValueOf(input)
	fmt.Println("Value of input is :", inputValue)

	// 如果input原始类型时指针，通过Elem()方法或者Indirect()获取指针指向的值
	if inputValue.Kind() == reflect.Ptr {
		inputValue = inputValue.Elem()
		// inputValue = reflect.Indirect(inputValue)
		fmt.Println("Value input points to is :", inputValue)
	}

	//使用NumField()得到结构体中字段的数量，遍历得到字段的值Field(i)和类型Field(i).Type()
	for i := 0; i < inputValue.NumField(); i++ {
		field := inputValue.Type().Field(i)
		value := inputValue.Field(i).Interface()
		fmt.Printf("%s: %v = %v\n", field.Name, field.Type, value)
	}

	// 获取方法
	for i := 0; i < inputType.NumMethod(); i++ {
		m := inputType.Method(i)
		fmt.Printf("%s: %v\n", m.Name, m.Type)
	}
}
