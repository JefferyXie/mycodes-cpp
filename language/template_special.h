#ifndef TEMPLATE_SPECIAL_H
#define TEMPLATE_SPECIAL_H

#include "../main/header.h"

// http://stackoverflow.com/questions/14926482/const-and-non-const-template-specialization
// http://stackoverflow.com/questions/13401716/selecting-a-member-function-using-different-enable-if-conditions

template<class T>
void Temp_Fun_specialization (T v) {
    cout << "Temp_Fun_specialization (T): " << v << endl;
    // there're three ways that give compile error if specialization is not provided
    // 1) don't even implement the default function
    // 2) use below code in the default function
    //    T::unimplemented_function;
    // 3) use static_assert as bleow
    //    static_assert(sizeof(T) == 0, "not yet implemented");
};

template<>
void Temp_Fun_specialization<int> (int v) {
    cout << "Temp_Fun_specialization<int> (int): " << v << endl;
};

template<>
void Temp_Fun_specialization<double> (double v) {
    cout << "Temp_Fun_specialization<double> (double): " << v << endl;
};

template<>
void Temp_Fun_specialization<char*> (char* v) {
    cout << "Temp_Fun_specialization<char*> (char*): " << v << endl;
};

template<>
void Temp_Fun_specialization<const char*> (const char* v) {
    cout << "Temp_Fun_specialization<const char*> (const char*): " << v << endl;
};

template<>
void Temp_Fun_specialization<string> (string v) {
    cout << "Temp_Fun_specialization<string> (string): " << v << endl;
};

template<>
void Temp_Fun_specialization<string&> (string& v) {
    cout << "Temp_Fun_specialization<string&> (string&): " << v << endl;
};

template<>
void Temp_Fun_specialization<const string&> (const string& v) {
    cout << "Temp_Fun_specialization<const string&> (const string&): " << v << endl;
};

// below 3 declarations won't compile, given the template's declaration, 
// you have to make sure the specialization type is same as parameter's type!
//void Temp_Fun_specialization<string> (const string& v) {};
//void Temp_Fun_specialization<string> (string& v) {};
//void Temp_Fun_specialization<string&> (string v) {};


/*
template<typename T>
void
Temp_Fun_specialization_ex (T v)
{
}

template<>
void
Temp_Fun_specialization_ex<char> (char v)
{
}
*/

// use enable_if and it's value as return type of the function to do specialization
// Especially, we combine const and non-const into one implementation in this way!!
// however, I not yet find a way to provide default template function like above lines!
template<typename T>
typename std::enable_if<
                        std::is_same<string, typename std::remove_const<T>::type>::value
                       >::type 
Temp_Fun_specialization_ex (T v)
{
    cout << "string:" <<  v << endl;
};
template<typename T>
typename std::enable_if<
                        std::is_same<int, typename std::remove_const<T>::type>::value
                       >::type 
Temp_Fun_specialization_ex (T v)
{
    cout << "int:" << v << endl;
};

void Run_Temp_specialization () 
{
    int v_int = 10;
    short v_short = 12;
    double v_double = 9.99;
    float v_float = 2.12f;
    char v_char = 'u';
    char v_char_arr[] = "abc";
    const char* v_char_p = "hello";
    string v_str = "world";
    string& v_str_ref = v_str;
    const string v_str_const = "linux";
    const string& v_str_const_ref = v_str;

    Temp_Fun_specialization_ex(v_str);
    Temp_Fun_specialization_ex(v_str_ref);
    Temp_Fun_specialization_ex(v_str_const);
    Temp_Fun_specialization_ex(v_str_const_ref);

    Temp_Fun_specialization_ex<string>(v_str);
    Temp_Fun_specialization_ex<const string>(v_str_const);

    // call straightforward
    cout << "1) call straightforward:" << endl;
    Temp_Fun_specialization(v_int);
    Temp_Fun_specialization(v_short);
    Temp_Fun_specialization(v_double);
    Temp_Fun_specialization(v_float);
    Temp_Fun_specialization(v_char);
    Temp_Fun_specialization(v_char_arr);
    Temp_Fun_specialization(v_char_p);
    Temp_Fun_specialization(v_str);
    Temp_Fun_specialization(v_str_ref);
    Temp_Fun_specialization(v_str_const);
    Temp_Fun_specialization(v_str_const_ref);

    // call with decltype(..)
    cout << "1-1) call with decltype:" << endl;
    Temp_Fun_specialization<decltype(v_str)>(v_str);
    Temp_Fun_specialization<decltype(v_str_ref)>(v_str_ref);
    Temp_Fun_specialization<decltype(v_str_const)>(v_str_const);
    Temp_Fun_specialization<decltype(v_str_const_ref)>(v_str_const_ref);

    // call with type specified
    cout << "2) call with type specified:" << endl;
    Temp_Fun_specialization<int>(v_int);
    Temp_Fun_specialization<short>(v_short);
    Temp_Fun_specialization<double>(v_double);
    Temp_Fun_specialization<float>(v_float);
    Temp_Fun_specialization<char>(v_char);
    Temp_Fun_specialization<char*>(v_char_arr);
    Temp_Fun_specialization<const char*>(v_char_p);
    Temp_Fun_specialization<string>(v_str);
    Temp_Fun_specialization<string&>(v_str_ref);
    Temp_Fun_specialization<const string>(v_str_const);
    Temp_Fun_specialization<const string&>(v_str_const_ref);

    // call with potential type conversion
    cout << "3) call with potential type conversion:" << endl;
    Temp_Fun_specialization<int>(v_int);
    Temp_Fun_specialization<int>(v_short);
    Temp_Fun_specialization<double>(v_double);
    Temp_Fun_specialization<double>(v_float);
    Temp_Fun_specialization<char>(v_char);
    Temp_Fun_specialization<const char*>(v_char_arr);
    Temp_Fun_specialization<const char*>(v_char_p);

    cout << "4) call with potential type conversion:" << endl;
    Temp_Fun_specialization<string>(v_char_p);
    Temp_Fun_specialization<string>(string(v_char_p));
    Temp_Fun_specialization<string>(v_char_arr);
    Temp_Fun_specialization<string>(string(v_char_arr));
    Temp_Fun_specialization<string>(v_str);
    Temp_Fun_specialization<string>(v_str_ref);
    Temp_Fun_specialization<string>(v_str_const);
    Temp_Fun_specialization<string>(v_str_const_ref);

    cout << "5) call with potential type conversion:" << endl;
    Temp_Fun_specialization<string&>(v_str);
    Temp_Fun_specialization<string&>(v_str_ref);
    //Temp_Fun_specialization<string&>(v_str_const);
    //Temp_Fun_specialization<string&>(v_str_const_ref);

    cout << "6) call with potential type conversion:" << endl;
    Temp_Fun_specialization<const string>(v_str);
    Temp_Fun_specialization<const string>(v_str_ref);
    Temp_Fun_specialization<const string>(v_str_const);
    Temp_Fun_specialization<const string>(v_str_const_ref);

    cout << "7) call with potential type conversion:" << endl;
    Temp_Fun_specialization(string(v_char_p));
    Temp_Fun_specialization(string(v_char_p));
    Temp_Fun_specialization(string(v_char_arr));
    Temp_Fun_specialization<const string&>(v_str);
    Temp_Fun_specialization<const string&>(v_str_ref);
    Temp_Fun_specialization<const string&>(v_str_const);
    Temp_Fun_specialization<const string&>(v_str_const_ref);
}

#endif

